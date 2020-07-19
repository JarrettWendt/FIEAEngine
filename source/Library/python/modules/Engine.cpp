// MIT License Copyright (c) 2020 Jarrett Wendt

#include "python/pch.h"
#include "python/modules/Engine.h"
#include "python/modules/Entity.h"
#include <Engine.h>

namespace Library::py::Engine
{
	EntityBinding* World()
	{
		return EntityBinding::FromEntity(Library::Engine::World().SharedFromThis());
	}
	
	static PyMethodDef methods[] =
	{
		{ "World", PyCFunction(World), METH_NOARGS, nullptr },

		{ nullptr, nullptr, 0, nullptr }
	};

	static PyModuleDef module =
	{
		PyModuleDef_HEAD_INIT,

		"Engine",
		"Python port of C++ Library::Engine",
		0,
		methods
	};

	PyObject* InitModule()
	{
		return PyModule_Create(&module);
	}
}
