#pragma once

#include <algorithm>
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

    // Only an opposite input after exactly one queued input can be represented by
    // the two-event simulation. Robot jumps are rescued on release only.
    constexpr bool canRescue(int queued, bool firstDown, bool nextDown, bool wave, bool ship, bool robot) {
        return queued == 1 && firstDown != nextDown &&
            (wave || ship || (robot && firstDown && !nextDown));
    }

    constexpr bool preserveQueueOnSpam(bool wave, bool oldWaveRules, bool queueEmpty) {
        return wave && !oldWaveRules && !queueEmpty;
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
