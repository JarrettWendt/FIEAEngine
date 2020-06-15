// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Python.h"

using namespace Library;

int main([[maybe_unused]] int argc, char* argv[])
{
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (!program)
	{
		std::cerr << "Fatal error: cannot decode argv[0]" << std::endl;
		std::terminate();
	}
	Py_SetProgramName(program);
	Py_Initialize();
	PyRun_SimpleString(R"(
from time import time, ctime
print('Today is', ctime(time()))
)");

	if (Py_FinalizeEx() < 0)
	{
		std::terminate();
	}
	PyMem_RawFree(program);
	
	return 0;
}
