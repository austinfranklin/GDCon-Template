#include "amplitude.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Amplitude::_bind_methods() {
    ClassDB::bind_method(D_METHOD("process", "audio"), &Amplitude::process);
    ClassDB::bind_method(D_METHOD("get_amplitude"), &Amplitude::get_amplitude);
}

void Amplitude::process(const PackedFloat32Array &audio) {
    int count = audio.size();
    if (count == 0) {
        last_amplitude = 0.0f;
        return;
    }

    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        float left = audio[i];
        sum += left * left; // Square for RMS
    }

    last_amplitude = sqrt(sum / count) / 0.418f;
}

float Amplitude::get_amplitude() const {
    return last_amplitude;
}
