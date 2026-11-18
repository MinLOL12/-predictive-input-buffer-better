#include "../src/attempt_input.hpp"
#include <cassert>

int main() {
    attempt_input::State input;
    input.isPlayer1 = false;
    input.realInput = true;
    input.appliedInput = true;
    input.shift = 12;
    input.releaseShift = 4;
    input.queue.push_back({ 3, false });
    input.sinceInput = 2;
    input.rapid = 5;
    input.clearAttempt(); // both death and restart use this; no stale hold or release
    assert(!input.realInput && !input.appliedInput && input.isPlayer1);
    assert(input.queue.empty() && input.shift == 0 && input.releaseShift == 0);
    assert(input.sinceInput == 999 && input.rapid == 0);

    input.realInput = true;
    input.autoReleased = input.autoDash = true;
    input.clearAttempt(); // auto-release must not swallow a new attempt's button-up
    assert(!input.realInput && !input.appliedInput && !input.autoReleased && !input.autoDash);

    input.realInput = input.appliedInput = true;
    input.queue.push_back({ 5, false });
    input.clearPending(); // a settings/stage change isn't a new attempt
    assert(input.queue.empty() && input.realInput && input.appliedInput);
}
