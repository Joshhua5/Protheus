//
//  ConnectionStorage.cpp
//  Protheus
//
//  Created by Joshua Waring on 28/04/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#include "ConnectionStorage.h"

using namespace Pro;
using namespace Networking;

ConnectionStorage::ConnectionStorage(){
}

ConnectionStorage::~ConnectionStorage(){
	for (auto connection : active_connections_)
		connection_pool_.Return(connection.second); 
}

Connection* ConnectionStorage::GetConnection(unsigned connection_id){
	return active_connections_.at(connection_id);
}

unsigned long long ConnectionStorage::ConnectionCount() const{
    return leased_connection_count_;
}

Connection* ConnectionStorage::GetNewConnection(){
    ++leased_connection_count_;
    auto connection =  connection_pool_.Get();
	active_connections_.insert({ connection->ID(), connection });
	return connection;
}

void ConnectionStorage::ReturnConnection(Connection* connection){
    --leased_connection_count_;
	active_connections_.erase(connection->ID());
	connection_pool_.Return(connection); 
}