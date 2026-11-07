#pragma once

namespace menu_shortcut {
    // v2.85: the buffer menu is opened with F3 (was Alt+F10). Use the state captured for
    // this keyboard event, not the current OS key state.
    constexpr bool shouldOpen(bool isF3, bool isKeyDown, bool isRepeat, bool inLevel) {
        return isF3 && isKeyDown && !isRepeat && inLevel;
    }
}
