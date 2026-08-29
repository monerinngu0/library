#pragma once
#pragma kbuild expand

#include "mone/header/notice.h"

#include <algorithm>
#include <chrono>

namespace mone {

class timer {
    using clock = std::chrono::steady_clock;

    clock::time_point started_at_ = clock::now();

public:
    void reset() {
        started_at_ = clock::now();
    }

    [[nodiscard]] double elapsed() const {
        return std::chrono::duration<double>(
            clock::now() - started_at_
        ).count();
    }

    [[nodiscard]] bool expired(double time_limit) const {
        return elapsed() >= time_limit;
    }

    [[nodiscard]] double progress(double time_limit) const {
        if (time_limit <= 0.0) {
            return 1.0;
        }

        return std::clamp(elapsed() / time_limit, 0.0, 1.0);
    }
};

}