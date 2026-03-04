enum LocalNamed { VALUE_A, VALUE_B, VALUE_C = 42 }

func test():
	final P = preload("../features/enum_from_outer.gd")
	var x: LocalNamed
	x = P.Named.VALUE_A
