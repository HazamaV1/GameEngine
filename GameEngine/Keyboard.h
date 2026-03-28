#ifndef KEYBOARD_H
#define KEYBOARD_H
//
#include "StandardIncludes.h"

class Keyboard
{
public:
	Keyboard();
	virtual ~Keyboard();

	const bool* GetKeyStates() const { return SDL_GetKeyboardState(nullptr); }

	void Initialize(SDL_Window* _window);
	bool KeyDown(SDL_Event _event, SDL_Keycode _key);
	bool KeyUp(SDL_Event _event, SDL_Keycode _key);
	string TextInput(SDL_Event _event);

private:
	const bool* m_keyStates;
};

#endif // !KEYBOARD_H
