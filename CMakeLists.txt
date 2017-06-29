#Protheus CMake build and dependencies

cmake_minimum_required(VERSION 2.6) 
project(Protheus)

#####################################################
## PROTHEUS PROJECT
#####################################################
  

#Define Utilities files

set(Utilities_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Utilities/Log.cpp
)
set(Utilities_HEADERS
    ${CMAKE_SOURCE_DIR}/src/Utilities/AlignedBuffer.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/AlignedReader.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/AlignedWriter.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/ArrayList.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/BitCompare.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Buffer.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/BufferBase.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/BufferIO.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/BufferReader.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/BufferWriter.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/ClassDefinition.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Deferred.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/DoubleBuffer.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/ErrorCheck.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Future.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/half.hpp
    ${CMAKE_SOURCE_DIR}/src/Utilities/LinkedList.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Log.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Matrix33.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Matrix44.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Member.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Endian.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/ObjectPool.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Parallel.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/ProMath.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Queue.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/smart_array.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/smart_ptr.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Timer.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Vector2.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Vector3.h
    ${CMAKE_SOURCE_DIR}/src/Utilities/Vector4.h
)

#Define core files

set(Core_HEADERS
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/Audio.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/AudioListener.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioBuffer.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioData.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioSource.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/IAudioEffect.h
    ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/CFile.h
    ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/EFile.h
    ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/FileSystem.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/IBitstreamDecoder.h
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/WavDecoder.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/AnimatedSprite.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Camera.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Lighting.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Mesh.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshLoader.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshObject.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Program.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Projection.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Shader.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Sprite.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/SpriteBatcher.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Texture.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureLoader.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureUnit.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Transformation.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/VertexArray.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/WindowDefinition.h
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/OpenGL.h 
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/Connection.h
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/ConnectionType.h
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/Server.h 
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/NetCommands.h  
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Mouse.h
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Keyboard.h
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Controller.h 
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Keys.h
    ${CMAKE_SOURCE_DIR}/src/Core/Profiler/Profiler.h
    ${CMAKE_SOURCE_DIR}/src/Core/Profiler/ProfilerState.h
)

set(Core_SDLNet 
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnet.c
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetselect.c
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetTCP.c
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetUDP.c
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDL_net.h
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/begin_code.h
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/close_code.h
    ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetsys.h
)

set(Core_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/Audio.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/AudioListener.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/WavDecoder.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioSource.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/CFile.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/AnimatedSprite.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Lighting.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshLoader.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshObject.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/OpenGL.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Program.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Projection.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Sprite.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/SpriteBatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureLoader.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureUnit.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Transformation.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Graphics/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Keyboard.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Input/Mouse.cpp 
    ${CMAKE_SOURCE_DIR}/src/Core/Profiler/Profiler.cpp
)

#OS Specific files

if(APPLE)
    set(Core_SOURCES ${Core_SOURCES} 
        ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window_OSX.cpp
	${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window_OSX.mm) 

    # For some reason, CMake doesn't know about .m
    # set_source_files_properties(${Core_SOURCES} PROPERTIES LANGUAGE C)

    # Add compiler flag for C++14 
    set(CMAKE_CXX_FLAGS -std=c++1y) 
    add_definitions(-DAPPLE)
    add_definitions(-DPRO_DEBUG)

elseif(WIN32 OR WIN64)
    add_definitions(-DWINDOWS)
    add_definitions(-DPRO_DEBUG)
    set(Core_SOURCES ${Core_SOURCES} ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window_WIN.cpp)
endif()

#Create Utilities Library

add_library(Utilities ${Utilities_SOURCES} ${Utilities_HEADERS})

#Create Core Library

add_library(Core ${Core_SOURCES} ${Core_HEADERS} ${Core_SDLNet})
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Graphics/extern)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Core/Audio/openal) 
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Utilities)
target_link_libraries(Core PUBLIC Utilities)

#Link Correct Libraries

if(APPLE) 
    target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Apple/libGLEW.a)
#${Core_LIBRARIES})
	
endif()

if(WIN32)
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win32/glew32s.lib) 
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win32/OpenAL32.lib)
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win32/OpenCL.lib)
	
endif()

if(WIN64) 
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win64/glew32s.lib) 
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win64/OpenAL32.lib)
	target_link_libraries(Core PUBLIC ${CMAKE_SOURCE_DIR}/externlib/Win64/OpenCL.lib)

endif()

# Create Test project

add_executable(Test
    ${CMAKE_SOURCE_DIR}/src/Core_Test/TextureLoading.cpp
)

target_include_directories(Test PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/Core 
    ${CMAKE_SOURCE_DIR}/src/Utilities
) 
target_link_libraries(Test PUBLIC Core Utilities)
