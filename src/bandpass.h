#ifndef BANDPASS_H
#define BANDPASS_H

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>

namespace godot {

class Bandpassta;

class BandpassInstance : public AudioEffectInstance {
	GDCLASS(BandpassInstance, AudioEffectInstance);
	friend class Bandpassta;

	Ref<Bandpassta> base;

	float center_freq;
	float q_factor;

	float b0, b1, b2, a1, a2;
	float x1[2], x2[2], y1[2], y2[2];

protected:
	static void _bind_methods();

	void calculate_coefficients(float sample_rate);

public:
	void _process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count) override;
};

class Bandpassta : public AudioEffect {
	GDCLASS(Bandpassta, AudioEffect);
	friend class BandpassInstance;

	float center_freq;
	float q_factor;

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> _instantiate() override;

	void set_center_freq(float freq);
	float get_center_freq() const;

	void set_q_factor(float q);
	float get_q_factor() const;

	Bandpassta();
	~Bandpassta();
};

}

#endif
