// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// TRANSITION, MSVC and EDG haven't implemented intrinsics needed for P2255R2.
#if defined(__clang__) && !defined(__EDG__)
#include <type_traits>

using namespace std;

enum class conv_explicity : bool {
    no,
    yes,
};

template <conv_explicity Explicity>
struct to_int_lvalue {
    static inline int n = 0;
    explicit(static_cast<bool>(Explicity)) operator int&() const {
        return n;
    }
};

template <conv_explicity Explicity>
struct to_int_xvalue {
    static inline int n = 0;
    explicit(static_cast<bool>(Explicity)) operator int&&() const {
        return static_cast<int&&>(n);
    }
};

template <class NonTempBindable>
void test_one_non_temporary_bindable_type() {
    static_assert(!reference_constructs_from_temporary<NonTempBindable, void>::value);

    static_assert(!reference_constructs_from_temporary<NonTempBindable, void()>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, void() const&>::value);

    static_assert(!reference_constructs_from_temporary<NonTempBindable, int>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, const int>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, int[]>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, const int[]>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, int[42]>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, const int[42]>::value);

    static_assert(!reference_constructs_from_temporary<NonTempBindable, int&>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, int&&>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, const int&>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, const int&&>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, void (&)()>::value);
    static_assert(!reference_constructs_from_temporary<NonTempBindable, void (&&)()>::value);

    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, void>);

    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, void()>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, void() const&>);

    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, int>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, const int>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, int[]>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, const int[]>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, int[42]>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, const int[42]>);

    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, int&>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, int&&>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, const int&>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, const int&&>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, void (&)()>);
    static_assert(!reference_constructs_from_temporary_v<NonTempBindable, void (&&)()>);
}

void test_non_temporary_bindable_types() {
    test_one_non_temporary_bindable_type<void>();

    test_one_non_temporary_bindable_type<void()>();
    test_one_non_temporary_bindable_type<void() const&>();

    test_one_non_temporary_bindable_type<int>();
    test_one_non_temporary_bindable_type<to_int_lvalue<conv_explicity::yes>>();

    test_one_non_temporary_bindable_type<int&>();
    test_one_non_temporary_bindable_type<to_int_lvalue<conv_explicity::yes>&>();

    test_one_non_temporary_bindable_type<const volatile int&>();
    test_one_non_temporary_bindable_type<const volatile to_int_lvalue<conv_explicity::yes>&>();

    test_one_non_temporary_bindable_type<void (&)()>();
    test_one_non_temporary_bindable_type<void (&&)()>();
}

template <class Obj>
void test_one_cvref_to_object() {
    static_assert(!reference_constructs_from_temporary<Obj&, Obj>::value);
    static_assert(!reference_constructs_from_temporary<Obj&, const Obj>::value);
    static_assert(!reference_constructs_from_temporary<Obj&, volatile Obj>::value);
    static_assert(!reference_constructs_from_temporary<Obj&, const volatile Obj>::value);
    static_assert(reference_constructs_from_temporary<const Obj&, Obj>::value);
    static_assert(reference_constructs_from_temporary<const Obj&, const Obj>::value);
    static_assert(reference_constructs_from_temporary<const Obj&, volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<const Obj&, const volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(!reference_constructs_from_temporary<volatile Obj&, Obj>::value);
    static_assert(!reference_constructs_from_temporary<volatile Obj&, const Obj>::value);
    static_assert(!reference_constructs_from_temporary<volatile Obj&, volatile Obj>::value);
    static_assert(!reference_constructs_from_temporary<volatile Obj&, const volatile Obj>::value);
    static_assert(!reference_constructs_from_temporary<const volatile Obj&, Obj>::value);
    static_assert(!reference_constructs_from_temporary<const volatile Obj&, const Obj>::value);
    static_assert(!reference_constructs_from_temporary<const volatile Obj&, volatile Obj>::value);
    static_assert(!reference_constructs_from_temporary<const volatile Obj&, const volatile Obj>::value);

    static_assert(reference_constructs_from_temporary<Obj&&, Obj>::value);
    static_assert(reference_constructs_from_temporary<Obj&&, const Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<Obj&&, volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<Obj&&, const volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<const Obj&&, Obj>::value);
    static_assert(reference_constructs_from_temporary<const Obj&&, const Obj>::value);
    static_assert(reference_constructs_from_temporary<const Obj&&, volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<const Obj&&, const volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<volatile Obj&&, Obj>::value);
    static_assert(reference_constructs_from_temporary<volatile Obj&&, const Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<volatile Obj&&, volatile Obj>::value);
    static_assert(reference_constructs_from_temporary<volatile Obj&&, const volatile Obj>::value == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary<const volatile Obj&&, Obj>::value);
    static_assert(reference_constructs_from_temporary<const volatile Obj&&, const Obj>::value);
    static_assert(reference_constructs_from_temporary<const volatile Obj&&, volatile Obj>::value);
    static_assert(reference_constructs_from_temporary<const volatile Obj&&, const volatile Obj>::value);

    static_assert(!reference_constructs_from_temporary_v<Obj&, Obj>);
    static_assert(!reference_constructs_from_temporary_v<Obj&, const Obj>);
    static_assert(!reference_constructs_from_temporary_v<Obj&, volatile Obj>);
    static_assert(!reference_constructs_from_temporary_v<Obj&, const volatile Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&, Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&, const Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&, volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&, const volatile Obj> == is_scalar_v<Obj>);
    static_assert(!reference_constructs_from_temporary_v<volatile Obj&, Obj>);
    static_assert(!reference_constructs_from_temporary_v<volatile Obj&, const Obj>);
    static_assert(!reference_constructs_from_temporary_v<volatile Obj&, volatile Obj>);
    static_assert(!reference_constructs_from_temporary_v<volatile Obj&, const volatile Obj>);
    static_assert(!reference_constructs_from_temporary_v<const volatile Obj&, Obj>);
    static_assert(!reference_constructs_from_temporary_v<const volatile Obj&, const Obj>);
    static_assert(!reference_constructs_from_temporary_v<const volatile Obj&, volatile Obj>);
    static_assert(!reference_constructs_from_temporary_v<const volatile Obj&, const volatile Obj>);

    static_assert(reference_constructs_from_temporary_v<Obj&&, Obj>);
    static_assert(reference_constructs_from_temporary_v<Obj&&, const Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<Obj&&, volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<Obj&&, const volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&&, Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&&, const Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&&, volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<const Obj&&, const volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<volatile Obj&&, Obj>);
    static_assert(reference_constructs_from_temporary_v<volatile Obj&&, const Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<volatile Obj&&, volatile Obj>);
    static_assert(reference_constructs_from_temporary_v<volatile Obj&&, const volatile Obj> == is_scalar_v<Obj>);
    static_assert(reference_constructs_from_temporary_v<const volatile Obj&&, Obj>);
    static_assert(reference_constructs_from_temporary_v<const volatile Obj&&, const Obj>);
    static_assert(reference_constructs_from_temporary_v<const volatile Obj&&, volatile Obj>);
    static_assert(reference_constructs_from_temporary_v<const volatile Obj&&, const volatile Obj>);
}

void test_object_types() {
 test_one_cvref_to_object<int>();
    test_one_cvref_to_object<int[1]>();
    test_one_cvref_to_object<to_int_lvalue<conv_explicity::yes>>();
    test_one_cvref_to_object<to_int_lvalue<conv_explicity::no>>();
    test_one_cvref_to_object<to_int_xvalue<conv_explicity::yes>>();
    test_one_cvref_to_object<to_int_xvalue<conv_explicity::no>>();
}

template <conv_explicity Explicity>
void test_to_int_ref() {
    static_assert(!reference_constructs_from_temporary<int&, to_int_lvalue<conv_explicity::yes>>::value);

    static_assert(!reference_constructs_from_temporary_v<int&, to_int_lvalue<conv_explicity::yes>>);
    static_assert(!reference_constructs_from_temporary_v<const int&, to_int_lvalue<conv_explicity::yes>>);

    static_assert(!reference_constructs_from_temporary_v<const long&, to_int_lvalue<conv_explicity::yes>>);
}
#endif // ^^^ no workaround ^^^
