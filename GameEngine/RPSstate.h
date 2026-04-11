#ifndef RPSSTATE_H
#define RPSSTATE_H

#include "StandardIncludes.h"

class RPS;
class NeutralState;
class RollingState;
class RockState;
class PaperState;
class ScissorState;

class RPSstate
{
public:
	RPSstate() {}
	virtual ~RPSstate() {}

	static NeutralState* GetNeutralState() { return &m_neutral; }
	static RollingState* GetRollingState() { return &m_rolling; }
	static RockState* GetRockState() { return &m_rock; }
	static PaperState* GetPaperState() { return &m_paper; }
	static ScissorState* GetScissorState() { return &m_scissor; }

	virtual void HandleInput(RPS* _rps, SDL_Event _event) = 0;
	virtual void Update(RPS* _rps, float _deltaTime) = 0;
	virtual string GetStateName() = 0;

private:
	static NeutralState m_neutral;
	static RollingState m_rolling;
	static RockState m_rock;
	static PaperState m_paper;
	static ScissorState m_scissor;
	string m_stateName;
};


class NeutralState : public RPSstate
{
public:
	NeutralState() {}
	virtual ~NeutralState() {}

	void HandleInput(RPS* _rps, SDL_Event _event) override;
	void Update(RPS* _rps, float _deltaTime) override;
	string GetStateName() override { return "Waiting to start..."; }
};

class RollingState : public RPSstate
{
public:
	RollingState() {}
	virtual ~RollingState() {}

	void HandleInput(RPS* _rps, SDL_Event _event) override;
	void Update(RPS* _rps, float _deltaTime) override;
	string GetStateName() override { return "Rolling..."; }
};

class RockState : public RPSstate
{
public:
	RockState() {}
	virtual ~RockState() {}

	void HandleInput(RPS* _rps, SDL_Event _event) override;
	void Update(RPS* _rps, float _deltaTime) override;
	string GetStateName() override { return "Rock"; }
};

class PaperState : public RPSstate
{
public:
	PaperState() {}
	virtual ~PaperState() {}

	void HandleInput(RPS* _rps, SDL_Event _event) override;
	void Update(RPS* _rps, float _deltaTime) override;
	string GetStateName() override { return "Paper"; }
};

class ScissorState : public RPSstate
{
public:
	ScissorState() {}
	virtual ~ScissorState() {}

	void HandleInput(RPS* _rps, SDL_Event _event) override;
	void Update(RPS* _rps, float _deltaTime) override;
	string GetStateName() override { return "Scissor"; }
};


#endif // !RPSSTATE_H
