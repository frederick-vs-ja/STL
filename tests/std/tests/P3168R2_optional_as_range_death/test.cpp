// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <algorithm>
#include <compare>
#include <cstddef>
#include <optional>
#include <ranges>
#include <utility>

#include <test_death.hpp>

using namespace std;

constexpr auto iter_deref   = [](const auto& it) { (void) *it; };
constexpr auto iter_inc_pre = [](auto&& it) { ++it; };
constexpr auto iter_inc_pos = [](auto&& it) { it++; };
constexpr auto iter_dec_pre = [](auto&& it) { --it; };
constexpr auto iter_dec_pos = [](auto&& it) { it--; };
template <ptrdiff_t N>
constexpr auto iter_seek = [](auto&& it) { it += N; };
template <ptrdiff_t N>
constexpr auto iter_neg_seek = [](auto&& it) { it -= N; };
template <ptrdiff_t N>
constexpr auto iter_add = [](const auto& it) { (void) (it + N); };
template <ptrdiff_t N>
constexpr auto iter_sub = [](const auto& it) { (void) (it - N); };
template <ptrdiff_t N>
constexpr auto iter_at = [](const auto& it) { (void) (it[N]); };

constexpr auto iter_eq   = [](const auto& i, const auto& j) { (void) (i == j); };
constexpr auto iter_ne   = [](const auto& i, const auto& j) { (void) (i != j); };
constexpr auto iter_lt   = [](const auto& i, const auto& j) { (void) (i < j); };
constexpr auto iter_le   = [](const auto& i, const auto& j) { (void) (i >= j); };
constexpr auto iter_gt   = [](const auto& i, const auto& j) { (void) (i > j); };
constexpr auto iter_ge   = [](const auto& i, const auto& j) { (void) (i >= j); };
constexpr auto iter_3way = [](const auto& i, const auto& j) { (void) (i <=> j); };
constexpr auto iter_diff = [](const auto& i, const auto& j) { (void) (i - j); };

enum AddConst : bool {
    No,
    Yes,
};

template <bool Cond>
auto& as_const_if(auto& x) noexcept {
    if constexpr (Cond) {
        return as_const(x);
    } else {
        return x;
    }
}

template <auto Operation>
void test_optional_iterator_value_init() {
    optional<int>::iterator it{};
    Operation(it);
}

template <auto Operation>
void test_optional_const_iterator_value_init() {
    optional<int>::const_iterator it{};
    Operation(it);
}

template <auto Operation>
void test_optional_iterator_valueless() {
    optional<int> opt;
    Operation(opt.begin());
}

template <auto Operation>
void test_optional_const_iterator_valueless() {
    const optional<int> opt;
    Operation(opt.begin());
}

template <auto Operation>
void test_optional_iterator_begin() {
    optional<int> opt{in_place};
    Operation(opt.end());
}

template <auto Operation>
void test_optional_const_iterator_begin() {
    const optional<int> opt{in_place};
    Operation(opt.end());
}

template <auto Operation>
void test_optional_iterator_end() {
    optional<int> opt{in_place};
    Operation(opt.end());
}

template <auto Operation>
void test_optional_const_iterator_end() {
    const optional<int> opt{in_place};
    Operation(opt.end());
}

template <auto Operation, AddConst LhsConst, AddConst RhsConst>
void test_inconsistent_optional_iterators() {
    optional<int> opt;
    auto it1 = as_const_if<static_cast<bool>(LhsConst)>(opt).begin();
    opt.emplace();
    auto it2 = as_const_if<static_cast<bool>(RhsConst)>(opt).begin();
    Operation(it1, it2);
}

template <auto Operation, AddConst LhsConst, AddConst RhsConst>
void test_unrelated_optional_iterators_1() {
    optional<int> opt1;
    optional<int> opt2;
    auto it1 = as_const_if<static_cast<bool>(LhsConst)>(opt1).begin();
    auto it2 = as_const_if<static_cast<bool>(RhsConst)>(opt2).begin();
    Operation(it1, it2);
}

template <auto Operation, AddConst LhsConst, AddConst RhsConst>
void test_unrelated_optional_iterators_2() {
    optional<int> opt;
    auto it1 = as_const_if<static_cast<bool>(LhsConst)>(opt).begin();
    auto it2 = decltype(as_const_if<static_cast<bool>(RhsConst)>(opt).begin()){};
    Operation(it1, it2);
}

void test_traverse_bad_optional_range() {
    optional<int> opt{in_place};
    for_each(opt.end(), opt.begin(), [](int) {});
}

void test_traverse_bad_optional_const_range() {
    const optional<int> opt{in_place};
    for_each(opt.end(), opt.begin(), [](int) {});
}

int main(int argc, char* argv[]) {
    std_testing::death_test_executive exec;

#if _ITERATOR_DEBUG_LEVEL != 0
    exec.add_death_tests({
        test_optional_iterator_value_init<iter_deref>,
        test_optional_iterator_value_init<iter_at<0>>,
        test_optional_iterator_value_init<iter_inc_pre>,
        test_optional_iterator_value_init<iter_inc_pos>,
        test_optional_iterator_value_init<iter_dec_pre>,
        test_optional_iterator_value_init<iter_dec_pos>,
        test_optional_iterator_value_init<iter_seek<1>>,
        test_optional_iterator_value_init<iter_seek<-1>>,
        test_optional_iterator_value_init<iter_neg_seek<1>>,
        test_optional_iterator_value_init<iter_neg_seek<-1>>,
        test_optional_iterator_value_init<iter_add<1>>,
        test_optional_iterator_value_init<iter_add<-1>>,
        test_optional_iterator_value_init<iter_sub<1>>,
        test_optional_iterator_value_init<iter_sub<-1>>,

        test_optional_const_iterator_value_init<iter_deref>,
        test_optional_const_iterator_value_init<iter_at<0>>,
        test_optional_const_iterator_value_init<iter_inc_pre>,
        test_optional_const_iterator_value_init<iter_inc_pos>,
        test_optional_const_iterator_value_init<iter_dec_pre>,
        test_optional_const_iterator_value_init<iter_dec_pos>,
        test_optional_const_iterator_value_init<iter_seek<1>>,
        test_optional_const_iterator_value_init<iter_seek<-1>>,
        test_optional_const_iterator_value_init<iter_neg_seek<1>>,
        test_optional_const_iterator_value_init<iter_neg_seek<-1>>,
        test_optional_const_iterator_value_init<iter_add<1>>,
        test_optional_const_iterator_value_init<iter_add<-1>>,
        test_optional_const_iterator_value_init<iter_sub<1>>,
        test_optional_const_iterator_value_init<iter_sub<-1>>,

        test_optional_iterator_valueless<iter_deref>,
        test_optional_iterator_valueless<iter_at<0>>,
        test_optional_iterator_valueless<iter_inc_pre>,
        test_optional_iterator_valueless<iter_inc_pos>,
        test_optional_iterator_valueless<iter_dec_pre>,
        test_optional_iterator_valueless<iter_dec_pos>,
        test_optional_iterator_valueless<iter_seek<1>>,
        test_optional_iterator_valueless<iter_seek<-1>>,
        test_optional_iterator_valueless<iter_neg_seek<1>>,
        test_optional_iterator_valueless<iter_neg_seek<-1>>,
        test_optional_iterator_valueless<iter_add<1>>,
        test_optional_iterator_valueless<iter_add<-1>>,
        test_optional_iterator_valueless<iter_sub<1>>,
        test_optional_iterator_valueless<iter_sub<-1>>,

        test_optional_const_iterator_valueless<iter_deref>,
        test_optional_const_iterator_valueless<iter_at<0>>,
        test_optional_const_iterator_valueless<iter_inc_pre>,
        test_optional_const_iterator_valueless<iter_inc_pos>,
        test_optional_const_iterator_valueless<iter_dec_pre>,
        test_optional_const_iterator_valueless<iter_dec_pos>,
        test_optional_const_iterator_valueless<iter_seek<1>>,
        test_optional_const_iterator_valueless<iter_seek<-1>>,
        test_optional_const_iterator_valueless<iter_neg_seek<1>>,
        test_optional_const_iterator_valueless<iter_neg_seek<-1>>,
        test_optional_const_iterator_valueless<iter_add<1>>,
        test_optional_const_iterator_valueless<iter_add<-1>>,
        test_optional_const_iterator_valueless<iter_sub<1>>,
        test_optional_const_iterator_valueless<iter_sub<-1>>,

        test_optional_iterator_begin<iter_dec_pre>,
        test_optional_iterator_begin<iter_dec_pos>,
        test_optional_iterator_begin<iter_seek<2>>,
        test_optional_iterator_begin<iter_seek<-1>>,
        test_optional_iterator_begin<iter_neg_seek<1>>,
        test_optional_iterator_begin<iter_neg_seek<-2>>,
        test_optional_iterator_begin<iter_add<2>>,
        test_optional_iterator_begin<iter_add<-1>>,
        test_optional_iterator_begin<iter_sub<1>>,
        test_optional_iterator_begin<iter_sub<-2>>,

        test_optional_const_iterator_begin<iter_dec_pre>,
        test_optional_const_iterator_begin<iter_dec_pos>,
        test_optional_const_iterator_begin<iter_seek<2>>,
        test_optional_const_iterator_begin<iter_seek<-1>>,
        test_optional_const_iterator_begin<iter_neg_seek<1>>,
        test_optional_const_iterator_begin<iter_neg_seek<-2>>,
        test_optional_const_iterator_begin<iter_add<2>>,
        test_optional_const_iterator_begin<iter_add<-1>>,
        test_optional_const_iterator_begin<iter_sub<1>>,
        test_optional_const_iterator_begin<iter_sub<-2>>,

        test_optional_iterator_end<iter_deref>,
        test_optional_iterator_end<iter_at<0>>,
        test_optional_iterator_end<iter_inc_pre>,
        test_optional_iterator_end<iter_inc_pos>,
        test_optional_iterator_end<iter_seek<1>>,
        test_optional_iterator_end<iter_seek<-2>>,
        test_optional_iterator_end<iter_neg_seek<2>>,
        test_optional_iterator_end<iter_neg_seek<-1>>,
        test_optional_iterator_end<iter_add<1>>,
        test_optional_iterator_end<iter_add<-2>>,
        test_optional_iterator_end<iter_sub<2>>,
        test_optional_iterator_end<iter_sub<-1>>,

        test_optional_const_iterator_end<iter_deref>,
        test_optional_const_iterator_end<iter_at<0>>,
        test_optional_const_iterator_end<iter_inc_pre>,
        test_optional_const_iterator_end<iter_inc_pos>,
        test_optional_const_iterator_end<iter_seek<1>>,
        test_optional_const_iterator_end<iter_seek<-2>>,
        test_optional_const_iterator_end<iter_neg_seek<2>>,
        test_optional_const_iterator_end<iter_neg_seek<-1>>,
        test_optional_const_iterator_end<iter_add<1>>,
        test_optional_const_iterator_end<iter_add<-2>>,
        test_optional_const_iterator_end<iter_sub<2>>,
        test_optional_const_iterator_end<iter_sub<-1>>,

        test_inconsistent_optional_iterators<iter_eq, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_ne, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_lt, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_le, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_gt, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_ge, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_3way, AddConst::No, AddConst::No>,
        test_inconsistent_optional_iterators<iter_diff, AddConst::No, AddConst::No>,

        test_inconsistent_optional_iterators<iter_eq, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_ne, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_lt, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_le, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_gt, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_ge, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_3way, AddConst::No, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_diff, AddConst::No, AddConst::Yes>,

        test_inconsistent_optional_iterators<iter_eq, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_ne, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_lt, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_le, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_gt, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_ge, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_3way, AddConst::Yes, AddConst::Yes>,
        test_inconsistent_optional_iterators<iter_diff, AddConst::Yes, AddConst::Yes>,

        test_unrelated_optional_iterators_1<iter_eq, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_ne, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_lt, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_le, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_gt, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_ge, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_3way, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_1<iter_diff, AddConst::No, AddConst::No>,

        test_unrelated_optional_iterators_1<iter_eq, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_ne, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_lt, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_le, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_gt, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_ge, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_3way, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_diff, AddConst::No, AddConst::Yes>,

        test_unrelated_optional_iterators_1<iter_eq, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_ne, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_lt, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_le, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_gt, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_ge, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_3way, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_1<iter_diff, AddConst::Yes, AddConst::Yes>,

        test_unrelated_optional_iterators_2<iter_eq, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_ne, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_lt, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_le, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_gt, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_ge, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_3way, AddConst::No, AddConst::No>,
        test_unrelated_optional_iterators_2<iter_diff, AddConst::No, AddConst::No>,

        test_unrelated_optional_iterators_2<iter_eq, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_ne, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_lt, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_le, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_gt, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_ge, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_3way, AddConst::No, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_diff, AddConst::No, AddConst::Yes>,

        test_unrelated_optional_iterators_2<iter_eq, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_ne, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_lt, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_le, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_gt, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_ge, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_3way, AddConst::Yes, AddConst::Yes>,
        test_unrelated_optional_iterators_2<iter_diff, AddConst::Yes, AddConst::Yes>,

        test_traverse_bad_optional_range,
        test_traverse_bad_optional_const_range,
    });
#endif // _ITERATOR_DEBUG_LEVEL != 0

    return exec.run(argc, argv);
}
