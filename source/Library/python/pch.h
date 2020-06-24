#pragma once

/**
 * There are numerous issues with linking against the debug version of Python.
 * 
 * The default (release) version of Python is python##.lib.
 * The debug version of Python is python##_d.lib and can easily be installed alongside the release version through additional steps in the interactive Python installer.
 *
 * So linking against python##_d.lib in a debug configuration isn't too difficult, but libraries are a different matter.
 * `py -m pip install _____` will only install release builds of packages.
 * If you want debug versions, you have to compile it manually from source.
 */

#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#endif

#include <Python.h>
#include <frameobject.h>
#include <structmember.h>

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif
