/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 02:03:2016 Waring J.

*************************************************************************/
#include "EventListener.h"

using namespace Pro;
using namespace Events;

EventListener::EventListener(function<void(const Event&)> func) {
	Callback(func);
}

EventListener::EventListener(const EventListener& rhs) {
	Callback(rhs.Callback());
	Active(rhs.Active());
}

void EventListener::Callback(function<void(const Event&)> func) {
	_callback = func;
}

const function<void(const Event&)>& EventListener::Callback() const {
	return _callback;
}

bool EventListener::Active() const {
	return _enabled;
}

void EventListener::Active(bool enabled) {
	_enabled = enabled;
}
 
unsigned EventListener::Handle() const {
	return _handle;
}

void EventListener::Handle(unsigned handle) {
	_handle = handle;
}