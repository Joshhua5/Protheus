#pragma once
#include <Endian.h>
#include <PreDefinition.h>

#if defined(__WINDOWS__)
//#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(POSIX)
#include <sys/socket.h>
#include <sys/types.h>
#endif

namespace Pro{
	namespace Networking{ 

		struct IPv4{
			union{
				unsigned char address_segment[4];
				unsigned int address;
                unsigned int host;
			}; 
			short endian = 1;
            unsigned short port; 
              
            IPv4(char first, char second, char third, char fourth, unsigned short port = 80){
				address_segment[0] = first;
				address_segment[1] = second;
				address_segment[2] = third;
				address_segment[3] = fourth;
                this->port = port;
            }
            
            IPv4(unsigned int host, unsigned short port){
                address = host;
                this->port = port;
            }

			IPv4() = default;
            
            
            void ToNetworkByteOrder(){ 
				if (((char*)&endian)[0] == 1) { // True if currently little Endian
					port = bswap_16(port);
					endian = bswap_16(endian);
					address = bswap_32(address);
				}
            }

			void ToCPUByteOrder() {
				if (((char*)&endian)[0] == IS_LITTLE_ENDIAN) {
                    port = bswap_16(port);
                    endian = bswap_16(endian);
                    address = bswap_32(address);
				}
			}
		};

		struct IPv6{  
			unsigned short address_segment[8];  
		};
	}
}