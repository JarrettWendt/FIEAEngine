#include "pch.h"
#include "FIEAEngine_module.h"

#include "Entity_module.h"
#include "Math_module.h"

PyObject* PyInit_FIEAEngine() noexcept
{
	PyObject* fiea = PyModule_Create(&FIEAEngine_module);
	if (!fiea) [[unlikely]]
	{
		return nullptr;
	}
	
	PyObject* math = PyInit_Math();
	Py_INCREF(math);
	PyModule_AddObject(fiea, "Math", math);

	PyObject* entity = Library::py::Init_Entity();
	Py_INCREF(entity);
	PyModule_AddObject(fiea, "Entity", entity);
	
	return fiea;
}
