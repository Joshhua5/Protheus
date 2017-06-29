#pragma once

#include <Vector2.h>
#include "Keys.h"
#include <atomic>
#include "MouseEvent.h"
#include <Pipe.h>

namespace Pro {
	namespace Input {
		class Mouse {
            std::shared_ptr<Util::Pipe<MouseEvent>> mouse_keys_;
            Math::Vector2<int>* position_;
		public:
            Math::Vector2<int> position();

            //! Sets vector as the position of the mouse
			template<typename T>
            void position(Math::Vector2<T>& vector);

			MouseEvent GetMouseKey();
			//! Returns true if new key events are avaliable
			bool HasMouseKey();

            void AttachWindow(std::shared_ptr<Util::Pipe<MouseEvent>>& reference, Math::Vector2<int>* keys);
		};

		template<typename T>
        void Mouse::position(Math::Vector2<T>& vector) {
			vector = position_->Cast<T>();
		}
	}
}
