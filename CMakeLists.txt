#Protheus CMake build
cmake_minimum_required(VERSION 2.6)
project(Protheus)

add_library(Utilities
  ${CMAKE_SOURCE_DIR}/src/Utilities/Log.cpp

  #Headers

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

add_library(Core
  #Audio

  ${CMAKE_SOURCE_DIR}/src/Core/Audio/Audio.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/AudioListener.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/WavDecoder.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioSource.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/Audio.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/AudioListener.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioBuffer.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioData.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/CAudioSource.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/IAudioEffect.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/IBitstreamDecoder.h
  ${CMAKE_SOURCE_DIR}/src/Core/Audio/WavDecoder.h

  #FileSystem

  ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/CFile.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/FileSystem.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/CFile.h
  ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/EFile.h
  ${CMAKE_SOURCE_DIR}/src/Core/FileSystem/FileSystem.h

  #Graphics

  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/AnimatedSprite.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Camera.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Lighting.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Mesh.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/MeshObject.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Program.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Projection.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Shader.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Sprite.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/SpriteBatcher.cpp
  #${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextRenderer.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Texture.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/TextureUnit.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Transformation.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/VertexArray.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Graphics/Window.cpp

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

  #Input

  ${CMAKE_SOURCE_DIR}/src/Core/Input/Keyboard.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Input/Mouse.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Input/Mouse.h
  ${CMAKE_SOURCE_DIR}/src/Core/Input/Keyboard.h
  ${CMAKE_SOURCE_DIR}/src/Core/Input/Controller.h
  ${CMAKE_SOURCE_DIR}/src/Core/Input/UserPointers.h
  ${CMAKE_SOURCE_DIR}/src/Core/Input/Keys.h

  #Networking

  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CClientTCPConnection.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CConnection.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CNetwork.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CServerTCPConnection.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CTCPConnection.cpp
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/CTCPServer.cpp


  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnet.c
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetselect.c
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetTCP.c
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetUDP.c
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDL_net.h
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/begin_code.h
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/close_code.h
  ${CMAKE_SOURCE_DIR}/src/Core/Networking/SDL_net/SDLnetsys.h

  #OpenCL is delayed until later notice
)

add_executable(Test
  ${CMAKE_SOURCE_DIR}/src/Core_Test/TextureLoading.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Graphics/extern)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Core/Audio/openal)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Networking/SDL_net)

target_link_libraries(Core PUBLIC Utilities)
target_link_libraries(Test PUBLIC Core Utilities)
