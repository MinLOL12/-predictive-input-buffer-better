#pragma once

#include <optional>

namespace wave_auto_release_policy {
    struct Prediction {
        int survived;
        bool died;
        bool uncertain;
        bool released;
    };

    struct Save {
        int holdTicks;
        int releaseTicks;
    };

    // probe(horizon, releaseAt) counts ticks from now, even for a delayed release.
    // -1 keeps holding; 0 releases immediately.
    template <typename Probe, typename OutOfTime>
    std::optional<Save> choose(int window, int clearance, Probe probe, OutOfTime outOfTime) {
        auto hold = probe(window, -1);
        if (!hold.died || hold.uncertain || hold.survived >= window || outOfTime()) {
            return std::nullopt;
        }

        auto now = probe(clearance, 0);
        if (!now.released || now.died || now.uncertain || now.survived < clearance || outOfTime()) {
            return std::nullopt;
        }

        // Compare every possible later release against the same endpoint. Include
        // the predicted collision tick: a release happens before that physics step.
        for (int delay = 1; delay <= hold.survived; ++delay) {
            auto later = probe(clearance, delay);
            if (!later.died || later.uncertain || outOfTime()) {
                return std::nullopt;
            }
        }
        return Save{hold.survived, now.survived};
    }
}
