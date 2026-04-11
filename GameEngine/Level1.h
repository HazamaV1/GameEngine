#ifndef LEVEL_H1
#define LEVEL_H1

#include "StandardIncludes.h"

class Renderer;
class Texture;
class TTFont;
class InputController;
class AudioController;
class Song;
class SpriteSheet;

class Level1
{
public:
	Level1();
	~Level1();

	void Initialize(Renderer* r, TTFont* _font, AudioController* _audio);
	void HandleInput(SDL_Event _event);
	void RunLevel();

	string GetFirstUser() { return m_inputText1; }
	string GetSecondUser() { return m_inputText2; }
	bool GetCompleted() { return m_complete; }
	

private:
	SDL_Event m_sdlEvent;
	Renderer* m_renderer;
	Texture* m_bg;
	TTFont* m_fArial20;
	bool m_quit;
	InputController* m_input;
	AudioController* m_audio;
	Song* m_song;
	SpriteSheet* m_sprite;
	bool m_input1Con;
	bool m_input2Con;
	bool m_complete;
	string m_inputText1;
	string m_inputText2;
	string m_errorMsg;
};

#endif // LEVEL_H1