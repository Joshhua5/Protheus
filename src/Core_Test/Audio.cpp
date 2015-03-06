
#include <Graphics\Window.h>
#include <Graphics\Sprite.h>
#include <Graphics\TextureLoader.h>
#include <Audio\WavDecoder.h>
#include <Audio\Audio.h>
#include <FileSystem\FileSystem.h>
#include <thread>
#include <Vector3.h> 

using namespace Pro;
using namespace Util;
using namespace Audio;
using namespace Math;

int main(int argc, char* args []) { 
	FileSystem fileIO;
	 
	auto audiof = fileIO.GetFile("test.wav");

	CAudio audio;
	CAudioListener listener;  
	 
	auto buffer = audio.loadAudio(AUDIO_FORMAT::WAV, &audiof);
	auto source = audio.createSource(buffer);  

	Vector3<float> pos;
	pos.x = pos.y = pos.z = 0;
	pos.z = 20;

	while (true) {
		this_thread::sleep_for(std::chrono::milliseconds(100));
		pos.x -= 4;
		if (pos.x == -100)
			pos.x = 100;
		source.position(pos); 
		source.play();
	} 
}