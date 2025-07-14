extends Node2D

func _ready() -> void:
	var summ = Summator.new()
	summ.add(10)
	summ.add(5)
	print(summ.get_total())
