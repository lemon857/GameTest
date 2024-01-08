#pragma once

#include <memory>
#include <vector>

class Sound;

class SoundSet
{
public:
	SoundSet();
	~SoundSet();

	void terminate();

	void play(size_t index);
	void play_random();

	void add_sound(std::unique_ptr<Sound> sound);

private:
	std::vector<std::unique_ptr<Sound>> m_sounds;
};