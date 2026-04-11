#ifndef LEVEL2_H
#define LEVEL2_H

#include "Resource.h"

class Renderer;
class TTFont;
class InputController;
class Keyboard;
class Gamepad;
class AudioController;
class SoundEffect;
class Song;
class SpriteSheet;
class SpriteAnim;
class Timing;
class RPS;

class Level2 : public Resource
{
public:
	Level2();
	virtual ~Level2();

	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;

	void Initialize(Renderer* r, TTFont* _font, AudioController* _audio, string _firstUser, string _secondUser);
	void HandleInput(SDL_Event _event);
	void RunLevel();

private:
	SDL_Event m_sdlEvent;
	Renderer* m_renderer;
	TTFont* m_fArial20;
	bool m_quit;
	InputController* m_input;
	AudioController* m_audio;
	SoundEffect* m_whoosh;
	SoundEffect* m_gunshot;
	Song* m_song;
	SpriteSheet* m_sprite;
	SpriteAnim* m_spriteAnim;
	Timing* t;
	RPS* m_rps1;
	RPS* m_rps2;
	string m_rps1name;
	string m_rps2name;
	string m_result;
	float m_rps1X;
	float m_rps1Y;
	float m_rps2X;
	float m_rps2Y;
	bool m_isRolling;
	bool m_resultSet;
	int m_p1win;
	int m_p1loss;
	int m_p2win;
	int m_p2loss;
	int m_saveCount;
	int m_loadCount;
};

#endif // !LEVEL2_H
