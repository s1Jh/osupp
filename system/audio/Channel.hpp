#pragma once

#include "define.hpp"

#include "Sound.hpp"
#include "Audio.hpp"

NS_BEGIN

class AudioDevice;

class Channel
{
	friend class AudioDevice;
public:
	Channel() = default;

	template <SoundType Type>
	bool setSound(std::shared_ptr<Sound<Type>> resource, SoundPriority priority = SoundPriority::Medium)
	{
		if (currentPriority > priority)
			return false;

		if constexpr (Type == SoundType::Sample) {

		} else if constexpr(Type == SoundType::Stream) {

		} else if constexpr(Type == SoundType::Record) {

		}
	}

	[[nodiscard]] ChannelState getState() const;
	[[nodiscard]] SoundPriority getSoundPriority() const;

	void play();
	void pause();
	void stop();
	float seek(float position);
	void setVolume(float fraction, float transitionTime = 0);
	void setLooping(bool isLooping);

private:
	void update();

	unsigned int ALSource;
	SoundPriority currentPriority;
	ChannelState state;
};

NS_END
