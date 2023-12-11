#pragma once

struct ma_sound;

class Sound
{
public:
	Sound();
	~Sound();

	int init(const char* path);
	void terminate();

	int play();

	// set relative listener
	void set_position(float x, float y, float z);
	void set_direction(float x, float y, float z);
	void set_voulme(float voulme);
	void set_looping(bool looping);
	void set_positioning(bool isAbsolute);

private:
	ma_sound* m_sound;
};