/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 02:03:2016 Waring J.

*************************************************************************/
#pragma once
#include <functional>
#include <atomic>
#include "Event.h"

namespace Pro {
	namespace Events {
		using namespace std;

		class EventListener {
			function<void(const Event&)> _callback;
			atomic<bool> _enabled;
			unsigned _handle;

		public:
			EventListener(function<void(const Event&)>);
			EventListener(const EventListener&);

			void Callback(function<void(const Event&)>);
			const function<void(const Event&)>& Callback() const;
			 
			bool Active() const;
			void Active(bool);
			 
			unsigned Handle() const;
			void Handle(unsigned);
		};
	}
}