#include "Level2.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "Timing.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "RPS.h"
#include <ctime>


Level2::Level2()
{
	m_sdlEvent = { };
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	m_input = nullptr;
	m_quit = false;
	m_audio = nullptr;
	m_whoosh = nullptr;
	m_gunshot = nullptr;
	m_song = nullptr;
	m_sprite = nullptr;
	m_spriteAnim = nullptr;
	t = nullptr;
	m_rps1 = nullptr;
	m_rps2 = nullptr;
	m_result = "";
	m_isRolling = false;
	m_resultSet = false;
	m_rps1X = 0;
	m_rps1Y = 0;
	m_rps2X = 0;
	m_rps2Y = 0;
	m_p1win = 0;
	m_p1loss = 0;
	m_p2win = 0;
	m_p2loss = 0;
	m_saveCount = 0;
	m_loadCount = 0;
}

Level2::~Level2()
{
	delete SpriteSheet::Pool;
	SpriteSheet::Pool = nullptr;
	delete SpriteAnim::Pool;
	SpriteAnim::Pool = nullptr;
}

void Level2::Serialize(std::ostream& _stream)
{
	int resultLen1 = m_rps1->GetResult().length();
	_stream.write(reinterpret_cast<char*>(&resultLen1), sizeof(resultLen1));
	_stream.write(m_rps1->GetResult().c_str(), resultLen1);
	int stateLen1 = m_rps1->GetStateName().length();
	_stream.write(reinterpret_cast<char*>(&stateLen1), sizeof(stateLen1));
	_stream.write(m_rps1->GetStateName().c_str(), stateLen1);
	_stream.write(reinterpret_cast<char*>(&m_rps1X), sizeof(m_rps1X));
	_stream.write(reinterpret_cast<char*>(&m_rps1Y), sizeof(m_rps1Y));

	int resultLen2 = m_rps2->GetResult().length();
	_stream.write(reinterpret_cast<char*>(&resultLen2), sizeof(resultLen2));
	_stream.write(m_rps2->GetResult().c_str(), resultLen2);
	int stateLen2 = m_rps2->GetStateName().length();
	_stream.write(reinterpret_cast<char*>(&stateLen2), sizeof(stateLen2));
	_stream.write(m_rps2->GetStateName().c_str(), stateLen2);
	_stream.write(reinterpret_cast<char*>(&m_rps2X), sizeof(m_rps2X));
	_stream.write(reinterpret_cast<char*>(&m_rps2Y), sizeof(m_rps2Y));

	_stream.write(reinterpret_cast<char*>(&m_p1win), sizeof(m_p1win));
	_stream.write(reinterpret_cast<char*>(&m_p2win), sizeof(m_p2win));
	_stream.write(reinterpret_cast<char*>(&m_p1loss), sizeof(m_p1loss));
	_stream.write(reinterpret_cast<char*>(&m_p2loss), sizeof(m_p2loss));
	Resource::Serialize(_stream);
}

void Level2::Deserialize(std::istream& _stream)
{
	int resultLen1;
	_stream.read(reinterpret_cast<char*>(&resultLen1), sizeof(resultLen1));
	string resultName1(resultLen1, ' ');
	_stream.read(&resultName1[0], resultLen1);
	m_rps1->SetResult(resultName1);
	int stateLen1;
	_stream.read(reinterpret_cast<char*>(&stateLen1), sizeof(stateLen1));
	string stateName1(stateLen1, ' ');
	_stream.read(&stateName1[0], stateLen1);
	if (stateName1 == "Waiting to start...") m_rps1->SetState(RPSstate::GetNeutralState());
	else if (stateName1 == "Rolling...") m_rps1->SetState(RPSstate::GetRollingState());
	else if (stateName1 == "Rock") m_rps1->SetState(RPSstate::GetRockState());
	else if (stateName1 == "Paper") m_rps1->SetState(RPSstate::GetPaperState());
	else if (stateName1 == "Scissor") m_rps1->SetState(RPSstate::GetScissorState());
	_stream.read(reinterpret_cast<char*>(&m_rps1X), sizeof(m_rps1X));
	_stream.read(reinterpret_cast<char*>(&m_rps1Y), sizeof(m_rps1Y));

	int resultLen2;
	_stream.read(reinterpret_cast<char*>(&resultLen2), sizeof(resultLen2));
	string resultName2(resultLen2, ' ');
	_stream.read(&resultName2[0], resultLen2);
	m_rps2->SetResult(resultName2);
	int stateLen2;
	_stream.read(reinterpret_cast<char*>(&stateLen2), sizeof(stateLen2));
	string stateName2(stateLen2, ' ');
	_stream.read(&stateName2[0], stateLen2);
	if (stateName2 == "Waiting to start...") { m_rps2->SetState(RPSstate::GetNeutralState()); }
	else if (stateName2 == "Rolling...") { m_rps2->SetState(RPSstate::GetRollingState()); }
	else if (stateName2 == "Rock") m_rps2->SetState(RPSstate::GetRockState());
	else if (stateName2 == "Paper") m_rps2->SetState(RPSstate::GetPaperState());
	else if (stateName2 == "Scissor") m_rps2->SetState(RPSstate::GetScissorState());
	_stream.read(reinterpret_cast<char*>(&m_rps2X), sizeof(m_rps2X));
	_stream.read(reinterpret_cast<char*>(&m_rps2Y), sizeof(m_rps2Y));

	_stream.read(reinterpret_cast<char*>(&m_p1win), sizeof(m_p1win));
	_stream.read(reinterpret_cast<char*>(&m_p2win), sizeof(m_p2win));
	_stream.read(reinterpret_cast<char*>(&m_p1loss), sizeof(m_p1loss));
	_stream.read(reinterpret_cast<char*>(&m_p2loss), sizeof(m_p2loss));
	Resource::Deserialize(_stream);
}

void Level2::ToString()
{

}

void Level2::AssignNonDefaultValues()
{

}

void Level2::Initialize(Renderer* r, TTFont* _font, AudioController* _audio, string _firstUser, string _secondUser)
{
	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
	m_renderer = r;
	t = &Timing::Instance();
	m_input = &InputController::Instance();
	m_input->Initialize(m_renderer->GetWindow());
	m_fArial20 = _font;
	m_audio = _audio;
	m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
	m_whoosh = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	m_gunshot = m_audio->LoadEffect("../Assets/Audio/Effects/DistantGunshot.mp3");
	m_rps1 = new RPS();
	m_rps2 = new RPS();
	m_rps1name = _firstUser;
	m_rps2name = _secondUser;
	m_result = "";
	m_rps1X = 20;
	m_rps1Y = 30;
	m_rps2X = 20;
	m_rps2Y = 330;
	srand(static_cast<unsigned int>(time(NULL)));
}

void Level2::HandleInput(SDL_Event _event)
{
	m_input->Process();

	string temp;
	if ((m_sdlEvent.type == SDL_EVENT_QUIT) ||
		(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
	{
		m_quit = true;
	}

	if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_F5))
	{
		m_saveCount += 1;
		ofstream writeStream("saveGame.bin", ios::out | ios::binary);
		this->Serialize(writeStream);
		writeStream.close();
	}

	if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_F7))
	{
		m_loadCount += 1;
		ifstream readStream("saveGame.bin", ios::in | ios::binary);
		this->Deserialize(readStream);
		readStream.close();
	}

	if (m_input->MS()->LDown())
	{
		m_rps1X = m_input->MS()->GetX() - 140;
		m_rps1Y = m_input->MS()->GetY() - 85;
		if (m_rps1X > m_renderer->GetWindowSize().x - 280)
		{
			m_rps1X = m_renderer->GetWindowSize().x - 280;
		}
		else if (m_rps1X <= 0)
		{
			m_rps1X = 0;
		}

		if (m_rps1Y >= m_renderer->GetWindowSize().y - 170)
		{
			m_rps1Y = m_renderer->GetWindowSize().y - 170;
		}
		else if (m_rps1Y <= 0)
		{
			m_rps1Y = 0;
		}
	}

	if (m_input->MS()->RDown())
	{
		m_rps2X = m_input->MS()->GetX() - 140;
		m_rps2Y = m_input->MS()->GetY() - 85;
		if (m_rps2X > m_renderer->GetWindowSize().x - 280)
		{
			m_rps2X = m_renderer->GetWindowSize().x - 280;
		}
		else if (m_rps2X <= 0)
		{
			m_rps2X = 0;
		}

		if (m_rps2Y >= m_renderer->GetWindowSize().y - 170)
		{
			m_rps2Y = m_renderer->GetWindowSize().y - 170;
		}
		else if (m_rps2Y <= 0)
		{
			m_rps2Y = 0;
		}
	}

	m_rps1->HandleInput(_event, t->GetDeltaTime());
	m_rps2->HandleInput(_event, t->GetDeltaTime());
}

void Level2::RunLevel()
{
	m_audio->Play(m_song);

	string state1;
	string state2;
	while (!m_quit)
	{
		t->Tick();
		m_renderer->SetDrawColor({ 255, 255, 255, 255 });
		m_renderer->ClearScreen();

		while (SDL_PollEvent(&m_sdlEvent))
		{
			HandleInput(m_sdlEvent);
		}

		m_rps1->Update(t->GetDeltaTime());
		m_rps2->Update(t->GetDeltaTime());
		m_rps1->Render(m_renderer, m_rps1X, m_rps1Y, m_fArial20, m_rps1name, m_p1win, m_p1loss);
		m_rps2->Render(m_renderer, m_rps2X, m_rps2Y, m_fArial20, m_rps2name, m_p2win, m_p2loss);
		
		state1 = m_rps1->GetState()->GetStateName();
		state2 = m_rps2->GetState()->GetStateName();

		if (state1 != "Waiting to start..." && state1 != "Rolling..." && state2 != "Waiting to start..." && state2 != "Rolling...")
		{
			m_isRolling = false;
			if (m_resultSet == false)
			{
				if (state1 == state2)
				{
					//m_result = "Tie!";
					m_rps1->SetResult("Tie!");
					m_rps2->SetResult("Tie!");
				}
				else if (state1 == "Rock" && state2 == "Scissor")
				{
					//m_result = m_rps1name + " wins!";
					m_rps1->SetResult("Winner!");
					m_rps2->SetResult("Loser!");
					m_p1win += 1;
					m_p2loss += 1;
				}
				else if (state1 == "Paper" && state2 == "Rock")
				{
					//m_result = m_rps1name + " wins!";
					m_rps1->SetResult("Winner!");
					m_rps2->SetResult("Loser!");
					m_p1win += 1;
					m_p2loss += 1;
				}
				else if (state1 == "Scissor" && state2 == "Paper")
				{
					//m_result = m_rps1name + " wins!";
					m_rps1->SetResult("Winner!");
					m_rps2->SetResult("Loser!");
					m_p1win += 1;
					m_p2loss += 1;
				}
				else
				{
					//m_result = m_rps2name + " wins!";
					m_rps2->SetResult("Winner!");
					m_rps1->SetResult("Loser!");
					m_p2win += 1;
					m_p1loss += 1;
				}
				m_audio->Play(m_gunshot);
				m_resultSet = true;
			}
		}
		else if (state1 == "Rolling..." && state2 == "Rolling..." && m_isRolling == false)
		{
			m_isRolling = true;
			m_resultSet = false;
			m_audio->Play(m_whoosh);
			m_rps2->SetResult("");
			m_rps1->SetResult("");
		}
		if (state1 == "Waiting to start..." && state2 == "Waiting to start...")
		{
			m_resultSet = false;
			m_rps2->SetResult("");
			m_rps1->SetResult("");
		}

		std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
		m_fArial20->Write(m_renderer->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 1, m_renderer->GetWindowSize().y - 45 });

		std::string gameTime = "Game Time: " + std::to_string(t->GetTimeInSeconds());
		m_fArial20->Write(m_renderer->GetRenderer(), gameTime.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 240, m_renderer->GetWindowSize().y - 45 });

		std::string saveCount = "Save: " + std::to_string(m_saveCount);
		m_fArial20->Write(m_renderer->GetRenderer(), saveCount.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 400, m_renderer->GetWindowSize().y - 45 });

		std::string loadCount = "Load: " + std::to_string(m_loadCount);
		m_fArial20->Write(m_renderer->GetRenderer(), loadCount.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 500, m_renderer->GetWindowSize().y - 45 });

		std::string keys = "[ESC] Quit   [SPACE] Next Game State   [F5] Save    [F7] Load";
		m_fArial20->Write(m_renderer->GetRenderer(), keys.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 1, m_renderer->GetWindowSize().y - 25 });

		SDL_RenderPresent(m_renderer->GetRenderer());

		t->CapFPS();
	}
}