#ifndef KEYMAP_H
#define KEYMAP_H

#include <map>
#include <string>
#include <windows.h>

// === map key ===
std::map<int, std::string> keyMap = {
    // Arrow keys
    {VK_LEFT, "h"},
    {VK_RIGHT, "l"},
    {VK_UP, "k"},
    {VK_DOWN, "j"},

    // Control keys
    {VK_ESCAPE, "esc"},
    {VK_TAB, "tab"},
    {VK_SPACE, "space"},
    {VK_RETURN, "enter"},
    {VK_BACK, "backspace"},

    // Function keys
    {VK_F1, "f1"},
    {VK_F2, "f2"},
    {VK_F3, "f3"},
    {VK_F4, "f4"},
    {VK_F5, "f5"},
    {VK_F6, "f6"},
    {VK_F7, "f7"},
    {VK_F8, "f8"},
    {VK_F9, "f9"},
    {VK_F10, "f10"},
    {VK_F11, "f11"},
    {VK_F12, "f12"},

    // Navigation keys
    {VK_HOME, "home"},
    {VK_END, "end"},
    {VK_PRIOR, "pgup"},
    {VK_NEXT, "pgdn"},
    {VK_INSERT, "ins"},
    {VK_DELETE, "del"},

    // Numpad keys
    {VK_NUMPAD0, "num0"},
    {VK_NUMPAD1, "num1"},
    {VK_NUMPAD2, "num2"},
    {VK_NUMPAD3, "num3"},
    {VK_NUMPAD4, "num4"},
    {VK_NUMPAD5, "num5"},
    {VK_NUMPAD6, "num6"},
    {VK_NUMPAD7, "num7"},
    {VK_NUMPAD8, "num8"},
    {VK_NUMPAD9, "num9"},
    {VK_MULTIPLY, "num*"},
    {VK_ADD, "num+"},
    {VK_SEPARATOR, "num,"},
    {VK_SUBTRACT, "num-"},
    {VK_DECIMAL, "num."},
    {VK_DIVIDE, "num/"},

    // Special keys
    {VK_NUMLOCK, "NUMLOCK"},
    {VK_SCROLL, "SCROLL"},
    {VK_CAPITAL, "CAPS"},
    {VK_PAUSE, "PAUSE"},
    {VK_PRINT, "PRINT"},
    {VK_SNAPSHOT, "PRTSCR"},

    // OEM keys (symbols)
    {VK_OEM_1, ";"},      // ;: for US
    {VK_OEM_PLUS, "="},   // =+
    {VK_OEM_COMMA, ","},  // ,<
    {VK_OEM_MINUS, "-"},  // -_
    {VK_OEM_PERIOD, "."}, // .>
    {VK_OEM_2, "/"},      // /? for US
    {VK_OEM_3, "`"},      // `~ for US
    {VK_OEM_4, "["},      // [{ for US
    {VK_OEM_5, "\\"},     // \| for US
    {VK_OEM_6, "]"},      // ]} for US
    {VK_OEM_7, "'"},      // '" for US
    {VK_OEM_8, "OEM_8"},
    {VK_OEM_102, "\\"}, // <> or \| on RT 102-key

    // Browser keys
    {VK_BROWSER_BACK, "BR_BACK"},
    {VK_BROWSER_FORWARD, "BR_FWD"},
    {VK_BROWSER_REFRESH, "BR_REFRESH"},
    {VK_BROWSER_STOP, "BR_STOP"},
    {VK_BROWSER_SEARCH, "BR_SEARCH"},
    {VK_BROWSER_FAVORITES, "BR_FAV"},
    {VK_BROWSER_HOME, "BR_HOME"},

    // Media keys
    {VK_VOLUME_MUTE, "VOL_MUTE"},
    {VK_VOLUME_DOWN, "VOL_DOWN"},
    {VK_VOLUME_UP, "VOL_UP"},
    {VK_MEDIA_NEXT_TRACK, "NEXT"},
    {VK_MEDIA_PREV_TRACK, "PREV"},
    {VK_MEDIA_STOP, "STOP"},
    {VK_MEDIA_PLAY_PAUSE, "PLAY"},

    // App keys
    {VK_LAUNCH_MAIL, "MAIL"},
    {VK_LAUNCH_MEDIA_SELECT, "MEDIA"},
    {VK_LAUNCH_APP1, "APP1"},
    {VK_LAUNCH_APP2, "APP2"},

    // IME keys
    {VK_KANA, "KANA"},
    {VK_KANJI, "KANJI"},
    {VK_CONVERT, "CONVERT"},
    {VK_NONCONVERT, "NONCONV"},

    // Other keys
    {VK_SELECT, "SELECT"},
    {VK_EXECUTE, "EXECUTE"},
    {VK_HELP, "HELP"},
    {VK_APPS, "MENU"},
    {VK_SLEEP, "SLEEP"},
    {VK_CLEAR, "CLEAR"},

    // Windows keys
    {VK_LWIN, "LWIN"},
    {VK_RWIN, "RWIN"},
};

#endif // KEYMAP_H
