#ifndef HEROINE_H
#define HEROINE_H

#include "HeroineState.h"
#include "RigidBody.h"

class SpriteSheet;

class Heroine
{
public:
	Heroine();
	virtual ~Heroine();

	HeroineState* GetState() { return m_state; }
	void SetState(HeroineState* _state) { m_state = _state; }
	SpriteSheet* GetAnimations() { return m_animations; }
	SDL_FRect* GetSrcRect() { return &m_srcRect; }
	RigidBody* GetRigidBody() { return m_rigidBody; }


	virtual void HandleInput(SDL_Event _event, float _deltaTime);
	virtual void Update(float _deltaTime);
	virtual void Render(Renderer* _renderer);

private:
	HeroineState* m_state;
	RigidBody* m_rigidBody;
	SpriteSheet* m_animations;
	SDL_FRect m_srcRect;
};

#endif // !HEROINE_H
