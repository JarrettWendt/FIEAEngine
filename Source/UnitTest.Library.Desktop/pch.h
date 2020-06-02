// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

// Standard
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cinttypes>
#include <crtdbg.h>
#include <filesystem>
#include <forward_list>
#include <iterator>
#include <limits>
#include <list>
#include <random>
#include <ranges>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <memory>

// External
#include <gsl/gsl>
#include <glm/glm.hpp>

#pragma warning(push)
#pragma warning(disable: 4201)		// nonstandard extension used : nameless struct/union
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

// Microsoft Native Unit Testing Framework
#include "CppUnitTest.h"

// Engine
// Containers
#include "Array.h"
#include "Datum.h"
#include "HashMap.h"
#include "SList.h"
#include "Stack.h"
#include "Queue.h"
// Util
#include "Concept.h"
#include "Coroutine.h"
#include "Literals.h"
#include "LibMath.h"
#include "Util.h"
#include "Reflection.h"
#include "../Library.Shared/Random.h"
// Events
#include "Event.h"
#include "EventManager.h"
// Engine
#include "Engine.h"
#include "EngineTime.h"
#include "Input.h"
// Hierarchy
#include "Scope.h"
#include "Attributed.h"

// UnitTests
#include "Digit.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "UnitTest.h"
#include "../UnitTest.Library.Desktop/Random.h"
#include "ToStrings.h"
