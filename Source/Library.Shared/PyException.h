#pragma once

#include <stdexcept>

#include "Macros.h"

namespace Library::py
{
	struct Exception : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
		/**
		 * Catches any errors brought up by the Python interpreter and converts them to C++ exceptions.
		 *
		 * @throws py::Exception
		 */
		static void HandleErrors();
	};
}
