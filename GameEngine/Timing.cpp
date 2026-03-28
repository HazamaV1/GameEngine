#include "Timing.h"

Timing::Timing()
{
	m_fpsCount = 0;
	m_fpsLast = 0;
	m_deltaTime = 0;
	m_currentTime = SDL_GetTicks();
	m_lastTime = m_currentTime;
	m_fpsStart = m_currentTime;
	m_baseTime = m_currentTime;
	SetFPS(80);
}

void Timing::Tick()
{
	m_currentTime = SDL_GetTicks();
	m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;
	if (m_fpsStart + 1000 <= m_currentTime)
	{
		m_fpsLast = m_fpsCount;
		m_fpsCount = 0;
		m_fpsStart = m_currentTime;
	}
	else
	{
		m_fpsCount++;
	}
	m_lastTime = m_currentTime;

	m_Seconds = (m_currentTime - m_baseTime) / 1000;
}

void Timing::SetFPS(int _fps) {
	m_targetFPS = _fps;
	m_ticksPerFrame = 1000 / m_targetFPS;
}

void Timing::CapFPS() {
	unsigned int delta = SDL_GetTicks() - m_currentTime;
	if (delta < m_ticksPerFrame) {
		SDL_Delay(m_ticksPerFrame - delta);
	}
}

void Timing::ResetTime()
{
	m_currentTime = SDL_GetTicks();
	m_baseTime = m_currentTime;
	m_Seconds = 0;
}