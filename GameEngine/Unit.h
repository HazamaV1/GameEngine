#ifndef UNIT_H
#define UNIT_H
//
#include "Resource.h"
#include "SoundEffect.h"
#include "FileChunk.h"

class InputController;
class SpriteSheet;

class Unit : public Resource
{
public:
	Unit();
	virtual ~Unit();

	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;

	void createPlayer(float screenW, float screenH);
	void updatePlayer(SDL_Event& _event, InputController* _input, float _deltaTime);
	void updateNpc(float _deltaTime, glm::vec2 _playerPos, int _npcSpeed);
	void Initialize();
	void npcInitialize();
	void Reset();

	glm::vec2 getPosition() const { return m_unitPosition; }
	bool getDirection() { return m_unitDirection; }
	bool getUnitMoving() { return m_unitIsMoving; }
	bool getNpcTagged() { return m_tagged; }
	SpriteSheet* getUnitSprite() { return m_playerSprite; }
	SpriteSheet* getNpcSprite() { return m_npcSprite; }

	void setPosition(glm::vec2 setPos) { m_unitPosition = setPos; }
	void setNpcTagged(bool _npcTag) { m_tagged = _npcTag; }
	
	static ObjectPool<Unit>* Pool;

private:
	glm::vec2 m_unitMove;
	glm::vec2 m_unitPosition;
	bool m_unitDirection;
	bool m_unitIsMoving;
	bool m_tagged;
	SoundEffect* m_soundEffect;
	FileChunk* m_chunk;
	SpriteSheet* m_playerSprite;
	SpriteSheet* m_npcSprite;
};

#endif // !UNIT_H
