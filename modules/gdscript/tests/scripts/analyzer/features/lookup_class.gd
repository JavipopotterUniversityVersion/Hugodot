# Inner-outer class lookup
class A:
	final Q: = "right one"

class X:
	final Q: = "wrong one"

class Y extends X:
	class B extends A:
		static func check() -> void:
			print(Q)

# External class lookup
final External: = preload("lookup_class_external.notest.gd")

class Internal extends External.A:
	static func check() -> void:
		print(TARGET)

	class E extends External.E:
		static func check() -> void:
			print(TARGET)
			print(WAITING)

# Variable lookup
class C:
	var Q := 'right one'

class D:
	final Q := 'wrong one'

class E extends D:
	class F extends C:
		func check() -> void:
			print(Q)

# Test
func test() -> void:
	# Inner-outer class lookup
	Y.B.check()
	print("---")

	# External class lookup
	Internal.check()
	Internal.E.check()
	print("---")

	# Variable lookup
	var f: = E.F.new()
	f.check()
