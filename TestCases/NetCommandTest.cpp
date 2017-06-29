//
//  NetCommandTest.cpp
//  Protheus
//
//  Created by Joshua Waring on 22/09/2015.
//  Copyright © 2015 Joshua Waring. All rights reserved.
//

#include <Networking/NetCommands.h>
#include <Networking/Connection.h>
#include <atomic>

using namespace Pro;
using namespace Util;
using namespace Networking;

//#define SERVER

std::atomic<bool> callback_called;

void callback(Connection& conn, const Buffer& data, void* userdata){
    global_log.Report<LogCode::MESSAGE>("CALLBACK", __FUNCTION__, __LINE__);
    callback_called = true;
}

#ifdef SERVER

int main(){
    callback_called = false;
    global_log.EchoOnConsole(true);
    
    Connection server;
    NetworkCommands commands;
    commands.RegisterNewCommand(&callback, "callback", nullptr);
    
    server.ConnectTo(IPv4(0, 0, 0, 0, 8080));
    Connection client;
    
    do{
        client = server.Listen();
        sleep_for(10);
    }while(client.IsOpen() == false);
    
    client.SetBlocking(false);
    
    // Connected
    
    commands.SendCommand(client, Buffer(), "callback");
    
    while(callback_called == false)
        commands.UpdateConnection(client);
    
    global_log.Close();
}

#else

int main(){
    callback_called = false;
    global_log.EchoOnConsole(true);
    
    Connection client;
    NetworkCommands commands;
    commands.RegisterNewCommand(&callback, "callback", nullptr);
    
    while(client.ConnectTo("localhost", "8080") == false)
        sleep_for(10);
    
    // Connected
    client.SetBlocking(false);
    
    commands.SendCommand(client, Buffer(), "callback");
    
    while(callback_called == false)
        commands.UpdateConnection(client);
    
    global_log.Close();
}

#endif