#include "bandpass.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>

using namespace godot;

// === BandpassInstance ===

void BandpassInstance::_bind_methods() {}

void BandpassInstance::calculate_coefficients(float sample_rate) {
	center_freq = base->get_center_freq();
	q_factor = base->get_q_factor();

	float omega = 2.0f * Math_PI * center_freq / sample_rate;
	float alpha = std::sin(omega) / (2.0f * q_factor);

	b0 = alpha;
	b1 = 0.0f;
	b2 = -alpha;
	float a0 = 1.0f + alpha;
	a1 = -2.0f * std::cos(omega);
	a2 = 1.0f - alpha;

	b0 /= a0;
	b1 /= a0;
	b2 /= a0;
	a1 /= a0;
	a2 /= a0;
}

void BandpassInstance::_process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count) {
	if (!base.is_valid()) return;

	const AudioFrame *src = static_cast<const AudioFrame *>(p_src_buffer);
	float sample_rate = 48000;//get_mix_rate();
	calculate_coefficients(sample_rate);

	for (int ch = 0; ch < 2; ch++) {
		x1[ch] = x2[ch] = y1[ch] = y2[ch] = 0.0f;
	}

	for (int i = 0; i < p_frame_count; i++) {
		for (int ch = 0; ch < 2; ch++) {
			float in = (ch == 0) ? src[i].left : src[i].right;

			if (std::fabs(in) < 1e-20f) {
				in += ((float)std::rand() / RAND_MAX - 0.5f) * 1e-10f;
			}

			float out = b0 * in + b1 * x1[ch] + b2 * x2[ch] - a1 * y1[ch] - a2 * y2[ch];

			x2[ch] = x1[ch];
			x1[ch] = in;
			y2[ch] = y1[ch];
			y1[ch] = out;

			if (ch == 0)
				p_dst_buffer[i].left = out;
			else
				p_dst_buffer[i].right = out;
		}
	}
}

// === Bandpassta ===

Bandpassta::Bandpassta() {
	center_freq = 440.0f;
	q_factor = 10.0f;
}

Bandpassta::~Bandpassta() {}

void Bandpassta::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_center_freq", "hz"), &Bandpassta::set_center_freq);
	ClassDB::bind_method(D_METHOD("get_center_freq"), &Bandpassta::get_center_freq);
	ClassDB::bind_method(D_METHOD("set_q_factor", "q"), &Bandpassta::set_q_factor);
	ClassDB::bind_method(D_METHOD("get_q_factor"), &Bandpassta::get_q_factor);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "center_freq", PROPERTY_HINT_RANGE, "10,20000,1,suffix:Hz"), "set_center_freq", "get_center_freq");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "q_factor", PROPERTY_HINT_RANGE, "0.1,50.0,0.1"), "set_q_factor", "get_q_factor");
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

Ref<AudioEffectInstance> Bandpassta::_instantiate() {
	Ref<BandpassInstance> inst;
	inst.instantiate();
	inst->base = Ref<Bandpassta>(this);
	return inst;
}
