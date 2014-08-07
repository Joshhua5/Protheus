/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores which key was pressed in a keyboard event
-------------------------------------------------------------------------
History:
- 10:06:2014: Waring J.
*************************************************************************/
#pragma once

namespace Pro{
	namespace Event{
		enum struct EKeyboardKey{
			UNKNOWN = 0,
			a = 'a',
			b = 'b',
			c = 'c',
			d = 'd',
			e = 'e',
			f = 'f',
			g = 'g',
			h = 'h',
			i = 'i',
			j = 'j',
			k = 'k',
			l = 'l',
			m = 'm',
			n = 'n',
			o = 'o',
			p = 'p',
			q = 'q',
			r = 'r',
			s = 's',
			t = 't',
			u = 'u',
			v = 'v',
			w = 'w',
			x = 'x',
			y = 'y',
			z = 'z',
			A = 'A',
			B = 'B',
			C = 'C',
			D = 'D',
			E = 'E',
			F = 'F',
			G = 'G',
			H = 'H',
			I = 'I',
			J = 'J',
			K = 'K',
			L = 'L',
			M = 'M',
			N = 'N',
			O = 'O',
			P = 'P',
			Q = 'Q',
			R = 'R',
			S = 'S',
			T = 'T',
			U = 'U',
			V = 'V',
			W = 'W',
			X = 'X',
			Y = 'Y',
			Z = 'Z',
			ARROW_UP,
			ARROW_DOWN,
			ARROW_LEFT,
			ARROW_RIGHT,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			HOME,
			INSERT,
			PAGE_DOWN,
			PAGE_UP,
			DELETE = 'DEL',
			SPACE = ' ',
			_1 = '1',
			_2 = '2',
			_3 = '3',
			_4 = '4',
			_5 = '5',
			_6 = '6',
			_7 = '7',
			_8 = '8',
			_9 = '9',
			_0 = '0',
			KEYPAD_1,
			KEYPAD_2,
			KEYPAD_3,
			KEYPAD_4,
			KEYPAD_5,
			KEYPAD_6,
			KEYPAD_7,
			KEYPAD_8,
			KEYPAD_9,
			KEYPAD_0,
			KEYPAD_PLUS,
			KEYPAD_MINUS,
			KEYPAD_ENTER,
			KEYPAD_ASTERISK,
			KEYPAD_DIVIDE,
			KEYPAD_FULLSTOP,
			KEYPAD_DELETE,
			EXCLAMANATION_MARK,
			AT = '@',
			HASH = '#',
			DOLLAR = '$',
			PERCENT = '%',
			CARNET = '^', // ^
			AMPERSAND = '&',
			ASTERISK = '^',
			LEFT_PARENTHESE = '(',
			RIGHT_PARENTHESE = ')',
			UNDERSCORE = '_',
			EQUALS = '=',
			PLUS = '+',
			MINUS = '-',
			TAB = '\t',
			CAPS_LOCK,
			SHIFT,
			CTRL,
			ALT,
			TILDE = '~',
			COMMA = ',',
			GRAVE_ACCENT = '`',
			QUOTATION_MARK = '"',
			PAUSE_BREAK,
			SCREEN_LOCK,
			PRINT_SCREEN,
			LEFT_BRACKET = '[',
			RIGHT_BRACKET = ']',
			LEFT_CURLY_BRACKET = '{',
			RIGHT_CURLY_BRACKET = '}',
			FORWARD_SLASH = '/',
			BACKWARD_SLASH = '\\',
			VERTICAL_BAR = '|',
			QUESTION_MARK = '?',
			LESS_THAN = '<',
			GREATER_THAN = '>',
			SEMI_COLON = ';',
			COLON = ':',
			APOSTROPHE = '\'',
			FULLSTOP = '.',
		};

		EKeyboardKey inline toKeyboardKey(const SDL_Scancode& key){
			return static_cast<EKeyboardKey>(SDL_GetKeyFromScancode(key)); 
		}

		static void defineKeyTable(lua_State* L){
			vector<pair<const string, double>> values(255);
			values.push_back({ "a", (double)EKeyboardKey::a });
			values.push_back({ "b", (double)EKeyboardKey::b });
			values.push_back({ "c", (double)EKeyboardKey::c });
			values.push_back({ "d", (double)EKeyboardKey::d });
			values.push_back({ "e", (double)EKeyboardKey::e });
			values.push_back({ "f", (double)EKeyboardKey::f });
			values.push_back({ "g", (double)EKeyboardKey::g });
			values.push_back({ "h", (double)EKeyboardKey::h });
			values.push_back({ "i", (double)EKeyboardKey::i });
			values.push_back({ "j", (double)EKeyboardKey::j });
			values.push_back({ "k", (double)EKeyboardKey::k });
			values.push_back({ "l", (double)EKeyboardKey::l });
			values.push_back({ "m", (double)EKeyboardKey::m });
			values.push_back({ "n", (double)EKeyboardKey::n });
			values.push_back({ "o", (double)EKeyboardKey::o });
			values.push_back({ "p", (double)EKeyboardKey::p });
			values.push_back({ "q", (double)EKeyboardKey::q });
			values.push_back({ "r", (double)EKeyboardKey::r });
			values.push_back({ "s", (double)EKeyboardKey::s });
			values.push_back({ "t", (double)EKeyboardKey::t });
			values.push_back({ "u", (double)EKeyboardKey::u });
			values.push_back({ "v", (double)EKeyboardKey::v });
			values.push_back({ "w", (double)EKeyboardKey::w });
			values.push_back({ "x", (double)EKeyboardKey::x });
			values.push_back({ "y", (double)EKeyboardKey::y });
			values.push_back({ "z", (double)EKeyboardKey::z });
			 
			values.push_back({ "A", (double)EKeyboardKey::A });
			values.push_back({ "B", (double)EKeyboardKey::B });
			values.push_back({ "C", (double)EKeyboardKey::C });
			values.push_back({ "D", (double)EKeyboardKey::D });
			values.push_back({ "E", (double)EKeyboardKey::E });
			values.push_back({ "F", (double)EKeyboardKey::F });
			values.push_back({ "G", (double)EKeyboardKey::G });
			values.push_back({ "H", (double)EKeyboardKey::H });
			values.push_back({ "I", (double)EKeyboardKey::I });
			values.push_back({ "J", (double)EKeyboardKey::J });
			values.push_back({ "K", (double)EKeyboardKey::K });
			values.push_back({ "L", (double)EKeyboardKey::M });
			values.push_back({ "M", (double)EKeyboardKey::M });
			values.push_back({ "N", (double)EKeyboardKey::N });
			values.push_back({ "O", (double)EKeyboardKey::O });
			values.push_back({ "P", (double)EKeyboardKey::P });
			values.push_back({ "Q", (double)EKeyboardKey::Q });
			values.push_back({ "R", (double)EKeyboardKey::R });
			values.push_back({ "S", (double)EKeyboardKey::S });
			values.push_back({ "T", (double)EKeyboardKey::T });
			values.push_back({ "U", (double)EKeyboardKey::U });
			values.push_back({ "V", (double)EKeyboardKey::V });
			values.push_back({ "W", (double)EKeyboardKey::W });
			values.push_back({ "X", (double)EKeyboardKey::X });
			values.push_back({ "Y", (double)EKeyboardKey::Y });
			values.push_back({ "Z", (double)EKeyboardKey::Z });
			 
			values.push_back({ "ARROW_DOWN",  (double)EKeyboardKey::ARROW_DOWN});
			values.push_back({ "ARROW_LEFT",  (double)EKeyboardKey::ARROW_LEFT });
			values.push_back({ "ARROW_RIGHT", (double)EKeyboardKey::ARROW_RIGHT });
			values.push_back({ "ARROW_UP",    (double)EKeyboardKey::ARROW_UP });

			values.push_back({ "F1", (double)EKeyboardKey::F1 });
			values.push_back({ "F2", (double)EKeyboardKey::F2 });
			values.push_back({ "F3", (double)EKeyboardKey::F3 });
			values.push_back({ "F4", (double)EKeyboardKey::F4 });
			values.push_back({ "F5", (double)EKeyboardKey::F5 });
			values.push_back({ "F6", (double)EKeyboardKey::F6 });
			values.push_back({ "F7", (double)EKeyboardKey::F7 });
			values.push_back({ "F8", (double)EKeyboardKey::F8 });
			values.push_back({ "F9", (double)EKeyboardKey::F9 });
			values.push_back({ "F10", (double)EKeyboardKey::F10 });
			values.push_back({ "F11", (double)EKeyboardKey::F11 });
			values.push_back({ "F12", (double)EKeyboardKey::F12 });

			values.push_back({ "HOME",		(double)EKeyboardKey::HOME });
			values.push_back({ "INSERT",	(double)EKeyboardKey::INSERT });
			values.push_back({ "PAGE_DOWN", (double)EKeyboardKey::PAGE_DOWN });
			values.push_back({ "PAGE_UP",	(double)EKeyboardKey::PAGE_UP });
			values.push_back({ "DELETE",	(double)EKeyboardKey::DELETE });
			values.push_back({ "SPACE",		(double)EKeyboardKey::SPACE }); 
			values.push_back({ "TAB",		(double) EKeyboardKey::TAB });

			values.push_back({ "1", (double)EKeyboardKey::_1 });
			values.push_back({ "2", (double)EKeyboardKey::_2 });
			values.push_back({ "3", (double)EKeyboardKey::_3 });
			values.push_back({ "4", (double)EKeyboardKey::_4 });
			values.push_back({ "5", (double)EKeyboardKey::_5 });
			values.push_back({ "6", (double)EKeyboardKey::_6 });
			values.push_back({ "7", (double)EKeyboardKey::_7 });
			values.push_back({ "8", (double)EKeyboardKey::_8 });
			values.push_back({ "9", (double)EKeyboardKey::_9 });
			 

			Util::luaP_pusharray<double>(L, values);
			lua_setglobal(L, "key");
		}
	}
}
