#pragma once
#include <unordered_map>
#include <stack>
#include <iostream>
namespace Pro{

	/*
	If a ID has an assigned name, the ID will be stored inside
	namMapping and in the allocatedBits 
	*/
	typedef unsigned int ID;

	class IDManager
	{
		// Grab 256K Bits 
		unsigned int allocatedBitCount = 0;
		std::unordered_map<std::string, ID> nameMapping;
		std::stack<unsigned int> releasedIDs;
	public:
		IDManager();
		~IDManager();

		// gets a ID which matches a user defined name
		ID getID(const std::string& name);
		// user must manage the ID as it will be lost with no name
		ID getID();
		void releaseID(const std::string& name);
		void releaseID(ID);
		ID getIDFromName(const std::string& name);

		std::string getName(ID _id);
	};

}