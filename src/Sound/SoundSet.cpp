#include "EngineCore/Sound/SoundSet.h"

#include "EngineCore/Sound/Sound.h"
#include "EngineCore/System/SysFunc.h"

SoundSet::SoundSet()
{
}

SoundSet::~SoundSet()
{
	for (auto& i : m_sounds)
	{
		i.release();
	}
}

void SoundSet::terminate()
{
	for (auto& i : m_sounds)
	{
		i->terminate();
	}
}

void SoundSet::play(size_t index)
{
	m_sounds[index]->play();
}

void SoundSet::play_random()
{
	int i = sysfunc::get_random(0, m_sounds.size() - 1);
	m_sounds[i]->play();
}

void SoundSet::add_sound(std::unique_ptr<Sound> sound)
{
	m_sounds.push_back(std::move(sound));
}
