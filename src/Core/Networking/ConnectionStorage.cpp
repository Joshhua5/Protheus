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
    for(unsigned x = 0; x < connection_pool_.BlockCount(); ++x){
        auto block = connection_pool_.GetBlock(x);
        for(unsigned y = 0; x < connection_pool_.blockSize(); ++y){
            if(block[y].ID() != 0)
                (block + y)->~Connection();
        }
    }
}

Connection* ConnectionStorage::GetConnection(unsigned connection_id){
    for(unsigned x = 0; x < connection_pool_.BlockCount(); ++x){
        auto block = connection_pool_.GetBlock(x);
        for(unsigned y = 0; x < connection_pool_.blockSize(); ++y){
            if(block[y].ID() == connection_id)
                return block + y;
        }
    }
    return nullptr;
}

unsigned long long ConnectionStorage::ConnectionCount() const{
    return leased_connection_count_;
}

Connection* ConnectionStorage::GetNewConnection(){
    ++leased_connection_count_;
    return connection_pool_.Get();
}

void ConnectionStorage::ReturnConnection(Connection* connection){
    --leased_connection_count_;
    connection_pool_.Return(connection);
}