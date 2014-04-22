
#include "Menu.h"


Menu::Menu()
{
}

bool Button::isClicked(int mouse_x, int mouse_y){
	if (mouse_x >= x && x <= mouse_x + texWidth)
	if (mouse_y >= y && mouse_y <= y + texHeight)
		return true;
	return false;
}

Menu::~Menu()
{
}

void Menu::setup(SDL_Renderer* sdl, SDL_Window* window){
	SDL_Color color;
	color.b = 0;
	color.g = 0;
	color.r = 0;
	color.a = 0;

	textures.add(loadText("Enter IP: ", 24, color, sdl), "request_ip");
	textures.add(loadText("Waiting for client to join", 24, color, sdl), "server_wait");
	textures.add(loadText("Connecting to server", 24, color, sdl), "client_wait");
	textures.add(loadTexture("textures\\MenuBackground.bmp", sdl), "background");

	Button Single, Multi, Exit, CreateServer, JoinServer, Join, Back;

	Single.setup(window, 3,
		textures.add(loadText("SinglePlayer", 24, color, sdl), "singleplayer"),
		MenuType::SINGLE,
		MenuType::MAIN);
	Multi.setup(window, 2,
		textures.add(loadText("MultiPlayer", 24, color, sdl), "multiplayer"),
		MenuType::MULTI_SERVER_CLIENT_CHOICE,
		MenuType::MAIN);
	Exit.setup(window, 1.5f,
		textures.add(loadText("Exit", 24, color, sdl), "exit"),
		MenuType::EXIT,
		MenuType::MAIN);
	CreateServer.setup(window, 1.5f,
		textures.add(loadText("Create Server", 24, color, sdl), "create_server"),
		MenuType::MULTI_SERVER_WAIT,
		MenuType::MULTI_SERVER_CLIENT_CHOICE);
	JoinServer.setup(window, 3,
		textures.add(loadText("Join Server", 24, color, sdl), "join_client"),
		MenuType::MULTI_CLIENT_IP_REQUEST,
		MenuType::MULTI_SERVER_CLIENT_CHOICE);
	Join.setup(window, 2,
		textures.add(loadText("Join", 24, color, sdl), "join"),
		MenuType::MULTI_CLIENT_WAIT,
		MenuType::MULTI_CLIENT_IP_REQUEST);
	Back.setup(window, 1,
		textures.add(loadText("Back", 24, color, sdl), "back"),
		MenuType::EXIT,
		MenuType::MAIN);

	elements.push_back(Single);
	elements.push_back(Multi);
	elements.push_back(Exit);
	elements.push_back(CreateServer);
	elements.push_back(JoinServer);
	elements.push_back(Join);
	elements.push_back(Back);
	initialized = true;
}


bool hasClicked(int *mouse_x, int *mouse_y){
	static unsigned long lastClick = SDL_GetTicks();
	if (lastClick + 1000 <= SDL_GetTicks() &&
		SDL_GetMouseState(mouse_x, mouse_y)&SDL_BUTTON_LMASK){
		lastClick = SDL_GetTicks();
		return true;
	}
	return false;
}

void Button::setup(SDL_Window *window, float offset_y, SDL_Texture *texture, MenuType type, MenuType state){
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	SDL_QueryTexture(texture, NULL, NULL, &texWidth, &texHeight);
	x = (int)(width - texWidth) / 4;
	y = (int)((height - texHeight) / offset_y);

	this->texture = texture;
	this->type = type;
	this->state = state;
}

void Menu::render(SDL_Renderer* renderer){
	SDL_Texture* tex;
	static SDL_Texture* background = textures.get("background");
	renderTexture(background, renderer, 0, 0);
	switch (state){
	case MenuType::MAIN:
		for each(const Button obj in elements){
			if (obj.state == MenuType::MAIN)
				renderTexture(obj.texture, renderer, obj.x, obj.y);
		}
		break;
	case MenuType::MULTI_SERVER_CLIENT_CHOICE:
		for each(const Button obj in elements){
			if (obj.state == MenuType::MULTI_SERVER_CLIENT_CHOICE)
				renderTexture(obj.texture, renderer, obj.x, obj.y);
		}
		break;

	case MenuType::MULTI_SERVER_WAIT:
		renderTexture(textures.get("server_wait"), renderer, 100, 200);
		break;
	case MenuType::MULTI_CLIENT_IP_REQUEST:
		renderTexture(textures.get("request_ip"), renderer, 100, 200);
		for each(Button obj in elements){
			if (obj.state == MenuType::MULTI_CLIENT_IP_REQUEST)
				renderTexture(obj.texture, renderer, obj.x, obj.y);
		}
		// highly inefficient, but easy -to be re worked-
		if (!ipInput.empty()){
			tex = loadText(&ipInput[0], 23, { 0, 0, 0, 0 }, renderer);
			renderTexture(tex, renderer, 100, 250);
			SDL_DestroyTexture(tex);
		}
		break;
	case MenuType::MULTI_CLIENT_WAIT:
		renderTexture(textures.get("client_wait"), renderer, 100, 200);
		break;
	}
}


GameType Menu::update(){
	int mouseX, mouseY;
	switch (state){
	case MenuType::MAIN:
		if (hasClicked(&mouseX, &mouseY)){
			for each(Button obj in elements){
				if (obj.state == MenuType::MAIN && obj.isClicked(mouseX, mouseY)){
					if (obj.type == MenuType::EXIT)
						return GameType::Exit;
					else if (obj.type == MenuType::SINGLE)
						return GameType::SinglePlayer;
					else
						state = obj.type;
				}
			}
		}
		break;
	case MenuType::MULTI_SERVER_CLIENT_CHOICE:
		if (hasClicked(&mouseX, &mouseY)){
			for each(Button obj in elements){
				if (obj.state == MenuType::MULTI_SERVER_CLIENT_CHOICE &&
					obj.isClicked(mouseX, mouseY)){
					state = obj.type;
				}
			}
		}
		break;
	case MenuType::MULTI_SERVER_WAIT:
		return GameType::MULTI_SERVER_WAIT;
		break;
	case MenuType::MULTI_CLIENT_WAIT:
		return GameType::MULTI_CLIENT_WAIT;
	case MenuType::MULTI_CLIENT_IP_REQUEST: 
		CST_PressedKeys keys = CST_GetKeyboardKeys(100);
		for each(CST_KEYBOARD_KEY obj in keys){ 
			if (obj != CST_KEY_BACKSPACE){
				const char *out = SDL_GetScancodeName((SDL_Scancode)obj);
				ipInput.push_back(out[0]);
			}
			else{
				if (!ipInput.empty()){
					ipInput[ipInput.size() - 1] = '\0';
					ipInput.erase(ipInput.begin() + ipInput.size() - 1);
				}
			}
		}

		if (hasClicked(&mouseX, &mouseY)){
			for each(Button obj in elements){
				if (obj.state == MenuType::MULTI_CLIENT_IP_REQUEST &&
					obj.isClicked(mouseX, mouseY)){
					state = obj.type;
				}
			}
		} 
		break;
	}
	// Get Mouse Position and Click status
	return GameType::Menu;
}

void Menu::cleanup(){
	initialized = false;
	textures.clean();
}

bool Menu::hasInitialized(){
	return initialized;
}

MenuType Menu::getState(){
	return state;
}

char* Menu::getAddress(){
	ipInput.push_back('\0');
	return &ipInput[0];

}