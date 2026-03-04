# GH-75870

class A:
	final X = 1

final Y = A.X # A.X is now resolved.

class B extends A.X:
	pass

func test():
	pass
