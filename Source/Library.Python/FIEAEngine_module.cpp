#include "pch.h"
#include "FIEAEngine_module.h"

#include "Entity_module.h"
#include "Math_module.h"

PyMODINIT_FUNC PyInit_FIEAEngine()
{
	PyObject* fiea = PyModule_Create(&FIEAEngine_module);
	
	PyObject* math = PyInit_Math();
	Py_INCREF(math);
	PyModule_AddObject(fiea, "Math", math);

	PyObject* entity = PyInit_Entity();
	Py_INCREF(entity);
	PyModule_AddObject(fiea, "Entity", entity);
	
	return fiea;
}
