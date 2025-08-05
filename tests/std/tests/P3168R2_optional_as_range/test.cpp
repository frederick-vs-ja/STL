// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <cassert>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>

using namespace std;

#ifndef _M_CEE // TRANSITION, VSO-1659496
template <class T>
struct holder {
    T t;
};

struct incomplete;
#endif // ^^^ no workaround ^^^

struct Pinned {
    Pinned()                         = default;
    Pinned(const Pinned&)            = delete;
    Pinned& operator=(const Pinned&) = delete;

    friend bool operator==(const Pinned&, const Pinned&) = default;
};

template <class, class = void>
constexpr bool has_memtype_iterator = false;
template <class T>
constexpr bool has_memtype_iterator<T, void_t<typename T::iterator>> = true;

template <class, class = void>
constexpr bool has_memtype_const_iterator = false;
template <class T>
constexpr bool has_memtype_const_iterator<T, void_t<typename T::const_iterator>> = true;

template <class, class = void>
constexpr bool has_memfun_begin = false;
template <class T>
constexpr bool has_memfun_begin<T, void_t<decltype(declval<T>().begin())>> = true;

template <class, class = void>
constexpr bool has_memfun_end = false;
template <class T>
constexpr bool has_memfun_end<T, void_t<decltype(declval<T>().end())>> = true;

template <class T, bool IsAdlUnfriendly>
void test_properties_one() { // COMPILE-ONLY
    static_assert(has_memtype_iterator<optional<T>>);
    static_assert(has_memtype_const_iterator<optional<T>>);

    static_assert(has_memfun_begin<optional<T>&>);
    static_assert(has_memfun_begin<const optional<T>&>);
    static_assert(has_memfun_begin<optional<T>>);
    static_assert(has_memfun_begin<const optional<T>>);

    static_assert(has_memfun_end<optional<T>&>);
    static_assert(has_memfun_end<const optional<T>&>);
    static_assert(has_memfun_end<optional<T>>);
    static_assert(has_memfun_end<const optional<T>>);

    static_assert(is_same_v<decltype(declval<optional<T>&>().begin()), typename optional<T>::iterator>);
    static_assert(is_same_v<decltype(declval<const optional<T>&>().begin()), typename optional<T>::const_iterator>);
    static_assert(is_same_v<decltype(declval<optional<T>>().begin()), typename optional<T>::iterator>);
    static_assert(is_same_v<decltype(declval<const optional<T>>().begin()), typename optional<T>::const_iterator>);

    static_assert(is_same_v<decltype(declval<optional<T>&>().end()), typename optional<T>::iterator>);
    static_assert(is_same_v<decltype(declval<const optional<T>&>().end()), typename optional<T>::const_iterator>);
    static_assert(is_same_v<decltype(declval<optional<T>>().end()), typename optional<T>::iterator>);
    static_assert(is_same_v<decltype(declval<const optional<T>>().end()), typename optional<T>::const_iterator>);

    static_assert(is_constructible_v<typename optional<T>::const_iterator, typename optional<T>::iterator&>);
    static_assert(is_constructible_v<typename optional<T>::const_iterator, const typename optional<T>::iterator&>);
    static_assert(is_constructible_v<typename optional<T>::const_iterator, typename optional<T>::iterator>);
    static_assert(is_constructible_v<typename optional<T>::const_iterator, const typename optional<T>::iterator>);
    static_assert(is_convertible_v<typename optional<T>::iterator&, typename optional<T>::const_iterator>);
    static_assert(is_convertible_v<const typename optional<T>::iterator&, typename optional<T>::const_iterator>);
    static_assert(is_convertible_v<typename optional<T>::iterator, typename optional<T>::const_iterator>);
    static_assert(is_convertible_v<const typename optional<T>::iterator, typename optional<T>::const_iterator>);

    static_assert(totally_ordered_with<typename optional<T>::iterator, const typename optional<T>::const_iterator>);
    static_assert(three_way_comparable_with<typename optional<T>::iterator, const typename optional<T>::const_iterator,
        strong_ordering>);

    static_assert(is_same_v<iter_value_t<typename optional<T>::iterator>, remove_cv_t<T>>);
    static_assert(is_same_v<iter_value_t<typename optional<T>::const_iterator>, remove_cv_t<T>>);

    static_assert(is_same_v<iter_reference_t<typename optional<T>::iterator>, T&>);
    static_assert(is_same_v<iter_reference_t<typename optional<T>::const_iterator>, const T&>);

    static_assert(ranges::range<optional<T>>);
    static_assert(ranges::range<const optional<T>>);

    static_assert(ranges::enable_view<optional<T>>);

    if constexpr (!IsAdlUnfriendly) {
        static_assert(ranges::view<optional<T>> == movable<T>);
        static_assert(!ranges::view<optional<const T>>);

        static_assert(ranges::viewable_range<optional<T>> == movable<T>);
        static_assert(ranges::viewable_range<optional<T>&> == (!movable<T> || constructible_from<T, T&>) );
        static_assert(ranges::viewable_range<const optional<T>> == (movable<T> && constructible_from<T, const T>) );
        static_assert(ranges::viewable_range<const optional<T>&> == (!movable<T> || copy_constructible<T>) );

        static_assert(!ranges::viewable_range<optional<const T>>);
        static_assert(!ranges::viewable_range<const optional<const T>>);
        static_assert(ranges::viewable_range<optional<const T>&>);
        static_assert(ranges::viewable_range<const optional<const T>&>);
    }

    if constexpr (!is_volatile_v<T>) { // TRANSITION, LWG-3813
        static_assert(ranges::constant_range<optional<T>> == is_const_v<T>);
        static_assert(ranges::constant_range<const optional<T>>);
    }

    if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
        if constexpr (!IsAdlUnfriendly) {
            static_assert(ranges::sized_range<optional<T>>);
            static_assert(ranges::sized_range<const optional<T>>);
        }

        static_assert(contiguous_iterator<typename optional<T>::iterator>);
        static_assert(contiguous_iterator<typename optional<T>::const_iterator>);

        static_assert(ranges::contiguous_range<optional<T>>);
        static_assert(ranges::contiguous_range<const optional<T>>);
    }

    static_assert(format_kind<optional<T>> == range_format::disabled);

    // Test that no volatile versions of begin()/end() are provided.

    static_assert(!has_memfun_begin<volatile optional<T>&>);
    static_assert(!has_memfun_begin<const volatile optional<T>&>);
    static_assert(!has_memfun_begin<volatile optional<T>>);
    static_assert(!has_memfun_begin<const volatile optional<T>>);

    static_assert(!has_memfun_end<volatile optional<T>&>);
    static_assert(!has_memfun_end<const volatile optional<T>&>);
    static_assert(!has_memfun_end<volatile optional<T>>);
    static_assert(!has_memfun_end<const volatile optional<T>>);

    if constexpr (!IsAdlUnfriendly) {
        static_assert(!ranges::range<volatile optional<T>>);
        static_assert(!ranges::range<const volatile optional<T>>);
    }
}

template <class T, bool IsAdlUnfriendly = false>
void test_properties_cv() { // COMPILE-ONLY
    test_properties_one<T, IsAdlUnfriendly>();
    test_properties_one<const T, IsAdlUnfriendly>();
    test_properties_one<volatile T, IsAdlUnfriendly>();
    test_properties_one<const volatile T, IsAdlUnfriendly>();
}

void test_properties() { // COMPILE-ONLY
    test_properties_cv<int>();
    test_properties_cv<string>();
    test_properties_cv<unique_ptr<int>>();
    test_properties_cv<Pinned>();
#ifndef _M_CEE // TRANSITION, VSO-1659496
    test_properties_one<holder<incomplete>*, true>();
#endif // ^^^ no workaround ^^^
}

template <class T, bool IsAdlUnfriendly>
constexpr void test_one() {
    {
        optional<T> opt;

        if constexpr (!IsAdlUnfriendly //
                      && (!is_volatile_v<T> || is_scalar_v<T>) ) { // TRANSITION, LWG-3813
            assert(ranges::empty(opt));
            assert(ranges::size(opt) == 0);
        }

        assert(opt.begin() == opt.end());
        assert(to_address(opt.begin()) == to_address(opt.end()));

        assert(opt.begin() == ranges::begin(opt));
        assert(opt.end() == ranges::end(opt));
        if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
            assert(opt.begin() == ranges::cbegin(opt));
            assert(opt.end() == ranges::cend(opt));
        }

        for ([[maybe_unused]] auto& _ : opt) { // TRANSITION, P2169R4
        }
    }
    {
        const optional<T> opt;

        if constexpr (!IsAdlUnfriendly //
                      && (!is_volatile_v<T> || is_scalar_v<T>) ) { // TRANSITION, LWG-3813
            assert(ranges::empty(opt));
            assert(ranges::size(opt) == 0);
        }
        assert(opt.begin() == opt.end());
        assert(to_address(opt.begin()) == to_address(opt.end()));

        assert(opt.begin() == ranges::begin(opt));
        assert(opt.end() == ranges::end(opt));
        if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
            assert(opt.begin() == ranges::cbegin(opt));
            assert(opt.end() == ranges::cend(opt));
        }

        for ([[maybe_unused]] auto& _ : opt) { // TRANSITION, P2169R4
        }
    }
    {
        optional<T> opt{in_place};

        if constexpr (!IsAdlUnfriendly //
                      && (!is_volatile_v<T> || is_scalar_v<T>) ) { // TRANSITION, LWG-3813
            assert(!ranges::empty(opt));
            assert(ranges::size(opt) == 1);
        }
        assert(opt.begin() < opt.end());
        assert(opt.end() - opt.begin() == 1);
        assert(to_address(opt.begin()) < to_address(opt.end()));
        assert(to_address(opt.end()) - to_address(opt.begin()) == 1);

        assert(opt.begin() == ranges::begin(opt));
        assert(opt.end() == ranges::end(opt));
        if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
            assert(opt.begin() == ranges::cbegin(opt));
            assert(opt.end() == ranges::cend(opt));
        }

        for (auto& elem : opt) {
            // Intentionally std::-qualify to_address and addressof to avoid ADL.
            assert(std::to_address(opt.begin()) == std::addressof(elem));
            if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
                assert(ranges::data(opt) == std::addressof(elem));
            }
        }
    }
    {
        const optional<T> opt{in_place};

        if constexpr (!IsAdlUnfriendly //
                      && (!is_volatile_v<T> || is_scalar_v<T>) ) { // TRANSITION, LWG-3813
            assert(!ranges::empty(opt));
            assert(ranges::size(opt) == 1);
        }
        assert(opt.begin() < opt.end());
        assert(opt.end() - opt.begin() == 1);
        assert(to_address(opt.begin()) < to_address(opt.end()));
        assert(to_address(opt.end()) - to_address(opt.begin()) == 1);

        assert(opt.begin() == ranges::begin(opt));
        assert(opt.end() == ranges::end(opt));
        if constexpr (!is_volatile_v<T> || is_scalar_v<T>) {
            assert(opt.begin() == ranges::cbegin(opt));
            assert(opt.end() == ranges::cend(opt));
        }

        for (auto& elem : opt) {
            // Intentionally std::-qualify to_address and addressof to avoid ADL.
            assert(std::to_address(opt.begin()) == std::addressof(elem));
            if constexpr (!is_volatile_v<T> || is_scalar_v<T>) { // TRANSITION, LWG-3813
                assert(ranges::data(opt) == std::addressof(elem));
            }
        }
    }
}

template <class T, bool IsAdlUnfriendly = false>
constexpr void test_cv() {
    test_one<T, IsAdlUnfriendly>();
    test_one<const T, IsAdlUnfriendly>();
    test_one<volatile T, IsAdlUnfriendly>();
    test_one<const volatile T, IsAdlUnfriendly>();
}

constexpr bool test() {
    test_cv<int>();
    test_cv<string>();
    test_cv<unique_ptr<int>>();
    test_cv<Pinned>();
#ifndef _M_CEE // TRANSITION, VSO-1659496
    test_cv<holder<incomplete>*, true>();
#endif // ^^^ no workaround ^^^

    return true;
}

int main() {
    test();
    static_assert(test());
}
