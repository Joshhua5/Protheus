/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 
 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.
 
 *************************************************************************/
#pragma once
#include "PreDefinition.h"
#include <stdint.h>


namespace Pro{

#define IS_LITTLE_ENDIAN (((char*)&one)[0] == 1)
#define IS_BIG_ENDIAN (!IS_LITTLE_ENDIAN)
    
	const uint32_t one = 1;
    
    static inline uint16_t bswap_16(uint16_t x) {
        return (x>>8) | (x<<8);     }
    
    static inline uint32_t bswap_32(uint32_t x) {
#ifdef GCC 
        __asm__("movl %0,%%eax \n"
                "bswap %%eax \n"
                "movl %%eax,%0 \n"
		: "=r" (x) 
		: "r" (x) 
		: "%eax");
#elif defined(MSVC)
        __asm{
            mov eax, x;
            bswap eax; 
            mov x, eax;
        } 
#endif
        return x;
    }
    
    static inline uint64_t bswap_64(uint64_t x) {
#if defined(GCC) & defined(__64bit__)
        __asm__("movl %0,%%rax \n"
                "bswap %%rax \n"
                "movl %%rax,%0" 
		: "=r" (x) 
		: "r" (x) 
		: "%rax");
        return x;
#elif defined(MSVC) & defined(__64bit__)
        __asm{
            movl rax, x;
            bswap rax;
            movl x, rax;
        }
        return x;
#else
        return ((unsigned long long)((unsigned long)bswap_32(x & 0xffffffff)) << 32) | (bswap_32(x >> 32));
#endif
    }
}
