/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2015.
 -------------------------------------------------------------------------
 
 Description:
 
 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.
 
 *************************************************************************/


#include <Networking/Server.h> 
#include <ObjectPool.h>

using namespace Pro;
using namespace Util;
using namespace Networking;

// Use of a Server to receive multiple connections 
#define SERVER
// Use of a connection to receive a connection
#define SERVER_CONNECTION
 
#ifdef SERVER 
int main(int argc, char* args[]) {
	global_log.EchoOnConsole(true);

	Server server(8080);

	std::string word("<html> <h> Test </h> </html>");
	 
	while (server.IsListening()) {
        auto conn_id = server.GetNewConnection();
		Connection* con = server.GetConnection(conn_id);
		if (con != nullptr && con->IsOpen()) {
			con->SetBlocking(false);
			int sent = (int)con->Send(word.data(), word.size());

			con->PollRecv();
			if (con->HasReceived()) {
				Util::Buffer buf;
				con->Recv(buf);
			}

			if (sent == -1)
				global_log.Report<LogCode::NETWORK>("Unable to send data", __FUNCTION__, __LINE__);
			else
				printf(" Sent %i \n", sent);
			con->Close();
		}
		else {
			server.CleanConnections();
			sleep_for(100);
		}
	}
	// Clean connections
	server.CleanConnections();
}
#elif SERVER_CONNECTION
int main(int argc, char* args []) { 
    Server server(8080);
    
    global_log.EchoOnConsole(true);
    std::string word("<html> <h> Test </h> </html>");
    
    Connection server;
    if(server.ConnectTo(IPv4(0, 0, 0, 0,8080)) == false){
        printf("Unable to establish server");
        global_log.Close();
        return -1;
    }
    while(server.IsOpen()){
        Connection con = server.Listen(); 
        if(con.IsOpen()){
            con.SetNonBlocking();
            int sent = con.Send(word.data(), word.size());
            
            con.PollRecv();
            if(con.HasReceived()){
                Util::Buffer buf;
                con.Recv(buf); 
            } 
            
            if(sent == -1)
                global_log.Report<LogCode::NETWORK>("Unable to send data", __FUNCTION__, __LINE__);
            else
                printf(" Sent %i \n", sent);
            con.Close();
        }else
			sleep_for(100);
    } 
}
#else
// Client

int main(int argc, char* args[]) { 
	global_log.EchoOnConsole(true);
	Connection tcp_client;
   
	if (!tcp_client.ConnectTo("protheus.net.au", "http"))
		global_log.Report<LogCode::NETWORK>("Unable to connect to server", __FUNCTION__, __LINE__);
    
    std::string http_request("GET / HTTP/1.1 Host: 74.125.237.176:80 Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 Accept-Language: en-us Connection: keep-alive Accept-Encoding: gzip, deflate User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/600.5.17 (KHTML, like Gecko) Version/8.0.5 Safari/600.5.17");
    
    while(!tcp_client.IsOpen());

    tcp_client.Send(http_request.data(), http_request.length()); 

	while (tcp_client.IsOpen()){
		if (tcp_client.HasReceived()){
			Buffer data;
			tcp_client.Recv(data);
			std::cout << data.data();
			tcp_client.Close();
		}
	}
	   
	global_log.Close();
}
#endif

