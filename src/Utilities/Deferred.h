/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once

#include <vector>
#include "smart_pointer.h"

namespace Pro {
	namespace Util {
		/*! Provides control on when an objects deconstructor should be called
			Objects will be destroyed when check() is called and only one reference remains
		*/
		template<typename T>
		class Deferred {
			std::vector<smart_ptr<T>> flagged_deleted;

		public:
			/*! Add a instance to be destoyed */
			void flag(smart_ptr<T>&& rhs) {
				flagged_deleted.push_back(rhs);
			}

			/*! Check instances that should be destroyed*/
			void check() {
				for (size_t i = 0; i < flagged_deleted.size(); ++i)
					if (flagged_deleted[i].references())
						// TEST the --i
						// if I erase, the current index will now contain the next
						// element 
						flagged_delete.erase(flagged_deleted.begin() + i--);
			}
		};
	}
}