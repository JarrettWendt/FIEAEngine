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

// Microsoft Native Unit Testing Framework
#include "CppUnitTest.h"

// Library.Shared
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
#include "EngineTime.h"
#include "Literals.h"
#include "LibMath.h"
#include "Util.h"
#include "Reflection.h"
#include "../Library.Shared/Random.h"
// Events
#include "Event.h"
#include "EventManager.h"
// Hierarchy
#include "Attributed.h"
#include "Entity.h"

// Library.Windows
#include "../Library.Windows/Engine.h"
#include "../Library.Windows/Input.h"

// UnitTests
#include "Digit.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "UnitTest.h"
#include "../UnitTest.Library.Windows/Random.h"
#include "ToStrings.h"
