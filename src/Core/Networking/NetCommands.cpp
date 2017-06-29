//
//  NetCommands.cpp
//  Protheus
//
//  Created by Joshua Waring on 1/05/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#include "NetCommands.h"

using namespace Pro;
using namespace Networking;
using namespace Util;

void NetworkCommands::RegisterNewCommand(std::function<void (Connection&, const Buffer&, void *)> callback,
                                             const std::string &command_name, void* userdata){
    commands_.insert({command_name, {callback, userdata}});
}


void NetworkCommands::SendCommand(Connection& connection,
                                  const Buffer& data,
                                  const std::string& command){
    Buffer command_data(data.size() + command.size() + sizeof(CommandHeader));
    BufferWriter writer(&command_data);
    
    CommandHeader header;
    
    // Populate Variabes with the correct endianess
    if(IS_LITTLE_ENDIAN){
        header.command_size = bswap_64(command_data.size());
        header.name_size = bswap_64(command.size());
        header.data_endianess = 'L';
    }else{
        header.command_size = command_data.size();
        header.name_size = command.size();
        header.data_endianess = 'B';
    }
    
    // Make sure this platform doesn't have a above normal size
    assert(sizeof(uint64_t) == 8);
    
    // Populate the buffer for sending
    writer.Write<uint64_t>(header.command_size);
    writer.Write<uint64_t>(header.name_size);
    writer.Write(command.data(), static_cast<unsigned>(command.size()));
    writer.Write<char>(header.data_endianess);
    writer.Write(data.data(), static_cast<unsigned>(data.size()));
    
    connection.Send(std::move(command_data));
}

void NetworkCommands::ProcessCommand(Connection& connection, CommandHeader& header, Buffer& command){
    BufferReader reader(&command);
    reader.head(sizeof(uint64_t) * 2);
    
    Buffer name = reader.Read(static_cast<unsigned>(header.name_size), false);
    std::string name_str = std::string(name.data<char>(), name.size());
    
    header.data_endianess = reader.Read<char>();
    uint64_t data_size = header.command_size - header.name_size - sizeof(CommandHeader);
    
    // Should we byteswap the data?
    Buffer data = reader.Read(static_cast<unsigned>(data_size), false);
    
    auto callback = commands_.at(name_str);
    callback.first(connection, data, callback.second);
}

inline void DownloadPendingCommand(Connection& connection, Buffer& buffer, BufferWriter& writer){
    connection.PollRecv();
    Buffer new_data;
    while(connection.Recv(new_data) == false){
        sleep_for(1);
        connection.PollRecv();
    }
    writer.Write(new_data.data(), (unsigned)new_data.size());
}

void NetworkCommands::DownloadCommand(Connection& connection, Buffer* data){
    Buffer recv_data;
    CommandHeader header;
    BufferWriter concat(&recv_data);
    BufferReader reader(&recv_data);
    
    // Update the connection and check for recieved data
    // only terminate if this function wasn't called with data
    connection.PollRecv();
    if(data == nullptr && !connection.HasReceived())
        return;
    
    // Make sure we have atleast 16 bytes in the data, otherwise we need to wait for more
    if(data != nullptr){
        recv_data = *data;
        concat.head((unsigned)recv_data.size());
        while(recv_data.size() < sizeof(uint64_t) * 2)
            DownloadPendingCommand(connection, recv_data, concat);
    }else{
        connection.Recv(recv_data);
        concat.head((unsigned)recv_data.size());
    }
    
    // Get the Command Header size
    if(IS_LITTLE_ENDIAN){
        header.command_size = bswap_64(reader.Read<uint64_t>());
        header.name_size = bswap_64(reader.Read<uint64_t>());
    }else{
        header.command_size = reader.Read<uint64_t>();
        header.name_size = reader.Read<uint64_t>();
    }
    
    if(header.command_size > recv_data.size())
        recv_data.Resize((unsigned)header.command_size);
    
    // Check if we need more data to complete the command
    while(concat.head() < header.command_size)
        DownloadPendingCommand(connection, recv_data, concat);
    
    // Process the command before dealing with the other command, so we process
    // in the order that commands were recieved
    ProcessCommand(connection, header, recv_data);
    
    if(header.command_size < concat.head()){
        // We've downloaded extra data of the next command
        Buffer overflow(recv_data.data<char>() + header.command_size,
                        (size_t)(concat.head() - header.command_size),
                        false);
        DownloadCommand(connection, &overflow);
    }
}

void NetworkCommands::UpdateConnection(Connection& connection){
    DownloadCommand(connection, nullptr);
}