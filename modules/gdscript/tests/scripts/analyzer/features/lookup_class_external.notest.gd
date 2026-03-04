class A:
	final TARGET: = "wrong"

	class B:
		final TARGET: = "wrong"
		final WAITING: = "godot"

		class D extends C:
			pass

class C:
	final TARGET: = "right"

class E extends A.B.D:
	pass
