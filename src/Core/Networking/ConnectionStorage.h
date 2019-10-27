//
//  ConnectionStorage.h
//  Protheus
//
//  Created by Joshua Waring on 28/04/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#pragma once
 
#include "Connection.h" 
#include <ObjectPool.h>
#include <unordered_map>

namespace Pro{
    namespace Networking{
        //! The ConnectionStorage stores connections and assigns a connection_id to each connection
        //! Also provides cleanup of these connections and GetConnection functionality
        class ConnectionStorage{
            Util::ObjectPool<Connection> connection_pool_;
			std::unordered_map<unsigned, Connection*> active_connections_;
            unsigned long long leased_connection_count_;
        public:
            ConnectionStorage();
            ~ConnectionStorage();
            
            //! Grab a new connection from storage
            //! The connection has an ID assigned to it
            Connection* GetNewConnection();
            
            //! Get's a conenction from it's ID
            Connection* GetConnection(unsigned connection_id);
            
            //! After a connection has been closed, return it to storage
            void ReturnConnection(Connection* connection);
            
            unsigned long long ConnectionCount() const; 
        };
    }
}