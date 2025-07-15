#include "bandpass.h"

using namespace godot;

// --- Bandpassta binding ---
void Bandpassta::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_center_freq", "hz"), &Bandpassta::set_center_freq);
    ClassDB::bind_method(D_METHOD("get_center_freq"), &Bandpassta::get_center_freq);
    ClassDB::bind_method(D_METHOD("set_q_factor", "q"), &Bandpassta::set_q_factor);
    ClassDB::bind_method(D_METHOD("get_q_factor"), &Bandpassta::get_q_factor);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "center_freq", PROPERTY_HINT_RANGE, "10,20000,1,suffix:Hz"), "set_center_freq", "get_center_freq");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "q_factor", PROPERTY_HINT_RANGE, "0.1,50.0,0.1"), "set_q_factor", "get_q_factor");
}

// --- BandpassInstance binding ---
void BandpassInstance::_bind_methods() {
    // No exposed methods currently, but binding function must exist
}

// --- Bandpassta implementation ---
Bandpassta::Bandpassta() {
    center_freq = 440.0f;
    q_factor = 10.0f;
}

void Bandpassta::set_center_freq(float freq) {
    center_freq = freq;
}

float Bandpassta::get_center_freq() const {
    return center_freq;
}

void Bandpassta::set_q_factor(float q) {
    q_factor = q;
}

float Bandpassta::get_q_factor() const {
    return q_factor;
}

Ref<AudioEffectInstance> Bandpassta::instantiate() {
    Ref<BandpassInstance> inst;
    inst.instantiate();

    // Assign this effect as base so instance can access params
    inst->base = Ref<Bandpassta>(this);

    return inst;
}

// --- BandpassInstance implementation ---

BandpassInstance::BandpassInstance() {
    // Initialize delay buffers to zero for both channels
    b0 = b1 = b2 = a1 = a2 = 0.0f;

    for (int ch = 0; ch < 2; ch++) {
        x1[ch] = 0.0f;
        x2[ch] = 0.0f;
        y1[ch] = 0.0f;
        y2[ch] = 0.0f;
    }
    sample_rate = 48000.0f;
}

void BandpassInstance::calculate_coefficients(float freq, float q) {
    float omega = 2.0f * Math_PI * freq / sample_rate;
    float alpha = std::sin(omega) / (2.0f * q);

    b0 = alpha;
    b1 = 0.0f;
    b2 = -alpha;
    float a0 = 1.0f + alpha;
    a1 = -2.0f * std::cos(omega);
    a2 = 1.0f - alpha;

    // Normalize coefficients by a0
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

void BandpassInstance::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
    if (base.is_null()) {
        return;
    }

    if (p_frame_count <= 0 || !p_src_frames || !p_dst_frames) {
        return;
    }

    // Update sample_rate if possible dynamically here, otherwise use default
    sample_rate = 48000.0f;

    calculate_coefficients(base->get_center_freq(), base->get_q_factor());

    for (int i = 0; i < p_frame_count; i++) {
        for (int ch = 0; ch < 2; ch++) {
            float x0 = (ch == 0) ? p_src_frames[i].left : p_src_frames[i].right;

            if (std::fabs(x0) < 1e-20f) {
                // Add tiny noise to avoid denormal numbers
                float dither = ((float)std::rand() / RAND_MAX - 0.5f) * 2.0f * 1e-10f;
                x0 += dither;
            }

            float y0 = b0 * x0 + b1 * x1[ch] + b2 * x2[ch] - a1 * y1[ch] - a2 * y2[ch];

            x2[ch] = x1[ch];
            x1[ch] = x0;
            y2[ch] = y1[ch];
            y1[ch] = y0;

            if (ch == 0) {
                p_dst_frames[i].left = y0;
            } else {
                p_dst_frames[i].right = y0;
            }
        }
    }
}
