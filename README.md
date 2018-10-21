Protheus
====

Protheus is a Open Source C++ Game Engine.

The entire engine is build around the idea of working effectively on multiple threads.
This project is a playground for learning low level 

The engine is built with 4 layers, each layer has access to the layers below, but no knowledge of the layers above.
Only the first 3 layers have been started.

1. Utilities
	> Contains structures for processing data
2. Core
	> Contains classes for interacting with hardware and APIs
3. Engine
	> Brings all the layers together
4. Editor

Features
====

### Engine
- Event Listener

### Core
- Posix TCP Networking
- OpenAL Audio
- Audio Effects
- OpenGL Wrapper
- Cross Platform Input 
- Mac Window support 
- .OBJ Loading
- .WAV Loading

### Utilities
- QuadTree
- Single producer single consumer (spsc) lock free queue
- Multiple producer multiple consumer (mpmc) queue
- Memory aligned buffers
- Linked List
- Dynamic Array
- Queue
- Buffer IO
- Logging
- Serialization