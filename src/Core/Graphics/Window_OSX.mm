//
//  Window_OSX.m
//  Protheus
//
//  Created by Joshua Waring on 22/03/2015.
//
//

#import "Window.h"
#import <Cocoa/Cocoa.h>

@interface PRApplication : NSApplication
{
@public
    Pro::Graphics::Window* wnd;
}

-(void)sendEvent:(NSEvent *)theEvent;
@end


@implementation PRApplication-(void)sendEvent:(NSEvent *)event{
    switch(event.type){
        case NSKeyDown:
            wnd->RegisterKeyboardEvent(event.keyCode, true);
            break;
        case NSKeyUp:{
            wnd->RegisterKeyboardEvent(event.keyCode, false);
            break;
        }
        //case NSFlagsChanged:
        case NSLeftMouseDown:{
            // 0x8x translates to mouse key's defined in KEY::
            wnd->RegisterMouseEvent(0x80, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , false);
            break;
        }
        case NSLeftMouseUp:{
            wnd->RegisterMouseEvent(0x80, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , true);
            break;
        }
        case NSLeftMouseDragged:
        case NSRightMouseDown:{
            wnd->RegisterMouseEvent(0x81, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , false);
            break;
        }
        case NSRightMouseUp:{
            wnd->RegisterMouseEvent(0x81, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , true);
            break;
        }
        case NSRightMouseDragged:
        case NSOtherMouseDown:{
            wnd->RegisterMouseEvent(0x82, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , false);
            break;
        }
        case NSOtherMouseDragged:
        case NSOtherMouseUp:{
            wnd->RegisterMouseEvent(0x82, [NSEvent mouseLocation].x, [NSEvent mouseLocation].y , true);
            break;
        }
            /*
        case NSMouseMoved:
        case NSMouseEntered:
        case NSMouseExited:
        case NSAppKitDefined:
        case NSApplicationDefined:
        case NSSystemDefined:
        case NSPeriodic:
        case NSCursorUpdate:
        case NSEventTypeRotate:
        case NSEventTypeBeginGesture:
        case NSEventTypeEndGesture:
        case NSEventTypeGesture:
        case NSEventTypeMagnify:
        case NSEventTypeQuickLook:
        case NSEventTypeSwipe:
        case NSScrollWheel:
        case NSTabletPoint:
        //case NSEventTypePressure:
        case NSTabletProximity:
            break;
             */
        default:
            [super sendEvent:event];
            break;
    }
    [event release];
}
@end

@interface ContentWindow : NSOpenGLContext


@end

static NSOpenGLPixelFormatAttribute glAttributes[] = {
    NSOpenGLPFADoubleBuffer, YES,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAStencilSize, 8,
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
    0 // 0 Terminated ArrayQ
};

struct CocoaWindow CreateCocoaWindow(Pro::Graphics::Window* proWindow, int width, int height, int x, int y,const char* title, unsigned char title_length, bool fullscreen){
    struct CocoaWindow cocoa_window;

    PRApplication *application = (PRApplication*)[PRApplication sharedApplication];
    //[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:application topLevelObjects:&tl];
    application->wnd = proWindow;

    [application activateIgnoringOtherApps:YES];

    int style = 0;
    style |= NSTitledWindowMask;
    style |= NSClosableWindowMask;
    style |= NSMiniaturizableWindowMask;
    style |= NSResizableWindowMask;
    if(fullscreen)
        style |= NSFullScreenWindowMask;

    NSRect frame = NSMakeRect(x, y, width, height);
    NSWindow* window = [[NSWindow alloc]
                        initWithContentRect:frame
                        styleMask:style
                        backing:NSBackingStoreBuffered defer:NO];

    NSString *nstitle = [[NSString alloc] initWithUTF8String:title];
    [window setTitle:nstitle];
    [window setBackgroundColor:[NSColor blackColor]];
    [window makeKeyAndOrderFront:application];

    NSOpenGLPixelFormat *pixelFormat =
        [[NSOpenGLPixelFormat alloc] initWithAttributes:glAttributes];

    NSOpenGLContext *glContext = [[NSOpenGLContext alloc]
                                  initWithFormat:pixelFormat shareContext:nil];

    [glContext setView:[window contentView]];
    [glContext makeCurrentContext];

    //Test if window can become key
    if([window canBecomeKeyWindow] == YES)
        [window makeKeyWindow];
    else
        Pro::global_log.Report<Pro::LogCode::FATAL>("Unable to create main window", __FUNCTION__, __LINE__);
    if([window canBecomeMainWindow] == YES)
        [window makeMainWindow];
    else
        Pro::global_log.Report<Pro::LogCode::FATAL>("Unable to create key window", __FUNCTION__, __LINE__);


    // Pack variables
    cocoa_window.window = (void*)window;
    cocoa_window.glContext = (void*)glContext;
    cocoa_window.App = (void*)application;

    Pro::Graphics::GL_CONTEXT_CREATED = true;

    // Set as a forground application
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);

    [application finishLaunching];

    return cocoa_window;
}

bool ResizeCocoaWindow(struct CocoaWindow* window, int width, int height){
    NSWindow* wnd =  (__bridge NSWindow * )window->window;
    if(!wnd.isResizable)
        return false;
    NSRect frame = wnd.frame;
    frame.size.width = width;
    frame.size.height = height;
    [wnd setFrame:frame display:YES];

    return true;
}

void SetBackgroundColour(struct CocoaWindow* window, unsigned char r,unsigned char g,unsigned char b, unsigned char a){
    CGFloat red = r / 255.0;
    CGFloat green = g / 255.0;
    CGFloat blue = b / 255.0;
    CGFloat alpha = a / 255.0;
    [(__bridge NSWindow * )window->window setBackgroundColor:[NSColor colorWithCalibratedRed:red green:green blue:blue alpha:alpha]];
}


void MakeCurrentContext(struct CocoaWindow* window){
    [CFBridgingRelease(window->glContext) makeCurrentContext];
}


void SwapBuffer(struct CocoaWindow* window){
    [(__bridge NSOpenGLContext * )window->glContext flushBuffer];
}

void ReleaseCocoaWindow(struct CocoaWindow* window){
    NSWindow *wnd = (__bridge NSWindow * )(window->window);
    NSOpenGLContext *glc = (__bridge NSOpenGLContext * )(window->glContext);

    [glc release];
    [wnd release];
    Pro::Graphics::GL_CONTEXT_CREATED = false;
    window->window = nullptr;
    window->glContext = nullptr;
}

void CocoaSetCurrent(struct CocoaWindow* window){
    NSWindow *wnd = (__bridge NSWindow * )(window->window);
    [wnd makeKeyWindow];
}

void CocoaUpdateWindow(struct CocoaWindow* window){
    PRApplication* app = (__bridge PRApplication * )(window->App);

    NSEvent *event = nil;
    [app setWindowsNeedUpdate:true];

    do{
        event = [app nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
        if(event != nil)
            [app sendEvent:event];
    }while(event != nil);

    [app updateWindows];
}

void CocoaRenameWindow(struct CocoaWindow* window, const char* name){
    NSWindow *wnd = (__bridge NSWindow * )(window->window);
    NSString *title = [[NSString alloc] initWithUTF8String:name];
    [wnd setTitle:title];
}
