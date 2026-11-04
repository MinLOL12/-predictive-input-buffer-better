#pragma once

namespace menu_shortcut {
    // Use the modifier state captured for this keyboard event, not the current OS key state.
    constexpr bool shouldOpen(bool isF10, bool isKeyDown, bool isRepeat, bool altPressed, bool inLevel) {
        return isF10 && isKeyDown && !isRepeat && altPressed && inLevel;
    }
}
