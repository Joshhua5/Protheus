#pragma once
#include <string>
namespace Pro{

	class Name
	{
		std::string name;
	public:
		void setName(const std::string& _name);
		const std::string getName();
	};

}