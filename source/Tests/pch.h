#pragma once

#include "../Library/pch.h"

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
#include <catch2/catch.hpp>

// Library
// Containers
#include "Array.h"
#include "Datum.h"
#include "HashMap.h"
#include "SList.h"
#include "Stack.h"
#include "Queue.h"
// Engine
#include "Coroutine.h"
#include "Engine.h"
#include "EngineTime.h"
#include "Input.h"
// Util
#include "Concept.h"
#include "Literals.h"
#include "LibMath.h"
#include "Util.h"
#include "Reflection.h"
#include "Random.h"
// Events
#include "Event.h"
#include "EventManager.h"
// Hierarchy
#include "Attributed.h"
#include "Entity.h"

// Utility
#include "MemLeakUtils.h"
#include "TestRandom.h"
#include "TestUtils.h"
