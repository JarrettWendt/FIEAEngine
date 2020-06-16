// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Engine_module.h"
#include "Entity_module.h"
#include "Engine.h"

namespace Library::py::Engine
{
	EntityBinding* World()
	{
		return EntityBinding::FromEntity(Library::Engine::World().shared_from_this());
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
