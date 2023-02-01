#pragma once
#include "blocker.hpp"

struct str
{
    void a();
    void b(auto a, auto d);

    using blocker_type = utils::blocker_t<decltype(a), decltype(b)>;
};

inline void str::a()
{
}

inline void str::b(auto c, auto d)
{
}
