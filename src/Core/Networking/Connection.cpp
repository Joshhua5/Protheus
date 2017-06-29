//
//  Connection.cpp
//  Protheus
//
//  Created by Joshua Waring on 30/04/2015.
//  Copyright (c) 2015 Joshua Waring. All rights reserved.
//

#include "Connection.h"

using namespace Pro;
using namespace Util;
using namespace Networking;

static std::atomic<unsigned long long> leased_connections(1);

#ifdef POSIX
static const char* POSIXEnumToString(int err) {
    switch (err) {
        case EACCES:
            return "EACCES";
        case EAGAIN:
            return "EAGAIN";
        case EAFNOSUPPORT:
            return "EAFNOSUPPORT";
        case EMFILE:
            return "EMFILE";
        case ENFILE:
            return "ENFILE";
        case ENOBUFS:
            return "ENOBUFS";
        case ECONNREFUSED:
            return "ECONNREFUSED";
        case ENOMEM:
            return "ENOMEM";
        case EPROTONOSUPPORT:
            return "EPROTONOSUPPORT";
        case EPROTOTYPE:
            return "EPROTOTYPE";
        case ENETDOWN:
            return "ENETDOWN";
        case ENETUNREACH:
            return "ENETUNREACH";
        case ENETRESET:
            return "ENETRESET";
        case ECONNABORTED:
            return "ECONNABORTED";
        case ECONNRESET:
            return "ECONNRESET";
        case EISCONN:
            return "EISCONN";
        case ENOTCONN:
            return "ENOTCONN";
        default:
            return "UNKNOWN";
    }
}
#elif defined(WINSOCK)
static WSAData wsa_data;
static const char* WSAEnumToString(int err) {
    switch (err) {
        case WSAEINTR:
            return "WSAEINTR";
        case WSAEBADF:
            return "WSAEBADF";
        case WSAEACCES:
            return "WSAEACCES";
        case WSAEFAULT:
            return "WSAEFAULT";
        case WSAEINVAL:
            return "WSAEINVAL";
        case WSAEMFILE:
            return "WSAEMFILE";
        case WSAEWOULDBLOCK:
            return "WSAWOULDBLOCK";
        case WSAEINPROGRESS:
            return "WSAINPROGRESS";
        case WSAEALREADY:
            return "WSAEALREADY";
        case WSAENOTSOCK:
            return "WSANOTSOCK";
        case WSAEDESTADDRREQ:
            return "WSADESTADDRREQ";
        case WSAEMSGSIZE:
            return "WSAEMSGSIZE";
        case WSAEPROTOTYPE:
            return "WSAPROTOTYPE";
        case WSAENOPROTOOPT:
            return "WSAENOPROTOOPT";
        case WSAEPROTONOSUPPORT:
            return "WSAPROTONOSUPPORT";
        case WSAESOCKTNOSUPPORT:
            return "WSAESOCKTNOSUPPORT";
        case WSAEOPNOTSUPP:
            return "WSAOPNOTSUPP";
        case WSAEPFNOSUPPORT:
            return "WSAEPFNOSUPPORT";
        case WSAEAFNOSUPPORT:
            return "WSAEAFNOSUPPORT";
        case WSAEADDRINUSE:
            return "WSAEADDRINUSE";
        case WSAEADDRNOTAVAIL:
            return "WSAEADDRNOTAVAIL";
        case WSAENETDOWN:
            return "WSAENETDOWN";
        case WSAENETUNREACH:
            return "WSAENETUNREACH";
        case WSAENETRESET:
            return "WSAENETRESET";
        case WSAECONNABORTED:
            return "WSAECONNABORTED";
        case WSAECONNRESET:
            return "WSAECONNRESET";
        case WSAENOBUFS:
            return "WSAENOBUFS";
        case WSAEISCONN:
            return "WSAEISCONN";
        case WSAENOTCONN:
            return "WSAENOTCONN";
        case WSAESHUTDOWN:
            return "WSAESHUTDOWN";
        case WSAETOOMANYREFS:
            return "WSAETOOMANYREFS";
        case WSAETIMEDOUT:
            return "WSAETIMEDOUT";
        case WSAECONNREFUSED:
            return "WSAECONNREFUSED";
        case WSAELOOP:
            return "WSAELOOP";
        case WSAENAMETOOLONG:
            return "WSAENAMETOOLONG";
        case WSAEHOSTDOWN:
            return "WSAEHOSTDOWN";
        case WSAEHOSTUNREACH:
            return "WSAEHOSTUNREACH";
        case WSAENOTEMPTY:
            return "WSAENOTEMPTY";
        case WSAEPROCLIM:
            return "WSAEPROCLIM";
        case WSAEUSERS:
            return "WSAEUSERS";
        case WSAEDQUOT:
            return "WSAEDQUOT";
        case WSAESTALE:
            return "WSAESTALE";
        case WSAEREMOTE:
            return "WSAEREMOTE";
        case WSASYSNOTREADY:
            return "WSASYSNOTREADY";
        case WSAVERNOTSUPPORTED:
            return "WSAVERNOTSUPPORTED";
        case WSANOTINITIALISED:
            return "WSANOTINITIALISED";
        case WSAEDISCON:
            return "WSAEDISCON";
        case WSAENOMORE:
            return "WSAENOMORE";
        case WSAECANCELLED:
            return "WSAECANCELLED";
        case WSAEINVALIDPROCTABLE:
            return "WSAEINVALIDPROCTABLE";
        case WSAEINVALIDPROVIDER:
            return "WSAEINVALIDPROVIDER";
        case WSAEPROVIDERFAILEDINIT:
            return "WSAEPROVIDERFAILEDINIT";
        case WSASYSCALLFAILURE:
            return "WSASYSCALLFAILURE";
        case WSASERVICE_NOT_FOUND:
            return "WSASERVICE_NOT_FOUND";
        case WSATYPE_NOT_FOUND:
            return "WSATYPE_NOT_FOUND";
        case WSA_E_NO_MORE:
            return "WSA_E_NO_MORE";
        case WSA_E_CANCELLED:
            return "WSA_E_CANCELLED";
        case WSAEREFUSED:
            return "WSAEREFUSED";
        case WSAHOST_NOT_FOUND:
            return "WSAHOST_NOT_FOUND";
        default:
            return "UNKNOWN";
    }
}
#endif

inline const std::string EnumToString(int code) {
#if defined(WINSOCK)
    return std::string(WSAEnumToString(code)) + "(" + std::to_string(code) + ")";
#elif defined(POSIX)
    return std::string(POSIXEnumToString(code)) + "(" + std::to_string(code) + ")";
#endif
}

Connection::Connection(int socket, unsigned host, unsigned short port) : Connection() {
    socket_ = socket;
    address4.host = host;
    address4.port = port;
}

Connection::Connection() : sent_data_(256), received_data_(256){
    // Initialise WinSock
#ifdef __WINDOWS__
    static bool first_run = true;
    if (first_run) {
        first_run = false;
        auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0) {
            global_log.Report<LogCode::NETWORK>("Error Occured while initialising WSA" + EnumToString(result), __FUNCTION__, __LINE__);
            first_run = true;
        }
    }
#endif
    connection_id_ = leased_connections++;
}

Connection::~Connection() {
    connection_id_ = 0;
    // Shutdown socket
    Close();
}

void Connection::Flush() {
    while (!sent_data_.Empty()) {
        Util::Buffer data;
        sent_data_.TopPop(data);
        Send(data.data<char>(), data.size());
    }
}

bool Connection::ConnectTo(IPv4 address) {
    address.ToNetworkByteOrder();
    addrinfo addr;
    memset(&addr, 0, sizeof(addrinfo));
    
    addr.ai_family = AF_INET;
    addr.ai_socktype = SOCK_STREAM;
    addr.ai_protocol = IPPROTO_TCP;
    
    addr.ai_addr = (sockaddr*) new sockaddr_in;
    memset(addr.ai_addr, 0, sizeof(sockaddr_in));
    addr.ai_addrlen = sizeof(sockaddr_in);
    
    reinterpret_cast<sockaddr_in*>(addr.ai_addr)->sin_family = AF_INET;
    reinterpret_cast<sockaddr_in*>(addr.ai_addr)->sin_port = address.port;
    reinterpret_cast<sockaddr_in*>(addr.ai_addr)->sin_addr.s_addr = address.host;
    
    bool ret = ConnectTo(addr);
    delete addr.ai_addr;
    return ret;
}

bool Connection::ConnectTo(const std::string& url, const std::string& service) {
    addrinfo hints;
    addrinfo* address;
    memset(&hints, 0, sizeof(addrinfo));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    int error = getaddrinfo(url.data(), service.data(), &hints, &address);
    if (error != 0)
        global_log.Report<LogCode::NETWORK>(EnumToString(error) + "Code:" + std::to_string(error), __FUNCTION__, __LINE__);
    bool result = ConnectTo(*address);
    freeaddrinfo(address);
    return result;
}

bool Connection::ConnectTo(addrinfo& connection) {
    bool isClient;
    
    // Initialise the Address4
    // sin_port will be in big endian, address4 will initially take it's data
    // in the endian of the cpu, therefor a little endian computer must flip IPv4.endian
    // to be able to handle sin_port and s_addr correctly
    if(IS_LITTLE_ENDIAN)
        address4.endian = bswap_16(address4.endian);
    address4.port = reinterpret_cast<sockaddr_in*>(connection.ai_addr)->sin_port;
    address4.host = reinterpret_cast<sockaddr_in*>(connection.ai_addr)->sin_addr.s_addr;
    
    
#ifdef POSIX
    socket_ = socket(connection.ai_family, connection.ai_socktype, connection.ai_protocol);
    if (socket_ == -1) {
        global_log.Report<LogCode::NETWORK>("Unable to get socket, errno: " + EnumToString(errno), __FUNCTION__, __LINE__);
        Close();
        return false;
    }
    
    switch (connection.ai_addrlen) {
        case sizeof(sockaddr_in) :
            isClient = reinterpret_cast<sockaddr_in*>(connection.ai_addr)->sin_addr.s_addr != 0;
            break;
        case sizeof(sockaddr_in6) :
            isClient = (
                        ((sockaddr_in6*)connection.ai_addr)->sin6_addr.__u6_addr.__u6_addr32[0] |
                        ((sockaddr_in6*)connection.ai_addr)->sin6_addr.__u6_addr.__u6_addr32[1] |
                        ((sockaddr_in6*)connection.ai_addr)->sin6_addr.__u6_addr.__u6_addr32[2] |
                        ((sockaddr_in6*)connection.ai_addr)->sin6_addr.__u6_addr.__u6_addr32[3]) != 0;
            break;
        default:
            global_log.Report<LogCode::NETWORK>("ai_addrlen doesn't match options IPv4 or IPv6", __FUNCTION__, __LINE__);
            return false;
    }
    
    if (isClient) {
        if (connect(socket_, connection.ai_addr, connection.ai_addrlen) == -1) {
            global_log.Report<LogCode::NETWORK>("Unable to connect, errno: " + EnumToString(errno), __FUNCTION__, __LINE__);
            Close();
            return false;
        }
        return true;
    }
    else {
        int yes = 1;
        setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
        
        SetBlocking(false);
        
        if (bind(socket_, connection.ai_addr, connection.ai_addrlen) == -1) {
            global_log.Report<LogCode::NETWORK>("Unable to bind socket, errno: " + EnumToString(errno), __FUNCTION__, __LINE__);
            Close();
            return false;
        }
        
        if (listen(socket_, 128) == -1) {
            global_log.Report<LogCode::NETWORK>("Unable to listen to socket, errno: " + EnumToString(errno), __FUNCTION__, __LINE__);
            Close();
            return false;
        }
        return true;
    }
#elif defined(WINSOCK)
    // Reset any error that was previously set and report it
    int error = WSAGetLastError();
    if (error) {
        global_log.Report<LogCode::FAULT>("Uncaught WSA Error, caught in Connection::ConnectTo" +
                                          EnumToString(error) + ", Code: " + std::to_string(error), __FUNCTION__, __LINE__);
    }
    
    socket_ = socket(connection.ai_family, connection.ai_socktype, connection.ai_protocol);
    
    if (socket_ == INVALID_SOCKET) {
        global_log.Report<LogCode::NETWORK>("Unable to create socket: " + EnumToString(socket_), __FUNCTION__, __LINE__);
        Close();
        return false;
    }
    
    switch (connection.ai_addrlen) {
        case sizeof(sockaddr_in) :
            isClient = reinterpret_cast<sockaddr_in*>(connection.ai_addr)->sin_addr.S_un.S_addr != 0;
            break;
        case sizeof(sockaddr_in6) :
            isClient = (
                        ((int64_t*)((sockaddr_in6*)connection.ai_addr)->sin6_addr.u.Word)[0] |
                        ((int64_t*)((sockaddr_in6*)connection.ai_addr)->sin6_addr.u.Word)[1]) != 0;
            break;
        default:
            global_log.Report<LogCode::NETWORK>("ai_addrlen doesn't match options IPv4 or IPv6", __FUNCTION__, __LINE__);
            return false;
    }
    
    if (isClient) {
        // Setup as client
        if (connect(socket_, connection.ai_addr, connection.ai_addrlen)) {
            error = WSAGetLastError();
            global_log.Report<LogCode::NETWORK>("Unable to connect to socket, " + EnumToString(error), __FUNCTION__, __LINE__);
            Close();
            return false;
        }
        return true;
    }
    // Setup as server
    SetBlocking(false);
    if (bind(socket_, connection.ai_addr, connection.ai_addrlen)) {
        error = WSAGetLastError();
        global_log.Report<LogCode::NETWORK>("Unable to bind to socket, " + EnumToString(error), __FUNCTION__, __LINE__);
        Close();
        return false;
    }
    
    if (listen(socket_, 128)) {
        error = WSAGetLastError();
        global_log.Report<LogCode::NETWORK>("Unable to listen to socket, " + EnumToString(error), __FUNCTION__, __LINE__);
        Close();
        return false;
    }
    
#endif
    return true;
}

Connection::Connection(Connection&& rhs) :  sent_data_(std::move(rhs.sent_data_)),
received_data_(std::move(rhs.received_data_)) {
    socket_ = rhs.socket_;
    address4 = rhs.address4;
    connection_id_ = rhs.connection_id_;
    
    rhs.socket_ = 0;
    rhs.address4.port = 0;
    rhs.address4.host = 0;
    rhs.connection_id_ = 0;
}


Connection& Connection::operator=(Connection&& rhs) {
    socket_ = rhs.socket_;
    address4 = rhs.address4;
    connection_id_ = rhs.connection_id_;
    
    sent_data_ = std::move(rhs.sent_data_);
    received_data_ = std::move(rhs.received_data_);
    
    rhs.socket_ = 0;
    rhs.address4.port = 0;
    rhs.address4.host = 0;
    rhs.connection_id_ = 0;
    return *this;
}



Connection Connection::Listen() {
    if (address4.host == 0 && socket_ != 0) {
        sockaddr addr;
        
#ifdef POSIX
        socklen_t socklen = sizeof(sockaddr_in);
        int r_socket = accept(socket_, &addr, &socklen);
        if (r_socket == -1) {
            if (errno != EAGAIN)
                global_log.Report<LogCode::NETWORK>("Unable to listen on port, stdin closed: " + EnumToString(errno), __FUNCTION__, __LINE__);
        }
        else
#elif defined(WINSOCK)
            int socklen = sizeof(sockaddr_in);
        int r_socket = accept(socket_, &addr, &socklen);
        if (r_socket == -1) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                global_log.Report<LogCode::NETWORK>("Unable to listen on port, stdin closed: " + EnumToString(error), __FUNCTION__, __LINE__);
            }
        }
        else
#endif
        { // This is the else statement for the above if statements
            sockaddr_in* sock_addr = (sockaddr_in*)&addr.sa_data[0];
            return Connection(r_socket, (unsigned)sock_addr->sin_addr.s_addr, (unsigned short)sock_addr->sin_port);
        }
    }
    return Connection();
}

size_t Connection::Send(const Util::Buffer& data, bool process_out_of_band) {
    return Send(data.data<char>(), data.size(), process_out_of_band);
}

size_t Connection::Send(const char *data, const size_t size, bool process_out_of_band) {
    size_t sent_size = send(socket_, data, size, process_out_of_band);
#if defined(WINSOCK)
    if (sent_size == INVALID_SOCKET) {
        int error = WSAGetLastError();
        switch (error) {
            case WSAEWOULDBLOCK:
                return 0;
            case WSAENOTCONN:
                Close();
            default:
                global_log.Report<LogCode::NETWORK>("Unable to send packet: " + EnumToString(error), __FUNCTION__, __LINE__);
        }
    }
#elif defined(POSIX)
    if (sent_size == -1) {
        switch (errno) {
            case ECONNRESET:
                Close();
            default:
                global_log.Report<LogCode::NETWORK>("Unable to send packet: " + EnumToString(errno), __FUNCTION__, __LINE__);
        }
    }
#endif
    return sent_size;
}

void Connection::QueueSend(const Util::Buffer& data) {
    sent_data_.Push(data);
}

void Connection::QueueSend(Util::Buffer&& data) {
    sent_data_.Push(std::move(data));
}

bool Connection::HasReceived() const {
    return !received_data_.Empty();
}

bool Connection::Recv(Util::Buffer& return_obj) {
    return received_data_.TopPop(return_obj);
}

bool Connection::PollRecv() {
    // 4KB Packet size
    Buffer packet_data(4096);
    size_t rec;
    // We want to return true if we successfully recieved one packet
    bool return_value = false;
    do{
        rec = recv(socket_, packet_data.data<char>(), packet_data.size(), 0);
#ifdef POSIX
        if (rec == -1) {
            switch(errno){
                case EAGAIN:
                    return return_value;
                case ENOTCONN:
                    Close();
                default:
                    global_log.Report<LogCode::NETWORK>("Unable to recv packet: " + EnumToString(errno), __FUNCTION__, __LINE__);
                    return return_value;
            }
        }
#elif defined(WINSOCK)
        if (rec == INVALID_SOCKET) {
            int error = WSAGetLastError();
            switch (error) {
                case WSAEWOULDBLOCK:
                    return return_value;
                case WSAENOTCONN:
                    Close();
                default:
                    global_log.Report<LogCode::NETWORK>("Unable to recv packet: " + EnumToString(error), __FUNCTION__, __LINE__);
                    return return_value;
            }
        }
#endif
        return_value = true;
    // Keep pushing until recv returns nothing or the pipe is full
    }while(received_data_.Push(Util::Buffer(packet_data.data(), rec)));
    return true;
}


void Connection::SetBlocking(bool blocking) {
#ifdef POSIX
    int flags = fcntl(socket_, F_GETFL, 0);
    if(blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(socket_, F_SETFL, flags);
#elif defined(WINSOCK)
    u_long yes = (blocking) ? 0 : 1;
    ioctlsocket(socket_, FIONBIO, &yes);
#endif
}

bool Connection::IsOpen() const {
    return socket_ != 0;
}

IPv4 Connection::address() const{
    return address4;
}

void Connection::Close() {
    if (socket_ == 0)
        return;
#if defined(POSIX)
    shutdown(socket_, SHUT_RD);
    Flush();
    shutdown(socket_, SHUT_RDWR);
    // Delay the close for 100ms
    GetGlobalWorkPool().Batch(&close, nullptr, nullptr, 100, socket_);
#elif defined(WINSOCK)
    closesocket(socket_);
#endif
    socket_ = 0;
}
