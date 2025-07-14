#include "amplitude.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Amplitude::_bind_methods() {
    ClassDB::bind_method(D_METHOD("process", "audio"), &Amplitude::process);
    ClassDB::bind_method(D_METHOD("get_amplitude"), &Amplitude::get_amplitude);
}

void Amplitude::process(const PackedVector2Array &audio) {
    int count = audio.size();
    if (count == 0) {
        last_amplitude = 0.0f;
        return;
    }

    float sum = 0.0f;
    for (int i = 0; i < count; i += 2) {
        float left = audio[i];
        float right = (i + 1 < count) ? audio[i + 1] : 0.0f;
        float mono = 0.5f * (left + right);
        sum += mono * mono;
    }

    last_amplitude = Math::sqrt(sum / (count / 2));
}

float Amplitude::get_amplitude() const {
    return last_amplitude;
}
