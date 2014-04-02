#include "CST_Input.h"
#include <SDL.h>  

namespace CST{



	struct _CST_Controller{
		SDL_GameController* controller;
	};

	CST_Controller* CST_GetController(){
		CST_Controller* out = new CST_Controller();
		int count = SDL_NumJoysticks();
		for (int i = 0; i < count; i++){
			if (SDL_IsGameController(i)){
				out->controller = SDL_GameControllerOpen(i);
				return out;
			}
		}
		return nullptr;
	}

	void CST_GetControllerLStick(CST_Controller *device, double* x, double *y){
		SDL_GameControllerUpdate();
		if (x != nullptr)
			*x = (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_LEFTX) / 32767.0;
		if (y != nullptr)
			*y = (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_LEFTY) / 32767.0;

	}

	void CST_GetControllerRStick(CST_Controller *device, double *x, double *y){
		SDL_GameControllerUpdate();
		if (x != nullptr)
			*x = (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0;
		if (y != nullptr)
			*y = (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0;

	}

	double CST_GetControllerLTrigger(CST_Controller *device){
		SDL_GameControllerUpdate();
		return (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0;
	}
	double CST_GetControllerRTrigger(CST_Controller *device){
		SDL_GameControllerUpdate();
		return (double) SDL_GameControllerGetAxis(device->controller,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0;
	}

	bool CST_GetControllerButton(CST_Controller *device, CST_CONTROLLER_BUTTON button){
		SDL_GameControllerUpdate();
		if (SDL_GameControllerGetButton(device->controller,
			(SDL_GameControllerButton) button))
			return true;
		return false;
	}

	void CST_CloseController(CST_Controller* device){
		SDL_GameControllerClose(device->controller);
		delete device;
	}

	bool CST_GetMouseButton(unsigned short delay, CST_MOUSE_BUTTON button){
		static unsigned int lastClicked[3] = { 0, 0, 0 };
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON((int) button) &&
			lastClicked[(int) button - 1] + delay <= SDL_GetTicks()){

			lastClicked[(int) button - 1] = SDL_GetTicks();
			return true;
		}
		return false;
	}

	void CST_GetMousePosition(int *x, int *y){
		SDL_GetMouseState(x, y);
	}


	CST_PressedKeys CST_GetKeyboardKeys(unsigned int delay){
		CST_PressedKeys out;
		SDL_PumpEvents();
		static const Uint8 *keys = SDL_GetKeyboardState(NULL);
		static unsigned int lastClicked[285];
		// Test is the keys pointer to the SDL buffer, and self updating 
		for (int x = 0; x < 285; x++){
			if (keys[x] && lastClicked[x] + delay <= SDL_GetTicks()){
				out.push_back((CST_KEYBOARD_KEY) x);
				lastClicked[x] = SDL_GetTicks();
			}
		}
		return out;
	}

}