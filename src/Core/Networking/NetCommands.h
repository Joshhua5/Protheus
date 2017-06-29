/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 
 A NetworkCommand shouldn't be used in conjunction manually sending data
 
 {} Variable Data
 <> Literal Data
 Endianess Of Data (EOD)
    B : Big Endian
    L : Little Endian
 
 Send Command
    {Command Size(bytes)}{Command Name Size(bytes)}{Command Name}{EOD}{Data}
    |==========================Command Size================================|
    |=====8 Bytes=======||=====8 Bytes============||==Name Size=|||=1B||=nB=|
    |=======Big Endian==||=======Big Endian=======|
 
 
 -------------------------------------------------------------------------
 History:
 - 19:09:2015: Waring J.
 *************************************************************************/


#pragma once
#include "Connection.h"
#include <Buffer.h>
#include <BufferWriter.h>
#include <BufferReader.h>
#include <unordered_map>
#include <functional>
 
namespace Pro{
	namespace Networking{

        class NetworkCommands{
            
            struct CommandHeader{
                uint64_t command_size;
                uint64_t name_size;
                char data_endianess;
            };
            
            std::unordered_map<std::string, std::pair<std::function<void (Connection&, const Util::Buffer&, void *)>, void*>> commands_;
            void ProcessCommand(Connection& connection, CommandHeader& header, Util::Buffer& command);
            void DownloadCommand(Connection& connection, Util::Buffer* data);
        public:
            //! Sends a registered command over a network, packaged with the data
            void SendCommand(Connection& connection,const Util::Buffer& data, const std::string& command);
            //! Recieves data from the connection and executes the registered callback
            //! Executes the connections data callbacks until the recieve queue is empty
			void UpdateConnection(Connection& connection);
            //! Register a new command for a connection
            void RegisterNewCommand(std::function<void(Connection&,const Util::Buffer&, void*)> callback,
                                        const std::string& command_name, void* userdata);
		};
	}
}