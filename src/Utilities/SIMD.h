//
//  SIMD.h
//  Protheus
//
//  Created by Joshua Waring on 11/10/2015.
//  Copyright © 2015 Joshua Waring. All rights reserved.
//
#pragma once

#include <immintrin.h> 
#include <string>
#include "PreDefinition.h"

#ifdef __SSE__
#include "extern/sse_mathfun.h"
#endif
#ifdef __AVX__
#include "extern/avx_mathfun.h"
#endif

namespace Pro{
    namespace Util{
        
        const std::string SIMDTitles[] = {
            "ERROR",
            "SCALAR",
            "SSE2",
            "AVX",
			"AVX2"
        };
        
        enum struct SIMDInstructions {
            NONE = 0,
            SCALAR = 1,
            SSE2 = 2,
            AVX = 3,
			AVX2 = 4
        };
        
        static inline SIMDInstructions GetRuntimeSupport(){
            uint32_t set1, set2, set3;
            set1 = set2 = 0;
#ifdef GCC
            __asm__("movl $1,%%eax\n"
                    "cpuid\n"
                    "movl %%edx, %0\n"
                    "movl %%ecx, %1\n"
                    "movl $7, %%eax\n"
                    "cpuid\n"
                    "movl %%ebx, %2\n" 
                    : "=r" (set1), "=r" (set2), "=r" (set3)
                    :
                    : "%eax", "%edx", "%ecx");
#elif defined(MSVC)
            __asm{
                mov eax, 1;
                cpuid;
                mov set1, edx;
                mov set2, ecx;

				mov eax, 7;
				cpuid;
				mov set3, ebx;
            } 
#endif
            // compiler must support AVX aswell
#ifdef __AVX2__
			if ((set3 >> 5) & 1)
				return SIMDInstructions::AVX2;
#endif
#ifdef __AVX__
            if((set2 >> 28) & 1) // AVX
                return SIMDInstructions::AVX;
#endif
#ifdef __SSE__
            if((set1 >> 26) & 1) // SSE2
                return SIMDInstructions::SSE2;
#endif
            return SIMDInstructions::SCALAR;
        }
        
        template<typename T>
        static inline SIMDInstructions SIMDCall(T avx2, T avx, T sse2, T scalar, T& output){
            const static SIMDInstructions best_supported = GetRuntimeSupport();
            switch(best_supported){
#ifdef __AVX2__
			case SIMDInstructions::AVX2:
				if (avx2 != nullptr) {
					output = avx2;
					return SIMD_Instructions::AVX2;
				}
#endif
#ifdef __AVX__
                case SIMDInstructions::AVX:
                    if(avx != nullptr){
                        output = avx;
                        return SIMD_Instructions::AVX;
                    }
#endif
#ifdef __SSE__
                case SIMDInstructions::SSE2:
                    if(sse2 != nullptr){
                        output = sse2;
                        return SIMDInstructions::SSE2;
                    }
#endif
                case SIMDInstructions::SCALAR:
                    if(scalar != nullptr){
                        output = scalar;
                        return SIMDInstructions::SCALAR;
                    }
                default:
                    break;
            }
            global_log.Report<LogCode::FAULT>("No viable SIMD/Scalar version to call", __FUNCTION__, __LINE__);
            return SIMDInstructions::NONE;
        }
    }
}


