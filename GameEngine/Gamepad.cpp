#include "Gamepad.h"

void Gamepad::Initialize()
{
	M_ASSERT(SDL_InitSubSystem(SDL_INIT_GAMEPAD), "Failed to initialize gamepad input.")
}

void Gamepad::Detect()
{
	if (!SDL_HasGamepad())
	{
		return;
	}

	int count = 0;
	SDL_JoystickID* ids = SDL_GetGamepads(&count);
	SDL_Gamepad* gamepad = NULL;

	for (unsigned int i = 0; i < count; i++)
	{
		Add(ids[i]);
	}
}

bool Gamepad::Added(SDL_Event _event)
{
	if (_event.type != SDL_EVENT_GAMEPAD_ADDED) return false;
	return Add(_event.gdevice.which);
}

bool Gamepad::Removed(SDL_Event _event)
{
	if (_event.type != SDL_EVENT_GAMEPAD_REMOVED) return false;
	for (unsigned int i = 0; i < m_gamepads.size(); i++){
		if (m_gamepads[i]->ID != _event.gdevice.which) continue;
		delete m_gamepads[i];
		m_gamepads.erase(m_gamepads.begin() + i);
		break;
	}
	return true;
}

bool Gamepad::Add(SDL_JoystickID _joystickId)
{
	for (const auto& gamepadInfo : m_gamepads)
	{
		if (gamepadInfo->ID == _joystickId) return false;
	}

	SDL_Gamepad* gamepad = SDL_OpenGamepad(_joystickId);
	M_ASSERT(gamepad != nullptr, "SDL_OpenGamepad failed.");
	GamepadInfo* gpInfo = new GamepadInfo();
	gpInfo->ID = _joystickId;
	gpInfo->Gamepad = gamepad;
	gpInfo->Name = SDL_GetGamepadName(gamepad);
	m_gamepads.push_back(gpInfo);
	return true;
}

SDL_GamepadButton GamepadInfo::ButtonDown(SDL_Event _event) const
{
	if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_DOWN) || (_event.gdevice.which != ID))
	{
		return SDL_GAMEPAD_BUTTON_INVALID;
	}
	return (SDL_GamepadButton)_event.gbutton.button;
}

SDL_GamepadButton GamepadInfo::ButtonUp(SDL_Event _event) const
{
	if ((_event.type != SDL_EVENT_GAMEPAD_BUTTON_UP) || (_event.gdevice.which != ID))
	{
		return SDL_GAMEPAD_BUTTON_INVALID;
	}
	return (SDL_GamepadButton)_event.gbutton.button;
}

bool GamepadInfo::ButtonHeld(SDL_GamepadButton _button) const
{
	return SDL_GetGamepadButton(Gamepad, _button);
}

SDL_FPoint& GamepadInfo::GetLeftStick()
{
	LeftStick.x = (float)SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_LEFTX);
	LeftStick.y = (float)SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_LEFTY);
	return LeftStick;
}

SDL_FPoint& GamepadInfo::GetRightStick()
{
	RightStick.x = (float)SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_RIGHTX);
	RightStick.y = (float)SDL_GetGamepadAxis(Gamepad, SDL_GAMEPAD_AXIS_RIGHTY);
	return RightStick;
}

bool GamepadInfo::ProcessMotion(SDL_Event _event)
{
	if ((_event.type != SDL_EVENT_GAMEPAD_AXIS_MOTION) || (ID != _event.cdevice.which))
	{
		return false;
	}

	if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
	{
		LeftStick.x = _event.gaxis.value;
	}
	else if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
	{
		LeftStick.y = _event.gaxis.value;
	}

	if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
	{
		RightStick.x = _event.gaxis.value;
	}
	else if (_event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
	{
		RightStick.y = _event.gaxis.value;
	}

	return true;
}

string GamepadInfo::ToString() const
{
	string s = "Buttons held: ";
	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
	{
		if (SDL_GetGamepadButton(Gamepad, (SDL_GamepadButton)i))
		{
			s += to_string(i) + " ";
		}
	}
	s += " | Sticks L: [" + to_string(LeftStick.x) + ", " + to_string(LeftStick.y) + "] ";
	s += " | Sticks R: [" + to_string(RightStick.x) + ", " + to_string(RightStick.y) + "] ";
	return s;
}

Gamepad::~Gamepad()
{
	for (unsigned int i = 0; i < m_gamepads.size(); i++)
	{
		SDL_CloseGamepad(m_gamepads[i]->Gamepad);
		delete m_gamepads[i];
	}
	m_gamepads.clear();
}

string Gamepad::ToString()
{
	int gpc = 0;
	string s = "Gamepads: ";
	for (GamepadInfo* gp : m_gamepads)
	{
		gpc++;
		s += to_string(gpc) + " - " + gp->Name + ": ";
		s += gp->ToString() + " ";
	}
	return s;
}