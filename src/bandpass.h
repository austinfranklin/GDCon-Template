#pragma once

#include <godot_cpp/classes/object.hpp>

namespace godot {

class Bandpass : public Object {
    GDCLASS(Bandpass, Object);

private:
    // Filter coefficients
    float b0, b1, b2, a1, a2;

    // State variables (filter delay history)
    float x1, x2; // previous inputs
    float y1, y2; // previous outputs

    // Parameters
    float sample_rate;
    float center_freq;
    float q_factor;

    float last_amplitude = 0.0f;

    void calculate_coefficients();

protected:
    static void _bind_methods();

public:
    Bandpass();

    // Parameters
    void set_params(float p_center_freq, float p_q_factor, float p_sample_rate);
    float get_amplitude() const;

    // Audio processing
    PackedFloat32Array process(const PackedFloat32Array &input);
};

}
