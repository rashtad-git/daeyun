#include "SoundManager.h"

#include <windows.h>

SoundManager::SoundManager()
{
	running = true;
	soundThread = std::thread(&SoundManager::SoundLoop, this);
}

SoundManager::~SoundManager()
{
	running = false;
	if (soundThread.joinable())
		soundThread.join();
}

void SoundManager::Play(int frequency, int duration)
{
	std::lock_guard<std::mutex> lock(beepMutex);
	beepQueue.push(std::make_pair(frequency, duration));
}

void SoundManager::SoundLoop()
{
	while (running)
	{
		std::pair<int, int> beep;
		bool hasBeep = false;

		{
			std::lock_guard<std::mutex> lock(beepMutex);
			if (!beepQueue.empty())
			{
				beep = beepQueue.front();
				beepQueue.pop();
				hasBeep = true;
			}
		}

		if (hasBeep)
		{
			Beep(beep.first, beep.second);
		}
		else
		{
			Sleep(1);
		}
	}
}
