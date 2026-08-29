#pragma once
#pragma kbuild expand

#include <algorithm>
#include <cmath>

namespace mone {

enum class cooling_schedule {
    linear,
    exponential,
};

class annealing {
    double start_temperature_;
    double end_temperature_;
    cooling_schedule schedule_;

public:
    annealing(
        double start_temperature,
        double end_temperature,
        cooling_schedule schedule = cooling_schedule::linear
    )
        : start_temperature_(start_temperature),
          end_temperature_(end_temperature),
          schedule_(schedule) {}

    [[nodiscard]] double temperature(double progress) const {
        progress = std::clamp(progress, 0.0, 1.0);

        if (
            schedule_ == cooling_schedule::exponential &&
            start_temperature_ > 0.0 &&
            end_temperature_ > 0.0
        ) {
            return start_temperature_ * std::pow(
                end_temperature_ / start_temperature_,
                progress
            );
        }

        return start_temperature_
             + (end_temperature_ - start_temperature_) * progress;
    }

    // improvement > 0 なら改善、improvement < 0 なら改悪
    template <class Random>
    [[nodiscard]] bool accept(
        double improvement,
        double progress,
        Random& random
    ) const {
        if (improvement >= 0.0) {
            return true;
        }

        const double temp = temperature(progress);
        if (temp <= 0.0) {
            return false;
        }

        return random.real() < std::exp(improvement / temp);
    }
};

}