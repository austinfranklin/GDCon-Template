#include "bandpass.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Bandpass::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_params", "center_freq", "q_factor", "sample_rate"), &Bandpass::set_params);
    ClassDB::bind_method(D_METHOD("get_amplitude"), &Bandpass::get_amplitude);
    ClassDB::bind_method(D_METHOD("process", "input"), &Bandpass::process);
}

Bandpass::Bandpass() {
    sample_rate = 44100.0f;
    center_freq = 440.0f;
    q_factor = 10.0f;
    last_amplitude = 0.0f;

    x1 = x2 = y1 = y2 = 0.0f;

    calculate_coefficients();
}

void Bandpass::set_params(float p_center_freq, float p_q_factor, float p_sample_rate) {
    center_freq = p_center_freq;
    q_factor = p_q_factor;
    sample_rate = p_sample_rate;
    calculate_coefficients();
}

void Bandpass::calculate_coefficients() {
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

float Bandpass::get_amplitude() const {
    return last_amplitude;
}

PackedFloat32Array Bandpass::process(const PackedFloat32Array &input) {
    PackedFloat32Array output;
    output.resize(input.size());

    float sum_amplitude = 0.0f;
    int size = input.size();

    for (int i = 0; i < size; i++) {
        float x0 = input[i];

        // Dither to avoid denormals
        if (std::fabs(x0) < 1e-20f) {
            float dither = ((float)std::rand() / RAND_MAX - 0.5f) * 2.0f * 1e-20f;
            x0 += dither;
        }

        float y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        y0 += 1e-20f; // Force flush of denormals

        output[i] = y0;

        if (std::fabs(x0) > 1e-6f) {
            sum_amplitude += std::fabs(y0);
        }

        // Detect instability
        if (std::fabs(y1) < 1e-20f || std::fabs(y2) < 1e-20f) {
            UtilityFunctions::print("Warning: Filter entering denormal range");
        }

        // Shift delay history
        x2 = x1;
        x1 = x0;
        y2 = y1;
        y1 = y0;
    }

    last_amplitude = (size > 0) ? (sum_amplitude / size) : 0.0f;
    return output;
}
