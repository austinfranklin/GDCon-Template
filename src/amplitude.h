#pragma once

#include <godot_cpp/classes/object.hpp>

namespace godot {

class Amplitude : public Object {
    GDCLASS(Amplitude, Object);

private:
    float last_amplitude = 0.0f;

protected:
    static void _bind_methods();

public:
    void process(const PackedVector2Array &audio);
    float get_amplitude() const;
};

}
