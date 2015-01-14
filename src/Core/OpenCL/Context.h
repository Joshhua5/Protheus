#pragma once
#include "extern\cl.h"
#include <smart_pointer.h>

namespace Pro{
	namespace OpenCL{ 
		using namespace Util;
		class Context{
			cl_context context;

		public:
			Context(const Context&) = delete;
			Context& operator=(const Context&) = delete;

			Context();
			Context(cl_context);
			Context(Context&&);
			Context& operator=(Context&&);
			Context& operator=(cl_context);

			cl_context getContext() const { return context; }
		};
	}
}