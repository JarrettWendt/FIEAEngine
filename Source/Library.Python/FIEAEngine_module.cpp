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

	//if (PyModule_AddObject(fiea, "Entity", reinterpret_cast<PyObject*>(&PyEntityType)) < 0)
	//{
	//	Py_DECREF(&PyEntityType);
	//}
	
	return fiea;
}
