#include "Keyboard.h"

using namespace Pro;
using namespace Input;
using namespace Util;
 
void Keyboard::AttachWindow(std::shared_ptr<Util::Pipe<std::pair<KEY_PRESSED, KEY>>>& reference, bool* keys) {
	keyboard_key_ = reference;
	keys_ = keys;
    
}
 
bool Keyboard::hasKey() {
	return !keyboard_key_->Empty();
}

std::pair<KEY_PRESSED, KEY> Keyboard::PollKey() {
	if (!hasKey())
		return { KEY_PRESSED::UNKNOWN, KEY::KEY_UNKNOWN };
    std::pair<KEY_PRESSED, KEY> k;
    keyboard_key_->TopPop(k);
	return k;
}

std::pair<KEY_PRESSED, KEY> Keyboard::PeekKey() {
    if (!hasKey())
        return { KEY_PRESSED::UNKNOWN, KEY::KEY_UNKNOWN };
    std::pair<KEY_PRESSED, KEY> k;
    keyboard_key_->Top(k);
    return k;
}

bool Keyboard::IsKeyDown(KEY key) {
    return keys_[(int) key];
}

KEY_PRESSED Keyboard::KeyState(KEY key){
	return (keys_[(int)key]) ? KEY_PRESSED::PRESSED : KEY_PRESSED::RELEASED;
}