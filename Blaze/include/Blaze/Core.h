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
#else
#define BLAZE_API __declspec(dllimport)
#endif

#endif // BLAZE_CORE_H