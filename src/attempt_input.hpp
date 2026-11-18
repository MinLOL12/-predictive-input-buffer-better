#pragma once

#include <deque>

namespace attempt_input {
    struct Turn {
        int ticksLeft;
        bool down;
    };

    struct State {
        std::deque<Turn> queue;
        int shift = 0;
        int releaseShift = 0;
        int sinceInput = 999;
        int rapid = 0;
        bool realInput = false;
        bool appliedInput = false;
        bool isPlayer1 = true;
        bool autoReleased = false;
        bool autoDash = false;

        void clearPending() {
            queue.clear();
            shift = releaseShift = 0;
        }

        void clearAttempt() {
            clearPending();
            realInput = appliedInput = autoReleased = autoDash = false;
            isPlayer1 = true;
            sinceInput = 999;
            rapid = 0;
        }
    };
}
