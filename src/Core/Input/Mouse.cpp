#include "Mouse.h"

using namespace Pro;
using namespace Input;
using namespace Util;
using namespace Math;

void Mouse::AttachWindow(std::shared_ptr<Pipe<MouseEvent>>& reference, Vector2<int>* position) {
	mouse_keys_ = reference;
	position_ = position;
}

Vector2<int> Mouse::position() {
    return *position_;
}

MouseEvent Mouse::GetMouseKey() {
	if (!HasMouseKey())
        return MouseEvent();
    MouseEvent k;
    if(!mouse_keys_->TopPop(k)){
        k.state = KEY_PRESSED::UNKNOWN;
        k.key = KEY::KEY_UNKNOWN;
        k.position = Vector2<int>(0, 0);
    }
	return k;
}

bool Mouse::HasMouseKey(){
	return !mouse_keys_->Empty();
}

