#pragma once
#include <PreDefinition.h>

#ifdef __WINDOWS__
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#ifndef WIN32
#define WIN32
#endif
#include "extern/glew.h"
#include "extern/wglew.h"
#elif defined (__APPLE__)
#include <OpenGL/gl3.h> 
#include <OpenGL/gl3ext.h>
#elif defined (__LINUX__)
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#include <string>
#include <PreDefinition.h>
#include <Log.h>

namespace Pro {
	namespace Graphics { 
		//! Flag used for activation of error checking
		//! Flag is unused if Protheus isn't compiled with PRO_DEBUG
		#ifdef PRO_DEBUG
		extern bool IsDebuggingEnabled;
		#endif

		extern bool GL_CONTEXT_CREATED;

		std::pair<GLint, GLint> GetOpenGLVersion();
 
		//! Converts the @error into a human readable error
		std::string glGetErrorString(GLint error = glGetError());

		void PushDebugStack(const std::string& msg);

		void PopDebugStack();
	}
}
