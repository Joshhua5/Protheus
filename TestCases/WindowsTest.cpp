#include <Graphics/Window.h>

using namespace Pro;
using namespace Graphics;

int main(){
    global_log.EchoOnConsole(true);
    char exit_status = 0;
    {
      // Basic Test
      Graphics::Window window("Test", {800, 600});
      if(window.IsOpen() == false)
        exit_status |= 1;
    }
    {
        // Test default window creation
        Graphics::Window window("Window Test", {800, 600});
        window.Focus();
        window.Resize({400, 300});
        window.Resize({800, 600});
        if(window.IsOpen() == false){
            exit_status |= 2;
            global_log.Report<LogCode::FAULT>("Window.IsOpen returned false",
             __FUNCTION__, __LINE__);
        }

        // Testing window renaming
        window.Rename("Renamed");
    }
    return exit_status;
}
