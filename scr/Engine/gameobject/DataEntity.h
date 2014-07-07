/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once
 
#include "Entity.h"
#include <unordered_map>
  
namespace Pro{
	namespace GameObject{ 
		 
		using namespace std;

		class DataEntity : 
			public Entity
		{ 
			unordered_map<string, void*> m_data;
		public:
			DataEntity(const string& name) : Entity(name){};
			DataEntity() : Entity(){}

			template<typename T> 
			void addData(pair<string, T*>& pair){
				m_data.insert(pair);
			}
			
			// Returns the data stored after casting
			template<typename T>
			T* getData(const string& key){
				return m_data.at(key);
			}
		};
	}
}

