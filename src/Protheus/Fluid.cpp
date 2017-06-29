/*
Copyright (c) 2013 Benedikt Bitterli

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <Graphics\Window.h>
#include <Graphics\Surface.h> 
#include <Graphics\SpriteBatcher.h>
#include <Graphics\BasicShapes.h>

using namespace std;

using namespace Pro;
using namespace Graphics;
using namespace Math;

/* This is the class representing fluid quantities such as density and velocity
 * on the MAC grid. It saves attributes such as offset from the top left grid
 * cell, grid width and height as well as cell size.
 *
 * It also contains two memory buffers: A source (_src) buffer and a
 * destination (_dst) buffer.
 * Most operations on fluid quantities can be done in-place; that is, they
 * write to the same buffer they're reading from (which is always _src).
 * However, some operations, such as advection, cannot be done in-place.
 * Instead, they will write to the _dst buffer. Once the operation is
 * completed, flip() can be called to swap the source and destination buffers,
 * such that the result of the operation is visible to subsequent operations.
 */
class FluidQuantity {
	/* Memory buffers for fluid quantity */
	double *_src;
	double *_dst;

	/* Width and height */
	int _w;
	int _h;
	/* X and Y offset from top left grid cell.
	 * This is (0.5,0.5) for centered quantities such as density,
	 * and (0.0, 0.5) or (0.5, 0.0) for jittered quantities like the velocity.
	 */
	double _ox;
	double _oy;
	/* Grid cell size */
	double _hx;

	/* Linear intERPolate between a and b for x ranging from 0 to 1 */
	double lerp(double a, double b, double x) const {
		return a*(1.0 - x) + b*x;
	}

	/* Simple forward Euler method for velocity integration in time */
	void euler(double &x, double &y, double timestep, const FluidQuantity &u, const FluidQuantity &v) const {
		double uVel = u.lerp(x, y) / _hx;
		double vVel = v.lerp(x, y) / _hx;

		x -= uVel*timestep;
		y -= vVel*timestep;
	}

public:
	FluidQuantity(int w, int h, double ox, double oy, double hx)
		: _w(w), _h(h), _ox(ox), _oy(oy), _hx(hx) {
		_src = new double[_w*_h];
		_dst = new double[_w*_h];

		memset(_src, 0, _w*_h*sizeof(double));
	}

	~FluidQuantity() {
		delete[] _src;
		delete[] _dst;
	}

	void flip() {
		swap(_src, _dst);
	}

	const double *src() const {
		return _src;
	}

	/* Read-only and read-write access to grid cells */
	double at(int x, int y) const {
		return _src[x + y*_w];
	}

	double &at(int x, int y) {
		return _src[x + y*_w];
	}

	/* Linear intERPolate on grid at coordinates (x, y).
	 * Coordinates will be clamped to lie in simulation domain
	 */
	double lerp(double x, double y) const {
		x = min(max(x - _ox, 0.0), _w - 1.001);
		y = min(max(y - _oy, 0.0), _h - 1.001);
		int ix = (int)x;
		int iy = (int)y;
		x -= ix;
		y -= iy;

		double x00 = at(ix + 0, iy + 0), x10 = at(ix + 1, iy + 0);
		double x01 = at(ix + 0, iy + 1), x11 = at(ix + 1, iy + 1);

		return lerp(lerp(x00, x10, x), lerp(x01, x11, x), y);
	}

	/* Advect grid in velocity field u, v with given timestep */
	void advect(double timestep, const FluidQuantity &u, const FluidQuantity &v) {
		for (int iy = 0, idx = 0; iy < _h; iy++) {
			for (int ix = 0; ix < _w; ix++, idx++) {
				double x = ix + _ox;
				double y = iy + _oy;

				/* First component: Integrate in time */
				euler(x, y, timestep, u, v);

				/* Second component: Interpolate from grid */
				_dst[idx] = lerp(x, y);
			}
		}
	}

	/* Sets fluid quantity inside the given rect to value `v' */
	void addInflow(double x0, double y0, double x1, double y1, double v) {
		int ix0 = (int)(x0 / _hx - _ox);
		int iy0 = (int)(y0 / _hx - _oy);
		int ix1 = (int)(x1 / _hx - _ox);
		int iy1 = (int)(y1 / _hx - _oy);

		for (int y = max(iy0, 0); y < min(iy1, _h); y++)
			for (int x = max(ix0, 0); x < min(ix1, _h); x++)
				if (fabs(_src[x + y*_w]) < fabs(v))
					_src[x + y*_w] = v;
	}
};

/* Fluid solver class. Sets up the fluid quantities, forces incompressibility
 * performs advection and adds inflows.
 */
class FluidSolver {
	/* Fluid quantities */
	FluidQuantity *_d;
	FluidQuantity *_u;
	FluidQuantity *_v;

	/* Width and height */
	int _w;
	int _h;

	/* Grid cell size and fluid density */
	double _hx;
	double _density;

	/* Arrays for: */
	double *_r; /* Right hand side of pressure solve */
	double *_p; /* Pressure solution */


	/* Builds the pressure right hand side as the negative divergence */
	void buildRhs() {
		double scale = 1.0 / _hx;

		for (int y = 0, idx = 0; y < _h; y++) {
			for (int x = 0; x < _w; x++, idx++) {
				_r[idx] = -scale*(_u->at(x + 1, y) - _u->at(x, y) +
					_v->at(x, y + 1) - _v->at(x, y));
			}
		}
	}

	/* Performs the pressure solve using Gauss-Seidel.
	 * The solver will run as long as it takes to get the relative error below
	 * a threshold, but will never exceed `limit' iterations
	 */
	void project(int limit, double timestep) {
		double scale = timestep / (_density*_hx*_hx);

		double maxDelta;
		for (int iter = 0; iter < limit; iter++) {
			maxDelta = 0.0;
			for (int y = 0, idx = 0; y < _h; y++) {
				for (int x = 0; x < _w; x++, idx++) {
					int idx = x + y*_w;

					double diag = 0.0, offDiag = 0.0;

					/* Here we build the matrix implicitly as the five-point
					 * stencil. Grid borders are assumed to be solid, i.e.
					 * there is no fluid outside the simulation domain.
					 */
					if (x > 0) {
						diag += scale;
						offDiag -= scale*_p[idx - 1];
					}
					if (y > 0) {
						diag += scale;
						offDiag -= scale*_p[idx - _w];
					}
					if (x < _w - 1) {
						diag += scale;
						offDiag -= scale*_p[idx + 1];
					}
					if (y < _h - 1) {
						diag += scale;
						offDiag -= scale*_p[idx + _w];
					}

					double newP = (_r[idx] - offDiag) / diag;

					maxDelta = max(maxDelta, fabs(_p[idx] - newP));

					_p[idx] = newP;
				}
			}

			if (maxDelta < 1e-5) {
				printf("Exiting solver after %d iterations, maximum change is %f\n", iter, maxDelta);
				return;
			}
		}

		printf("Exceeded budget of %d iterations, maximum change was %f\n", limit, maxDelta);
	}

	/* Applies the computed pressure to the velocity field */
	void applyPressure(double timestep) {
		double scale = timestep / (_density*_hx);

		for (int y = 0, idx = 0; y < _h; y++) {
			for (int x = 0; x < _w; x++, idx++) {
				_u->at(x, y) -= scale*_p[idx];
				_u->at(x + 1, y) += scale*_p[idx];
				_v->at(x, y) -= scale*_p[idx];
				_v->at(x, y + 1) += scale*_p[idx];
			}
		}

		for (int y = 0; y < _h; y++)
			_u->at(0, y) = _u->at(_w, y) = 0.0;
		for (int x = 0; x < _w; x++)
			_v->at(x, 0) = _v->at(x, _h) = 0.0;
	}

public:
	FluidSolver(int w, int h, double density) : _w(w), _h(h), _density(density) {
		_hx = 1.0 / min(w, h);

		_d = new FluidQuantity(_w, _h, 0.5, 0.5, _hx);
		_u = new FluidQuantity(_w + 1, _h, 0.0, 0.5, _hx);
		_v = new FluidQuantity(_w, _h + 1, 0.5, 0.0, _hx);

		_r = new double[_w*_h];
		_p = new double[_w*_h];

		memset(_p, 0, _w*_h*sizeof(double));
	}

	~FluidSolver() {
		delete _d;
		delete _u;
		delete _v;

		delete[] _r;
		delete[] _p;
	}

	void update(double timestep) {
		buildRhs();
		project(600, timestep);
		applyPressure(timestep);

		_d->advect(timestep, *_u, *_v);
		_u->advect(timestep, *_u, *_v);
		_v->advect(timestep, *_u, *_v);

		/* Make effect of advection visible, since it's not an in-place operation */
		_d->flip();
		_u->flip();
		_v->flip();
	}

	/* Set density and x/y velocity in given rectangle to d/u/v, respectively */
	void addInflow(double x, double y, double w, double h, double d, double u, double v) {
		_d->addInflow(x, y, x + w, y + h, d);
		_u->addInflow(x, y, x + w, y + h, u);
		_v->addInflow(x, y, x + w, y + h, v);
	}

	/* Returns the maximum allowed timestep. Note that the actual timestep
	 * taken should usually be much below this to ensure accurate
	 * simulation - just never above.
	 */
	double maxTimestep() {
		double maxVelocity = 0.0;
		for (int y = 0; y < _h; y++) {
			for (int x = 0; x < _w; x++) {
				/* Average velocity at grid cell center */
				double u = _u->lerp(x + 0.5, y + 0.5);
				double v = _v->lerp(x + 0.5, y + 0.5);

				double velocity = sqrt(u*u + v*v);
				maxVelocity = max(maxVelocity, velocity);
			}
		}

		/* Fluid should not flow more than two grid cells per iteration */
		double maxTimestep = 2.0*_hx / maxVelocity;

		/* Clamp to sensible maximum value in case of very small velocities */
		return min(maxTimestep, 1.0);
	}

	/* Convert fluid density to RGBA image */
	void toImage(unsigned char *rgba) {
		for (int i = 0; i < _w*_h; i++) {
			int shade = (int)((1.0 - _d->src()[i])*255.0);
			shade = max(min(shade, 255), 0);

			rgba[i * 4 + 0] = shade;
			rgba[i * 4 + 1] = shade;
			rgba[i * 4 + 2] = shade;
			rgba[i * 4 + 3] = 0xFF;
		}
	}

	void toSurface(Surface& surface) {
		Vector2<int> point = { 0, 0 };
		for (; point.x < _w; ++point.x)
			for (; point.y < _h; ++point.y) {
				char shade = (int)((1.0 - _d->src()[(point.x * _w) + point.y])*255.0);
				shade = max(min(shade, 255), 0);
				surface.DrawPoint(point, Vector4<char>({ shade, shade, shade, (char)0xFF }));
			}
	}
};

const char* vertex_shader = 
"	#version 420								\n"
"	out vec2 tex_coord;							\n"
"	in vec2 tex_coord_in;						\n"
"	in vec3 vertex_in;							\n"
"	void main() {								\n"
"		tex_coord = tex_coord_in;				\n"
"		gl_Position = vec4(vertex_in, 1);		\n"
"	}											\n";
const char* fragme_shader =
"	#version 420								\n"
"	in vec2 tex_coord;							\n"
"	out vec4 out_color;							\n"
"	uniform sampler2D tex;						\n"
"	void main(){								\n"
"		out_color = texture(tex, tex_coord);	\n"
"	}											\n" 
"												\n";
const char* comput_shader = "#version 420";
  

int main() {
	const unsigned sizeX = 256;
	const unsigned sizeY = 256;

	global_log.EchoOnConsole(true);

	Window window("Title", { (int)sizeX, (int)sizeY });
	shared_ptr<Texture> fluid(new Texture({ sizeX, sizeY }, GL_RGBA, GL_UNSIGNED_BYTE));

	GLuint tbo;
	glGenBuffers(1, &tbo);
	  
	const double density = 0.1;
	const double timestep = 0.005;

	Program render_program;
	render_program.Use();
	Shader vertex(std::string(vertex_shader), GL_VERTEX_SHADER);
	Shader frament(std::string(fragme_shader), GL_FRAGMENT_SHADER);
	render_program.AttachShader(vertex);
	render_program.AttachShader(frament); 
	render_program.Link();

	//Program compute_program;
	//compute_program.Use();
	//Shader compute(std::string(comput_shader), GL_COMPUTE_SHADER);
	//compute_program.AttachShader(compute);
	//compute_program.Link();
	  
	GLuint quad;
	glGenBuffers(1, &quad);
	glBindBuffer(GL_ARRAY_BUFFER, quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), QUAD, GL_STATIC_DRAW);
	 
	  
	double time = 0.0;
	int iterations = 0; 
	Buffer surface_data(4 * sizeX * sizeY);
	Surface surface(surface_data.data(), { (int)sizeX, (int)sizeY });
	while (!window.IsExitRequested()) {
		window.Update();
		window.StartFrame();
		
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		render_program.Use();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//solver->addInflow(0.45, 0.2, 0.1, 0.01, 1.0, 0.0, 3.0);
		//solver->update(timestep);
		//time += timestep;
		 
		//solver->toSurface(surface);
		  
		//glActiveTexture(GL_TEXTURE0);
		//glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		//glBufferData(GL_TEXTURE_BUFFER, surface_data.size(), surface_data.data(), GL_DYNAMIC_DRAW); 
		//glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA, tbo);

		// Draw the texture here 
		window.EndFrame();
	}
	global_log.Flush();
	fluid->Unbind(); 
	window.Close();
}
