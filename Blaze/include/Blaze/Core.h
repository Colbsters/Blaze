#pragma once

#ifndef BLAZE_CORE_H
#define BLAZE_CORE_H

#include <string>
#include <array>
#include <vector>
#include <memory>
#include <utility>
#include <cstdint>

#include <Blaze/Error.h>

#ifdef BLAZE_EXPORTS
#define BLAZE_API __declspec(dllexport)
#define BLAZE_STL_EXTERN
#else
#define BLAZE_API __declspec(dllimport)
#define BLAZE_STL_EXTERN extern
#endif

#endif // BLAZE_CORE_H