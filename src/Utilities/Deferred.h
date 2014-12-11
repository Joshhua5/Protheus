#pragma once

#include <vector>
#include "smart_pointer.h"

namespace Pro {

	template<typename T>
	class Deferred {
		std::vector<smart_pointer<T>> flagged_deleted;

	public:
		void flag(smart_pointer<T>&& rhs) {
			flagged_deleted.push_back(rhs);
		}

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