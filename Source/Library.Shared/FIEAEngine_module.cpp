// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "FIEAEngine_module.h"

#include "Engine_module.h"
#include "Entity_module.h"
#include "Math_module.h"

PyObject* PyInit_FIEAEngine() noexcept
{
	using namespace Library::py;
	
	PyObject* fiea = PyModule_Create(&FIEAEngine_module);
	if (!fiea) [[unlikely]]
	{
		return nullptr;
	}

	PyObject* engine = Engine::InitModule();
	Py_INCREF(engine);
	PyModule_AddObject(fiea, "Engine", engine);
	
	PyObject* math = Math::InitModule();
	Py_INCREF(math);
	PyModule_AddObject(fiea, "Math", math);

	PyObject* entity = InitEntityModule();
	Py_INCREF(entity);
	PyModule_AddObject(fiea, "Entity", entity);
	
	return fiea;
}
