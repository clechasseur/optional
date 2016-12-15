// Copyright (c) 2016, Charles Lechasseur
// Distributed under the MIT license (see LICENSE).

#include "cl/optional/optional_tests.h"

#include <cl/optional.h>
#include <coveo/test_framework.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cl_tests {
namespace optional {
namespace detail {

// Struct with complex constructor
struct pair_int_str {
    int i_;
    std::string s_;

    pair_int_str(int i, const std::string& s)
        : i_(i), s_(s) { }
};

// Implementation of C++14 equal for compilers that do not have it
template<typename It1, typename It2>
bool equal(const It1& ibeg1, const It1& iend1,
           const It2& ibeg2, const It2& iend2)
{
    return std::distance(ibeg1, iend1) == std::distance(ibeg2, iend2) &&
           std::equal(ibeg1, iend1, ibeg2);
}

} // detail

// Runs all tests for cl::optional
void optional_tests()
{
    {
        cl::optional<std::string> o;
        COVEO_ASSERT(!o);
        COVEO_ASSERT(!o.has_value());
    }
    {
        cl::optional<std::string> o(cl::nullopt);
        COVEO_ASSERT(!o.has_value());
    }
    {
        const std::string s = "Life";
        cl::optional<std::string> o(s);
        COVEO_ASSERT(o);
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(*o == "Life");
    }
    {
        cl::optional<std::string> o(std::string("Life"));
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(*o == "Life");
    }
    {
        cl::optional<std::string> o1(std::string("Life"));
        cl::optional<std::string> o2(o1);
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        COVEO_ASSERT(o1.has_value());
    }
    {
        cl::optional<std::string> o1(std::string("Life"));
        cl::optional<std::string> o2(std::move(o1));
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        COVEO_ASSERT(o1.has_value()); // Moved optional still has a (moved) value
    }
#if _CL_OPT_VARIADIC
    {
        cl::optional<detail::pair_int_str> o(cl::in_place, 42, "Life");
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(o->i_ == 42);
        COVEO_ASSERT(o->s_ == "Life");
    }
    {
        cl::optional<std::vector<int>> o(cl::in_place, { 42, 23, 66 });
        COVEO_ASSERT(o.has_value());
        const std::vector<int> v = { 42, 23, 66 };
        COVEO_ASSERT(detail::equal(o->begin(), o->end(), v.begin(), v.end()));
    }
#endif // _CL_OPT_VARIADIC
    {
        cl::optional<int> o(42);
        COVEO_ASSERT(o.has_value());
        o = cl::nullopt;
        COVEO_ASSERT(!o.has_value());
    }
    {
        cl::optional<std::string> o1(std::string("Life"));
        cl::optional<std::string> o2;
        o2 = o1;
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        COVEO_ASSERT(o1.has_value());
    }
    {
        cl::optional<std::string> o1(std::string("Life"));
        cl::optional<std::string> o2;
        o2 = std::move(o1);
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        COVEO_ASSERT(o1.has_value()); // Moved optional still has a (moved) value
    }
#if _CL_OPT_TEMPL_FUNC_DEF_PARAMS
    {
        cl::optional<std::string> o;
        o = std::string("Life");
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(*o == "Life");
    }
#endif
    {
        cl::optional<std::string> o(std::string("Life"));
        std::string s1, s2;
        s1 = *o;
        COVEO_ASSERT(s1 == "Life");
        s2 = std::move(*o);
        COVEO_ASSERT(s2 == "Life");
    }
    {
        cl::optional<std::string> o;
        COVEO_ASSERT_THROW(o.value());
        o = std::string("Life");
        COVEO_ASSERT(o.value() == "Life");
    }
#if _CL_OPT_MEMBERS_REF_QUALIFIERS
    {
        cl::optional<std::string> o;
        COVEO_ASSERT_THROW(std::move(o.value()));
        o = std::string("Life");
        COVEO_ASSERT(std::move(o.value()) == "Life");
    }
#endif // _CL_OPT_MEMBERS_REF_QUALIFIERS
    {
        cl::optional<std::string> o;
        COVEO_ASSERT(o.value_or("Hangar") == "Hangar");
        o = std::string("Life");
        COVEO_ASSERT(o.value_or("Hangar") == "Life");
    }
#if _CL_OPT_MEMBERS_REF_QUALIFIERS
    {
        cl::optional<std::string> o;
        COVEO_ASSERT(std::move(o.value_or("Hangar")) == "Hangar");
        o = std::string("Life");
        COVEO_ASSERT(std::move(o.value_or("Hangar")) == "Life");
    }
#endif // _CL_OPT_MEMBERS_REF_QUALIFIERS
    {
        cl::optional<std::string> o1, o2;
        o1.swap(o2);
        COVEO_ASSERT(!o1.has_value());
        COVEO_ASSERT(!o2.has_value());
        using std::swap;
        swap(o1, o2);
        COVEO_ASSERT(!o1.has_value());
        COVEO_ASSERT(!o2.has_value());
    }
    {
        cl::optional<std::string> o1("Life");
        cl::optional<std::string> o2;
        o1.swap(o2);
        COVEO_ASSERT(!o1.has_value());
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        using std::swap;
        swap(o1, o2);
        COVEO_ASSERT(o1.has_value());
        COVEO_ASSERT(*o1 == "Life");
        COVEO_ASSERT(!o2.has_value());
    }
    {
        cl::optional<std::string> o1("Life");
        cl::optional<std::string> o2("Hangar");
        o1.swap(o2);
        COVEO_ASSERT(o1.has_value());
        COVEO_ASSERT(*o1 == "Hangar");
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Life");
        using std::swap;
        swap(o1, o2);
        COVEO_ASSERT(o1.has_value());
        COVEO_ASSERT(*o1 == "Life");
        COVEO_ASSERT(o2.has_value());
        COVEO_ASSERT(*o2 == "Hangar");
    }
    {
        cl::optional<int> o(42);
        COVEO_ASSERT(o.has_value());
        o.reset();
        COVEO_ASSERT(!o.has_value());
    }
    {
        cl::optional<int> o(42);
        COVEO_ASSERT(o.has_value());
        o = cl::nullopt;
        COVEO_ASSERT(!o.has_value());
    }
#if _CL_OPT_VARIADIC
    {
        cl::optional<detail::pair_int_str> o;
        o.emplace(42, "Life");
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(o->i_ == 42);
        COVEO_ASSERT(o->s_ == "Life");
    }
    {
        cl::optional<std::vector<int>> o;
        o.emplace({ 42, 23, 66 });
        COVEO_ASSERT(o.has_value());
        const std::vector<int> v = { 42, 23, 66 };
        COVEO_ASSERT(detail::equal(o->begin(), o->end(), v.begin(), v.end()));
    }
#endif // _CL_OPT_VARIADIC
    {
        cl::optional<std::string> o1("Life");
        cl::optional<std::string> o2("Life");
        cl::optional<std::string> o3("Hangar");
        cl::optional<std::string> o4;
        const std::string s1 = "Life";
        const std::string s2 = "Hangar";
        COVEO_ASSERT(o1 == o2);
        COVEO_ASSERT(o1 == s1);
        COVEO_ASSERT(o1 != o3);
        COVEO_ASSERT(o1 != o4);
        COVEO_ASSERT(o1 != cl::nullopt);
        COVEO_ASSERT(o4 == cl::nullopt);
        COVEO_ASSERT(cl::nullopt != o1);
        COVEO_ASSERT(cl::nullopt == o4);
        COVEO_ASSERT(o1 == s1);
        COVEO_ASSERT(o1 != s2);
        COVEO_ASSERT(o4 != s1);
        COVEO_ASSERT(s1 == o1);
        COVEO_ASSERT(s2 != o1);
        COVEO_ASSERT(s1 != o4);
        COVEO_ASSERT(!(o1 < o2));
        COVEO_ASSERT(o3 < o1);
        COVEO_ASSERT(o4 < o1);
        COVEO_ASSERT(!(o1 < cl::nullopt));
        COVEO_ASSERT(cl::nullopt < o1);
        COVEO_ASSERT(!(cl::nullopt < o4));
        COVEO_ASSERT(!(o1 < s1));
        COVEO_ASSERT(s2 < o1);
        COVEO_ASSERT(o4 < s1);
        COVEO_ASSERT(o1 <= o2);
        COVEO_ASSERT(o3 <= o1);
        COVEO_ASSERT(o4 <= o1);
        COVEO_ASSERT(!(o1 <= cl::nullopt));
        COVEO_ASSERT(cl::nullopt <= o1);
        COVEO_ASSERT(cl::nullopt <= o4);
        COVEO_ASSERT(o1 <= s1);
        COVEO_ASSERT(s2 <= o1);
        COVEO_ASSERT(!(o1 > o2));
        COVEO_ASSERT(o1 > o3);
        COVEO_ASSERT(o1 > o4);
        COVEO_ASSERT(o1 > cl::nullopt);
        COVEO_ASSERT(!(cl::nullopt > o1));
        COVEO_ASSERT(!(cl::nullopt > o4));
        COVEO_ASSERT(!(o1 > s1));
        COVEO_ASSERT(!(s2 > o1));
        COVEO_ASSERT(o1 >= o2);
        COVEO_ASSERT(o1 >= o3);
        COVEO_ASSERT(o1 >= o4);
        COVEO_ASSERT(o1 >= cl::nullopt);
        COVEO_ASSERT(!(cl::nullopt >= o1));
        COVEO_ASSERT(cl::nullopt >= o4);
        COVEO_ASSERT(o1 >= s1);
        COVEO_ASSERT(!(s2 >= o1));
    }
    {
        auto o = cl::make_optional(std::string("Life"));
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(*o == "Life");
    }
#if _CL_OPT_VARIADIC
    {
        auto o = cl::make_optional<detail::pair_int_str>(42, "Life");
        COVEO_ASSERT(o.has_value());
        COVEO_ASSERT(o->i_ == 42);
        COVEO_ASSERT(o->s_ == "Life");
    }
    {
        auto o = cl::make_optional<std::vector<int>>({ 42, 23, 66 });
        COVEO_ASSERT(o.has_value());
        const std::vector<int> v = { 42, 23, 66 };
        COVEO_ASSERT(detail::equal(o->begin(), o->end(), v.begin(), v.end()));
    }
#endif // _CL_OPT_VARIADIC
    {
        std::unordered_set<cl::optional<std::string>> s;
        s.insert(cl::optional<std::string>("Life"));
        s.insert(cl::optional<std::string>(cl::nullopt));
        s.insert(cl::optional<std::string>("Hangar"));
        for (auto&& elem : s) {
            std::cout << elem.value_or("(null)") << " ";
        }
        std::cout << std::endl;
    }
}

} // optional
} // cl_tests
