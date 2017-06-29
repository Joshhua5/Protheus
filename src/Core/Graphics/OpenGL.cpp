//
//  OpenGL.cpp
//  Protheus
//
//  Created by Joshua Waring on 23/03/2015.
//
//

#include "OpenGL.h"

using namespace Pro;
using namespace Graphics;

#ifdef PRO_DEBUG
bool Pro::Graphics::IsDebuggingEnabled = true;
#endif


static char debug_compatibility_check = 2; // 0 unsupported, 1 supported, 2 unrun
bool Pro::Graphics::GL_CONTEXT_CREATED = false;
 
std::string Pro::Graphics::glGetErrorString(GLint error){
    switch(error){
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "UNKNOWN_ERROR";
    }
}

std::pair<GLint, GLint> Pro::Graphics::GetOpenGLVersion(){
    static std::pair<GLint, GLint> version = {0,0};
    static bool first_run = true;
    if(first_run && GL_CONTEXT_CREATED){
        glGetIntegerv(GL_MAJOR_VERSION, &version.first);
        glGetIntegerv(GL_MINOR_VERSION, &version.second);
        first_run = false;
    }
    return version;
}

// Returns true if debug functionality is supported at runtime
bool CheckDebugCompatibility(){
    switch(debug_compatibility_check){
        case 2:
        {
            auto version = GetOpenGLVersion();
            if(version.first == 0)
                debug_compatibility_check = 2;
            if(version.first >= 5 || (version.first == 4 && version.second >= 3))
                debug_compatibility_check = 1;
            return debug_compatibility_check == 1;
        }
        case 1:
            return true;
    }
    return false;
}

void Pro::Graphics::PushDebugStack(const std::string& msg) {
#ifdef GL_VERSION_4_3
    static GLuint id = 0;
    static GLint max_length = 0;
    if (IsDebuggingEnabled == false)
        return;
    
    if(debug_compatibility_check != 1){
        if(debug_compatibility_check == 0)
            return;
        if(CheckDebugCompatibility() == false)
            return;
        // We have compatibility
        // So we should get the max length here
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &max_length);
    }
    
    if((unsigned)max_length <= msg.size()){
        Pro::global_log.Report<LogCode::FAULT>("Unable to pass debug stack, Message too long.\n" + msg, __FUNCTION__, __LINE__);
        return;
    }
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION,
                     id++,
                     msg.size(),
                     msg.data());
#endif
}

void Pro::Graphics::PopDebugStack() {
#ifdef GL_VERSION_4_3
    if (IsDebuggingEnabled == false)
        return;
    
    if(debug_compatibility_check != 1){
        if(debug_compatibility_check == 0)
            return;
        if(CheckDebugCompatibility() == false)
            return;
    }
    
    glPopDebugGroup();
#endif
}