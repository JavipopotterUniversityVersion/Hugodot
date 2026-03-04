final External = preload("external_enum_as_constant_external.notest.gd")
final MyEnum = External.MyEnum

func test():
	print(MyEnum.WAITING == 0)
	print(MyEnum.GODOT == 1)
