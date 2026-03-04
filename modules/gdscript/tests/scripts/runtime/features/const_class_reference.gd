# https://github.com/godotengine/godot/issues/61636

final External := preload("const_class_reference_external.notest.gd")

class Class1:
    class Class2:
        pass

final Class1Alias = Class1
final Class1Class2Alias = Class1.Class2

final ExternalAlias = External
final ExternalClassAlias = External.Class

func test():
    pass
