//
//  PreDefinition.h
//  Protheus
//
//  Created by Joshua Waring on 17/05/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#pragma once

#ifndef PRO_DEBUG
#define PRO_DEBUG
#endif

#if defined(__APPLE_CC__)
#undef __APPLE__
#define __APPLE__
#endif

#if defined(__APPLE__)
#define POSIX
#define GCC
#endif

#if defined(__linux__) | defined(__unix__)
#define GCC
#define POSIX
#define __LINUX__
#endif

#if defined(WIN32) | defined(WIN64) | defined(_WIN32)
#if _M_IX86_FP != 0
#define __SSE__
#endif

#define __WINDOWS__
#define WINSOCK
#define MSVC
#endif
