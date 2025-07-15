#pragma once

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>

namespace godot {

class Bandpassta;

class BandpassInstance : public AudioEffectInstance {
    GDCLASS(BandpassInstance, AudioEffectInstance);

private:
    friend class Bandpassta;

    // Filter coefficients
    float b0, b1, b2, a1, a2;

    // Delay buffers for 2 channels, initialized to zero in constructor
    float x1[2]{}, x2[2]{}, y1[2]{}, y2[2]{};

    float sample_rate;

    Ref<Bandpassta> base;

    void calculate_coefficients(float freq, float q);

public:
    BandpassInstance();  // Constructor initializes delay buffers and sample rate

    static void _bind_methods();

    virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
};

class Bandpassta : public AudioEffect {
    GDCLASS(Bandpassta, AudioEffect);

    friend class BandpassInstance;

private:
    float center_freq = 440.0f;
    float q_factor = 10.0f;

protected:
    static void _bind_methods();

public:
    Bandpassta();

    Ref<AudioEffectInstance> instantiate();

    void set_center_freq(float freq);
    float get_center_freq() const;

    void set_q_factor(float q);
    float get_q_factor() const;
};

}
