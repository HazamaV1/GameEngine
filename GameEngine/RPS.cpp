#include "RPS.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "TTFont.h"
#include "AudioController.h"

RPS::RPS()
{
	m_state = RPSstate::GetNeutralState();

	m_animations = SpriteSheet::Pool->GetResource();
	m_animations->Load("../Assets/Textures/RockPaperScissors.tga");
	m_animations->SetSize(4, 4, 280, 170);
	m_animations->AddAnimation(EN_AN_IDLE, 0, 1, 0.0f);
	m_animations->AddAnimation(EN_AN_ROLLING, 0, 3, 4.0f);
	m_animations->AddAnimation(EN_AN_ROCK, 12, 3, 4.0f);
	m_animations->AddAnimation(EN_AN_PAPER, 4, 3, 4.0f);
	m_animations->AddAnimation(EN_AN_SCISSOR, 8, 3, 4.0f);
	m_animations->SetBlendMode(SDL_BLENDMODE_BLEND);
}

RPS::~RPS()
{
}

void RPS::HandleInput(SDL_Event _event, float _deltaTime)
{
	m_state->HandleInput(this, _event);
}

void RPS::Update(float _deltaTime)
{
	m_state->Update(this, _deltaTime);
}

void RPS::Render(Renderer* _renderer, float x, float y, TTFont* _font, string _userName, int _win, int _loss)
{
	m_srcRect.w = 280;
	_renderer->RenderTexture(m_animations,m_srcRect, SDL_FRect{x, y, 280, 170});
	_font->Write(_renderer->GetRenderer(), _userName.c_str(), SDL_Color{ 0, 200 , 0 }, SDL_Point{ (int)x, (int)y });
	if (m_result != "")
	{
		_font->Write(_renderer->GetRenderer(), m_result.c_str(), SDL_Color{ 255, 0, 0 }, SDL_Point{ (int)x, (int)y + 20 });
	}
	else if (m_state->GetStateName() == "Waiting to start..." || m_state->GetStateName() == "Rolling...")
	{
		_font->Write(_renderer->GetRenderer(), m_state->GetStateName().c_str(), SDL_Color{255, 0 , 0}, SDL_Point{(int)x, (int)y + 20});
	}
	std::string win = "Wins: " + std::to_string(_win);
	std::string loss = "Losses: " + std::to_string(_loss);
	_font->Write(_renderer->GetRenderer(), win.c_str(), SDL_Color{ 0, 0 , 255 }, SDL_Point{ (int)x, (int)y + 40 });
	_font->Write(_renderer->GetRenderer(), loss.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ (int)x, (int)y + 60 });
}