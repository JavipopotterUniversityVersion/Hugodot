final A = preload("res://completion/class_a.notest.gd")

class LocalInnerClass:
    final AInner = preload("res://completion/class_a.notest.gd")
    enum LocalInnerInnerEnum {}
    class LocalInnerInnerClass:
        pass

enum LocalInnerEnum {}

var test_var: A➡
