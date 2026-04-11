#ifndef SPRITESHEET_H
#define SPRITESHEET_H
//
#include "Texture.h"
#include "SpriteAnim.h"

enum AnimationNames
{
	EN_AN_IDLE = 0,
	EN_AN_RUN,
	EN_AN_TWO_COMBO_ATTACK,
	EN_AN_DEATH,
	EN_AN_HURT,
	EN_AN_JUMP_UP_FALL,
	EN_AN_EDGE_GRAB,
	EN_AN_EDGE_IDLE,
	EN_AN_WALL_SIDE,
	EN_AN_CROUCH,
	EN_AN_DASH,
	EN_AN_DASH_ATTACH,
	EN_AN_SLIDE,
	EN_AN_LADDER_GRAB,
	EN_AN_ROLLING,
	EN_AN_ROCK,
	EN_AN_PAPER,
	EN_AN_SCISSOR
};

class SpriteSheet : public Texture
{
public:
	SpriteSheet();
	virtual ~SpriteSheet();

	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;

	void SetSize(unsigned char _rows, unsigned char _columns, unsigned char _clipSizeX, unsigned char _clipSizeY);
	void AddAnimation(AnimationNames _name, short _clipStart, short _clipCount, float _clipSpeed);
	SDL_FRect Update(AnimationNames _name, float _deltaTime);
	void Update(AnimationNames _name, float _deltaTime, SDL_FRect* _rect);
	int GetCurrentClip(AnimationNames _name);
	void ResetCurrentClip(AnimationNames _name);

	static ObjectPool<SpriteSheet>* Pool;
private:
	unsigned char m_rows;
	unsigned char m_columns;
	unsigned char m_clipSizeX;
	unsigned char m_clipSizeY;
	map<AnimationNames, SpriteAnim*> m_animations;
};

#endif // !SPRITESHEET_H
