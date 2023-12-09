#ifndef YOCKCRAFT_DEFINES_HPP
#define YOCKCRAFT_DEFINES_HPP

#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

template <typename T>
using Scope = std::unique_ptr<T>;

#endif // !YOCKCRAFT_DEFINES_HPP
