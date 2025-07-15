#ifndef MYAMP_H
#define MYAMP_H

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>

namespace godot {

class MyAmp;

class MyAmpInstance : public AudioEffectInstance {
	GDCLASS(MyAmpInstance, AudioEffectInstance);
	friend class MyAmp;
	Ref<MyAmp> base;

	float mix_volume_db;

protected:
	static void _bind_methods();

public:
	void _process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count) override;
};

class MyAmp : public AudioEffect {
	GDCLASS(MyAmp, AudioEffect);

	friend class MyAmpInstance;
	float volume_db;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> _instantiate() override;
	void set_volume_db(float p_volume);
	float get_volume_db() const;

	MyAmp();
    ~MyAmp();
};

}

#endif