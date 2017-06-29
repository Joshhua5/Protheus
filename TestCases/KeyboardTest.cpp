#include <Input/Keyboard.h>
#include <Graphics/Window.h>
#include <chrono>
#include <thread>

using namespace Pro;
using namespace Graphics;
using namespace Input;

int main(){
    global_log.EchoOnConsole(true);
    int exit_status = 0;
    {
        Window wnd("Input Test", {800, 600});
        wnd.CurrentContext();
        wnd.Focus();
        Keyboard key_input = wnd.keyboard();
        
        std::cout << "Please press 'c' to continue" << std::endl;
        
        while(!key_input.IsKeyDown(KEY::KEY_C)){
            wnd.Update();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        std::cout << "Success" << std::endl;
        std::cout << "Testing event based input, Please press 'd' to continue" << std::endl;
        // Event based input
        
        while(key_input.PollKey().second != KEY::KEY_D){
            wnd.Update();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        std::cout << "Success" << std::endl;
        std::cout << "Now entering echo mode, press esc to quit" << std::endl;
        
        while(key_input.PeekKey().second != KEY::KEY_ESCAPE){
            wnd.Update();
            auto key = key_input.PollKey();
            if(key.second != KEY::KEY_UNKNOWN)
                std::cout << std::to_string((int)key.second) << std::endl;
        }
    }
    return exit_status;
}