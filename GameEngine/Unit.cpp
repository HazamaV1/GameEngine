#include "Unit.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
//
ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit()
{
	m_unitPosition = {};
	m_unitMove = {};
	m_soundEffect = nullptr;
	m_chunk = nullptr;
}

Unit::~Unit()
{
}

void Unit::AssignNonDefaultValues()
{
	m_soundEffect = SoundEffect::Pool->GetResource();
	m_soundEffect->AssignNonDefaultValues();
	Resource::AssignNonDefaultValues();
}

void Unit::Serialize(std::ostream& _stream)
{
	SerializePointer(_stream, m_chunk);
	Resource::Serialize(_stream);
}

void Unit::Deserialize(std::istream& _stream)
{
	DeserializePointer(_stream, m_chunk);
	Resource::Deserialize(_stream);
}

void Unit::ToString()
{
	cout << "UNIT" << endl;
	m_chunk->ToString();
	Resource::ToString();
}

void Unit::createPlayer(float screenW, float screenH)
{
	m_unitPosition = {screenW / 2.0f, screenH / 2.0f};
}

void Unit::updatePlayer(SDL_Event& _event, InputController* _input, float _deltaTime)
{
	m_unitMove = { 0.0f, 0.0f };

	_input->Process();

	if (_input->KB()->GetKeyStates()[SDL_SCANCODE_UP])
	{
		m_unitMove.y -= 1.0f;
	}
	if (_input->KB()->GetKeyStates()[SDL_SCANCODE_DOWN])
	{
		m_unitMove.y += 1.0f;
	}
	if (_input->KB()->GetKeyStates()[SDL_SCANCODE_LEFT])
	{
		m_unitMove.x -= 1.0f;
	}
	if (_input->KB()->GetKeyStates()[SDL_SCANCODE_RIGHT])
	{
		m_unitMove.x += 1.0f;
	}
	
	m_unitPosition += m_unitMove * 200.0f * _deltaTime;
}