// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

using namespace std;

template <class T>
concept CanUnaryPlus = requires(T t) { +t; };

template <class T>
concept CanUnaryMinus = requires(T t) { -t; };

template <class T>
concept CanComplement = requires(T t) { ~t; };

template <class T>
concept CanNegate = requires(T t) { !t; };

template <class T>
concept CanDereference = requires(T t) { *t; };

template <class T>
concept CanUnaryAnd = requires(T t) { &t; };

template <class T>
concept CanUnaryAndRvalue = requires(T t) { &static_cast<T&&>(t); };

template <class T, class U>
concept CanPlus = requires(T t, U u) { t + u; };

template <class T, class U>
concept CanMinus = requires(T t, U u) { t - u; };

template <class T, class U>
concept CanMultiply = requires(T t, U u) { t * u; };

template <class T, class U>
concept CanDivide = requires(T t, U u) { t / u; };

template <class T, class U>
concept CanModulo = requires(T t, U u) { t % u; };

template <class T, class U>
concept CanShl = requires(T t, U u) { t << u; };

template <class T, class U>
concept CanShr = requires(T t, U u) { t >> u; };

template <class T, class U>
concept CanBitAnd = requires(T t, U u) { t & u; };

template <class T, class U>
concept CanBitOr = requires(T t, U u) { t | u; };

template <class T, class U>
concept CanXor = requires(T t, U u) { t ^ u; };

template <class T, class U>
concept CanLogicalAnd = requires(T t, U u) { t && u; };

template <class T, class U>
concept CanLogicalOr = requires(T t, U u) { t || u; };

template <class T, class U>
concept CanThreeWayCompare = requires(T t, U u) { t <=> u; };

template <class T, class U>
concept CanLess = requires(T t, U u) { t < u; };

template <class T, class U>
concept CanLessEqual = requires(T t, U u) { t <= u; };

template <class T, class U>
concept CanEqual = requires(T t, U u) { t == u; };

template <class T, class U>
concept CanUnequal = requires(T t, U u) { t != u; };

template <class T, class U>
concept CanGreater = requires(T t, U u) { t > u; };

template <class T, class U>
concept CanGreaterEqual = requires(T t, U u) { t >= u; };

template <class T, class U>
concept CanComma = requires(T t, U u) { t, u; };

template <class T, class U>
concept CanArrowStar = requires(T t, U u) { t->*u; };

template <class T>
concept CanPseudoPreIncrement = requires(const T t) { ++t; };

template <class T>
concept CanPseudoPostIncrement = requires(const T t) { t++; };

template <class T>
concept CanPseudoPreDecrement = requires(const T t) { --t; };

template <class T>
concept CanPseudoPostDecrement = requires(const T t) { t--; };

template <class T, class U>
concept CanPseudoPlusAssign = requires(const T t, U u) { t += u; };

template <class T, class U>
concept CanPseudoMinusAssign = requires(const T t, U u) { t -= u; };

template <class T, class U>
concept CanPseudoMultiplyAssign = requires(const T t, U u) { t *= u; };

template <class T, class U>
concept CanPseudoDivideAssign = requires(const T t, U u) { t /= u; };

template <class T, class U>
concept CanPseudoModuloAssign = requires(const T t, U u) { t %= u; };

template <class T, class U>
concept CanPseudoBitAndAssign = requires(const T t, U u) { t &= u; };

template <class T, class U>
concept CanPseudoBitOrAssign = requires(const T t, U u) { t |= u; };

template <class T, class U>
concept CanPseudoXorAssign = requires(const T t, U u) { t ^= u; };

template <class T, class U>
concept CanPseudoShlAssign = requires(const T t, U u) { t <<= u; };

template <class T, class U>
concept CanPseudoShrAssign = requires(const T t, U u) { t >>= u; };

template <class T, class... Indices>
concept CanSubscript = requires(T&& t, Indices&&... ids) { static_cast<T&&>(t)[static_cast<Indices&&>(ids)...]; };

template <auto V>
    requires (!is_scalar_v<decltype(V)>)
constexpr const auto& template_parameter_object_of = V;

namespace AdlTest {
    struct S {
        int val_;

        // Operators are intentionally noexcept(false).

        struct UnaryPlus {
            int res_;
        };

        friend constexpr UnaryPlus operator+(S s) {
            return {s.val_};
        }

        struct UnaryMinus {
            int res_;
        };

        friend constexpr UnaryMinus operator-(S s) {
            return {-s.val_};
        }

        struct Complement {
            int res_;
        };

        friend constexpr Complement operator~(S s) {
            return {~s.val_};
        }

        struct Negate {
            bool res_;
        };

        friend constexpr Negate operator!(S s) {
            return {!s.val_};
        }

        struct DerefCopy {
            int res_;
        };

        friend constexpr DerefCopy operator*(S s) {
            return {s.val_};
        }

        struct UnaryAnd {
            const int* ptr_;
        };

        friend constexpr UnaryAnd operator&(const S& s) {
            return {&s.val_};
        }

        struct Plus {
            int res_;
        };
        struct PlusLeftInt {
            int res_;
        };
        struct PlusRightInt {
            int res_;
        };

        friend constexpr Plus operator+(S m, S n) {
            return {m.val_ + n.val_};
        }
        friend constexpr PlusLeftInt operator+(int m, S n) {
            return {m + n.val_};
        }
        friend constexpr PlusRightInt operator+(S m, int n) {
            return {m.val_ + n};
        }

        struct Minus {
            int res_;
        };
        struct MinusLeftInt {
            int res_;
        };
        struct MinusRightInt {
            int res_;
        };

        friend constexpr Minus operator-(S m, S n) {
            return {m.val_ - n.val_};
        }
        friend constexpr MinusLeftInt operator-(int m, S n) {
            return {m - n.val_};
        }
        friend constexpr MinusRightInt operator-(S m, int n) {
            return {m.val_ - n};
        }

        struct Multiply {
            int res_;
        };
        struct MultiplyLeftInt {
            int res_;
        };
        struct MultiplyRightInt {
            int res_;
        };

        friend constexpr Multiply operator*(S m, S n) {
            return {m.val_ * n.val_};
        }
        friend constexpr MultiplyLeftInt operator*(int m, S n) {
            return {m * n.val_};
        }
        friend constexpr MultiplyRightInt operator*(S m, int n) {
            return {m.val_ * n};
        }

        struct Divide {
            int res_;
        };
        struct DivideLeftInt {
            int res_;
        };
        struct DivideRightInt {
            int res_;
        };

        friend constexpr Divide operator/(S m, S n) {
            return {m.val_ / n.val_};
        }
        friend constexpr DivideLeftInt operator/(int m, S n) {
            return {m / n.val_};
        }
        friend constexpr DivideRightInt operator/(S m, int n) {
            return {m.val_ / n};
        }

        struct Modulo {
            int res_;
        };
        struct ModuloLeftInt {
            int res_;
        };
        struct ModuloRightInt {
            int res_;
        };

        friend constexpr Modulo operator%(S m, S n) {
            return {m.val_ % n.val_};
        }
        friend constexpr ModuloLeftInt operator%(int m, S n) {
            return {m % n.val_};
        }
        friend constexpr ModuloRightInt operator%(S m, int n) {
            return {m.val_ % n};
        }

        struct Shl {
            int res_;
        };
        struct ShlLeftInt {
            int res_;
        };
        struct ShlRightInt {
            int res_;
        };

        friend constexpr Shl operator<<(S m, S n) {
            return {m.val_ << n.val_};
        }
        friend constexpr ShlLeftInt operator<<(int m, S n) {
            return {m << n.val_};
        }
        friend constexpr ShlRightInt operator<<(S m, int n) {
            return {m.val_ << n};
        }

        struct Shr {
            int res_;
        };
        struct ShrLeftInt {
            int res_;
        };
        struct ShrRightInt {
            int res_;
        };

        friend constexpr Shr operator>>(S m, S n) {
            return {m.val_ >> n.val_};
        }
        friend constexpr ShrLeftInt operator>>(int m, S n) {
            return {m >> n.val_};
        }
        friend constexpr ShrRightInt operator>>(S m, int n) {
            return {m.val_ >> n};
        }

        struct BitAnd {
            int res_;
        };
        struct BitAndLeftInt {
            int res_;
        };
        struct BitAndRightInt {
            int res_;
        };

        friend constexpr BitAnd operator&(S m, S n) {
            return {m.val_ & n.val_};
        }
        friend constexpr BitAndLeftInt operator&(int m, S n) {
            return {m & n.val_};
        }
        friend constexpr BitAndRightInt operator&(S m, int n) {
            return {m.val_ & n};
        }

        struct BitOr {
            int res_;
        };
        struct BitOrLeftInt {
            int res_;
        };
        struct BitOrRightInt {
            int res_;
        };

        friend constexpr BitOr operator|(S m, S n) {
            return {m.val_ | n.val_};
        }
        friend constexpr BitOrLeftInt operator|(int m, S n) {
            return {m | n.val_};
        }
        friend constexpr BitOrRightInt operator|(S m, int n) {
            return {m.val_ | n};
        }

        struct Xor {
            int res_;
        };
        struct XorLeftInt {
            int res_;
        };
        struct XorRightInt {
            int res_;
        };

        friend constexpr Xor operator^(S m, S n) {
            return {m.val_ ^ n.val_};
        }
        friend constexpr XorLeftInt operator^(int m, S n) {
            return {m ^ n.val_};
        }
        friend constexpr XorRightInt operator^(S m, int n) {
            return {m.val_ ^ n};
        }

        struct LogicalAnd {
            bool res_;
        };
        struct LogicalAndLeftBool {
            bool res_;
        };
        struct LogicalAndRightBool {
            bool res_;
        };

        friend constexpr LogicalAnd operator&&(S m, S n) {
            return {m.val_ && n.val_};
        }
        friend constexpr LogicalAndLeftBool operator&&(same_as<bool> auto m, S n) {
            return {m && n.val_};
        }
        friend constexpr LogicalAndRightBool operator&&(S m, same_as<bool> auto n) {
            return {m.val_ && n};
        }

        struct LogicalOr {
            bool res_;
        };
        struct LogicalOrLeftBool {
            bool res_;
        };
        struct LogicalOrRightBool {
            bool res_;
        };

        friend constexpr LogicalOr operator||(S m, S n) {
            return {m.val_ || n.val_};
        }
        friend constexpr LogicalOrLeftBool operator||(same_as<bool> auto m, S n) {
            return {m || n.val_};
        }
        friend constexpr LogicalOrRightBool operator||(S m, same_as<bool> auto n) {
            return {m.val_ || n};
        }

        // avoid divergence on structurality of comparison category types

        struct ThreeWayCmp {
            int res_;
        };
        struct ThreeWayCmpLeftInt {
            int res_;
        };
        struct ThreeWayCmpRightInt {
            int res_;
        };

        friend constexpr ThreeWayCmp operator<=>(S m, S n) {
            auto raw_result = m.val_ <=> n.val_;
            return {raw_result < 0 ? -1 : raw_result > 0 ? 1 : 0};
        }
        friend constexpr ThreeWayCmpLeftInt operator<=>(int m, S n) {
            auto raw_result = m <=> n.val_;
            return {raw_result < 0 ? -1 : raw_result > 0 ? 1 : 0};
        }
        friend constexpr ThreeWayCmpRightInt operator<=>(S m, int n) {
            auto raw_result = m.val_ <=> n;
            return {raw_result < 0 ? -1 : raw_result > 0 ? 1 : 0};
        }

        struct Less {
            bool res_;
        };
        struct LessLeftInt {
            bool res_;
        };
        struct LessRightInt {
            bool res_;
        };

        friend constexpr Less operator<(S m, S n) {
            return {m.val_ < n.val_};
        }
        friend constexpr LessLeftInt operator<(int m, S n) {
            return {m < n.val_};
        }
        friend constexpr LessRightInt operator<(S m, int n) {
            return {m.val_ < n};
        }

        struct LessEqual {
            bool res_;
        };
        struct LessEqualLeftInt {
            bool res_;
        };
        struct LessEqualRightInt {
            bool res_;
        };

        friend constexpr LessEqual operator<=(S m, S n) {
            return {m.val_ <= n.val_};
        }
        friend constexpr LessEqualLeftInt operator<=(int m, S n) {
            return {m <= n.val_};
        }
        friend constexpr LessEqualRightInt operator<=(S m, int n) {
            return {m.val_ <= n};
        }

        struct Equal {
            bool res_;
        };
        struct EqualLeftInt {
            bool res_;
        };
        struct EqualRightInt {
            bool res_;
        };

        friend constexpr Equal operator==(S m, S n) {
            return {m.val_ == n.val_};
        }
        friend constexpr EqualLeftInt operator==(int m, S n) {
            return {m == n.val_};
        }
        friend constexpr EqualRightInt operator==(S m, int n) {
            return {m.val_ == n};
        }

        struct Unequal {
            bool res_;
        };
        struct UnequalLeftInt {
            bool res_;
        };
        struct UnequalRightInt {
            bool res_;
        };

        friend constexpr Unequal operator!=(S m, S n) {
            return {m.val_ != n.val_};
        }
        friend constexpr UnequalLeftInt operator!=(int m, S n) {
            return {m != n.val_};
        }
        friend UnequalRightInt operator!=(S m, int n) {
            return {m.val_ != n};
        }

        struct Greater {
            bool res_;
        };
        struct GreaterLeftInt {
            bool res_;
        };
        struct GreaterRightInt {
            bool res_;
        };

        friend constexpr Greater operator>(S m, S n) {
            return {m.val_ > n.val_};
        }
        friend constexpr GreaterLeftInt operator>(int m, S n) {
            return {m > n.val_};
        }
        friend constexpr GreaterRightInt operator>(S m, int n) {
            return {m.val_ > n};
        }

        struct GreaterEqual {
            bool res_;
        };
        struct GreaterEqualLeftInt {
            bool res_;
        };
        struct GreaterEqualRightInt {
            bool res_;
        };

        friend GreaterEqual operator>=(S m, S n) {
            return {m.val_ >= n.val_};
        }
        friend GreaterEqualLeftInt operator>=(int m, S n) {
            return {m >= n.val_};
        }
        friend GreaterEqualRightInt operator>=(S m, int n) {
            return {m.val_ >= n};
        }

        struct ArrowStar {
            int res_;
        };
        struct ArrowStarLeftInt {
            int res_;
        };
        struct ArrowStarRightInt {
            int res_;
        };

        friend constexpr ArrowStar operator->*(S m, S n) {
            return {m.val_ * n.val_};
        }
        friend constexpr ArrowStarLeftInt operator->*(int m, S n) {
            return {m * n.val_};
        }
        friend constexpr ArrowStarRightInt operator->*(S m, int n) {
            return {m.val_ * n};
        }

        struct PreInc {
            int res_;
        };

        friend constexpr PreInc operator++(S s) {
            return {s.val_ + 1};
        }

        struct PostInc {
            int res_;
        };

        friend constexpr PostInc operator++(S s, int) {
            return {s.val_ + 1};
        }

        struct PreDec {
            int res_;
        };

        friend constexpr PreDec operator--(S s) {
            return {s.val_ - 1};
        }

        struct PostDec {
            int res_;
        };

        friend constexpr PostDec operator--(S s, int) {
            return {s.val_ - 1};
        }

        struct PlusAssign {
            int res_;
        };
        struct PlusAssignLeftInt {
            int res_;
        };
        struct PlusAssignRightInt {
            int res_;
        };

        friend constexpr PlusAssign operator+=(S m, S n) {
            return {m.val_ + n.val_};
        }
        friend constexpr PlusAssignLeftInt operator+=(int m, S n) {
            return {m + n.val_};
        }
        friend constexpr PlusAssignRightInt operator+=(S m, int n) {
            return {m.val_ + n};
        }

        struct MinusAssign {
            int res_;
        };
        struct MinusAssignLeftInt {
            int res_;
        };
        struct MinusAssignRightInt {
            int res_;
        };

        friend constexpr MinusAssign operator-=(S m, S n) {
            return {m.val_ - n.val_};
        }
        friend constexpr MinusAssignLeftInt operator-=(int m, S n) {
            return {m - n.val_};
        }
        friend constexpr MinusAssignRightInt operator-=(S m, int n) {
            return {m.val_ - n};
        }

        struct MultiplyAssign {
            int res_;
        };
        struct MultiplyAssignLeftInt {
            int res_;
        };
        struct MultiplyAssignRightInt {
            int res_;
        };

        friend constexpr MultiplyAssign operator*=(S m, S n) {
            return {m.val_ * n.val_};
        }
        friend constexpr MultiplyAssignLeftInt operator*=(int m, S n) {
            return {m * n.val_};
        }
        friend constexpr MultiplyAssignRightInt operator*=(S m, int n) {
            return {m.val_ * n};
        }

        struct DivideAssign {
            int res_;
        };
        struct DivideAssignLeftInt {
            int res_;
        };
        struct DivideAssignRightInt {
            int res_;
        };

        friend constexpr DivideAssign operator/=(S m, S n) {
            return {m.val_ / n.val_};
        }
        friend constexpr DivideAssignLeftInt operator/=(int m, S n) {
            return {m / n.val_};
        }
        friend constexpr DivideAssignRightInt operator/=(S m, int n) {
            return {m.val_ / n};
        }

        struct ModuloAssign {
            int res_;
        };
        struct ModuloAssignLeftInt {
            int res_;
        };
        struct ModuloAssignRightInt {
            int res_;
        };

        friend constexpr ModuloAssign operator%=(S m, S n) {
            return {m.val_ % n.val_};
        }
        friend constexpr ModuloAssignLeftInt operator%=(int m, S n) {
            return {m % n.val_};
        }
        friend constexpr ModuloAssignRightInt operator%=(S m, int n) {
            return {m.val_ % n};
        }

        struct ShlAssign {
            int res_;
        };
        struct ShlAssignLeftInt {
            int res_;
        };
        struct ShlAssignRightInt {
            int res_;
        };

        friend constexpr ShlAssign operator<<=(S m, S n) {
            return {m.val_ << n.val_};
        }
        friend constexpr ShlAssignLeftInt operator<<=(int m, S n) {
            return {m << n.val_};
        }
        friend constexpr ShlAssignRightInt operator<<=(S m, int n) {
            return {m.val_ << n};
        }

        struct ShrAssign {
            int res_;
        };
        struct ShrAssignLeftInt {
            int res_;
        };
        struct ShrAssignRightInt {
            int res_;
        };

        friend constexpr ShrAssign operator>>=(S m, S n) {
            return {m.val_ >> n.val_};
        }
        friend constexpr ShrAssignLeftInt operator>>=(int m, S n) {
            return {m >> n.val_};
        }
        friend constexpr ShrAssignRightInt operator>>=(S m, int n) {
            return {m.val_ >> n};
        }

        struct BitAndAssign {
            int res_;
        };
        struct BitAndAssignLeftInt {
            int res_;
        };
        struct BitAndAssignRightInt {
            int res_;
        };

        friend constexpr BitAndAssign operator&=(S m, S n) {
            return {m.val_ & n.val_};
        }
        friend constexpr BitAndAssignLeftInt operator&=(int m, S n) {
            return {m & n.val_};
        }
        friend constexpr BitAndAssignRightInt operator&=(S m, int n) {
            return {m.val_ & n};
        }

        struct BitOrAssign {
            int res_;
        };
        struct BitOrAssignLeftInt {
            int res_;
        };
        struct BitOrAssignRightInt {
            int res_;
        };

        friend constexpr BitOrAssign operator|=(S m, S n) {
            return {m.val_ | n.val_};
        }
        friend constexpr BitOrAssignLeftInt operator|=(int m, S n) {
            return {m | n.val_};
        }
        friend constexpr BitOrAssignRightInt operator|=(S m, int n) {
            return {m.val_ | n};
        }

        struct XorAssign {
            int res_;
        };
        struct XorAssignLeftInt {
            int res_;
        };
        struct XorAssignRightInt {
            int res_;
        };

        friend constexpr XorAssign operator^=(S m, S n) {
            return {m.val_ ^ n.val_};
        }
        friend constexpr XorAssignLeftInt operator^=(int m, S n) {
            return {m ^ n.val_};
        }
        friend constexpr XorAssignRightInt operator^=(S m, int n) {
            return {m.val_ ^ n};
        }

        struct Assign {
            int val_;
        };
        struct AssignRightInt {
            int val_;
        };

        constexpr Assign operator=(const S& other) const {
            return {other.val_};
        }

        constexpr AssignRightInt operator=(int n) const {
            return {n};
        }
    };
} // namespace AdlTest

namespace AdlTest2 {
    struct Nonaddressable {
        template <class T>
        friend void operator&(T&&) = delete;
    };
} // namespace AdlTest2

struct NoOperation {};


constexpr int i_array[1]{172};
int i_mut_array[1]{};

constexpr void noop_fun() {}
constexpr void noop_noexcept_fun() noexcept {}


constexpr bool test() {
    using AdlTest::S, AdlTest2::Nonaddressable;

    // cw
    static_assert(is_same_v<decltype(cw<42>), const constant_wrapper<42>>);
    static_assert(is_same_v<decltype((cw<42>) ), const constant_wrapper<42>&>);

    static_assert(is_same_v<decltype(cw<2.25>), const constant_wrapper<2.25>>);
    static_assert(is_same_v<decltype((cw<2.25>) ), const constant_wrapper<2.25>&>);

    static_assert(is_same_v<decltype(cw<nullptr>), const constant_wrapper<nullptr>>);
    static_assert(is_same_v<decltype((cw<nullptr>) ), const constant_wrapper<nullptr>&>);

    static_assert(is_same_v<decltype(cw<S{123}>), const constant_wrapper<S{123}>>);
    static_assert(is_same_v<decltype((cw<S{123}>) ), const constant_wrapper<S{123}>&>);

    static_assert(is_same_v<decltype(cw<i_array>), const constant_wrapper<i_array>>);
    static_assert(is_same_v<decltype((cw<i_array>) ), const constant_wrapper<i_array>&>);

    static_assert(is_same_v<decltype(cw<i_mut_array>), const constant_wrapper<i_mut_array>>);
    static_assert(is_same_v<decltype((cw<i_mut_array>) ), const constant_wrapper<i_mut_array>&>);

    static_assert(is_same_v<decltype(cw<noop_fun>), const constant_wrapper<noop_fun>>);
    static_assert(is_same_v<decltype((cw<noop_fun>) ), const constant_wrapper<noop_fun>&>);

    static_assert(is_same_v<decltype(cw<noop_noexcept_fun>), const constant_wrapper<noop_noexcept_fun>>);
    static_assert(is_same_v<decltype((cw<noop_noexcept_fun>) ), const constant_wrapper<noop_noexcept_fun>&>);

    // default template argument
    static_assert(is_same_v<constant_wrapper<42>, constant_wrapper<42, int>>);
    static_assert(is_same_v<constant_wrapper<2.25>, constant_wrapper<2.25, double>>);
    static_assert(is_same_v<constant_wrapper<nullptr>, constant_wrapper<nullptr, nullptr_t>>);
    static_assert(is_same_v<constant_wrapper<S{123}>, constant_wrapper<S{123}, S>>);
    static_assert(is_same_v<constant_wrapper<i_array>, constant_wrapper<i_array, const int*>>);
    static_assert(is_same_v<constant_wrapper<i_mut_array>, constant_wrapper<i_mut_array, int*>>);
    static_assert(is_same_v<constant_wrapper<noop_fun>, constant_wrapper<noop_fun, void (*)()>>);
    static_assert(
        is_same_v<constant_wrapper<noop_noexcept_fun>, constant_wrapper<noop_noexcept_fun, void (*)() noexcept>>);
    static_assert(is_same_v<constant_wrapper<&constant_wrapper<'*'>::value>,
        constant_wrapper<&constant_wrapper<'*'>::value, const char*>>);

    // constant_wrapper::value
    static_assert(is_same_v<decltype(constant_wrapper<42>::value), const int>);
    static_assert(is_same_v<decltype((constant_wrapper<42>::value)), const int&>);
    assert(constant_wrapper<42>::value == 42);

    static_assert(is_same_v<decltype(constant_wrapper<2.25>::value), const double>);
    static_assert(is_same_v<decltype((constant_wrapper<2.25>::value)), const double&>);
    assert(constant_wrapper<2.25>::value == 2.25);

    static_assert(is_same_v<decltype(constant_wrapper<nullptr>::value), const nullptr_t>);
    static_assert(is_same_v<decltype((constant_wrapper<nullptr>::value)), const nullptr_t&>);
    assert(constant_wrapper<nullptr>::value == nullptr);

    static_assert(is_same_v<decltype(constant_wrapper<S{123}>::value), const S&>);
    static_assert(is_same_v<decltype((constant_wrapper<S{123}>::value)), const S&>);
    assert(constant_wrapper<S{123}>::value.val_ == 123);
    assert(addressof(template_parameter_object_of<S{123}>) == addressof(constant_wrapper<S{123}>::value));

    static_assert(is_same_v<decltype(constant_wrapper<NoOperation{}>::value), const NoOperation&>);
    static_assert(is_same_v<decltype((constant_wrapper<NoOperation{}>::value)), const NoOperation&>);
    assert(addressof(template_parameter_object_of<NoOperation{}>) == addressof(constant_wrapper<NoOperation{}>::value));

    static_assert(is_same_v<decltype(constant_wrapper<i_array>::value), const int* const>);
    static_assert(is_same_v<decltype((constant_wrapper<i_array>::value)), const int* const&>);
    assert(constant_wrapper<i_array>::value == i_array + 0);

    static_assert(is_same_v<decltype(constant_wrapper<i_mut_array>::value), int* const>);
    static_assert(is_same_v<decltype((constant_wrapper<i_mut_array>::value)), int* const&>);
    assert(constant_wrapper<i_mut_array>::value == i_mut_array + 0);

    static_assert(is_same_v<decltype(constant_wrapper<noop_fun>::value), void (*const)()>);
    static_assert(is_same_v<decltype((constant_wrapper<noop_fun>::value)), void (*const&)()>);
    assert(constant_wrapper<noop_fun>::value == &noop_fun);

    static_assert(is_same_v<decltype(constant_wrapper<noop_noexcept_fun>::value), void (*const)() noexcept>);
    static_assert(is_same_v<decltype((constant_wrapper<noop_noexcept_fun>::value)), void (*const&)() noexcept>);
    assert(constant_wrapper<noop_noexcept_fun>::value == &noop_noexcept_fun);

    // constant_wrapper::type
    static_assert(is_same_v<constant_wrapper<42>::type, constant_wrapper<42>>);
    static_assert(is_same_v<constant_wrapper<2.25>::type, constant_wrapper<2.25>>);
    static_assert(is_same_v<constant_wrapper<nullptr>::type, constant_wrapper<nullptr>>);
    static_assert(is_same_v<constant_wrapper<S{456}>::type, constant_wrapper<S{456}>>);
    static_assert(is_same_v<constant_wrapper<i_array>::type, constant_wrapper<i_array>>);
    static_assert(is_same_v<constant_wrapper<i_mut_array>::type, constant_wrapper<i_mut_array>>);
    static_assert(is_same_v<constant_wrapper<noop_fun>::type, constant_wrapper<noop_fun>>);
    static_assert(is_same_v<constant_wrapper<noop_noexcept_fun>::type, constant_wrapper<noop_noexcept_fun>>);
    static_assert(is_same_v<constant_wrapper<&constant_wrapper<'*'>::value>::type,
        constant_wrapper<&constant_wrapper<'*'>::value>>);

    // constant_wrapper::value_type
    static_assert(is_same_v<constant_wrapper<42>::value_type, int>);
    static_assert(is_same_v<constant_wrapper<nullptr>::value_type, nullptr_t>);
    static_assert(is_same_v<constant_wrapper<S{789}>::value_type, S>);
    static_assert(is_same_v<constant_wrapper<i_array>::value_type, const int*>);
    static_assert(is_same_v<constant_wrapper<i_mut_array>::value_type, int*>);
    static_assert(is_same_v<constant_wrapper<noop_fun>::value_type, void (*)()>);
    static_assert(is_same_v<constant_wrapper<noop_noexcept_fun>::value_type, void (*)() noexcept>);
    static_assert(is_same_v<constant_wrapper<&constant_wrapper<'*'>::value>::value_type, const char*>);

    // operator decltype(value)
    {
        static_assert(is_nothrow_convertible_v<constant_wrapper<84>, int>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<2.25>, double>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<static_cast<const void*>(nullptr)>, const void*>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<i_array>, const int*>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<i_mut_array>, int*>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<noop_fun>, void (*)()>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<noop_noexcept_fun>, void (*)() noexcept>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<S{1729}>, const S&>);

        static_assert(is_nothrow_convertible_v<constant_wrapper<84>, int&&>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<2.25>, double&&>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<static_cast<const void*>(nullptr)>, const void*&&>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<i_array>, const int*&&>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<i_mut_array>, int*&&>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<noop_fun>, void (*&&)()>);
        static_assert(is_nothrow_convertible_v<constant_wrapper<noop_noexcept_fun>, void (*&&)() noexcept>);
        static_assert(!is_convertible_v<constant_wrapper<S{1729}>, S&&>);

        int v1                = cw<84>;
        double v2             = cw<2.25>;
        const void* v3        = cw<static_cast<const void*>(nullptr)>;
        const int* v4         = cw<i_array>;
        int* v5               = cw<i_mut_array>;
        void (*v6)()          = cw<noop_fun>;
        void (*v7)() noexcept = cw<noop_noexcept_fun>;
        const S& r1           = cw<S{1729}>;

        assert(v1 == 84);
        assert(v2 == 2.25);
        assert(v3 == nullptr);
        assert(v4 == +i_array);
        assert(v5 == +i_mut_array);
        assert(v6 == &noop_fun);
        assert(v7 == &noop_noexcept_fun);
        assert(&r1.val_ == &constant_wrapper<S{1729}>::value.val_);
        assert(r1.val_ == 1729);
    }

    // unary operator+
    {
        static_assert(CanUnaryPlus<constant_wrapper<'\052'>>);
        static_assert(CanUnaryPlus<constant_wrapper<2.25>>);
        static_assert(CanUnaryPlus<constant_wrapper<i_array>>);
        static_assert(CanUnaryPlus<constant_wrapper<S{1729}>>);

        static_assert(noexcept(+cw<'\052'>));
        static_assert(noexcept(+cw<2.25>));
        static_assert(noexcept(+cw<i_array>));
        static_assert(noexcept(+cw<S{1729}>));

        [[maybe_unused]] same_as<constant_wrapper<42>> decltype(auto) cw1                 = +cw<'\052'>;
        [[maybe_unused]] same_as<constant_wrapper<2.25>> decltype(auto) cw2               = +cw<2.25>;
        [[maybe_unused]] same_as<constant_wrapper<i_array>> decltype(auto) cw3            = +cw<i_array>;
        [[maybe_unused]] same_as<constant_wrapper<S::UnaryPlus{1729}>> decltype(auto) cw4 = +cw<S{1729}>;
    }

    // unary operator-
    {
        static_assert(CanUnaryMinus<constant_wrapper<'\052'>>);
        static_assert(CanUnaryMinus<constant_wrapper<2.25>>);
        static_assert(!CanUnaryMinus<constant_wrapper<i_array>>);
        static_assert(CanUnaryMinus<constant_wrapper<S{1729}>>);

        static_assert(noexcept(-cw<'\052'>));
        static_assert(noexcept(-cw<2.25>));
        static_assert(noexcept(-cw<S{1729}>));

        [[maybe_unused]] same_as<constant_wrapper<-42>> decltype(auto) cw1                  = -cw<'\052'>;
        [[maybe_unused]] same_as<constant_wrapper<-2.25>> decltype(auto) cw2                = -cw<2.25>;
        [[maybe_unused]] same_as<constant_wrapper<S::UnaryMinus{-1729}>> decltype(auto) cw3 = -cw<S{1729}>;
    }

    // operator~
    {
        static_assert(CanComplement<constant_wrapper<'\052'>>);
        static_assert(!CanComplement<constant_wrapper<2.25>>);
        static_assert(!CanComplement<constant_wrapper<i_array>>);
        static_assert(CanComplement<constant_wrapper<S{1729}>>);

        static_assert(noexcept(~cw<'\052'>));
        static_assert(noexcept(~cw<S{1729}>));

        [[maybe_unused]] same_as<constant_wrapper<-43>> decltype(auto) cw1                  = ~cw<'\052'>;
        [[maybe_unused]] same_as<constant_wrapper<S::Complement{-1730}>> decltype(auto) cw2 = ~cw<S{1729}>;
    }

    // operator!
    {
        static_assert(CanNegate<constant_wrapper<'*'>>);
        static_assert(CanNegate<constant_wrapper<'\0'>>);
        static_assert(CanNegate<constant_wrapper<2.25>>);
        static_assert(CanNegate<constant_wrapper<0.0>>);
        static_assert(CanNegate<constant_wrapper<i_mut_array>>);
        static_assert(CanNegate<constant_wrapper<static_cast<int*>(nullptr)>>);
        static_assert(CanNegate<constant_wrapper<S{1729}>>);
        static_assert(CanNegate<constant_wrapper<S{}>>);

        static_assert(noexcept(!cw<'*'>));
        static_assert(noexcept(!cw<'\0'>));
        static_assert(noexcept(!cw<2.25>));
        static_assert(noexcept(!cw<0.0>));
        static_assert(noexcept(!cw<i_mut_array>));
        static_assert(noexcept(!cw<static_cast<int*>(nullptr)>));
        static_assert(noexcept(!cw<S{1729}>));
        static_assert(noexcept(!cw<S{}>));

        [[maybe_unused]] same_as<constant_wrapper<false>> decltype(auto) cw1 = !cw<'*'>;
        [[maybe_unused]] same_as<constant_wrapper<true>> decltype(auto) cw2  = !cw<'\0'>;
        [[maybe_unused]] same_as<constant_wrapper<false>> decltype(auto) cw3 = !cw<2.25>;
        [[maybe_unused]] same_as<constant_wrapper<true>> decltype(auto) cw4  = !cw<0.0>;
        [[maybe_unused]] same_as<constant_wrapper<false>> decltype(auto) cw5 = !cw<i_mut_array>;
        [[maybe_unused]] same_as<constant_wrapper<true>> decltype(auto) cw6  = !cw<static_cast<int*>(nullptr)>;
        [[maybe_unused]] same_as<constant_wrapper<S::Negate{false}>> decltype(auto) cw7 = !cw<S{1729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::Negate{true}>> decltype(auto) cw8  = !cw<S{}>;
    }

    // unary operator*
    {
        static_assert(!CanDereference<constant_wrapper<'*'>>);
        static_assert(!CanDereference<constant_wrapper<2.25>>);
#ifndef __EDG__ // TRANSITION
        static_assert(CanDereference<constant_wrapper<i_array>>);
#endif // ^^^ no workaround ^^^
        static_assert(CanDereference<constant_wrapper<S{729}>>);

#ifndef __EDG__ // TRANSITION
        static_assert(noexcept(*cw<i_array>));
#endif // ^^^ no workaround ^^^
        static_assert(noexcept(*cw<S{729}>));

        [[maybe_unused]] same_as<constant_wrapper<S::DerefCopy{729}>> decltype(auto) cw1 = *cw<S{729}>;
#ifndef __EDG__ // TRANSITION
        [[maybe_unused]] same_as<constant_wrapper<172>> decltype(auto) cw2 = *cw<i_array>;
#endif // ^^^ no workaround ^^^
    }

    // unary operator&
    {
        struct UsualNonaddressable {
            void operator&() const volatile = delete;
        };

        static_assert(CanUnaryAnd<constant_wrapper<'*'>>);
        static_assert(CanUnaryAnd<constant_wrapper<2.25>>);
        static_assert(CanUnaryAnd<constant_wrapper<i_array>>);
        static_assert(CanUnaryAnd<constant_wrapper<S{1729}>>);
        static_assert(CanUnaryAnd<constant_wrapper<NoOperation{}>>);
        static_assert(CanUnaryAnd<constant_wrapper<UsualNonaddressable{}>>);
        static_assert(!CanUnaryAnd<constant_wrapper<Nonaddressable{}>>);

        static_assert(CanUnaryAndRvalue<constant_wrapper<'*'>>);
        static_assert(CanUnaryAndRvalue<constant_wrapper<2.25>>);
        static_assert(CanUnaryAndRvalue<constant_wrapper<i_array>>);
        static_assert(CanUnaryAndRvalue<constant_wrapper<S{1729}>>);
        static_assert(CanUnaryAndRvalue<constant_wrapper<NoOperation{}>>);
#if defined(__clang__) && !defined(__EDG__) // TRANSITION
        static_assert(!CanUnaryAndRvalue<constant_wrapper<UsualNonaddressable{}>>);
#endif // ^^^ no workaround ^^^
        static_assert(!CanUnaryAndRvalue<constant_wrapper<Nonaddressable{}>>);

        static_assert(noexcept(&cw<'*'>));
        static_assert(noexcept(&cw<2.25>));
        static_assert(noexcept(&cw<i_array>));
        static_assert(noexcept(&cw<S{1729}>));
        static_assert(noexcept(&cw<NoOperation{}>));
        static_assert(noexcept(&cw<UsualNonaddressable{}>));

        [[maybe_unused]] same_as<constant_wrapper<&constant_wrapper<'*'>::value>> decltype(auto) cw1     = &cw<'*'>;
        [[maybe_unused]] same_as<constant_wrapper<&constant_wrapper<2.25>::value>> decltype(auto) cw2    = &cw<2.25>;
        [[maybe_unused]] same_as<constant_wrapper<&constant_wrapper<i_array>::value>> decltype(auto) cw3 = &cw<i_array>;
        [[maybe_unused]] same_as<constant_wrapper<S::UnaryAnd{&constant_wrapper<S{1729}>::value.val_}>> decltype(auto)
            cw4 = &cw<S{1729}>;
        [[maybe_unused]] same_as<constant_wrapper<&constant_wrapper<NoOperation{}>::value>> decltype(auto) cw5 =
            &cw<NoOperation{}>;

        // &cw<UsualNonaddressable{}> uses built-in operator&.
        same_as<const constant_wrapper<UsualNonaddressable{}>*> decltype(auto) v6 = &cw<UsualNonaddressable{}>;
        assert(v6 == addressof(cw<UsualNonaddressable{}>));
    }

    // operator+
    {
        static_assert(CanPlus<constant_wrapper<'\052'>, int>);
        static_assert(CanPlus<char, constant_wrapper<16>>);
        static_assert(CanPlus<constant_wrapper<'\040'>, constant_wrapper<42.0>>);
        static_assert(!CanPlus<int, constant_wrapper<nullptr>>);
        static_assert(!CanPlus<constant_wrapper<nullptr>, char>);
        static_assert(!CanPlus<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanPlus<constant_wrapper<i_array>, int>);
        static_assert(CanPlus<long, constant_wrapper<i_mut_array>>);
        static_assert(CanPlus<constant_wrapper<ptrdiff_t{1}>, constant_wrapper<i_array>>);
        static_assert(CanPlus<constant_wrapper<i_array>, constant_wrapper<ptrdiff_t{0}>>);
        static_assert(CanPlus<constant_wrapper<S{421}>, constant_wrapper<S{729}>>);
        static_assert(CanPlus<constant_wrapper<S{21}>, constant_wrapper<96>>);
        static_assert(CanPlus<constant_wrapper<-128>, constant_wrapper<S{127}>>);

        static_assert(noexcept(cw<'\052'> + 42));
        static_assert(noexcept('\101' + cw<16>));
        static_assert(noexcept(cw<'\040'> + cw<42.0>));
        static_assert(noexcept(cw<i_array> + 1));
        static_assert(noexcept(0L + cw<i_mut_array>));
        static_assert(noexcept(cw<ptrdiff_t{1}> + cw<i_array>));
        static_assert(noexcept(cw<i_array> + cw<ptrdiff_t{0}>));
        static_assert(noexcept(cw<S{421}> + cw<S{729}>));
        static_assert(noexcept(cw<-128> + cw<S{127}>));
        static_assert(noexcept(cw<S{21}> + cw<96>));

        same_as<int> decltype(auto) v1        = cw<'\052'> + 42;
        same_as<int> decltype(auto) v2        = '\101' + cw<16>;
        same_as<const int*> decltype(auto) v3 = cw<i_array> + 1;
        same_as<int*> decltype(auto) v4       = 0L + cw<i_mut_array>;

        assert(v1 == 84);
        assert(v2 == 81);
        assert(v3 == i_array + 1);
        assert(v4 == +i_mut_array);

        [[maybe_unused]] same_as<constant_wrapper<74.0>> decltype(auto) cw1          = cw<'\040'> + cw<42.0>;
        [[maybe_unused]] same_as<constant_wrapper<S::Plus{1150}>> decltype(auto) cw2 = cw<S{421}> + cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<i_array + 1>> decltype(auto) cw3   = cw<ptrdiff_t{1}> + cw<i_array>;
        [[maybe_unused]] same_as<constant_wrapper<i_array + 0>> decltype(auto) cw4   = cw<i_array> + cw<ptrdiff_t{0}>;
        [[maybe_unused]] same_as<constant_wrapper<S::PlusLeftInt{-1}>> decltype(auto) cw5   = cw<-128> + cw<S{127}>;
        [[maybe_unused]] same_as<constant_wrapper<S::PlusRightInt{117}>> decltype(auto) cw6 = cw<S{21}> + cw<96>;
    }

    // operator-
    {
        static_assert(CanMinus<constant_wrapper<'\052'>, int>);
        static_assert(CanMinus<char, constant_wrapper<16>>);
        static_assert(CanMinus<constant_wrapper<'\040'>, constant_wrapper<42.0>>);
        static_assert(!CanMinus<int, constant_wrapper<nullptr>>);
        static_assert(!CanMinus<constant_wrapper<nullptr>, char>);
        static_assert(!CanMinus<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanMinus<constant_wrapper<i_array>, int>);
        static_assert(!CanMinus<long, constant_wrapper<i_mut_array>>);
        static_assert(!CanMinus<constant_wrapper<ptrdiff_t{1}>, constant_wrapper<i_array>>);
        static_assert(CanMinus<constant_wrapper<i_array>, constant_wrapper<ptrdiff_t{0}>>);
        static_assert(CanMinus<constant_wrapper<i_array + 0>, constant_wrapper<i_array + 1>>);
        static_assert(CanMinus<constant_wrapper<S{729}>, constant_wrapper<S{421}>>);
        static_assert(CanMinus<constant_wrapper<S{21}>, constant_wrapper<96>>);
        static_assert(CanMinus<constant_wrapper<-128>, constant_wrapper<S{127}>>);

        static_assert(noexcept(cw<'\052'> - 42));
        static_assert(noexcept('\101' - cw<16>));
        static_assert(noexcept(cw<'\040'> - cw<42.0>));
        static_assert(noexcept(cw<i_array + 0> - cw<i_array + 1>));
        static_assert(noexcept(cw<S{729}> - cw<S{421}>));
        static_assert(noexcept(cw<-128> - cw<S{127}>));
        static_assert(noexcept(cw<S{21}> - cw<96>));

        same_as<int> decltype(auto) v1 = cw<'\052'> - 42;
        same_as<int> decltype(auto) v2 = '\101' - cw<16>;

        assert(v1 == 0);
        assert(v2 == 49);

        [[maybe_unused]] same_as<constant_wrapper<-10.0>> decltype(auto) cw1   = cw<'\040'> - cw<42.0>;
        [[maybe_unused]] same_as<constant_wrapper<i_array>> decltype(auto) cw2 = cw<i_array> - cw<ptrdiff_t{0}>;
        [[maybe_unused]] same_as<constant_wrapper<ptrdiff_t{-1}>> decltype(auto) cw3 =
            cw<i_array + 0> - cw<i_array + 1>;
        [[maybe_unused]] same_as<constant_wrapper<S::Minus{308}>> decltype(auto) cw4         = cw<S{729}> - cw<S{421}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MinusLeftInt{-255}>> decltype(auto) cw5 = cw<-128> - cw<S{127}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MinusRightInt{-75}>> decltype(auto) cw6 = cw<S{21}> - cw<96>;
    }

    // operator*
    {
        static_assert(CanMultiply<constant_wrapper<'\052'>, int>);
        static_assert(CanMultiply<char, constant_wrapper<2L>>);
        static_assert(CanMultiply<constant_wrapper<'\040'>, constant_wrapper<2.5>>);
        static_assert(!CanMultiply<int, constant_wrapper<nullptr>>);
        static_assert(!CanMultiply<constant_wrapper<nullptr>, char>);
        static_assert(!CanMultiply<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanMultiply<constant_wrapper<S{17}>, constant_wrapper<29>>);
        static_assert(CanMultiply<constant_wrapper<-1>, constant_wrapper<S{729}>>);
        static_assert(CanMultiply<constant_wrapper<S{172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<'\052'> * 2));
        static_assert(noexcept('\040' * cw<2L>));
        static_assert(noexcept(cw<'\040'> * cw<2.5>));
        static_assert(noexcept(cw<S{17}> * cw<29>));
        static_assert(noexcept(cw<-1> * cw<S{729}>));
        static_assert(noexcept(cw<S{172}> * cw<S{-9}>));

        same_as<int> decltype(auto) v1  = cw<'\052'> * 2;
        same_as<long> decltype(auto) v2 = '\040' * cw<2L>;

        assert(v1 == 84);
        assert(v2 == 64L);

        [[maybe_unused]] same_as<constant_wrapper<80.0>> decltype(auto) cw1                     = cw<'\040'> * cw<2.5>;
        [[maybe_unused]] same_as<constant_wrapper<S::Multiply{493}>> decltype(auto) cw2         = cw<S{17}> * cw<S{29}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MultiplyLeftInt{-729}>> decltype(auto) cw3 = cw<-1> * cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MultiplyRightInt{-1548}>> decltype(auto) cw4 = cw<S{172}> * cw<-9>;
    }

    // operator/
    {
        static_assert(CanDivide<constant_wrapper<'\052'>, int>);
        static_assert(CanDivide<char, constant_wrapper<2L>>);
        static_assert(CanDivide<constant_wrapper<'\040'>, constant_wrapper<8.0>>);
        static_assert(!CanDivide<int, constant_wrapper<nullptr>>);
        static_assert(!CanDivide<constant_wrapper<nullptr>, char>);
        static_assert(!CanDivide<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanDivide<constant_wrapper<S{1729}>, constant_wrapper<42>>);
        static_assert(CanDivide<constant_wrapper<-126>, constant_wrapper<S{-3}>>);
        static_assert(CanDivide<constant_wrapper<S{172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<'\052'> / 2));
        static_assert(noexcept('\040' / cw<2L>));
        static_assert(noexcept(cw<'\040'> / cw<8.0>));
        static_assert(noexcept(cw<S{17}> / cw<29>));
        static_assert(noexcept(cw<-126> / cw<S{-3}>));
        static_assert(noexcept(cw<S{172}> / cw<S{-9}>));

        same_as<int> decltype(auto) v1  = cw<'\052'> / 2;
        same_as<long> decltype(auto) v2 = '\040' / cw<2L>;

        assert(v1 == 21);
        assert(v2 == 16L);

        [[maybe_unused]] same_as<constant_wrapper<4.0>> decltype(auto) cw1                    = cw<'\040'> / cw<8.0>;
        [[maybe_unused]] same_as<constant_wrapper<S::Divide{41}>> decltype(auto) cw2          = cw<S{1729}> / cw<S{42}>;
        [[maybe_unused]] same_as<constant_wrapper<S::DivideLeftInt{42}>> decltype(auto) cw3   = cw<-126> / cw<S{-3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::DivideRightInt{-19}>> decltype(auto) cw4 = cw<S{172}> / cw<-9>;
    }

    // operator%
    {
        static_assert(CanModulo<constant_wrapper<'\052'>, int>);
        static_assert(CanModulo<char, constant_wrapper<-23L>>);
        static_assert(!CanModulo<constant_wrapper<'\040'>, constant_wrapper<8.0>>);
        static_assert(!CanModulo<constant_wrapper<42.5>, constant_wrapper<5>>);
        static_assert(!CanModulo<int, constant_wrapper<nullptr>>);
        static_assert(!CanModulo<constant_wrapper<nullptr>, char>);
        static_assert(!CanModulo<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanModulo<constant_wrapper<S{1729}>, constant_wrapper<42>>);
        static_assert(CanModulo<constant_wrapper<-126>, constant_wrapper<S{-3}>>);
        static_assert(CanModulo<constant_wrapper<S{-172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<'\052'> % 5));
        static_assert(noexcept('\040' % cw<3L>));
        static_assert(noexcept(cw<S{17}> % cw<29>));
        static_assert(noexcept(cw<-126> % cw<S{-3}>));
        static_assert(noexcept(cw<S{-172}> % cw<S{-9}>));

        same_as<int> decltype(auto) v1  = cw<'\052'> % 5;
        same_as<long> decltype(auto) v2 = '\040' % cw<-23L>;

        assert(v1 == 2);
        assert(v2 == 9L);

        [[maybe_unused]] same_as<constant_wrapper<S::Modulo{17}>> decltype(auto) cw1         = cw<S{17}> % cw<S{29}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ModuloLeftInt{0}>> decltype(auto) cw2   = cw<-126> % cw<S{-3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ModuloRightInt{-1}>> decltype(auto) cw3 = cw<S{-172}> % cw<-9>;
    }

    // operator<<
    {
        static_assert(CanShl<constant_wrapper<42>, int>);
        static_assert(CanShl<unsigned int, constant_wrapper<3L>>);
        static_assert(CanShl<constant_wrapper<84UL>, constant_wrapper<5LL>>);
        static_assert(!CanShl<constant_wrapper<42>, double>);
        static_assert(!CanShl<float, constant_wrapper<3L>>);
        static_assert(!CanShl<constant_wrapper<84UL>, constant_wrapper<5.0L>>);
        static_assert(CanShl<constant_wrapper<byte{42}>, int>);
        static_assert(CanShl<constant_wrapper<byte{3}>, constant_wrapper<3L>>);
        static_assert(!CanShl<constant_wrapper<byte{42}>, byte>);
        static_assert(!CanShl<constant_wrapper<byte{3}>, constant_wrapper<byte{3}>>);
        static_assert(!CanShl<int, constant_wrapper<nullptr>>);
        static_assert(!CanShl<constant_wrapper<nullptr>, char>);
        static_assert(!CanShl<constant_wrapper<nullptr>, constant_wrapper<4>>);
        static_assert(CanShl<constant_wrapper<S{1729}>, constant_wrapper<4>>);
        static_assert(CanShl<constant_wrapper<-126>, constant_wrapper<S{3}>>);
        static_assert(CanShl<constant_wrapper<S{-17}>, constant_wrapper<9>>);

        static_assert(noexcept(cw<42> << 4));
        static_assert(noexcept(52U << cw<3L>));
        static_assert(noexcept(cw<84UL> << cw<5LL>));
        static_assert(noexcept(cw<byte{42}> << 2));
        static_assert(noexcept(cw<byte{3}> << cw<3L>));
        static_assert(noexcept(cw<S{1729}> << cw<S{4}>));
        static_assert(noexcept(cw<-126> << cw<S{3}>));
        static_assert(noexcept(cw<S{-17}> << cw<9>));

        same_as<int> decltype(auto) v1          = cw<42> << 4;
        same_as<unsigned int> decltype(auto) v2 = 52U << cw<3L>;
        same_as<byte> decltype(auto) v3         = cw<byte{42}> << 2;

        assert(v1 == 672);
        assert(v2 == 416U);
        assert(v3 == byte{168});

        [[maybe_unused]] same_as<constant_wrapper<2688UL>> decltype(auto) cw1                = cw<84UL> << cw<5LL>;
        [[maybe_unused]] same_as<constant_wrapper<byte{24}>> decltype(auto) cw2              = cw<byte{3}> << cw<3L>;
        [[maybe_unused]] same_as<constant_wrapper<S::Shl{27664}>> decltype(auto) cw3         = cw<S{1729}> << cw<S{4}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShlLeftInt{-1008}>> decltype(auto) cw4  = cw<-126> << cw<S{3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShlRightInt{-8704}>> decltype(auto) cw5 = cw<S{-17}> << cw<9>;
    }

    // operator>>
    {
        static_assert(CanShr<constant_wrapper<42>, int>);
        static_assert(CanShr<unsigned int, constant_wrapper<3L>>);
        static_assert(CanShr<constant_wrapper<84UL>, constant_wrapper<5LL>>);
        static_assert(!CanShr<constant_wrapper<42>, double>);
        static_assert(!CanShr<float, constant_wrapper<3L>>);
        static_assert(!CanShr<constant_wrapper<84UL>, constant_wrapper<5.0L>>);
        static_assert(CanShr<constant_wrapper<byte{42}>, int>);
        static_assert(CanShr<constant_wrapper<byte{129}>, constant_wrapper<3L>>);
        static_assert(!CanShr<constant_wrapper<byte{72}>, byte>);
        static_assert(!CanShr<constant_wrapper<byte{255}>, constant_wrapper<byte{3}>>);
        static_assert(!CanShr<int, constant_wrapper<nullptr>>);
        static_assert(!CanShr<constant_wrapper<nullptr>, char>);
        static_assert(!CanShr<constant_wrapper<nullptr>, constant_wrapper<4>>);
        static_assert(CanShr<constant_wrapper<S{1729}>, constant_wrapper<4>>);
        static_assert(CanShr<constant_wrapper<-126>, constant_wrapper<S{3}>>);
        static_assert(CanShr<constant_wrapper<S{-32768}>, constant_wrapper<9>>);

        static_assert(noexcept(cw<42> >> 4));
        static_assert(noexcept(52U >> cw<3L>));
        static_assert(noexcept(cw<84UL> >> cw<5LL>));
        static_assert(noexcept(cw<byte{42}> >> 2));
        static_assert(noexcept(cw<byte{3}> >> cw<3L>));
        static_assert(noexcept(cw<S{1729}> >> cw<S{4}>));
        static_assert(noexcept(cw<-126> >> cw<S{3}>));
        static_assert(noexcept(cw<S{-32768}> >> cw<9>));

        same_as<int> decltype(auto) v1          = cw<42> >> 4;
        same_as<unsigned int> decltype(auto) v2 = 52U >> cw<3L>;
        same_as<byte> decltype(auto) v3         = cw<byte{42}> >> 2;

        assert(v1 == 2);
        assert(v2 == 6U);
        assert(v3 == byte{10});

        [[maybe_unused]] same_as<constant_wrapper<2UL>> decltype(auto) cw1                 = cw<84UL> >> cw<5LL>;
        [[maybe_unused]] same_as<constant_wrapper<byte{0}>> decltype(auto) cw2             = cw<byte{3}> >> cw<3L>;
        [[maybe_unused]] same_as<constant_wrapper<S::Shr{108}>> decltype(auto) cw3         = cw<S{1729}> >> cw<S{4}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShrLeftInt{-16}>> decltype(auto) cw4  = cw<-126> >> cw<S{3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShrRightInt{-64}>> decltype(auto) cw5 = cw<S{-32768}> >> cw<9>;
    }

    // operator&
    {
        static_assert(CanBitAnd<int, constant_wrapper<'\052'>>);
        static_assert(CanBitAnd<constant_wrapper<255U>, char16_t>);
        static_assert(CanBitAnd<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanBitAnd<double, constant_wrapper<'\052'>>);
        static_assert(!CanBitAnd<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanBitAnd<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(CanBitAnd<byte, constant_wrapper<byte{121}>>);
        static_assert(CanBitAnd<constant_wrapper<byte{251}>, byte>);
        static_assert(CanBitAnd<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanBitAnd<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanBitAnd<constant_wrapper<byte{251}>, int>);
        static_assert(!CanBitAnd<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanBitAnd<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanBitAnd<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanBitAnd<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(27 & cw<'\052'>));
        static_assert(noexcept(cw<255U> & u'\u0325'));
        static_assert(noexcept(cw<421L> & cw<729LL>));
        static_assert(noexcept(byte{37} & cw<byte{121}>));
        static_assert(noexcept(cw<byte{251}> & byte{104}));
        static_assert(noexcept(cw<byte{151}> & cw<byte{242}>));
        static_assert(noexcept(cw<S{32589}> & cw<S{-12751}>));
        static_assert(noexcept(cw<21124> & cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> & cw<95>));

        same_as<int> decltype(auto) v1          = 27 & cw<'\052'>;
        same_as<unsigned int> decltype(auto) v2 = cw<255U> & u'\u0325';
        same_as<byte> decltype(auto) v3         = byte{37} & cw<byte{121}>;
        same_as<byte> decltype(auto) v4         = cw<byte{251}> & byte{104};

        assert(v1 == 10);
        assert(v2 == 37U);
        assert(v3 == byte{33});
        assert(v4 == byte{104});

        [[maybe_unused]] same_as<constant_wrapper<129LL>> decltype(auto) cw1            = cw<421L> & cw<729LL>;
        [[maybe_unused]] same_as<constant_wrapper<byte{146}>> decltype(auto) cw2        = cw<byte{151}> & cw<byte{242}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitAnd{19969}>> decltype(auto) cw3 = cw<S{32589}> & cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitAndLeftInt{20612}>> decltype(auto) cw4 =
            cw<21124> & cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitAndRightInt{91}>> decltype(auto) cw5 = cw<S{123}> & cw<95>;
    }

    // operator|
    {
        static_assert(CanBitOr<int, constant_wrapper<'\052'>>);
        static_assert(CanBitOr<constant_wrapper<255U>, char16_t>);
        static_assert(CanBitOr<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanBitOr<double, constant_wrapper<'\052'>>);
        static_assert(!CanBitOr<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanBitOr<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(CanBitOr<byte, constant_wrapper<byte{121}>>);
        static_assert(CanBitOr<constant_wrapper<byte{251}>, byte>);
        static_assert(CanBitOr<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanBitOr<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanBitOr<constant_wrapper<byte{251}>, int>);
        static_assert(!CanBitOr<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanBitOr<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanBitOr<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanBitOr<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(27 | cw<'\052'>));
        static_assert(noexcept(cw<255U> | u'\u0325'));
        static_assert(noexcept(cw<421L> | cw<729LL>));
        static_assert(noexcept(byte{37} | cw<byte{121}>));
        static_assert(noexcept(cw<byte{251}> | byte{104}));
        static_assert(noexcept(cw<byte{151}> | cw<byte{242}>));
        static_assert(noexcept(cw<S{32589}> | cw<S{-12751}>));
        static_assert(noexcept(cw<21124> | cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> | cw<95>));

        same_as<int> decltype(auto) v1          = 27 | cw<'\052'>;
        same_as<unsigned int> decltype(auto) v2 = cw<255U> | u'\u0325';
        same_as<byte> decltype(auto) v3         = byte{37} | cw<byte{121}>;
        same_as<byte> decltype(auto) v4         = cw<byte{251}> | byte{104};

        assert(v1 == 59);
        assert(v2 == 1023U);
        assert(v3 == byte{125});
        assert(v4 == byte{251});

        [[maybe_unused]] same_as<constant_wrapper<1021LL>> decltype(auto) cw1         = cw<421L> | cw<729LL>;
        [[maybe_unused]] same_as<constant_wrapper<byte{247}>> decltype(auto) cw2      = cw<byte{151}> | cw<byte{242}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitOr{-131}>> decltype(auto) cw3 = cw<S{32589}> | cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitOrLeftInt{-2073}>> decltype(auto) cw4 =
            cw<21124> | cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitOrRightInt{127}>> decltype(auto) cw5 = cw<S{123}> | cw<95>;
    }

    // operator^
    {
        static_assert(CanXor<int, constant_wrapper<'\052'>>);
        static_assert(CanXor<constant_wrapper<255U>, char16_t>);
        static_assert(CanXor<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanXor<double, constant_wrapper<'\052'>>);
        static_assert(!CanXor<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanXor<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(CanXor<byte, constant_wrapper<byte{121}>>);
        static_assert(CanXor<constant_wrapper<byte{251}>, byte>);
        static_assert(CanXor<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanXor<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanXor<constant_wrapper<byte{251}>, int>);
        static_assert(!CanXor<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanXor<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanXor<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanXor<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(27 ^ cw<'\052'>));
        static_assert(noexcept(cw<255U> ^ u'\u0325'));
        static_assert(noexcept(cw<421L> ^ cw<729LL>));
        static_assert(noexcept(byte{37} ^ cw<byte{121}>));
        static_assert(noexcept(cw<byte{251}> ^ byte{104}));
        static_assert(noexcept(cw<byte{151}> ^ cw<byte{242}>));
        static_assert(noexcept(cw<S{32589}> ^ cw<S{-12751}>));
        static_assert(noexcept(cw<21124> ^ cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> ^ cw<95>));

        constexpr same_as<int> decltype(auto) v1          = 27 ^ cw<'\052'>;
        constexpr same_as<unsigned int> decltype(auto) v2 = cw<255U> ^ u'\u0325';
        constexpr same_as<byte> decltype(auto) v3         = byte{37} ^ cw<byte{121}>;
        constexpr same_as<byte> decltype(auto) v4         = cw<byte{251}> ^ byte { 104 };

        assert(v1 == 49);
        assert(v2 == 986U);
        assert(v3 == byte{92});
        assert(v4 == byte{147});

        [[maybe_unused]] same_as<constant_wrapper<892LL>> decltype(auto) cw1          = cw<421L> ^ cw<729LL>;
        [[maybe_unused]] same_as<constant_wrapper<byte{101}>> decltype(auto) cw2      = cw<byte{151}> ^ cw<byte{242}>;
        [[maybe_unused]] same_as<constant_wrapper<S::Xor{-20100}>> decltype(auto) cw3 = cw<S{32589}> ^ cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::XorLeftInt{-22685}>> decltype(auto) cw4 = cw<21124> ^ cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::XorRightInt{36}>> decltype(auto) cw5    = cw<S{123}> ^ cw<95>;
    }

    // operator&&
    {
        static_assert(CanLogicalAnd<constant_wrapper<true>, int>);
        static_assert(CanLogicalAnd<double, constant_wrapper<false>>);
        static_assert(CanLogicalAnd<constant_wrapper<true>, constant_wrapper<false>>);
        static_assert(CanLogicalAnd<constant_wrapper<static_cast<const char*>(nullptr)>, constant_wrapper<i_array>>);
        static_assert(!CanLogicalAnd<constant_wrapper<S{567}>, const int*>);
        static_assert(!CanLogicalAnd<nullptr_t, constant_wrapper<S{789}>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{0}>, constant_wrapper<S{0}>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{0}>, constant_wrapper<S{256}>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{172}>, constant_wrapper<S{0}>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{421}>, constant_wrapper<S{729}>>);
        static_assert(CanLogicalAnd<constant_wrapper<true>, constant_wrapper<S{}>>);
        static_assert(CanLogicalAnd<constant_wrapper<true>, constant_wrapper<S{129}>>);
        static_assert(CanLogicalAnd<constant_wrapper<false>, constant_wrapper<S{}>>);
        static_assert(CanLogicalAnd<constant_wrapper<false>, constant_wrapper<S{-678}>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{}>, constant_wrapper<true>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{129}>, constant_wrapper<true>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{}>, constant_wrapper<false>>);
        static_assert(CanLogicalAnd<constant_wrapper<S{-678}>, constant_wrapper<false>>);

        static_assert(noexcept(cw<true> && 42));
        static_assert(noexcept(cw<true> && 0));
        static_assert(noexcept(1.0 && cw<false>));
        static_assert(noexcept(0.0 && cw<false>));
        static_assert(noexcept(cw<static_cast<const char*>(nullptr)> && cw<i_array>));
        static_assert(noexcept(cw<true> && cw<false>));
        static_assert(noexcept(cw<S{0}> && cw<S{0}>));
        static_assert(noexcept(cw<S{0}> && cw<S{256}>));
        static_assert(noexcept(cw<S{172}> && cw<S{0}>));
        static_assert(noexcept(cw<S{421}> && cw<S{729}>));
        static_assert(noexcept(cw<true> && cw<S{}>));
        static_assert(noexcept(cw<true> && cw<S{129}>));
        static_assert(noexcept(cw<false> && cw<S{}>));
        static_assert(noexcept(cw<false> && cw<S{-678}>));
        static_assert(noexcept(cw<S{}> && cw<true>));
        static_assert(noexcept(cw<S{129}> && cw<true>));
        static_assert(noexcept(cw<S{}> && cw<false>));
        static_assert(noexcept(cw<S{-678}> && cw<false>));

        same_as<bool> decltype(auto) v1 = cw<true> && 42;
        same_as<bool> decltype(auto) v2 = cw<true> && 0;
        same_as<bool> decltype(auto) v3 = 1.0 && cw<false>;
        same_as<bool> decltype(auto) v4 = 0.0 && cw<false>;
        same_as<bool> decltype(auto) v5 = cw<static_cast<const char*>(nullptr)> && cw<i_array>;
        same_as<bool> decltype(auto) v6 = cw<true> && cw<false>;

        assert(v1);
        assert(!v2);
        assert(!v3);
        assert(!v4);
        assert(!v5);
        assert(!v6);

        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAnd{false}>> decltype(auto) cw1 = cw<S{0}> && cw<S{0}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAnd{false}>> decltype(auto) cw2 = cw<S{0}> && cw<S{256}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAnd{false}>> decltype(auto) cw3 = cw<S{172}> && cw<S{0}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAnd{true}>> decltype(auto) cw4  = cw<S{421}> && cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndLeftBool{false}>> decltype(auto) cw5 =
            cw<true> && cw<S{}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndLeftBool{true}>> decltype(auto) cw6 =
            cw<true> && cw<S{129}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndLeftBool{false}>> decltype(auto) cw7 =
            cw<false> && cw<S{}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndLeftBool{false}>> decltype(auto) cw8 =
            cw<false> && cw<S{-678}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndRightBool{false}>> decltype(auto) cw9 =
            cw<S{}> && cw<true>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndRightBool{true}>> decltype(auto) cw10 =
            cw<S{129}> && cw<true>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndRightBool{false}>> decltype(auto) cw11 =
            cw<S{}> && cw<false>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalAndRightBool{false}>> decltype(auto) cw12 =
            cw<S{-678}> && cw<false>;
    }

    // operator||
    {
        static_assert(CanLogicalOr<constant_wrapper<true>, int>);
        static_assert(CanLogicalOr<double, constant_wrapper<false>>);
        static_assert(CanLogicalOr<constant_wrapper<true>, constant_wrapper<false>>);
        static_assert(CanLogicalOr<constant_wrapper<static_cast<const wchar_t*>(nullptr)>, constant_wrapper<i_array>>);
        static_assert(!CanLogicalOr<constant_wrapper<S{567}>, const int*>);
        static_assert(!CanLogicalOr<nullptr_t, constant_wrapper<S{789}>>);
        static_assert(CanLogicalOr<constant_wrapper<S{0}>, constant_wrapper<S{0}>>);
        static_assert(CanLogicalOr<constant_wrapper<S{0}>, constant_wrapper<S{256}>>);
        static_assert(CanLogicalOr<constant_wrapper<S{172}>, constant_wrapper<S{0}>>);
        static_assert(CanLogicalOr<constant_wrapper<S{421}>, constant_wrapper<S{729}>>);
        static_assert(CanLogicalOr<constant_wrapper<true>, constant_wrapper<S{}>>);
        static_assert(CanLogicalOr<constant_wrapper<true>, constant_wrapper<S{129}>>);
        static_assert(CanLogicalOr<constant_wrapper<false>, constant_wrapper<S{}>>);
        static_assert(CanLogicalOr<constant_wrapper<false>, constant_wrapper<S{-678}>>);
        static_assert(CanLogicalOr<constant_wrapper<S{}>, constant_wrapper<true>>);
        static_assert(CanLogicalOr<constant_wrapper<S{129}>, constant_wrapper<true>>);
        static_assert(CanLogicalOr<constant_wrapper<S{}>, constant_wrapper<false>>);
        static_assert(CanLogicalOr<constant_wrapper<S{-678}>, constant_wrapper<false>>);

        static_assert(noexcept(cw<true> || 42));
        static_assert(noexcept(cw<true> || 0));
        static_assert(noexcept(1.0 || cw<false>));
        static_assert(noexcept(0.0 || cw<false>));
        static_assert(noexcept(cw<static_cast<const wchar_t*>(nullptr)> || cw<i_array>));
        static_assert(noexcept(cw<true> || cw<false>));
        static_assert(noexcept(cw<S{0}> || cw<S{0}>));
        static_assert(noexcept(cw<S{0}> || cw<S{256}>));
        static_assert(noexcept(cw<S{172}> || cw<S{0}>));
        static_assert(noexcept(cw<S{421}> || cw<S{729}>));
        static_assert(noexcept(cw<true> || cw<S{}>));
        static_assert(noexcept(cw<true> || cw<S{129}>));
        static_assert(noexcept(cw<false> || cw<S{}>));
        static_assert(noexcept(cw<false> || cw<S{-678}>));
        static_assert(noexcept(cw<S{}> || cw<true>));
        static_assert(noexcept(cw<S{129}> || cw<true>));
        static_assert(noexcept(cw<S{}> || cw<false>));
        static_assert(noexcept(cw<S{-678}> || cw<false>));

        same_as<bool> decltype(auto) v1 = cw<true> || 42;
        same_as<bool> decltype(auto) v2 = cw<true> || 0;
        same_as<bool> decltype(auto) v3 = 1.0 || cw<false>;
        same_as<bool> decltype(auto) v4 = 0.0 || cw<false>;
        same_as<bool> decltype(auto) v5 = cw<static_cast<const wchar_t*>(nullptr)> || cw<i_array>;
        same_as<bool> decltype(auto) v6 = cw<true> || cw<false>;

        assert(v1);
        assert(v2);
        assert(v3);
        assert(!v4);
        assert(v5);
        assert(v6);

        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOr{false}>> decltype(auto) cw1 = cw<S{0}> || cw<S{0}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOr{true}>> decltype(auto) cw2  = cw<S{0}> || cw<S{256}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOr{true}>> decltype(auto) cw3  = cw<S{172}> || cw<S{0}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOr{true}>> decltype(auto) cw4  = cw<S{421}> || cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrLeftBool{true}>> decltype(auto) cw5 = cw<true> || cw<S{}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrLeftBool{true}>> decltype(auto) cw6 =
            cw<true> || cw<S{129}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrLeftBool{false}>> decltype(auto) cw7 =
            cw<false> || cw<S{}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrLeftBool{true}>> decltype(auto) cw8 =
            cw<false> || cw<S{-678}>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrRightBool{true}>> decltype(auto) cw9 =
            cw<S{}> || cw<true>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrRightBool{true}>> decltype(auto) cw10 =
            cw<S{129}> || cw<true>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrRightBool{false}>> decltype(auto) cw11 =
            cw<S{}> || cw<false>;
        [[maybe_unused]] same_as<constant_wrapper<S::LogicalOrRightBool{true}>> decltype(auto) cw12 =
            cw<S{-678}> || cw<false>;
    }

    // operator<=>

    // operator<
    {
        static_assert(CanLess<constant_wrapper<u'\u0000'>, unsigned int>);
        static_assert(CanLess<constant_wrapper<u'*'>, unsigned short>);
        static_assert(CanLess<constant_wrapper<u'~'>, unsigned long long>);
        static_assert(CanLess<unsigned char, constant_wrapper<u'\u0000'>>);
        static_assert(CanLess<unsigned int, constant_wrapper<u'*'>>);
        static_assert(CanLess<unsigned long, constant_wrapper<u'~'>>);
        static_assert(CanLess<constant_wrapper<u'\u0000'>, constant_wrapper<42U>>);
        static_assert(CanLess<constant_wrapper<u'*'>, constant_wrapper<static_cast<unsigned short>(42U)>>);
        static_assert(CanLess<constant_wrapper<u'~'>, constant_wrapper<42ULL>>);
        static_assert(CanLess<constant_wrapper<static_cast<unsigned char>(42U)>, constant_wrapper<u'\u0000'>>);
        static_assert(CanLess<constant_wrapper<42U>, constant_wrapper<u'*'>>);
        static_assert(CanLess<constant_wrapper<42UL>, constant_wrapper<u'~'>>);
        static_assert(CanLess<constant_wrapper<42>, float>);
        static_assert(CanLess<double, constant_wrapper<84.0F>>);
        static_assert(CanLess<constant_wrapper<1234.5>, constant_wrapper<123.0L>>);
        static_assert(!CanLess<constant_wrapper<i_mut_array>, char>);
        static_assert(!CanLess<int, constant_wrapper<nullptr>>);
        static_assert(!CanLess<constant_wrapper<i_array>, constant_wrapper<noop_fun>>);
        static_assert(CanLess<constant_wrapper<i_array>, const int*>);
        static_assert(CanLess<const int*, constant_wrapper<i_array + 1>>);
        static_assert(CanLess<constant_wrapper<i_array>, constant_wrapper<i_array + 1>>);
        static_assert(CanLess<constant_wrapper<i_array>, constant_wrapper<i_array>>);
        static_assert(CanLess<constant_wrapper<i_array + 1>, constant_wrapper<i_array>>);
        static_assert(CanLess<constant_wrapper<S{-17}>, constant_wrapper<S{29}>>);
        static_assert(CanLess<constant_wrapper<S{94}>, constant_wrapper<S{94}>>);
        static_assert(CanLess<constant_wrapper<S{1729}>, constant_wrapper<S{42}>>);
        static_assert(CanLess<constant_wrapper<-17>, constant_wrapper<S{29}>>);
        static_assert(CanLess<constant_wrapper<94>, constant_wrapper<S{94}>>);
        static_assert(CanLess<constant_wrapper<1729>, constant_wrapper<S{42}>>);
        static_assert(CanLess<constant_wrapper<S{-17}>, constant_wrapper<29>>);
        static_assert(CanLess<constant_wrapper<S{94}>, constant_wrapper<94>>);
        static_assert(CanLess<constant_wrapper<S{1729}>, constant_wrapper<42>>);

        static_assert(noexcept(cw<u'\u0000'> < 42U));
        static_assert(noexcept(cw<u'*'> < static_cast<unsigned short>(42U)));
        static_assert(noexcept(cw<u'~'> < 42ULL));
        static_assert(noexcept(static_cast<unsigned char>(42U) < cw<u'\u0000'>));
        static_assert(noexcept(42U < cw<u'*'>));
        static_assert(noexcept(42UL < cw<u'~'>));
        static_assert(noexcept(cw<42> < 40.0F));
        static_assert(noexcept(72.5 < cw<84.0F>));
        static_assert(noexcept(cw<1234.5> < cw<123.0L>));
        static_assert(noexcept(cw<i_array> < i_array + 1));
        static_assert(noexcept(i_array, cw<i_array + 1>));
        static_assert(noexcept(cw<i_array> < cw<i_array + 1>));
        static_assert(noexcept(cw<i_array> < cw<i_array>));
        static_assert(noexcept(cw<i_array + 1> < cw<i_array>));
        static_assert(noexcept(cw<S{-17}> < cw<S{29}>));
        static_assert(noexcept(cw<S{94}> < cw<S{94}>));
        static_assert(noexcept(cw<S{1729}> < cw<S{42}>));
        static_assert(noexcept(cw<-17> < cw<S{29}>));
        static_assert(noexcept(cw<94> < cw<S{94}>));
        static_assert(noexcept(cw<1729> < cw<S{42}>));
        static_assert(noexcept(cw<S{-17}> < cw<29>));
        static_assert(noexcept(cw<S{94}> < cw<94>));
        static_assert(noexcept(cw<S{1729}> < cw<42>));
    }

    // operator<=

    // operator==

    // operator!=

    // operator>

    // operator>=

    // operator, (deleted)
    static_assert(CanComma<constant_wrapper<'*'>, int>);
    static_assert(CanComma<S, constant_wrapper<nullptr>>);
    static_assert(!CanComma<constant_wrapper<42>, constant_wrapper<42>>);
    static_assert(!CanComma<constant_wrapper<'*'>, constant_wrapper<2.25>>);
    static_assert(!CanComma<constant_wrapper<S{123}>, constant_wrapper<NoOperation{}>>);

    static_assert(std::is_same_v<decltype(cw<'*'>, 0), int>);
    static_assert(std::is_same_v<decltype((S{}, cw<nullptr>) ), const constant_wrapper<nullptr>&>);

    // operator->*
    {
        static_assert(!CanArrowStar<constant_wrapper<'*'>, int>);
        static_assert(!CanArrowStar<char, constant_wrapper<42>>);
        static_assert(!CanArrowStar<constant_wrapper<'*'>, constant_wrapper<42>>);
        static_assert(!CanArrowStar<constant_wrapper<nullptr>, ptrdiff_t>);
        static_assert(!CanArrowStar<double, constant_wrapper<i_mut_array>>);
        static_assert(!CanArrowStar<constant_wrapper<i_array>, constant_wrapper<1ULL>>);
        static_assert(CanArrowStar<constant_wrapper<S{17}>, constant_wrapper<S{-29}>>);
        static_assert(CanArrowStar<constant_wrapper<-123>, constant_wrapper<S{-45}>>);
        static_assert(CanArrowStar<constant_wrapper<S{42}>, constant_wrapper<84>>);

        constexpr auto ptr = addressof(constant_wrapper<S{1729}>::value);
        static_assert(CanArrowStar<constant_wrapper<ptr>, int S::*>);
#ifndef __EDG__ // TRANSITION
        static_assert(CanArrowStar<constant_wrapper<ptr>, constant_wrapper<&S::val_>>);
#endif

        static_assert(noexcept(cw<S{17}>->*cw<S{-29}>));
        static_assert(noexcept(cw<ptr>->*&S::val_));

#ifndef __EDG__ // TRANSITION
        same_as<const int&>
#endif
            decltype(auto) v1 = cw<ptr>->*&S::val_;

        assert(&v1 == &ptr->val_);

        [[maybe_unused]] same_as<constant_wrapper<S::ArrowStar{-493}>> decltype(auto) cw1 = cw<S{17}>->*cw<S{-29}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ArrowStarLeftInt{5535}>> decltype(auto) cw2 =
            cw<-123>->*cw<S{-45}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ArrowStarRightInt{3528}>> decltype(auto) cw3 = cw<S{42}>->*cw<84>;
#ifndef __EDG__ // TRANSITION
        [[maybe_unused]] same_as<constant_wrapper<1729>> decltype(auto) cw4 = cw<ptr>->*cw<&S::val_>;
#endif
    }

    // pre operator++
    {
        static_assert(!CanPseudoPreIncrement<constant_wrapper<'*'>>);
        static_assert(!CanPseudoPreIncrement<constant_wrapper<2.25>>);
        static_assert(!CanPseudoPreIncrement<constant_wrapper<i_array>>);
        static_assert(CanPseudoPreIncrement<constant_wrapper<S{123}>>);

        static_assert(noexcept(++cw<S{123}>));

        [[maybe_unused]] same_as<constant_wrapper<S::PreInc{124}>> decltype(auto) cw1 = ++cw<S{123}>;
    }

    // post operator++
    {
        static_assert(!CanPseudoPostIncrement<constant_wrapper<'*'>>);
        static_assert(!CanPseudoPostIncrement<constant_wrapper<2.25>>);
        static_assert(!CanPseudoPostIncrement<constant_wrapper<i_array>>);
        static_assert(CanPseudoPostIncrement<constant_wrapper<S{234}>>);

        static_assert(noexcept(cw<S{234}> ++));

        [[maybe_unused]] same_as<constant_wrapper<S::PostInc{235}>> decltype(auto) cw1 = cw<S{234}> ++;
    }

    // pre operator--
    {
        static_assert(!CanPseudoPreDecrement<constant_wrapper<'*'>>);
        static_assert(!CanPseudoPreDecrement<constant_wrapper<2.25>>);
        static_assert(!CanPseudoPreDecrement<constant_wrapper<i_array>>);
        static_assert(CanPseudoPreDecrement<constant_wrapper<S{345}>>);

        static_assert(noexcept(--cw<S{345}>));

        [[maybe_unused]] same_as<constant_wrapper<S::PreDec{344}>> decltype(auto) cw1 = --cw<S{345}>;
    }

    // post operator--
    {
        static_assert(!CanPseudoPostDecrement<constant_wrapper<'*'>>);
        static_assert(!CanPseudoPostDecrement<constant_wrapper<2.25>>);
        static_assert(!CanPseudoPostDecrement<constant_wrapper<i_array>>);
        static_assert(CanPseudoPostDecrement<constant_wrapper<S{456}>>);

        static_assert(noexcept(cw<S{456}> --));

        [[maybe_unused]] same_as<constant_wrapper<S::PostDec{455}>> decltype(auto) cw1 = cw<S{456}> --;
    }

    // operator+=
    {
        static_assert(!CanPseudoPlusAssign<constant_wrapper<'\052'>, int>);
        static_assert(!CanPseudoPlusAssign<char, constant_wrapper<16>>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<'\040'>, constant_wrapper<42.0>>);
        static_assert(!CanPseudoPlusAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<i_array>, int>);
        static_assert(!CanPseudoPlusAssign<long, constant_wrapper<i_mut_array>>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<ptrdiff_t{1}>, constant_wrapper<i_array>>);
        static_assert(!CanPseudoPlusAssign<constant_wrapper<i_array>, constant_wrapper<ptrdiff_t{0}>>);
        static_assert(CanPseudoPlusAssign<constant_wrapper<S{421}>, constant_wrapper<S{729}>>);
        static_assert(CanPseudoPlusAssign<constant_wrapper<S{21}>, constant_wrapper<96>>);
        static_assert(CanPseudoPlusAssign<constant_wrapper<-128>, constant_wrapper<S{127}>>);

        static_assert(noexcept(cw<S{421}> += cw<S{729}>));
        static_assert(noexcept(cw<-128> += cw<S{127}>));
        static_assert(noexcept(cw<S{21}> += cw<96>));

        [[maybe_unused]] same_as<constant_wrapper<S::PlusAssign{1150}>> decltype(auto) cw1 = cw<S{421}> += cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::PlusAssignLeftInt{-1}>> decltype(auto) cw2 = cw<-128> +=
            cw<S{127}>;
        [[maybe_unused]] same_as<constant_wrapper<S::PlusAssignRightInt{117}>> decltype(auto) cw3 = cw<S{21}> += cw<96>;
    }

    // operator-=
    {
        static_assert(!CanPseudoMinusAssign<constant_wrapper<'\052'>, int>);
        static_assert(!CanPseudoMinusAssign<char, constant_wrapper<16>>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<'\040'>, constant_wrapper<42.0>>);
        static_assert(!CanPseudoMinusAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(!CanPseudoMinusAssign<long, constant_wrapper<i_mut_array>>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<ptrdiff_t{1}>, constant_wrapper<i_array>>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<i_array>, constant_wrapper<ptrdiff_t{0}>>);
        static_assert(!CanPseudoMinusAssign<constant_wrapper<i_array + 0>, constant_wrapper<i_array + 1>>);
        static_assert(CanPseudoMinusAssign<constant_wrapper<S{729}>, constant_wrapper<S{421}>>);
        static_assert(CanPseudoMinusAssign<constant_wrapper<S{21}>, constant_wrapper<96>>);
        static_assert(CanPseudoMinusAssign<constant_wrapper<-128>, constant_wrapper<S{127}>>);

        static_assert(noexcept(cw<S{729}> -= cw<S{421}>));
        static_assert(noexcept(cw<-128> -= cw<S{127}>));
        static_assert(noexcept(cw<S{21}> -= cw<96>));

        [[maybe_unused]] same_as<constant_wrapper<S::MinusAssign{308}>> decltype(auto) cw2 = cw<S{729}> -= cw<S{421}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MinusAssignLeftInt{-255}>> decltype(auto) cw3 = cw<-128> -=
            cw<S{127}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MinusAssignRightInt{-75}>> decltype(auto) cw4 = cw<S{21}> -=
            cw<96>;
    }

    // operator*=
    {
        static_assert(!CanPseudoMultiplyAssign<constant_wrapper<'\052'>, int>);
        static_assert(!CanPseudoMultiplyAssign<char, constant_wrapper<2L>>);
        static_assert(!CanPseudoMultiplyAssign<constant_wrapper<'\040'>, constant_wrapper<2.5>>);
        static_assert(!CanPseudoMultiplyAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoMultiplyAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoMultiplyAssign<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanPseudoMultiplyAssign<constant_wrapper<S{17}>, constant_wrapper<29>>);
        static_assert(CanPseudoMultiplyAssign<constant_wrapper<-1>, constant_wrapper<S{729}>>);
        static_assert(CanPseudoMultiplyAssign<constant_wrapper<S{172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<S{17}> *= cw<29>));
        static_assert(noexcept(cw<-1> *= cw<S{729}>));
        static_assert(noexcept(cw<S{172}> *= cw<S{-9}>));

        [[maybe_unused]] same_as<constant_wrapper<S::MultiplyAssign{493}>> decltype(auto) cw1 = cw<S{17}> *= cw<S{29}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MultiplyAssignLeftInt{-729}>> decltype(auto) cw2 = cw<-1> *=
            cw<S{729}>;
        [[maybe_unused]] same_as<constant_wrapper<S::MultiplyAssignRightInt{-1548}>> decltype(auto) cw3 = cw<S{172}> *=
            cw<-9>;
    }

    // operator/=
    {
        static_assert(!CanPseudoDivideAssign<constant_wrapper<'\052'>, int>);
        static_assert(!CanPseudoDivideAssign<char, constant_wrapper<2L>>);
        static_assert(!CanPseudoDivideAssign<constant_wrapper<'\040'>, constant_wrapper<8.0>>);
        static_assert(!CanPseudoDivideAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoDivideAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoDivideAssign<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanPseudoDivideAssign<constant_wrapper<S{1729}>, constant_wrapper<42>>);
        static_assert(CanPseudoDivideAssign<constant_wrapper<-126>, constant_wrapper<S{-3}>>);
        static_assert(CanPseudoDivideAssign<constant_wrapper<S{172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<S{17}> /= cw<29>));
        static_assert(noexcept(cw<-126> /= cw<S{-3}>));
        static_assert(noexcept(cw<S{172}> /= cw<S{-9}>));

        [[maybe_unused]] same_as<constant_wrapper<S::DivideAssign{41}>> decltype(auto) cw1 = cw<S{1729}> /= cw<S{42}>;
        [[maybe_unused]] same_as<constant_wrapper<S::DivideAssignLeftInt{42}>> decltype(auto) cw2 = cw<-126> /=
            cw<S{-3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::DivideAssignRightInt{-19}>> decltype(auto) cw3 = cw<S{172}> /=
            cw<-9>;
    }

    // operator%=
    {
        static_assert(!CanPseudoModuloAssign<constant_wrapper<'\052'>, int>);
        static_assert(!CanPseudoModuloAssign<char, constant_wrapper<-23L>>);
        static_assert(!CanPseudoModuloAssign<constant_wrapper<'\040'>, constant_wrapper<8.0>>);
        static_assert(!CanModulo<constant_wrapper<42.5>, constant_wrapper<5>>);
        static_assert(!CanPseudoModuloAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoModuloAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoModuloAssign<constant_wrapper<nullptr>, constant_wrapper<42>>);
        static_assert(CanModulo<constant_wrapper<S{1729}>, constant_wrapper<42>>);
        static_assert(CanPseudoModuloAssign<constant_wrapper<-126>, constant_wrapper<S{-3}>>);
        static_assert(CanPseudoModuloAssign<constant_wrapper<S{-172}>, constant_wrapper<-9>>);

        static_assert(noexcept(cw<S{17}> %= cw<29>));
        static_assert(noexcept(cw<-126> %= cw<S{-3}>));
        static_assert(noexcept(cw<S{-172}> %= cw<S{-9}>));

        [[maybe_unused]] same_as<constant_wrapper<S::ModuloAssign{17}>> decltype(auto) cw1 = cw<S{17}> %= cw<S{29}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ModuloAssignLeftInt{0}>> decltype(auto) cw2 = cw<-126> %=
            cw<S{-3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ModuloAssignRightInt{-1}>> decltype(auto) cw3 = cw<S{-172}> %=
            cw<-9>;
    }

    // operator&=
    {
        static_assert(!CanPseudoBitAndAssign<int, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<255U>, char16_t>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanPseudoBitAndAssign<double, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(!CanPseudoBitAndAssign<byte, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<byte{251}>, byte>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanPseudoBitAndAssign<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<byte{251}>, int>);
        static_assert(!CanPseudoBitAndAssign<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanPseudoBitAndAssign<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanPseudoBitAndAssign<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanPseudoBitAndAssign<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(cw<S{32589}> &= cw<S{-12751}>));
        static_assert(noexcept(cw<21124> &= cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> &= cw<95>));

        [[maybe_unused]] same_as<constant_wrapper<S::BitAndAssign{19969}>> decltype(auto) cw1 = cw<S{32589}> &=
            cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitAndAssignLeftInt{20612}>> decltype(auto) cw2 = cw<21124> &=
            cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitAndAssignRightInt{91}>> decltype(auto) cw3 = cw<S{123}> &=
            cw<95>;
    }

    // operator|=
    {
        static_assert(!CanPseudoBitOrAssign<int, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<255U>, char16_t>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanPseudoBitOrAssign<double, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(!CanPseudoBitOrAssign<byte, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<byte{251}>, byte>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanPseudoBitOrAssign<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<byte{251}>, int>);
        static_assert(!CanPseudoBitOrAssign<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanPseudoBitOrAssign<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanPseudoBitOrAssign<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanPseudoBitOrAssign<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(cw<S{32589}> |= cw<S{-12751}>));
        static_assert(noexcept(cw<21124> |= cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> |= cw<95>));

        [[maybe_unused]] same_as<constant_wrapper<S::BitOrAssign{-131}>> decltype(auto) cw1 = cw<S{32589}> |=
            cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitOrAssignLeftInt{-2073}>> decltype(auto) cw2 = cw<21124> |=
            cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::BitOrAssignRightInt{127}>> decltype(auto) cw3 = cw<S{123}> |=
            cw<95>;
    }

    // operator^=
    {
        static_assert(!CanPseudoXorAssign<int, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<255U>, char16_t>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<421L>, constant_wrapper<729LL>>);
        static_assert(!CanPseudoXorAssign<double, constant_wrapper<'\052'>>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<255U>, nullptr_t>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<421L>, constant_wrapper<i_array>>);
        static_assert(!CanPseudoXorAssign<byte, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<byte{251}>, byte>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<byte{151}>, constant_wrapper<byte{242}>>);
        static_assert(!CanPseudoXorAssign<unsigned char, constant_wrapper<byte{121}>>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<byte{251}>, int>);
        static_assert(!CanPseudoXorAssign<constant_wrapper<byte{151}>, constant_wrapper<242U>>);
        static_assert(CanPseudoXorAssign<constant_wrapper<S{32589}>, constant_wrapper<S{-12751}>>);
        static_assert(CanPseudoXorAssign<constant_wrapper<21124>, constant_wrapper<S{-2585}>>);
        static_assert(CanPseudoXorAssign<constant_wrapper<S{123}>, constant_wrapper<95>>);

        static_assert(noexcept(cw<S{32589}> ^= cw<S{-12751}>));
        static_assert(noexcept(cw<21124> ^= cw<S{-2585}>));
        static_assert(noexcept(cw<S{123}> ^= cw<95>));

        [[maybe_unused]] same_as<constant_wrapper<S::XorAssign{-20100}>> decltype(auto) cw1 = cw<S{32589}> ^=
            cw<S{-12751}>;
        [[maybe_unused]] same_as<constant_wrapper<S::XorAssignLeftInt{-22685}>> decltype(auto) cw2 = cw<21124> ^=
            cw<S{-2585}>;
        [[maybe_unused]] same_as<constant_wrapper<S::XorAssignRightInt{36}>> decltype(auto) cw3 = cw<S{123}> ^= cw<95>;
    }

    // operator<<=
    {
        static_assert(!CanPseudoShlAssign<constant_wrapper<42>, int>);
        static_assert(!CanPseudoShlAssign<unsigned int, constant_wrapper<3L>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<84UL>, constant_wrapper<5LL>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<42>, double>);
        static_assert(!CanPseudoShlAssign<float, constant_wrapper<3L>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<84UL>, constant_wrapper<5.0L>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<byte{42}>, int>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<byte{3}>, constant_wrapper<3L>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<byte{42}>, byte>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<byte{3}>, constant_wrapper<byte{3}>>);
        static_assert(!CanPseudoShlAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoShlAssign<constant_wrapper<nullptr>, constant_wrapper<4>>);
        static_assert(CanPseudoShlAssign<constant_wrapper<S{1729}>, constant_wrapper<4>>);
        static_assert(CanPseudoShlAssign<constant_wrapper<-126>, constant_wrapper<S{3}>>);
        static_assert(CanPseudoShlAssign<constant_wrapper<S{-17}>, constant_wrapper<9>>);

        static_assert(noexcept(cw<S{1729}> <<= cw<S{4}>));
        static_assert(noexcept(cw<-126> <<= cw<S{3}>));
        static_assert(noexcept(cw<S{-17}> <<= cw<9>));

        [[maybe_unused]] same_as<constant_wrapper<S::ShlAssign{27664}>> decltype(auto) cw1 = cw<S{1729}> <<= cw<S{4}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShlAssignLeftInt{-1008}>> decltype(auto) cw2 = cw<-126> <<=
            cw<S{3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShlAssignRightInt{-8704}>> decltype(auto) cw3 = cw<S{-17}> <<=
            cw<9>;
    }

    // operator>>=
    {
        static_assert(!CanPseudoShrAssign<constant_wrapper<42>, int>);
        static_assert(!CanPseudoShrAssign<unsigned int, constant_wrapper<3L>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<84UL>, constant_wrapper<5LL>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<42>, double>);
        static_assert(!CanPseudoShrAssign<float, constant_wrapper<3L>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<84UL>, constant_wrapper<5.0L>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<byte{42}>, int>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<byte{129}>, constant_wrapper<3L>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<byte{72}>, byte>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<byte{255}>, constant_wrapper<byte{3}>>);
        static_assert(!CanPseudoShrAssign<int, constant_wrapper<nullptr>>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<nullptr>, char>);
        static_assert(!CanPseudoShrAssign<constant_wrapper<nullptr>, constant_wrapper<4>>);
        static_assert(CanPseudoShrAssign<constant_wrapper<S{1729}>, constant_wrapper<4>>);
        static_assert(CanPseudoShrAssign<constant_wrapper<-126>, constant_wrapper<S{3}>>);
        static_assert(CanPseudoShrAssign<constant_wrapper<S{-32768}>, constant_wrapper<9>>);

        static_assert(noexcept(cw<S{1729}> >>= cw<S{4}>));
        static_assert(noexcept(cw<-126> >>= cw<S{3}>));
        static_assert(noexcept(cw<S{-32768}> >>= cw<9>));

        [[maybe_unused]] same_as<constant_wrapper<S::ShrAssign{108}>> decltype(auto) cw3 = cw<S{1729}> >>= cw<S{4}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShrAssignLeftInt{-16}>> decltype(auto) cw4 = cw<-126> >>= cw<S{3}>;
        [[maybe_unused]] same_as<constant_wrapper<S::ShrAssignRightInt{-64}>> decltype(auto) cw5 = cw<S{-32768}> >>=
            cw<9>;
    }

    // operator=
    {
        static_assert(!is_assignable_v<constant_wrapper<42>&, int>);
        static_assert(!is_assignable_v<constant_wrapper<42>&, constant_wrapper<84>>);
        static_assert(!is_assignable_v<constant_wrapper<'*'>&, int>);
        static_assert(!is_assignable_v<constant_wrapper<'*'>&, constant_wrapper<' '>>);
        static_assert(!is_assignable_v<constant_wrapper<i_array>&, const int*>);
        static_assert(!is_assignable_v<constant_wrapper<i_array>&, constant_wrapper<i_mut_array>>);
        static_assert(!is_assignable_v<constant_wrapper<noop_fun>&, void (*)() noexcept>);
        static_assert(!is_assignable_v<constant_wrapper<noop_fun>&, constant_wrapper<noop_noexcept_fun>>);
        static_assert(!is_assignable_v<constant_wrapper<S{42}>&, int>);
        static_assert(!is_assignable_v<constant_wrapper<S{42}>&, S>);

        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>&, constant_wrapper<S{172}>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>&, constant_wrapper<S{172}>>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>, constant_wrapper<S{172}>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>, constant_wrapper<S{172}>>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>&, const constant_wrapper<S{172}>&>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>&, const constant_wrapper<S{172}>&>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>, const constant_wrapper<S{172}>&>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>, const constant_wrapper<S{172}>&>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>&, constant_wrapper<729>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>&, constant_wrapper<729>>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>, constant_wrapper<729>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>, constant_wrapper<729>>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>&, const constant_wrapper<729>&>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>&, const constant_wrapper<729>&>);
        static_assert(is_nothrow_assignable_v<constant_wrapper<S{42}>, const constant_wrapper<729>&>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{42}>, const constant_wrapper<729>&>);

        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{1729}>&, constant_wrapper<S{1729}>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{1729}>, constant_wrapper<S{1729}>>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{1729}>&, const constant_wrapper<S{1729}>&>);
        static_assert(is_nothrow_assignable_v<const constant_wrapper<S{1729}>, const constant_wrapper<S{1729}>&>);

        constexpr auto cw_1729 = cw<S{1729}>; // workaround for Clang's -Wself-assign-overloaded

        [[maybe_unused]] same_as<constant_wrapper<S::Assign{172}>> decltype(auto) cw1 = cw<S{42}> = cw<S{172}>;
        [[maybe_unused]] same_as<constant_wrapper<S::Assign{1729}>> decltype(auto) cw2 = cw<S{1729}> = cw_1729;
        [[maybe_unused]] same_as<constant_wrapper<S::AssignRightInt{729}>> decltype(auto) cw3 = cw<S{42}> = cw<729>;
    }

    // operator()

    // operator[]

    return true;
}

void test_nonconstexpr() {}

template <class T>
void test_triviality_and_layout() { // COMPILE-ONLY
    // The following properties should hold for constant_wrapper if the implementation doesn't do silly thing.
    static_assert(is_trivially_copyable_v<T>);
    static_assert(is_trivially_default_constructible_v<T>);
    static_assert(is_trivially_copy_constructible_v<T>);
    static_assert(is_trivially_constructible_v<T, T&>);
    static_assert(is_trivially_move_constructible_v<T>);
    static_assert(is_trivially_constructible_v<T, const T>);
    static_assert(is_trivially_copy_assignable_v<T>);
    static_assert(is_trivially_assignable_v<T&, T&>);
    static_assert(is_trivially_move_assignable_v<T>);
    static_assert(is_trivially_assignable_v<T&, const T>);
    static_assert(is_trivially_destructible_v<T>);

    static_assert(is_nothrow_default_constructible_v<T>);
    static_assert(is_nothrow_copy_constructible_v<T>);
    static_assert(is_nothrow_constructible_v<T, T&>);
    static_assert(is_nothrow_move_constructible_v<T>);
    static_assert(is_nothrow_constructible_v<T, const T>);
    static_assert(is_nothrow_copy_assignable_v<T>);
    static_assert(is_nothrow_assignable_v<T&, T&>);
    static_assert(is_nothrow_move_assignable_v<T>);
    static_assert(is_nothrow_assignable_v<T&, const T>);
    static_assert(is_nothrow_destructible_v<T>);

    static_assert(is_standard_layout_v<T>);
    static_assert(is_empty_v<T>);
}

void test_miscelleneous() { // COMPILE-ONLY
    // SFINAE-friendliness for constant_wrapper<NoOperation{}>

    static_assert(!CanUnaryPlus<constant_wrapper<NoOperation{}>>);
    static_assert(!CanUnaryMinus<constant_wrapper<NoOperation{}>>);
    static_assert(!CanComplement<constant_wrapper<NoOperation{}>>);
    static_assert(!CanNegate<constant_wrapper<NoOperation{}>>);
    static_assert(!CanDereference<constant_wrapper<NoOperation{}>>);
    // unary operator& is exempted because constant_wrapper provides such an overload.
    static_assert(!CanPlus<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanMinus<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanMultiply<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanDivide<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanModulo<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanShl<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanShr<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanBitAnd<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanBitOr<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanXor<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanLogicalAnd<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanLogicalOr<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanThreeWayCompare<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanLess<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanLessEqual<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanEqual<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanUnequal<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanGreater<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanGreaterEqual<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    // operator, is exempt because builtin or deleted versions are selected.
    static_assert(!CanArrowStar<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoPreIncrement<constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoPostIncrement<constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoPreDecrement<constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoPostDecrement<constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoPlusAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoMinusAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoMultiplyAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoDivideAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoModuloAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoBitAndAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoBitOrAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoXorAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoShlAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanPseudoShrAssign<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!is_assignable_v<const constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!is_assignable_v<const constant_wrapper<NoOperation{}>&, constant_wrapper<NoOperation{}>>);
    static_assert(!is_assignable_v<const constant_wrapper<NoOperation{}>, const constant_wrapper<NoOperation{}>&>);
    static_assert(!is_assignable_v<const constant_wrapper<NoOperation{}>&, const constant_wrapper<NoOperation{}>&>);
    static_assert(!invocable<constant_wrapper<NoOperation{}>>);
    static_assert(!invocable<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!invocable<constant_wrapper<NoOperation{}>, NoOperation>);
    static_assert(!CanSubscript<constant_wrapper<NoOperation{}>>);
    static_assert(!CanSubscript<constant_wrapper<NoOperation{}>, constant_wrapper<NoOperation{}>>);
    static_assert(!CanSubscript<constant_wrapper<NoOperation{}>, NoOperation>);

    test_triviality_and_layout<constant_wrapper<'*'>>();
    test_triviality_and_layout<constant_wrapper<1729>>();
    test_triviality_and_layout<constant_wrapper<2.25>>();
    test_triviality_and_layout<constant_wrapper<nullptr>>();
    test_triviality_and_layout<constant_wrapper<i_array>>();
    test_triviality_and_layout<constant_wrapper<i_mut_array>>();
    test_triviality_and_layout<constant_wrapper<noop_fun>>();
    test_triviality_and_layout<constant_wrapper<noop_noexcept_fun>>();
    test_triviality_and_layout<constant_wrapper<cw<nullptr>>>();
    test_triviality_and_layout<constant_wrapper<AdlTest::S{42}>>();
    test_triviality_and_layout<constant_wrapper<NoOperation{}>>();

    // side effects of cw-operators
    struct Weird : constant_wrapper<42>, constant_wrapper<1729LL> {};
    static_assert(!is_standard_layout_v<Weird>);
}

int main() {
    test();
    test_nonconstexpr();
    static_assert(test());
}
