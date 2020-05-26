// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <exception>
#include <stdexcept>

#include "Array.h"

namespace Library
{
	class NotImplementedException : public std::runtime_error {};

	class AggregateException : public std::exception
	{
	public:
		Array<std::exception_ptr> exceptions{};
	};
}
