# See also `parser-warnings/shadowed-constant.gd`.
final TEST = 25


func test():
	# Error here (trying to set a new value to a constant).
	TEST = 50
