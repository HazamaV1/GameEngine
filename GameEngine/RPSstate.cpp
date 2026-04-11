#include "RPSstate.h"
#include "InputController.h"
#include "Keyboard.h"
#include "RPS.h"
#include "SpriteSheet.h"


NeutralState RPSstate::m_neutral;
RollingState RPSstate::m_rolling;
RockState RPSstate::m_rock;
PaperState RPSstate::m_paper;
ScissorState RPSstate::m_scissor;

void NeutralState::HandleInput(RPS* _rps, SDL_Event _event)
{
	if (InputController::Instance().KB()->KeyDown(_event, SDLK_SPACE))
	{
		_rps->SetState(RPSstate::GetRollingState());
	}
}

void NeutralState::Update(RPS* _rps, float _deltaTime)
{
	*_rps->GetSrcRect() = _rps->GetAnimations()->Update(EN_AN_IDLE, _deltaTime);
}

void RollingState::HandleInput(RPS* _rps, SDL_Event _event)
{
	if (InputController::Instance().KB()->KeyDown(_event, SDLK_SPACE))
	{
		int randomChoice = rand() % 3;
		if (randomChoice == 0)
		{
			_rps->SetState(RPSstate::GetRockState());
		}
		else if (randomChoice == 1)
		{
			_rps->SetState(RPSstate::GetPaperState());
		}
		else if (randomChoice == 2)
		{
			_rps->SetState(RPSstate::GetScissorState());
		}
	}
}

void RollingState::Update(RPS* _rps, float _deltaTime)
{
	_rps->GetAnimations()->Update(EN_AN_ROLLING, _deltaTime, _rps->GetSrcRect());
	_rps->GetSrcRect()->y = 0;
	_rps->GetSrcRect()->x = (_rps->GetAnimations()->GetCurrentClip(EN_AN_ROLLING) % 4) * 280;
}

void RockState::HandleInput(RPS* _rps, SDL_Event _event)
{
	if (InputController::Instance().KB()->KeyDown(_event, SDLK_SPACE))
	{
		_rps->SetState(RPSstate::GetNeutralState());
	}
}

void RockState::Update(RPS* _rps, float _deltaTime)
{
	_rps->GetAnimations()->Update(EN_AN_ROCK, _deltaTime, _rps->GetSrcRect());
	_rps->GetSrcRect()->y = 3 * 175;
	_rps->GetSrcRect()->x = (_rps->GetAnimations()->GetCurrentClip(EN_AN_ROCK) % 4) * 280;
}

void PaperState::HandleInput(RPS* _rps, SDL_Event _event)
{
	if (InputController::Instance().KB()->KeyDown(_event, SDLK_SPACE))
	{
		_rps->SetState(RPSstate::GetNeutralState());
	}
}

void PaperState::Update(RPS* _rps, float _deltaTime)
{
	_rps->GetAnimations()->Update(EN_AN_PAPER, _deltaTime, _rps->GetSrcRect());
	_rps->GetSrcRect()->y = 1 * 175;
	_rps->GetSrcRect()->x = (_rps->GetAnimations()->GetCurrentClip(EN_AN_PAPER) % 4) * 280;
}

void ScissorState::HandleInput(RPS* _rps, SDL_Event _event)
{
	if (InputController::Instance().KB()->KeyDown(_event, SDLK_SPACE))
	{
		_rps->SetState(RPSstate::GetNeutralState());
	}
}

void ScissorState::Update(RPS* _rps, float _deltaTime)
{
	_rps->GetAnimations()->Update(EN_AN_SCISSOR, _deltaTime, _rps->GetSrcRect());
	_rps->GetSrcRect()->y = 2 * 175;
	_rps->GetSrcRect()->x = (_rps->GetAnimations()->GetCurrentClip(EN_AN_SCISSOR)%4) * 280;
}