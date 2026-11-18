#pragma once

#include <algorithm>
#include <array>
#include <optional>
#include <unordered_set>

namespace wave_rescue_policy {
    template <typename Object>
    class GravityPortalTracker {
        std::unordered_set<Object*> m_used1, m_used2;
    public:
        bool activate(Object* portal, bool second) {
            return portal && (second ? m_used2 : m_used1).insert(portal).second;
        }
        void clear() {
            m_used1.clear();
            m_used2.clear();
        }
    };

    // Keep separate up/down intervals: a narrow zigzag need not be symmetric.
    class Rhythm {
        std::array<int, 2> m_intervals{};
        std::optional<bool> m_last;
    public:
        void observe(bool down, int gap, int limit) {
            if (m_last && *m_last != down && gap <= limit) {
                m_intervals[*m_last] = std::max(1, gap);
            }
            else {
                m_intervals = {};
            }
            m_last = down;
        }
        int interval(bool down) const { return m_intervals[down]; }
        bool tight() const { return interval(true) > 0 && interval(false) > 0; }
    };

    // A buffered press/release pair must have a physics step between its edges.
    constexpr int nextTurnTick(int previous, int requested) {
        return std::max(requested, std::max(0, previous) + 1);
    }

    // Look for the closest safe tick, not just more delay. probe(tick, end) returns
    // actual ticks survived from now, or -1 for an inconclusive run.
    template <typename Probe, typename OutOfTime>
    std::optional<int> retimeWaveTurn(int natural, int earliest, int latest, int clearance, Probe probe, OutOfTime outOfTime) {
        if (outOfTime()) {
            return std::nullopt;
        }
        int survived = probe(natural, natural + clearance);
        if (survived < earliest || survived >= natural + clearance) {
            return std::nullopt; // death before the earliest turn, unknown, or already safe
        }
        if (survived < natural) {
            latest = std::min(latest, survived); // all turns after the crash are too late
        }
        for (int distance = std::max(1, natural - latest); distance <= std::max(natural - earliest, latest - natural); ++distance) {
            for (int candidate : { natural - distance, natural + distance }) {
                if (candidate < earliest || candidate > latest) {
                    continue;
                }
                if (outOfTime()) {
                    return std::nullopt;
                }
                if (probe(candidate, candidate + clearance) >= candidate + clearance) {
                    return candidate;
                }
            }
        }
        return std::nullopt;
    }

    // Only an opposite input after exactly one queued input can be represented by
    // the two-event simulation. Robot jumps are rescued on release only.
    constexpr bool canRescuePair(int queued, bool firstDown, bool nextDown, bool ship, bool robot) {
        return queued == 1 && firstDown != nextDown && (ship || (robot && firstDown && !nextDown));
    }

    constexpr bool usesSpamGuard(bool wave, bool oldWaveRules, bool otherModeUsesGuard) {
        // In mixed duals, the other mode must not turn off precise wave buffering.
        return wave ? oldWaveRules : otherModeUsesGuard;
    }

    constexpr int lookahead(int baseHorizon, int interval, int clearance) {
        return std::max(baseHorizon, interval + clearance);
    }

    constexpr bool portalIsSafe(bool rejectPortalSafety, int tick, int delay) {
        return !rejectPortalSafety && tick >= delay;
    }

    constexpr bool diedBeforeNextTurn(int survived, int interval) {
        return survived < interval;
    }

    constexpr bool clearsNextTurn(bool turnFired, int survived, int interval, int clearance) {
        return turnFired && survived >= interval + clearance;
    }
}
