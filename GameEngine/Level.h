#ifndef LEVEL_H
#define LEVEL_H
//
#include "Unit.h"

class Renderer;
class TTFont;
class InputController;
class AudioController;
class SoundEffect;
class Song;
class SpriteSheet;
class Timing;
class WAVDraw;

class Level : public Resource
{
public:
	Level();
	~Level();

	// Methods
	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;
	
	void Initialize();
	void HandleInput(SDL_Event _event);
	void RunLevel();

private:
	SDL_Event m_sdlEvent;
	Renderer* m_renderer;
	TTFont* m_fArial20;
	bool m_quit;
	InputController* m_input;
	AudioController* m_audio;
	SoundEffect* m_effect;
	Song* m_song;
	vector<Unit*> m_units;
	SpriteSheet* m_sprite;
	Timing* t;
};

#endif // LEVEL_H