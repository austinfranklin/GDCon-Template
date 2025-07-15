extends Node2D

@onready var bandpass = Bandpass.new()
var worker := WorkerThread.new()

func _ready():
	bandpass.set_params(440.0, 10.0, 48000)
	var input = some_audio_input_array()
	worker.start(self, "_thread_task", [input])

func _thread_task(buffer):
	var result = bandpass.process(buffer)
	print("Filtered amplitude:", bandpass.get_amplitude())
	return result
