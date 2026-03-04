class InnerClass:
	enum InnerEnum {A = 2}
	final INNER_CONST = "INNER_CONST"

enum Enum {A = 1}

final Other = preload("./local_const_as_type.notest.gd")

func test():
	final IC = InnerClass
	final IE = IC.InnerEnum
	final E = Enum
	# Doesn't work in CI, but works in the editor. Looks like an unrelated bug. TODO: Investigate it.
	# Error: Invalid call. Nonexistent function 'new' in base 'GDScript'.
	var a1: IC = null # IC.new()
	var a2: IE = IE.A
	var a3: IC.InnerEnum = IE.A
	var a4: E = E.A
	print(a1.INNER_CONST)
	print(a2)
	print(a3)
	print(a4)

	final O = Other
	final OV: Variant = Other # Removes metatype.
	final OIC = O.InnerClass
	final OIE = OIC.InnerEnum
	final OE = O.Enum
	var b: O = O.new()
	@warning_ignore("unsafe_method_access")
	var bv: OV = OV.new()
	var b1: OIC = OIC.new()
	var b2: OIE = OIE.A
	var b3: O.InnerClass.InnerEnum = OIE.A
	var b4: OE = OE.A
	print(b.CONST)
	print(bv.CONST)
	print(b1.INNER_CONST)
	print(b2)
	print(b3)
	print(b4)
