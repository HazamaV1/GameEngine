#include "Unit.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SpriteAnim.h"
#include "SpriteSheet.h"
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

void Unit::Initialize()
{
	m_playerSprite = SpriteSheet::Pool->GetResource();
	m_playerSprite->Load("../Assets/Textures/Warrior.tga");
	m_playerSprite->SetSize(17, 6, 69, 44);
	m_playerSprite->setColorRed(0);
	m_playerSprite->setColorGreen(0);
	m_playerSprite->setColorBlue(255);
	m_playerSprite->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
	m_playerSprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
}

void Unit::npcInitialize()
{
	m_npcSprite = SpriteSheet::Pool->GetResource();
	m_npcSprite->Load("../Assets/Textures/Warrior.tga");
	m_npcSprite->SetSize(17, 6, 69, 44);
	m_npcSprite->setColorRed(0);
	m_npcSprite->setColorGreen(255);
	m_npcSprite->setColorBlue(0);
	m_npcSprite->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
	m_npcSprite->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
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
	Initialize();
	m_unitPosition = {screenW / 2.0f, screenH / 2.0f};
}

void Unit::updatePlayer(SDL_Event& _event, InputController* _input, float _deltaTime)
{
	m_unitMove = { 0.0f, 0.0f };

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
		m_unitDirection = true;
	}

	if (_input->KB()->GetKeyStates()[SDL_SCANCODE_RIGHT])
	{
		m_unitMove.x += 1.0f;
		m_unitDirection = false;
	}

	m_unitPosition += m_unitMove * 100.0f * _deltaTime;

	if (m_unitMove.x == 0.0f && m_unitMove.y == 0.0f)
	{
		m_unitIsMoving = false;
	}
	else
	{
		m_unitIsMoving = true;
	}
	
}

