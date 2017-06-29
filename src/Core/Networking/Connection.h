#pragma once
#include "ConnectionType.h" 
#include "../Profiler/Profiler.h"
#include <Pipe.h> 
#include <thread>
#include <Timer.h>
#include <chrono>
#include <Buffer.h>
#include <Parallel.h>
#include <PreDefinition.h>

#ifdef POSIX

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#elif defined(WINSOCK) 

#include <WinSock2.h>
 
#endif
 
namespace Pro {
	namespace Networking {
        //!
        //!
        //!
        //! If the connection belongs to a server,
		//! the object will be deconstructed after a Disconnect or
		//! Call to Close() and is no longer safe to use.
		class Connection {
#ifdef WINSOCK
			SOCKET socket_;
#elif defined(POSIX)
            int socket_;
			int domain_;
            int type_;
            int protocol_;
#endif 
            union{
                IPv4 address4;
                IPv6 address6;
            };
            
			//! Data waiting to be read from the connection
			Util::Pipe<Util::Buffer> received_data_;

			//! Container for data waiting to be sent
			Util::Pipe<Util::Buffer> sent_data_;

			//! Used for server connections
			unsigned long long connection_id_;
  
			std::atomic<bool> terminate_;

			bool server_established_;
			bool ConnectTo(addrinfo& connection);
		public:
            Connection(int socket, unsigned remote_host, unsigned short port);
            Connection();
            Connection(Connection&&);
            Connection(const Connection&) = delete;
            ~Connection();
            
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&);

			//! Connects to a remote computer, returns true on success
            bool ConnectTo(IPv4 address); 
			bool ConnectTo(const std::string& url, const std::string& service);

            //! Listen on this socket for a new connection
            Connection Listen(); 

			//! Queues a buffer to be sent at next flush
            void QueueSend(Util::Buffer&& data);
 
            //! Queues a buffer to be sent at next flush
            void QueueSend(const Util::Buffer& data);

            //! Returns the address assosiated with this connection
            IPv4 address() const;
            
			//! Send the data over the connection
            size_t Send(const char* data, const size_t size, bool process_out_of_band = false);
            size_t Send(const Util::Buffer& data, bool process_out_of_band = false);

			//! Returns a buffer of the data recieved throuogh @data
            //! If false is returned than there was no data to receive and data is unmodified
            bool Recv(Util::Buffer& data);
            
            //! Polls the socket for new data and stores it in the queue
            bool PollRecv();

			//! Returns true if data is waiting to be read
            bool HasReceived() const;
            
            //! Sets the socket to not block the thread
            void SetBlocking(bool);
            
			//! Returns true if the connection is open
            bool IsOpen() const;
  
            //! Flushes the data waiting to be transmitted
            void Flush();
            
			//! Flushes and then Closes the connection
            //! If data is waiting to be read and Close is called, received data is dumped
            void Close();
            
			//! When a connection is created by the server, it's provided an with an id
			inline unsigned long long ID() const{
				return connection_id_;
			}
		};
	}
}
