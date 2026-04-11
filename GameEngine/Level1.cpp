#include "Level1.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "AudioController.h"
#include "Song.h"

Level1::Level1()
{
	m_sdlEvent = { };
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	m_input = nullptr;
	m_quit = false;
	m_complete = false;
	m_audio = nullptr;
	m_song = nullptr;
	m_input1Con = false;
	m_input2Con = false;
	m_inputText1 = "";
	m_inputText2 = "";
	m_errorMsg = "";
}

Level1::~Level1()
{
	Texture::Pool->ReleaseResource(m_bg);
	delete Texture::Pool;
	Texture::Pool = nullptr;
}

void Level1::Initialize(Renderer* r, TTFont* _font, AudioController* _audio)
{
	m_renderer = r;
	m_bg = Texture::Pool->GetResource();
	m_bg->Load("../Assets/Textures/Background.tga");
	m_input = &InputController::Instance();
	m_input->Initialize(m_renderer->GetWindow());
	m_fArial20 = _font;
	m_audio = _audio;
	m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}

void Level1::HandleInput(SDL_Event _event)
{
	m_input->Process();

	string temp1;
	string temp2;
	if ((m_sdlEvent.type == SDL_EVENT_QUIT) ||
		(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
	{
		m_quit = true;
	}
	
	if ((temp1 = m_input->KB()->TextInput(_event)) != "" && m_input1Con == false)
	{
		m_inputText1 += temp1;
	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_BACKSPACE) && m_input1Con == false)
	{
		m_inputText1.pop_back();
	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN) && m_input1Con == false)
	{
		if (m_inputText1.length() < 1 || m_inputText1.length() > 21)
		{
			m_errorMsg = "Nickname must be > 1 or < 20 characters.";
		}
		else
		{
			m_errorMsg = "";
			m_input1Con = true;
		}
	}
	else if ((temp2 = m_input->KB()->TextInput(_event)) != "" && m_input1Con == true)
	{
		m_inputText2 += temp2;
	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_BACKSPACE) && m_input1Con == true)
	{
		m_inputText2.pop_back();
	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN) && m_input1Con == true)
	{
		if (m_inputText2.length() < 1 || m_inputText2.length() > 21)
		{
			m_errorMsg = "Nickname must be > 1 or < 20 characters.";
		}
		else
		{
			m_errorMsg = "";
			m_input2Con = true;
			m_complete = true;
			m_quit = true;
		}
	}
}

void Level1::RunLevel()
{
	m_audio->Play(m_song);

	while (!m_quit)
	{

		m_renderer->SetDrawColor({ 255, 255, 255, 255 });
		m_renderer->ClearScreen();
		m_renderer->RenderTexture(m_bg, SDL_Point{ 0, 0});
		while (SDL_PollEvent(&m_sdlEvent))
		{
			HandleInput(m_sdlEvent);
		}

		m_fArial20->Initialize(40);
		m_fArial20->Write(m_renderer->GetRenderer(), m_inputText1.c_str(), SDL_Color{ 255, 255, 0 }, SDL_Point{ 40, 65 });
		m_fArial20->Write(m_renderer->GetRenderer(), m_inputText2.c_str(), SDL_Color{ 255, 255, 0 }, SDL_Point{ 40, 210 });
		m_fArial20->Write(m_renderer->GetRenderer(), m_errorMsg.c_str(), SDL_Color{ 255, 0 ,0 }, SDL_Point{ 40, 280 });

		m_fArial20->Initialize(20);
		std::string keys = "[ESC] Quit   [RETURN] Next Nickname";
		m_fArial20->Write(m_renderer->GetRenderer(), keys.c_str(), SDL_Color{ 255, 255, 255 }, SDL_Point{ 5, m_renderer->GetWindowSize().y - 25});

		SDL_RenderPresent(m_renderer->GetRenderer());
	}


}
