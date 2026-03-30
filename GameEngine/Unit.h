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
	void updateNpc(float _deltaTime, glm::vec2 _playerPos);
	void Initialize();
	void npcInitialize();

	glm::vec2 getPosition() const { return m_unitPosition; }
	bool getDirection() { return m_unitDirection; }
	bool getUnitMoving() { return m_unitIsMoving; }
	SpriteSheet* getUnitSprite() { return m_playerSprite; }
	SpriteSheet* getNpcSprite() { return m_npcSprite; }
	void setPosition(glm::vec2 setPos) { m_unitPosition = setPos; }
	
	static ObjectPool<Unit>* Pool;

private:
	glm::vec2 m_unitMove;
	glm::vec2 m_unitPosition;
	bool m_unitDirection;
	bool m_unitIsMoving;
	SoundEffect* m_soundEffect;
	FileChunk* m_chunk;
	SpriteSheet* m_npcSprite;
	SpriteSheet* m_playerSprite;
};

#endif // !UNIT_H
