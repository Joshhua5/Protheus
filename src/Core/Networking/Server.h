#pragma once 
#include "Connection.h"
#include <vector>
#include <thread>
#include <ObjectPool.h>
#include <Parallel.h>
#include <LinkedList.h> 

namespace Pro{
	namespace Networking{
        class Server{
            Connection server_;
            
			std::string host_name;
            struct ThreadStatus{
                std::atomic<bool> listening_;
                std::atomic<bool> terminate_;
                
            } status;
			bool initialised;
            
            // Listener thread accepts new connections
            void listener_thread(ThreadStatus*);
            // Send and receives data from connections
            void send_recv_thread(ThreadStatus*); 
            
            // List of connected connections
            // TODO replace this with a Thread Safe B-Tree
			// Until then I'll use a mutex and std::vector
			// Server will suffer with large connection counts, until this is fixed
			std::mutex connection_storage_mut_;
            std::vector<Connection> connection_storage_;
			std::thread* listener_thread_ = nullptr;
			std::thread* recv_thread_ = nullptr;
            
            // List of connections recently recieved
            // Moved into connections_ once NewConnection has been called
            // This allows for Connection Initialisation to occur on the application level
            
            // Data to be broadcasted over all connected sockets
            Util::Pipe<Util::Buffer> broadcast_pending_;
            
            // List of dropped connections
            Util::Pipe<unsigned long long> dropped_connection_;
            Util::Pipe<unsigned long long> connection_with_event_;
            Util::Pipe<unsigned long long> new_connection_;
            
            inline void StartThreads();
		public:
            Server();
            Server(unsigned short listening_port);
            Server(IPv4 address);
            ~Server();

            bool StartListening(unsigned short listening_port);

            bool StartListening(IPv4 address);

            unsigned long long ConnectionCount() const;
            
            //! Returns true if a new connection has been established
            bool HasNewConnection() const;
            
            //! Returns a connection recently established
            //! Once a new connection has been retrieved, it's moved to connection_
            unsigned long long GetNewConnection();
            
            //! Returns true if a connection has dropped
            bool HasConnectionDropped() const;
            
			//! Closes all connections that have disconnected
			//! And removed all closed connections from storage
			void CleanConnections();

            //! Returns the id of the dropped connection
            unsigned long long DroppedConnection();
            
            //! Copies the data buffer and broadcasts the data over all
            //! active connections
            void Broadcast(const Util::Buffer& data);
            //! Claims ownership of the data buffer and broadcasts the data over all
            //! active connections
            void Broadcast(Util::Buffer&& data);
            
            //! Returns a connection based on it's id
            Connection* GetConnection(unsigned long long connection);
            
            //! Returns true if a connection has received data
            bool HasUpdatedConnections() const;

			//! Returns true if the server is listening for new connections
			bool IsListening() const;
            
            //! Returns the id of a connection which has received data
            unsigned long long GetUpdatedConnection();
		};
    }
}