//
//  Server.cpp
//  Protheus
//
//  Created by Joshua Waring on 1/05/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//
#include "Server.h"

using namespace std;
using namespace chrono;
using namespace Pro;
using namespace Networking;

Server::Server() :  dropped_connection_(2048),
					connection_with_event_(4096),
					new_connection_(2048),
                    broadcast_pending_(512)
{
	connection_storage_.reserve(1024);
	initialised = false;
}

Server::Server(unsigned short listening_port) : Server() {
	StartListening(listening_port);
}

Server::Server(IPv4 address) : Server() {
	StartListening(address);
}

Server::~Server() {
	status.terminate_.store(true);
	status.listening_.store(false);
	global_log.Report<LogCode::NETWORK>("Terminating server workers", __FILE__, __LINE__);
	if (recv_thread_ != nullptr)
		recv_thread_->join();
	if (listener_thread_ != nullptr)
		listener_thread_->join();
	CleanConnections();
}

bool Server::StartListening(unsigned short listening_port) {
	return StartListening(IPv4(0, 0, 0, 0, listening_port));
}

bool Server::StartListening(IPv4 address) {
	if (initialised)
		return false;
	global_log.Report<LogCode::NETWORK>("Launching server", __FILE__, __LINE__);
	server_.ConnectTo(address);
	StartThreads();
	return true;
}

unsigned long long Server::ConnectionCount() const {
	return static_cast<unsigned long long>(connection_storage_.size());
}

bool Server::HasNewConnection() const {
	return new_connection_.size() != 0;
}

//! Returns a connection recently established
//! Once a new connection has been retrieved, it's moved to connection_
unsigned long long Server::GetNewConnection() {
	unsigned long long index = 0;
    new_connection_.TopPop(index);
    return index;
}

//! Returns true if a connection has dropped
bool Server::HasConnectionDropped() const {
	return dropped_connection_.size() != 0;
}

//! Returns the id of the dropped connection
unsigned long long Server::DroppedConnection() {
	unsigned long long index = 0;
	dropped_connection_.TopPop(index);
	return index;
}

//! Returns a connection based on it's id
Connection* Server::GetConnection(unsigned long long connection) { 
	connection_storage_mut_.lock();
    for(auto& con : connection_storage_)
		if (con.ID() == connection) {
			connection_storage_mut_.unlock();
			return &con;
		}
	connection_storage_mut_.unlock();
    return nullptr;
}

//! Returns true if a connection has received data
bool Server::HasUpdatedConnections() const {
	return connection_with_event_.size() != 0;
}

//! Returns the id of a connection which has received data
unsigned long long Server::GetUpdatedConnection() {
	unsigned long long index = 0;
	connection_with_event_.TopPop(index);
	return index;
}

inline void Server::StartThreads() {
	status.listening_.store(true);
	status.terminate_.store(false);

	listener_thread_ = new std::thread(&Server::listener_thread, this, &status);
	recv_thread_ = new std::thread(&Server::send_recv_thread, this, &status);
}

bool Server::IsListening() const {
    return status.listening_.load();
}

void Server::Broadcast(const Util::Buffer& data){
    broadcast_pending_.Push(data);
}

void Server::Broadcast(Util::Buffer&& data){
    broadcast_pending_.Push(std::move(data));
}

void Server::listener_thread(ThreadStatus* status) {
    server_.SetBlocking(true);
    while(status->terminate_.load() == false){
        if(status->listening_.load() == false)
            sleep_for(100);
        // Listen for a new connection
        bool has_new;
        // If we have a new connection, immediatly check
        // for another new connection without a sleep
        do{
            Connection new_con = server_.Listen();
            has_new = new_con.IsOpen();
            if(has_new){
                // The server required non blocking sockets
                new_con.SetBlocking(false);
                // Process the new connection
				auto conn_id = new_con.ID();
				connection_storage_mut_.lock();
                connection_storage_.push_back(std::move(new_con));
				connection_storage_mut_.unlock();
                new_connection_.Push(conn_id);
            }
        }while(has_new);
        
        sleep_for(100);
    }
} 

void Server::send_recv_thread(ThreadStatus* status) {
    while(status->terminate_.load() == false){
        if(status->listening_.load() == false)
            sleep_for(100);
        // Get latest broadcast
        Util::Buffer broadcast;
        bool had_broadcast = broadcast_pending_.TopPop(broadcast);
        
		connection_storage_mut_.lock();
		for(auto& con : connection_storage_){
			if (con.IsOpen()) {
				// Receive data from socket
				if(con.PollRecv())
                    connection_with_event_.Push(con.ID());
				// Send pending data
				con.Flush();
				if (had_broadcast)
					con.Send(broadcast.data<char>(), broadcast.size()); 
			} 
		}
		connection_storage_mut_.unlock();
        sleep_for(10);
    }
}

void Server::CleanConnections() {
	connection_storage_mut_.lock();
	for (size_t x = 0; x < connection_storage_.size(); ++x) {
		auto& con = connection_storage_[x]; 
		if(con.IsOpen() == false) {
			dropped_connection_.Push(con.ID());
			connection_storage_.erase(connection_storage_.begin() + x);
			--x;
		}
	}
	connection_storage_mut_.unlock();
}
