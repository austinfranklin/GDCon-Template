#include "my_amp.hpp"

using namespace godot;

void MyAmpInstance::_process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count)  {
	//multiply volume interpolating to avoid clicks if this changes
	float volume_db = base->volume_db;
	float vol = Math::db2linear(mix_volume_db);
	float vol_inc = (Math::db2linear(volume_db) - vol) / float(p_frame_count);
    const AudioFrame *src = static_cast<const AudioFrame*>(p_src_buffer);
	for (int i = 0; i < p_frame_count; i++) {
		p_dst_buffer[i].left = src[i].left * vol;
		p_dst_buffer[i].right = src[i].right * vol;
		vol += vol_inc;
	}
	//set volume for next mix
	mix_volume_db = volume_db;
}
void MyAmpInstance::_bind_methods() {
}

Ref<AudioEffectInstance> MyAmp::_instantiate() {
	Ref<MyAmpInstance> ins;
	ins.instantiate();
	ins->base = Ref<MyAmp>(this);
	ins->mix_volume_db = volume_db;
	return ins;
}

void MyAmp::set_volume_db(float p_volume) {
	volume_db = p_volume;
}

float MyAmp::get_volume_db() const {
	return volume_db;
}

void MyAmp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_volume_db", "volume"), &MyAmp::set_volume_db);
	ClassDB::bind_method(D_METHOD("get_volume_db"), &MyAmp::get_volume_db);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db", "get_volume_db");
}

MyAmp::MyAmp() {
	volume_db = 0;
}

MyAmp::~MyAmp() {
}