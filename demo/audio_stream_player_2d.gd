#extends AudioStreamPlayer2D
#
#var capture := AudioEffectCapture.new()
#var detector := Amplitude.new()
#var filter = Bandpass.new()
#
#func _ready():
	#var bus_index = AudioServer.get_bus_index("Master")
	#AudioServer.add_bus_effect(bus_index, capture)
	#
	#filter.set_params(440, 12, 44100) # 1kHz center, Q=1, 44100Hz sample rate
	#
	#self.volume_db = 0.0
	#self.play()
	#set_process(true)
#
#func _process_audio(_delta):
	#if capture.get_frames_available() > 0:
		#var buffer: PackedVector2Array = capture.get_buffer(capture.get_frames_available())
		#var interleaved := convert_to_interleaved(buffer)
		#
		##detector.process(interleaved)
		##print(detector.get_amplitude())
#
		#var _filtered_audio = filter.process(interleaved)
		#var rms_amp = filter.get_amplitude()
		#print("Filtered RMS amplitude: ", rms_amp)
#
#func convert_to_interleaved(stereo: PackedVector2Array) -> PackedFloat32Array:
	#var result := PackedFloat32Array()
	#for frame in stereo:
		#result.append(frame.x)  # Left
	#return result
