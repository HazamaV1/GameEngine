#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "WavDraw.h"
#include "Level.h"
//
GameController::GameController()
{
	//m_sdlEvent = { };
	//m_renderer = nullptr;
	//m_fArial20 = nullptr;
	//m_input = nullptr;
	//m_quit = false;
	//m_audio = nullptr;
	//m_effect = nullptr;
	//m_song = nullptr;
	//m_wavDraw = nullptr;
	//m_zoomY = 5;
}

GameController::~GameController()
{
	ShutDown();
}

void GameController::Initialize()
{
	//AssetController::Instance().Initialize(10000000);
	//m_renderer = &Renderer::Instance();
	//m_renderer->Initialize();
	//m_input = &InputController::Instance();
	//m_input->Initialize(m_renderer->GetWindow());
	//m_fArial20 = new TTFont();
	//m_fArial20->Initialize(20);
	//m_audio = &AudioController::Instance();
	//m_effect = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	//m_wavDraw = new WAVDraw();
}

void GameController::ShutDown()
{
	delete m_fArial20;
	m_fArial20 = nullptr;
	delete m_wavDraw;
}

//void GameController::HandleInput(SDL_Event _event)
//{
//	m_input->Process();
//
//	string temp;
//	if ((m_sdlEvent.type == SDL_EVENT_QUIT) ||
//		(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
//	{
//		m_quit = true;
//	}
//
//	if (m_input->KB()->KeyUp(_event, SDLK_A))
//	{
//		m_zoomY += 0.5f;
//	}
//	else if (m_input->KB()->KeyUp(_event, SDLK_S))
//	{
//		m_zoomY -= 0.5f;
//	}
//}

void GameController::RunGame()
{
	Level* level = new Level;
	level->RunLevel();
	delete level;
}





/*

originally in HandleInput()

if ((m_input->GP()->Added(_event)) || (m_input->GP()->Removed(_event)))
	{
		m_gpInfo = m_input->GP()->ToString();
	}

	for (GamepadInfo* gp : m_input->GP()->GetGamepads())
	{
		if ((gp->ButtonDown(_event) != SDL_GAMEPAD_BUTTON_INVALID) || (gp->ButtonUp(_event) != SDL_GAMEPAD_BUTTON_INVALID) || (gp->ProcessMotion(_event)))
		{
			m_gpInfo = m_input->GP()->ToString();
		}
	}

	m_inputText = std::to_string(m_input->MS()->GetX()) + ", " + std::to_string(m_input->MS()->GetY());


originally in RunGame()

m_fArial20->Write(m_renderer->GetRenderer(), m_keyStates.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });
m_fArial20->Write(m_renderer->GetRenderer(), m_inputText.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 220 });
m_fArial20->Write(m_renderer->GetRenderer(), m_gpInfo.c_str(), SDL_Color{ 255, 0, 0 }, SDL_Point{ 250, 300 });

this as well for output 2 of lab 7

if (m_audio->GetMusic() != nullptr)
		{
			string musInfo = ("Music: Name- " + m_audio->GetMusicTitle());
			musInfo += ("       Position - " + m_audio->MusicPosition() + "/" + m_audio->GetMusicLength()).c_str();
			m_fArial20->Write(m_renderer->GetRenderer(), musInfo.c_str(), SDL_Color{ 0, 0, 255 }, { 50, 50 });
		}

		int count = 0;
		for (auto const& effect : m_audio->GetEffects())
		{
			string effInfo = format("Effect {}: Name - ", count);
			effInfo += (effect->m_name);
			m_fArial20->Write(m_renderer->GetRenderer(), effInfo.c_str(), SDL_Color{ 255, 0, 0 }, { 50, 100 + count * 30 });
			count++;
		}
*/


/*

all code from previous

Timing* t = &Timing::Instance();
	Renderer* r = &Renderer::Instance();
	r->Initialize();
	r->EnumerateDisplayModes();
	r->ChangeDisplayMode(&r->GetResolutions()[120], true); // 120 30

	TTFont* font = new TTFont();
	font->Initialize(20);

	AssetController::Instance().Initialize(10000000);
	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
	SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
	sheet->Load("../Assets/Textures/Warrior.tga");
	sheet->SetSize(17, 6, 69, 44);
	sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);

	RenderTarget* rt = new RenderTarget();
	rt->Create(NATIVE_XRES, NATIVE_YRES);

	//sheet->SetBlendMode(SDL_BLENDMODE_BLEND);
	//sheet->SetBlendAlpha(128);

	while (m_sdlEvent.type != SDL_EVENT_QUIT)
	{
		t->Tick();
		rt->Start();

		SDL_PollEvent(&m_sdlEvent);
		r->SetDrawColor(SDL_Color{ 255, 255, 255, 255 });
		r->ClearScreen();
		r->SetDrawColor(SDL_Color{ 0, 0, 0, 255 });
		r->RenderFillRectangle(SDL_FRect{0, 150, 69*3, 44*3});
		r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), SDL_FRect{ 0, 150, 69 * 3, 44 * 3 });


		std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
		font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });

		std::string res = "Resolution: " + std::to_string(r->GetWindowSize().x) + "x" + std::to_string(r->GetWindowSize().y);
		font->Write(r->GetRenderer(), res.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 20 });

		std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
		font->Write(r->GetRenderer(), s.c_str(), SDL_Color{0, 255, 0}, SDL_Point{250, 200});

		rt->Stop();
		r->SetDrawColor(SDL_Color{0, 0, 0, 255});
		r->ClearScreen();
		rt->Render(t->GetDeltaTime());
		SDL_RenderPresent(r->GetRenderer());

		if ((m_sdlEvent.type == SDL_EVENT_KEY_DOWN) && (m_sdlEvent.key.key == SDLK_ESCAPE))
		{
			break;
		}

		t->CapFPS();
	}

	delete rt;
	delete SpriteAnim::Pool;
	delete SpriteSheet::Pool;
	font->Shutdown();
	delete font;
	r->Shutdown();

*/