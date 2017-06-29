#pragma once


#include <Vector2.h>
#include <Pipe.h>
#include "Keys.h"

namespace Pro {
	namespace Input {
		//! Keyboards data comes from the Window Class and therefor captures no data,
		//! until a window has been bound.
		class Keyboard {
			//! Reference to a pointer inside Window
            std::shared_ptr<Util::Pipe<std::pair<KEY_PRESSED, KEY>>> keyboard_key_;
			//! Reference to key states inside Window
			bool* keys_;
		public:
			bool hasKey();

            //! Returns the next key pressed
			std::pair<KEY_PRESSED, KEY> PollKey();
            //! Returns the next key pressed, without removing it
            std::pair<KEY_PRESSED, KEY> PeekKey();
			
			bool IsKeyDown(KEY);

			KEY_PRESSED KeyState(KEY);

			// depreciated
            void AttachWindow(std::shared_ptr<Util::Pipe<std::pair<KEY_PRESSED, KEY>>>& reference, bool* keys);
		};
	}
}