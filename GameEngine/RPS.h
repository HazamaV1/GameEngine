#ifndef RPS_H
#define RPS_H

#include "RPSstate.h"

class SpriteSheet;
class Renderer;
class TTFont;

class RPS
{
public:
	RPS();
	virtual ~RPS();
	RPSstate* GetState() { return m_state; }
	void SetState(RPSstate* _state) { m_state = _state; }
	string GetStateName() { return m_state->GetStateName(); }
	SpriteSheet* GetAnimations() { return m_animations; }
	SDL_FRect* GetSrcRect() { return &m_srcRect; }
	string GetResult() { return m_result; }
	void SetResult(string _result) { m_result = _result; }

	virtual void HandleInput(SDL_Event _event, float _deltaTime);
	virtual void Update(float _deltaTime);
	virtual void Render(Renderer* _renderer, float x, float y, TTFont* _font, string _userName, int _win, int _loss);

private:
	RPSstate* m_state;
	SpriteSheet* m_animations;
	SDL_FRect m_srcRect;
	string m_result;
};

#endif // !RPS_H
