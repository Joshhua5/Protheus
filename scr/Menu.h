#pragma once   
#include "CST_Input.h"
#include <vector>

enum struct MenuType{
	MAIN, 
	MULTI_SERVER_CLIENT_CHOICE,
	MULTI_SERVER_WAIT,
	MULTI_CLIENT_IP_REQUEST,
	MULTI_CLIENT_WAIT,
	EXIT,
	SINGLE

};

enum struct GameType{
	Menu,
	SinglePlayer,
	MultiPlayer,
	MULTI_SERVER_WAIT,
	MULTI_CLIENT_WAIT,
	Exit
}; 
struct MultiPlayerInfo{

};

struct Button{
	Button(){
		texture = nullptr;
	}
	SDL_Texture* texture;
	int x, y;
	int texWidth, texHeight;
	bool isClicked(int x, int y);
	void setPosition(SDL_Window *window, float offset_y);
	void setup(SDL_Window *window, float offset_y, SDL_Texture *texture, MenuType type, MenuType state);
	MenuType type;
	MenuType state;
};

class Menu
{
private: 
	bool initialized = false;
	std::vector<Button> elements;
	MenuType state = MenuType::MAIN; 
	std::vector<char> ipInput;
public:
	Menu();
	~Menu();
	void setup(SDL_Renderer*, SDL_Window*);
	char* getAddress();
	GameType update();
	void render(SDL_Renderer*);
	void cleanup();
	MenuType getState();
	bool hasInitialized();
};

// Three buttons, Exit, SinglePlayer, Multiplayer