// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#define _SILENCE_CLANG_COROUTINE_MESSAGE

// Standard
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iostream>
#include <new>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <experimental/coroutine>
#else
#include <coroutine>
#endif

// External
#include <gsl/gsl>
