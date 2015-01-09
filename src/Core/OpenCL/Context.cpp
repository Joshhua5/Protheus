#include "Context.h"

using namespace Pro;
using namespace OpenCL;

Context::Context() {
	context = NULL;
}

Context::Context(cl_context cont) {
	context = cont;
}

Context::Context(Context&& rhs) {
	context = rhs.context;
	rhs.context = NULL;
}

Context& Context::operator=(Context&& rhs) {
	context = rhs.context;
	rhs.context = NULL;
	return *this;
}

Context& Context::operator=(cl_context rhs) {
	context = rhs;
	return *this;
}