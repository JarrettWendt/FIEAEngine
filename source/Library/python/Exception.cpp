#include <pch.h>
#include "python/pch.h"
#include "python/Exception.h"
#include "python/Util.h"

namespace Library::py
{
	void Exception::HandleErrors()
	{
		PyObject* type, * value;
		PyTracebackObject* traceback;
		PyErr_Fetch(&type, &value, reinterpret_cast<PyObject**>(&traceback));
		if (type)
		{
			std::stringstream stream;
			stream << "file: " << Util::ToString(traceback->tb_frame->f_code->co_filename) << std::endl;
			stream << "line: " << traceback->tb_lineno << std::endl;
			stream << "error: " << Util::ToString(value);
			PyErr_Clear();
			const std::string str = stream.str();
			throw py::Exception(str);
		}
	}
}
