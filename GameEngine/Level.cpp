#include "Level.h"
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
#include "WavDraw.h"
#include <ctime>
//
Level::Level()
{
	m_sdlEvent = { };
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	m_input = nullptr;
	m_quit = false;
	m_audio = nullptr;
	m_effect = nullptr;
	m_song = nullptr;
	m_sprite = nullptr;
	t = nullptr;
	m_units.clear();
}

Level::~Level()
{
	m_units.clear();
	delete SoundEffect::Pool;
	delete Unit::Pool;
	delete m_fArial20;
	m_fArial20 = nullptr;
	delete SpriteSheet::Pool;
	AssetController::Instance().Clear(); // Free 10 MB
}

void Level::AssignNonDefaultValues()
{
	for (int count = 0; count < 5; count++)
	{
		Unit* unit = Unit::Pool->GetResource();
		unit->AssignNonDefaultValues();
		m_units.push_back(unit);
	}

	Resource::AssignNonDefaultValues();
}

void Level::Initialize()
{
	AssetController::Instance().Initialize(10000000);
	Unit::Pool = new ObjectPool<Unit>();
	SoundEffect::Pool = new ObjectPool<SoundEffect>();
	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
	t = &Timing::Instance();
	m_renderer = &Renderer::Instance();
	m_renderer->Initialize();
	m_input = &InputController::Instance();
	m_input->Initialize(m_renderer->GetWindow());
	m_fArial20 = new TTFont();
	m_fArial20->Initialize(20);
	m_audio = &AudioController::Instance();
	m_effect = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	m_player = Unit::Pool->GetResource();
	m_player->createPlayer((float)m_renderer->GetWindowSize().x,(float)m_renderer->GetWindowSize().y );
	m_npcSpeed = 60;
	
	srand(static_cast<unsigned int>(time(NULL)));
	glm::vec2 playerPos = m_player->getPosition();

	for (int i = 0; i < 10; i++)
	{
		Unit* npcs = Unit::Pool->GetResource();
		npcs->npcInitialize();
		float npcRandomAngle = rand() % 360;
		float npcRadians = npcRandomAngle * (3.14159f / 180.0f);
		float xOffset = playerPos.x + 150 * cos(npcRadians);
		float yOffset = playerPos.y + 150 * sin(npcRadians);
		glm::vec2 npcPos = { xOffset, yOffset };
		npcs->setPosition(npcPos);
		m_units.push_back(npcs);
	}
	
}

void Level::Serialize(std::ostream& _stream)
{

	int numberOfUnits = m_units.size();
	_stream.write(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));
	for (int count = 0; count < numberOfUnits; count++)
	{
		SerializePointer(_stream, m_units[count]);
	}

	Resource::Serialize(_stream);
}

void Level::Deserialize(std::istream& _stream)
{

	int numberOfUnits;
	_stream.read(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));
	for (int count = 0; count < numberOfUnits; count++)
	{
		Unit* unit;
		DeserializePointer(_stream, unit);
		m_units.push_back(unit);
	}

	Resource::Deserialize(_stream);
}

void Level::ToString()
{
	cout << "LEVEL" << endl;
	for (int count = 0; count < m_units.size(); count++)
	{
		m_units[count]->ToString();
	}
	Resource::ToString();
}

void Level::HandleInput(SDL_Event _event)
{
	m_input->Process();

	string temp;
	if ((m_sdlEvent.type == SDL_EVENT_QUIT) ||
		(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
	{
		m_quit = true;
	}

	if (m_npcSpeed < 60)
	{
		if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_I))
		{
			m_npcSpeed += 10;
		}
	}
	if (m_npcSpeed > 0)
	{
		if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_D))
		{
			m_npcSpeed -= 10;
		}
	}
}

void Level::RunLevel()
{
	Initialize();

	while (!m_quit)
	{
		t->Tick();
		glm::vec2 playerPos = m_player->getPosition();
		m_renderer->SetDrawColor({ 255, 255, 255, 255 });
		m_renderer->ClearScreen();
		while (SDL_PollEvent(&m_sdlEvent))
		{
			HandleInput(m_sdlEvent);
		}

		float playerDirection;
		float playerX;
		if (m_player->getDirection() == true)
		{
			playerDirection = -1;
			playerX = playerPos.x + 69.0f * 1.25f;
		}
		else
		{
			playerDirection = 1;
			playerX = playerPos.x;
		}

		m_player->updatePlayer(m_sdlEvent, m_input, t->GetDeltaTime());

		if (m_player->getUnitMoving() == false)
		{
			m_renderer->RenderTexture(m_player->getUnitSprite(), m_player->getUnitSprite()->Update(EN_AN_IDLE, t->GetDeltaTime()), SDL_FRect{playerX, playerPos.y, playerDirection * 69.0f * 1.25f, 44.0f * 1.25f});
		}
		else 
		{
			m_renderer->RenderTexture(m_player->getUnitSprite(), m_player->getUnitSprite()->Update(EN_AN_RUN, t->GetDeltaTime()), SDL_FRect{playerX, playerPos.y, playerDirection * 69.0f * 1.25f, 44.0f * 1.25f});
		}

		for (int i = 0; i < m_units.size(); i++)
		{
			m_units[i]->updateNpc(t->GetDeltaTime(), playerPos, m_npcSpeed);

			if (m_units[i]->getUnitMoving() == true)
			{
				m_renderer->RenderTexture(m_units[i]->getNpcSprite(), m_units[i]->getNpcSprite()->Update(EN_AN_RUN, t->GetDeltaTime()), SDL_FRect{ m_units[i]->getPosition().x, m_units[i]->getPosition().y, 69.0f * 1.25f, 44.0f * 1.25f });
			}
			else
			{
				m_renderer->RenderTexture(m_units[i]->getNpcSprite(), m_units[i]->getNpcSprite()->Update(EN_AN_IDLE, t->GetDeltaTime()), SDL_FRect{ m_units[i]->getPosition().x, m_units[i]->getPosition().y, 69.0f * 1.25f, 44.0f * 1.25f });
			}
		}

		SDL_RenderPresent(m_renderer->GetRenderer());
	}
}







//void Level::Level1()
//{
//	srand(static_cast<unsigned int>(time(NULL)));
//	Timing* t = &Timing::Instance();
//	Renderer* r = &Renderer::Instance();
//	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
//	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
//
//	SpriteSheet* spriteSheets[20];
//	float movementSpeed[20];
//	float xPos[20] = { 0 };
//	int curLvl = 1;
//	float yPosLvl2[20] = { 0 };
//
//	for (int a = 0; a < 20; a++)
//	{
//		spriteSheets[a] = SpriteSheet::Pool->GetResource();
//		if (a < 10) {
//			spriteSheets[a]->Load("../Assets/Textures/Warrior.tga");
//			spriteSheets[a]->SetSize(17, 6, 69, 44);
//			spriteSheets[a]->AddAnimation(EN_AN_RUN, 6, 8, 1.0f);
//		}
//		else
//		{
//			spriteSheets[a]->Load("../Assets/Textures/Rock.tga");
//			yPosLvl2[a] = -10;
//			spriteSheets[a]->SetSize(1, 1, 16, 16);
//			spriteSheets[a]->AddAnimation(EN_AN_RUN, 0, 1, 0.0f);
//		}
//
//
//		movementSpeed[a] = 80.0f + (static_cast<float>(rand() % 201) / 10.0f);
//	}
//
//	bool serializedProperly = false;
//
//	static std::string autoSave = "Auto Save: No";
//	float yPos = 10;
//	float stride = 100.0f; // Ai logic part
//	float totalFrames = 8.0f; // Ai logic part
//	t->Tick();
//
//		r->SetDrawColor(SDL_Color{ 128, 128, 128, 128 });
//
//		r->ClearScreen();
//		for (int i = 0; i < (curLvl * 10); i++) {
//			float animSpeed = (movementSpeed[i] / stride) * totalFrames;
//			//((movementSpeed[i] - 80.0f) / (100.0f - 80.0f)) * ((6.0f - 4.8f) + 4.8f));
//			//if (i < 10) // Level 1 section
//			//{
//			xPos[i] += movementSpeed[i] * t->GetDeltaTime();
//			r->RenderTexture(spriteSheets[i], spriteSheets[i]->Update(EN_AN_RUN, t->GetDeltaTime() * animSpeed), SDL_FRect{ xPos[i], yPos, 69 * 1.8, 44 * 1.8 });
//			yPos += 100;
//			if (xPos[i] > 1920) //&& curLvl == 1)
//			{
//				curLvl += 1;
//				for (int r = 0; r < 10; r++)
//				{
//					xPos[r] = 0.0f;
//				}
//				t->ResetTime();
//				serializedProperly = false;
//				autoSave = "Auto Save: No";
//			}
//			//	/*else if (xPos[i] > 1920 && curLvl == 2)
//			//	{
//			//		m_sdlEvent.type = SDL_EVENT_QUIT;
//			//		break;
//			//	}*/
//
//			//}
//			//else // Level 2 section
//			//{
//			//	float xRockPos = 50.0f + ((i - 10) * 180.0f);
//			//	yPosLvl2[i] += movementSpeed[i] * t->GetDeltaTime();
//
//			//	/*for (int w = 0; w < 10; w++)
//			//	{
//			//		SDL_FRect warrior = { xPos[w], yPos, 69 * 1.8f, 44 * 1.8f };
//			//		for (int roc = 10; roc < 20; roc++)
//			//		{
//			//			SDL_FRect rocc = { xRockPos, yPosLvl2[roc], 15, 20 };
//
//			//			if (SDL_HasRectIntersectionFloat(&warrior, &rocc))
//			//			{
//			//				SpriteSheet::Pool->ReleaseResource(spriteSheets[w]);
//			//				SpriteSheet::Pool->ReleaseResource(spriteSheets[roc]);
//
//			//				spriteSheets[w] = nullptr;
//			//				spriteSheets[roc] = nullptr;
//
//			//				break;
//			//			}
//			//		}
//			//	}*/
//
//			//	r->RenderTexture(spriteSheets[i], spriteSheets[i]->Update(EN_AN_RUN, 0), SDL_FRect{ xRockPos, yPosLvl2[i], 15, 20 });
//			//}
//		}
//
//		if (t->GetTimeInSeconds() >= 5 && !serializedProperly)
//		{
//			ofstream writeStream("level1.bin", ios::out | ios::binary);
//			for (int i = 0; i < 10; i++) {
//				spriteSheets[i]->Serialize(writeStream);
//			}
//			writeStream.close();
//			ifstream readStream("level1.bin", ios::in | ios::binary);
//			for (int i = 0; i < 10; i++)
//			{
//				spriteSheets[i]->Deserialize(readStream);
//			}
//			readStream.close();
//		}
//
//		autoSave = "Auto Save: Yes";
//
//		serializedProperly = true;
//	
//}