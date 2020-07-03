// MIT License Copyright (c) 2020 Jarrett Wendt

#include "python/pch.h"
#include "python/modules/FIEAEngine.h"
#include "python/modules/Engine.h"
#include "python/modules/Entity.h"
#include "python/modules/Math.h"
#include "python/modules/Time.h"

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

	PyObject* time = Time::InitModule();
	Py_INCREF(time);
	PyModule_AddObject(fiea, "Time", time);
	
	PyObject* math = Math::InitModule();
	Py_INCREF(math);
	PyModule_AddObject(fiea, "Math", math);

	PyObject* entity = InitEntityModule();
	Py_INCREF(entity);
	PyModule_AddObject(fiea, "Entity", entity);
	
	return fiea;
}
