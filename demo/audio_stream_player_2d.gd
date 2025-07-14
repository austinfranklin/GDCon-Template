extends AudioStreamPlayer2D

var capture := AudioEffectCapture.new()
var detector := Amplitude.new()

func _ready():
	var bus_index = AudioServer.get_bus_index("Master")
	AudioServer.add_bus_effect(bus_index, capture)

	self.volume_db = 0.0
	self.play()
	set_process(true)

func _process(_delta):
	if capture.get_frames_available() > 0:
		var stereo_buffer: PackedVector2Array = capture.get_buffer(capture.get_frames_available())
		var mono := get_mono_buffer(stereo_buffer)
		detector.process(mono)
		var amp = detector.get_amplitude()
		print("Amplitude: ", amp)

func get_mono_buffer(stereo_buffer: PackedVector2Array) -> PackedFloat32Array:
	var mono_buffer := PackedFloat32Array()
	for frame in stereo_buffer:
		var mono_sample = 0.5 * (frame.x + frame.y)
		mono_buffer.append(mono_sample)
	return mono_buffer
