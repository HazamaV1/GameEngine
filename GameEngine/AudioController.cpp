#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"

AudioController::AudioController()
{
	M_ASSERT(SDL_InitSubSystem(SDL_INIT_AUDIO) == true, "Failed to initialize SDL Audio Subsystem");
	M_ASSERT(MIX_Init() == true, "Failed to initialize SDL Mixer");
	m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	M_ASSERT((m_mixer != nullptr), "Failed to initialize Mix Audio");
	SoundEffect::Pool = new ObjectPool<SoundEffect>();
	m_music = nullptr;
	vector<TrackInfo*> m_effects;
}

AudioController::~AudioController()
{
	Shutdown();
}

SoundEffect* AudioController::LoadEffect(string _guid)
{
	SoundEffect* effect = SoundEffect::Pool->GetResource();
	effect->Load(_guid);
	return effect;
}

Song* AudioController::LoadSong(string _guid)
{
	SoundEffect* song = SoundEffect::Pool->GetResource();
	song->Load(_guid);
	return static_cast<Song*>(song);
}

MIX_Audio* AudioController::GetSDLAudio(SoundEffect* _effect)
{
	Asset* asset = _effect->GetData();
	string guid = asset->GetGUID();
	if (m_audio.count(guid) == 0)
	{
		SDL_IOStream* stream = SDL_IOFromConstMem((void*)asset->GetData(), asset->GetDataSize());
		MIX_Audio* audio = MIX_LoadAudio_IO(m_mixer, stream, true, true);

		m_audio[guid] = audio;
	}

	return m_audio[guid];
}

void SDLCALL TrackStoppedCallback(void* _userdata, MIX_Track* _track)
{
	if (_track == AudioController::Instance().GetMusic())
	{
		AudioController::Instance().StopMusic();
	}
	TrackInfo* effect = AudioController::Instance().FindTrack(_track); // Kept this so when space is pressed twice, program doesnt exit
	if (effect != nullptr)
	{
		AudioController::Instance().StopEffect(_track);
	}
}

TrackInfo* AudioController::FindTrack(MIX_Track* _track)
{
	vector<TrackInfo*>& effects = GetEffects();
	auto it = std::find_if(effects.begin(), effects.end(), [_track](TrackInfo* info) {
		return info->m_track == _track;
	});
	return (it != effects.end()) ? *it : nullptr;
}

void AudioController::PlayTrack(MIX_Track* _track, SoundEffect* _effect)
{
	M_ASSERT(MIX_SetTrackAudio(_track, GetSDLAudio(_effect)) == true, "Failed to set SFX audio");
	SDL_PropertiesID props = SDL_CreateProperties();
	M_ASSERT(SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, 0) == true, "Failed to set SFX loop property");
	M_ASSERT(MIX_PlayTrack(_track, props) == true, "Failed to play SFX");
	M_ASSERT(MIX_SetTrackStoppedCallback(_track, TrackStoppedCallback, nullptr) == true, "Failed to set SFX stopped callback");
}

void AudioController::Play(SoundEffect* _effect)
{
	if (m_effects.size() >= MaxEffectTracks) return;
	TrackInfo* trackInfo = new TrackInfo();
	trackInfo->m_track = MIX_CreateTrack(m_mixer);
	trackInfo->m_name = _effect->GetData()->GetGUID();
	m_effects.push_back(trackInfo);
	M_ASSERT(trackInfo->m_track != nullptr, "Failed to create SFX track");
	PlayTrack(trackInfo->m_track, _effect);
}

void AudioController::Play(Song* _song)
{
	if (m_music != nullptr)
	{
		StopMusic();
	}
	m_music = MIX_CreateTrack(m_mixer);
	M_ASSERT(m_music != nullptr, "Failed to create SFX track");
	PlayTrack(m_music, _song);

	MIX_Audio* audio = MIX_GetTrackAudio(GetMusic());
	Sint64 duration = MIX_GetAudioDuration(audio);
	duration = MIX_AudioFramesToMS(audio, duration);
	m_musicLength = to_string(duration / 1000);
	SDL_PropertiesID props = MIX_GetAudioProperties(audio);
	m_musicTitle = SDL_GetStringProperty(props, MIX_PROP_METADATA_TITLE_STRING, "Unknown Title");
}

void AudioController::StopEffect(MIX_Track* _track)
{
	if (_track == nullptr) return;
	MIX_StopTrack(_track, 0);
	MIX_DestroyTrack(_track);
	TrackInfo* effect = FindTrack(_track);
	if (effect != nullptr)
	{
		auto it = std::find(m_effects.begin(), m_effects.end(), effect);
		if (it != m_effects.end())
		{
			m_effects.erase(it);
		}
		delete effect;
	}
}

string AudioController::MusicPosition()
{
	if (m_music != nullptr)
	{
		Sint64 pos = MIX_GetTrackPlaybackPosition(m_music);
		pos = MIX_TrackFramesToMS(m_music, pos);
		return to_string(pos / 1000);
	}
	return "";
}

void AudioController::StopMusic()
{
	if (m_music == nullptr) return;
	MIX_StopTrack(m_music, 0);
	MIX_DestroyTrack(m_music);
	m_music = nullptr;
	m_musicLength = "";
	m_musicTitle = "";
}

void AudioController::PauseMusic()
{
	if (m_music == nullptr) return;
	MIX_PauseTrack(m_music);
}

void AudioController::ResumeMusic()
{
	if (m_music == nullptr) return;
	MIX_ResumeTrack(m_music);
}

void AudioController::Shutdown()
{
	for (auto ie = m_audio.begin(); ie != m_audio.end(); ie++)
	{
		MIX_DestroyAudio(ie->second);
	}
	m_audio.clear();

	if (m_music != nullptr)
	{
		MIX_DestroyTrack(m_music);
		m_music = nullptr;
	}

	/*if (m_effect != nullptr)
	{
		MIX_DestroyTrack(m_effect);
		m_effect = nullptr;
	}*/

	if (m_mixer != nullptr)
	{
		MIX_DestroyMixer(m_mixer);
		m_mixer = nullptr;
	}

	delete SoundEffect::Pool;
	SoundEffect::Pool = nullptr;

	MIX_Quit();
	SDL_Quit();
}