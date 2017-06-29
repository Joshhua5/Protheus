#include "EventManager.h"

using namespace Pro;
using namespace Util;
using namespace Events;


EventManager::EventManager() {
	_handle_count = 1;
}

EventManager::~EventManager() {

}

void EventManager::RegisterEvent(const Event& evnt, const string& channel_name) {
	RegisterEvent(evnt, hash<string>()(channel_name));
}

void EventManager::RegisterListener(EventListener& listener, const string& channel_name) {
	RegisterListener(listener, hash<string>()(channel_name));
}
 
void EventManager::RegisterEvent(const Event& evnt, const size_t hash) {
	auto& listeners = _listeners.find(hash);
	if (listeners != _listeners.end())
		for (auto& listener : listeners->second) 
			if (listener.Active()) 
				listener.Callback()(evnt); 
}
 
void EventManager::RegisterListener(EventListener& listener, const size_t hash) {
	auto& listeners = _listeners.find(hash);
	if (listeners != _listeners.end()) {
		listener.Handle(_handle_count.fetch_add(1));
		listeners->second.push_back(listener);
	}
	else
	{
		std::vector<EventListener> new_channel;
		new_channel.push_back(listener);
		_listeners.insert({ hash, std::move(new_channel) }); 
	}
}


EventListener* EventManager::RetrieveListener(const unsigned handle) {
	for (auto& channels : _listeners)
		for (auto& listener : channels.second)
			if (listener.Handle() == handle)
				return &listener; 
}

EventListener* EventManager::RetrieveListener(const unsigned handle, const string& channel_name) {
	return RetrieveListener(handle, hash<string>()(channel_name));
}

EventListener* EventManager::RetrieveListener(const unsigned handle, const size_t hashed_channel_name) {
	auto& listeners = _listeners.find(hashed_channel_name);
	if (listeners != _listeners.end())
		for (auto& listener : listeners->second)
			if (listener.Handle() == handle)
				return &listener;
	return nullptr;
}
