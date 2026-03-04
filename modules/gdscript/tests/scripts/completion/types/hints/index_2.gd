final A = preload("res://completion/class_a.notest.gd")

class LocalInnerClass:
    final AInner = preload("res://completion/class_a.notest.gd")
    class InnerInnerClass:
        final AInnerInner = preload("res://completion/class_a.notest.gd")
        enum InnerInnerInnerEnum {}
        class InnerInnerInnerClass:
            pass
    enum InnerInnerEnum {}

enum TestEnum {}

var test_var: LocalInnerClass.InnerInnerClass.➡
