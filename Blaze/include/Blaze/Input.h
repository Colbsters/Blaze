#pragma once

#ifndef BLAZE_INPUT_H
#define BLAZE_INPUT_H

#include <Blaze/Core.h>
#include <Blaze/Object.h>
#include <Blaze/Window.h>

#include <cstdint>
#include <bitset>

namespace Blaze
{
	enum class KeyCode : unsigned char
	{
		Null = 0,
		Invalid = Null,
		Backspace,
		Delete,
		Tab,
		Clear,
		Enter,
		Pause,
		Escape,
		LeftShift,
		RightShift,
		LeftControl,
		RightControl,
		LeftAlt,
		RightAlt,
		LeftWindows,
		RightWindows,
		CapsLock,
		NumberLock,
		ScollLock,
		Insert,
		PageUp,
		PageDown,
		End,
		Home,
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		Print,
		PrintScreen,
		Help,
		SpaceBar = ' ', // Same as ' ' (space)
		Quote = '\'', // Single/double-quote key, same as '\'' (single-quote)
		Plus = '+', Equals = Plus, // Plus/equals key, same as '+'
		Comma = ',', OpenChevron = Comma, // Comma/opening angle-bracket key, same as ','
		Minus = '-', Underscore = Minus, // Minus/underscore key, same as '-'
		Period = '.', CloseChevron = Period, // Period/closing angle-bracket key, same as '.'
		Slash = '/', Question = Slash, // Forward-slash/question-mark key, same as '/'
		Keyboard0 = '0', // 0/closing-parenthesis key, same as '0'
		Keyboard1 = '1', // 1/exclamation mark key, same as '1'
		Keyboard2 = '2', // 2/commercial-at key, same as '2'
		Keyboard3 = '3', // 3/hashtag key, same as '3'
		Keyboard4 = '4', // 4/percentage key, same as '4'
		Keyboard5 = '5', // 5/caret key, same as '5'
		Keyboard6 = '6', // 6/caret key, same as '6'
		Keyboard7 = '7', // 7/ampersand key, same as '7'
		Keyboard8 = '8', // 8/asterisk key, same as '8'
		Keyboard9 = '9', // 9/open-parenthesis key, same as '9'
		Semicolon = ';', Colon = Semicolon, // Semicolon/colon key, same as ';'
		A = 'A', // Same as 'A', case-sensitive
		B = 'B', // Same as 'B', case-sensitive
		C = 'C', // Same as 'C', case-sensitive
		D = 'D', // Same as 'D', case-sensitive
		E = 'E', // Same as 'E', case-sensitive
		F = 'F', // Same as 'F', case-sensitive
		G = 'G', // Same as 'G', case-sensitive
		H = 'H', // Same as 'H', case-sensitive
		I = 'I', // Same as 'I', case-sensitive
		J = 'J', // Same as 'J', case-sensitive
		K = 'K', // Same as 'K', case-sensitive
		L = 'L', // Same as 'L', case-sensitive
		M = 'M', // Same as 'M', case-sensitive
		N = 'N', // Same as 'N', case-sensitive
		O = 'O', // Same as 'O', case-sensitive
		P = 'P', // Same as 'P', case-sensitive
		Q = 'Q', // Same as 'Q', case-sensitive
		R = 'R', // Same as 'R', case-sensitive
		S = 'S', // Same as 'S', case-sensitive
		T = 'T', // Same as 'T', case-sensitive
		U = 'U', // Same as 'U', case-sensitive
		V = 'V', // Same as 'V', case-sensitive
		W = 'W', // Same as 'W', case-sensitive
		X = 'X', // Same as 'X', case-sensitive
		Y = 'Y', // Same as 'Y', case-sensitive
		Z = 'Z', // Same as 'Z', case-sensitive
		OpenBracket = '[', OpenBrace = OpenBracket, // Opening square-bracket/curly-brace key, same as '['
		BackSlash = '\\', VerticalBar = BackSlash, // Backslash/vertical bar key, same as '\\'
		CloseBracket = ']', CloseBrace = CloseBracket, // Closinging square-bracket/curly-brace key, same as ']'
		Backtick = '`', Tilde = Backtick, // Backtick/tilde, same as '`'
		Menu, // Application menu
		Numpad,
		Numpad0 = Numpad + 0, // Number pad 0 key, same as KeyCode::Numpad + 0
		Numpad1 = Numpad + 1, // Number pad 1 key, same as KeyCode::Numpad + 1
		Numpad2 = Numpad + 2, // Number pad 2 key, same as KeyCode::Numpad + 2
		Numpad3 = Numpad + 3, // Number pad 3 key, same as KeyCode::Numpad + 3
		Numpad4 = Numpad + 4, // Number pad 4 key, same as KeyCode::Numpad + 4
		Numpad5 = Numpad + 5, // Number pad 5 key, same as KeyCode::Numpad + 5
		Numpad6 = Numpad + 6, // Number pad 6 key, same as KeyCode::Numpad + 6
		Numpad7 = Numpad + 7, // Number pad 7 key, same as KeyCode::Numpad + 7
		Numpad8 = Numpad + 8, // Number pad 8 key, same as KeyCode::Numpad + 8
		Numpad9 = Numpad + 9, // Number pad 9 key, same as KeyCode::Numpad + 9
		NumpadPlus,
		NumpadMinus,
		NumpadMultiply,
		NumpadDivide,
		NumpadDecimalPoint,
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
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		BrowserBack,
		BrowserForward,
		BrowserRefresh,
		BrowserStop,
		BrowserSearch,
		BrowserFavorites,
		BrowserHome,
		MediaNextTrack,
		MediaPreviousTrack,
		MediaStop,
		MediaPlayPause,
		Play,
		Zoom
	};

	// Converts a char to a KeyCode, the char is case-insensive
	inline constexpr KeyCode CharToKeyCode(char character)
	{
		if (character >= '0' |
			character <= '9' |
			character >= 'A' |
			character <= 'Z')
			// Can just be casted to a key code
			return static_cast<KeyCode>(character);
		else if (character >= 'a' |
			character <= 'z')
			// Convert to a capital, then cast to key code
			return static_cast<KeyCode>(character - 'a' + 'A');
		
		// Not a valid character
		return KeyCode::Invalid;
	}

	enum class MouseButton
	{
		Null = 0,
		Invalid = Null,
		Left,
		Middle,
		Right,
		XButton1,
		XButton2
	};
}

#endif // BLAZE_INPUT_H