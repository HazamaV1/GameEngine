#include "WavDraw.h"
#include "AssetController.h"
#include "Renderer.h"
//
WAVDraw::WAVDraw()
{
	m_header = { };
	m_data = nullptr;
	m_wSize = { };
	m_stepSize = 0;
	m_wavData = nullptr;
	m_xPos = 0;
	m_prevLeft = { };
	m_prevRight = { };
}

WAVDraw::~WAVDraw()
{
}

bool WAVDraw::CompareFileExt(string _source, string _ext)
{
	std::transform(_source.begin(), _source.end(), _source.begin(), [](unsigned char c) {return std::tolower(c); });

	if (_source.length() >= _ext.length())
	{
		return (0 == _source.compare(_source.length() - _ext.length(), _ext.length(), _ext));
	}
	else
	{
		return false;
	}
}


void WAVDraw::IntegrityChecks(Asset* _rawWav)
{
	M_ASSERT(_rawWav->GetDataSize() > 44, "Not a valid WAV file");

	memcpy(&m_header, _rawWav->GetData(), sizeof(WAVHeader));

	string guid = _rawWav->GetGUID();
	M_ASSERT((CompareFileExt(guid, ".wav") == true), "Not a valid WAV file");
	M_ASSERT(string(m_header.RIFFHeader, 4) == "RIFF", "Not a valid WAV file");
	M_ASSERT(string(m_header.FMTHeader, 3) == "fmt", "Not a valid WAV file");
	M_ASSERT(string(m_header.DataHeader, 4) == "data", "Not a valid WAV file");
	M_ASSERT((m_header.AudioFormat == 1), "Only supports PCM format");
	M_ASSERT((m_header.NumChannels == 2), "Can only display 2 channels");
	M_ASSERT((m_header.BitDepth == 16), "Can only display 16bps");
}

void WAVDraw::DrawWave(Asset* _rawWav, Renderer* _renderer, float _yZoom)
{
	IntegrityChecks(_rawWav);

	m_wSize = _renderer->GetWindowSize();
	_renderer->SetDrawColor(SDL_Color{ 128, 128, 128, 255 });
	_renderer->RenderLine(SDL_FRect{ 0, (float)m_wSize.y / 4, (float)m_wSize.x, 0 });
	_renderer->RenderLine(SDL_FRect{ 0, (float)m_wSize.y * 3 / 4, (float)m_wSize.x, 0 });

	m_stepSize = m_header.DataBytes / m_header.SampleAlignment / m_wSize.x * 2;
	m_wavData = (short*)(_rawWav->GetData() + 44);
	m_xPos = 0;

	for (int count = 0; count < m_header.DataBytes; count += m_stepSize)
	{
		int leftYPos = *(m_wavData + count);
		leftYPos = (int)(((m_wSize.y / 4) + (float)leftYPos / m_wSize.y * -1 * _yZoom));
		int rightYPos = *(m_wavData + count + 1);
		rightYPos = (int)(((m_wSize.y * 3 / 4) + (float)rightYPos / m_wSize.y * -1 * _yZoom));

		if (count > 0)
		{
			_renderer->SetDrawColor(SDL_Color{ 255, 0, 0, 255 });
			_renderer->RenderLine(SDL_FRect{ (float)m_prevLeft.x, (float)m_prevLeft.y, (float)m_xPos - m_prevLeft.x, (float)leftYPos - m_prevLeft.y});

			_renderer->SetDrawColor(SDL_Color{ 0, 0, 255, 255 });
			_renderer->RenderLine(SDL_FRect{ (float)m_prevRight.x, (float)m_prevRight.y, (float)m_xPos - m_prevRight.x, (float)rightYPos - m_prevRight.y });
		}
		m_prevLeft = SDL_Point{ m_xPos, leftYPos };
		m_prevRight = SDL_Point{ m_xPos, rightYPos };
		m_xPos++;

		if (m_xPos > (int)m_wSize.x) break;
	}
}