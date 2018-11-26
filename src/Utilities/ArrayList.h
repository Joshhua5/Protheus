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
#include <initializer_list>
#include <assert.h>
#include <vector>
#include <iterator>
#include "Log.h"
#include "smart_array.h"

namespace Pro {
	namespace Util {
		/*! BufferVector is used to store Data in a dynamically expanding buffer

			It just wraps around a std::vector for now, but this allows us to add our own implementation later
		*/
		template<typename T>
        class alignas(16) ArrayList : public std::vector<T> {
            
 		public:
            ArrayList() : std::vector<T>(){}
            ArrayList(size_t size) : std::vector<T>(size){}
            
            inline bool IsValidIndex(size_t index){
                return (index >= 0 && index < this->size());
            }
        };
	}
}