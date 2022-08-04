#pragma once

#include "define.hpp"

#include "Resource.hpp"
#include "Audio.hpp"

NS_BEGIN

namespace detail {
template <SoundType Type>
class BaseSound {
public:
	[[nodiscard]] SoundType getType() const { return type; }

protected:
	unsigned int buffer;
	const static SoundType type = Type;
};
}

template <SoundType Type>
class Sound {

};

template<>
class Sound<SoundType::Sample> : public detail::Resource, public detail::BaseSound<SoundType::Sample> {
public:
	bool load(const std::filesystem::path &path) override;
	bool create() override;
};

using SoundSample = Sound<SoundType::Sample>;
using SoundSampleP = std::shared_ptr<SoundSample>;

NS_END