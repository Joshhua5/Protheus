/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once

#include <vector>
#include <memory>

namespace Pro {
	namespace Util {
		/*! Provides control on when an objects deconstructor should be called
			Objects will be destroyed when check() is called and only one reference remains
		*/
		template<typename T>
		class Deferred {
			std::vector<std::shared_ptr<T>> flagged_deleted_;

		public:
			
			~Deferred(){
				Check();
				// Give error if deferred was deleted with objects still alive.
				if (!flagged_deleted_.empty())
					global_log.ReportErrorNR("Deferred deleted before objects were released.");
			}
			
			/*! Add a instance to be destoyed */
			void Flag(smart_ptr<T>&& rhs) {
				flagged_deleted_.push_back(rhs);
			}

			/*! Check instances that should be destroyed*/
			void Check() {
				for (size_t i = 0; i < flagged_deleted_.size(); ++i)
					if (flagged_deleted_[i].references())
						// TEST the --i
						// if I erase, the current index will now contain the next
						// element 
						flagged_delete.erase(flagged_deleted_.begin() + i--);
			}
		};
	}
}