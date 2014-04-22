TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += \
    ../BombMan/EntryPoint.cpp \
    ../BombMan/Game.cpp \
    ../BombMan/Menu.cpp \
    ../BombMan/Engine/AnimatedEntity.cpp \
    ../BombMan/Engine/AnimatedSprite.cpp \
    ../BombMan/Engine/Avatar.cpp \
    ../BombMan/Engine/Camera.cpp \
    ../BombMan/Engine/Core.cpp \
    ../BombMan/Engine/Entity.cpp \
    ../BombMan/Engine/EventHandeler.cpp \
    ../BombMan/Engine/GUI.cpp \
    ../BombMan/Engine/GUIButton.cpp \
    ../BombMan/Engine/GUICollapsibleMenu.cpp \
    ../BombMan/Engine/GUIContext.cpp \
    ../BombMan/Engine/GUIEditor.cpp \
    ../BombMan/Engine/GUIEntity.cpp \
    ../BombMan/Engine/GUIEvent.cpp \
    ../BombMan/Engine/GUILabel.cpp \
    ../BombMan/Engine/GUISlider.cpp \
    ../BombMan/Engine/GUIText.cpp \
    ../BombMan/Engine/GUIWindow.cpp \
    ../BombMan/Engine/IDManager.cpp \
    ../BombMan/Engine/Input.cpp \
    ../BombMan/Engine/Inventory.cpp \
    ../BombMan/Engine/Item.cpp \
    ../BombMan/Engine/Map.cpp \
    ../BombMan/Engine/Name.cpp \
    ../BombMan/Engine/Network.cpp \
    ../BombMan/Engine/Renderer.cpp \
    ../BombMan/Engine/Scene.cpp \
    ../BombMan/Engine/Sprite.cpp \
    ../BombMan/Engine/SpriteEntity.cpp \
    ../BombMan/Engine/SpriteManager.cpp \
    ../BombMan/Engine/Tile.cpp \
    ../BombMan/Engine/Timer.cpp \
    ../BombMan/Engine/Window.cpp
HEADERS += \
    ../BombMan/Game.h \
    ../BombMan/Menu.h \
    ../BombMan/Engine/AnimatedEntity.h \
    ../BombMan/Engine/AnimatedSprite.h \
    ../BombMan/Engine/Avatar.h \
    ../BombMan/Engine/Camera.h \
    ../BombMan/Engine/Core.h \
    ../BombMan/Engine/Entity.h \
    ../BombMan/Engine/EventHandeler.h \
    ../BombMan/Engine/GUI.h \
    ../BombMan/Engine/GUIButton.h \
    ../BombMan/Engine/GUICollapsibleMenu.h \
    ../BombMan/Engine/GUIContext.h \
    ../BombMan/Engine/GUIEditor.h \
    ../BombMan/Engine/GUIEntity.h \
    ../BombMan/Engine/GUIEvent.h \
    ../BombMan/Engine/GUILabel.h \
    ../BombMan/Engine/GUISlider.h \
    ../BombMan/Engine/GUIText.h \
    ../BombMan/Engine/GUIWindow.h \
    ../BombMan/Engine/IDManager.h \
    ../BombMan/Engine/Input.h \
    ../BombMan/Engine/Inventory.h \
    ../BombMan/Engine/Item.h \
    ../BombMan/Engine/Map.h \
    ../BombMan/Engine/Name.h \
    ../BombMan/Engine/Network.h \
    ../BombMan/Engine/Renderer.h \
    ../BombMan/Engine/Scene.h \
    ../BombMan/Engine/Sprite.h \
    ../BombMan/Engine/SpriteEntity.h \
    ../BombMan/Engine/SpriteManager.h \
    ../BombMan/Engine/Tile.h \
    ../BombMan/Engine/Timer.h \
    ../BombMan/Engine/Window.h
unix|win32: LIBS += -lSDL2
unix|win32: LIBS += -lSDL2main
