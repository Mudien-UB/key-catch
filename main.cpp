#include "keymap.h"
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

// === Global state ===
std::set<int> pressedKeys;
std::vector<std::string> history;
std::string liveText = "";
bool wantToExit = false;
bool needRedraw = true;
HHOOK keyboardHook;

// === cek key yang di ignore ===
bool ignoredKey(int k) {
  return (k == VK_APPS || k == 255 || k == 183 || k == VK_LWIN || k == VK_RWIN);
}

// === cek key modifier ===
bool isModifier(int k) {
  return k == VK_CONTROL || k == VK_MENU || k == VK_SHIFT;
}

// === format kombinasi key ===
std::string formatCombo(const std::set<int> &codes) {
  bool ctrl = false, alt = false, shift = false;
  std::string mainKey = "";

  for (int k : codes) {
    if (ignoredKey(k))
      continue;

    if (k == VK_LCONTROL || k == VK_RCONTROL || k == VK_CONTROL)
      ctrl = true;
    else if (k == VK_LMENU || k == VK_RMENU || k == VK_MENU)
      alt = true;
    else if (k == VK_LSHIFT || k == VK_RSHIFT || k == VK_SHIFT)
      shift = true;
    else if (!isModifier(k)) {
      if (keyMap.count(k))
        mainKey = keyMap.at(k);
      else if (k >= 'A' && k <= 'Z')
        mainKey = std::string(1, (char)k);
      else if (k >= '0' && k <= '9')
        mainKey = std::string(1, (char)k);
    }
  }

  if (mainKey == "")
    mainKey = " ";

  // jika hanya modifier tanpa key lain, tidak format sebagai combo
  if (mainKey == " " && (ctrl || alt || shift))
    return "";

  std::string out = "";
  bool hasModifier = ctrl || alt || shift;

  if (hasModifier) {
    out = "<";
    if (ctrl)
      out += "C-";
    if (alt)
      out += "A-";
    if (shift)
      out += "S-";
    out += mainKey + ">";
  } else {
    out = mainKey;
  }

  return out;
}

// === bersihkan screen terminal ===
void clearFullScreen() {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD written;

  GetConsoleScreenBufferInfo(h, &csbi);
  DWORD size = csbi.dwSize.X * csbi.dwSize.Y;

  FillConsoleOutputCharacter(h, ' ', size, {0, 0}, &written);
  FillConsoleOutputAttribute(h, csbi.wAttributes, size, {0, 0}, &written);
  SetConsoleCursorPosition(h, {0, 0});
}

// === view kotak ===
void printBox(const std::string &text, int index) {
  int w = 14;
  int pad = w - text.size();
  if (pad < 1)
    pad = 1;

  std::cout << ((index == 0) ? "+" : (std::to_string(index - 1)))
            << std::string(w, '-') << "+\n";
  std::cout << "| " << text << std::string(pad - 1, ' ') << "|\n";
  std::cout << "+" << std::string(w, '-') << "+\n";
}

// === update display ===
void updateDisplay() {
  clearFullScreen();
  std::cout << "=== KEY OVERLAY (HOOK MODE) ===\n";

  if (wantToExit) {
    std::cout << "[ EXIT MODE ACTIVE - Press 'end' to exit ]\n";
  } else {
    std::cout << "Press 'tab' then 'end' to enter exit mode\n";
  }
  std::cout << "\n";

  std::cout << "[ LIVE ]\n";
  printBox(liveText == "" ? "no display" : liveText, 0);

  std::cout << "\n[ HISTORY ]\n";
  for (size_t i = 0; i < history.size(); i++) {
    printBox(history[i], i + 1);
  }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)lParam;
    int vkCode = kbd->vkCode;

    if (ignoredKey(vkCode)) {
      return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      // Key pressed
      pressedKeys.insert(vkCode);

      // Update live text
      liveText = formatCombo(pressedKeys);
      needRedraw = true;

    } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
      // Key released

      // Simpan ke history sebelum menghapus dari pressedKeys
      if (!pressedKeys.empty()) {
        std::string finalKey = formatCombo(pressedKeys);

        if (finalKey != "" && !finalKey.empty()) {
          history.push_back(finalKey);

          // Check exit sequence: tab -> end
          if (history.size() >= 2) {
            std::string prev = history[history.size() - 2];
            std::string curr = history[history.size() - 1];

            if (prev == "tab" && curr == "end") {
              // Masuk exit mode
              wantToExit = true;
            } else if (wantToExit && curr == "end") {
              // Exit confirmed - tekan end saat exit mode aktif
              PostQuitMessage(0);
            } else if (curr != "end") {
              // Reset exit mode jika bukan end
              wantToExit = false;
            }
          }

          // Keep only last 2 in history
          if (history.size() > 3)
            history.erase(history.begin());
        }
      }

      pressedKeys.erase(vkCode);

      // Update live text
      if (pressedKeys.empty()) {
        liveText = "";
      } else {
        liveText = formatCombo(pressedKeys);
      }

      needRedraw = true;
    }
  }

  return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// === main program ===
int main() {
  // Install low-level keyboard hook
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

  if (!keyboardHook) {
    std::cerr << "Failed to install keyboard hook!\n";
    return 1;
  }

  std::cout << "Keyboard hook installed. Starting overlay...\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Initial display
  updateDisplay();

  // Message loop with display update thread
  MSG msg;
  std::thread displayThread([&]() {
    while (true) {
      if (needRedraw) {
        updateDisplay();
        needRedraw = false;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
  });

  // Message pump
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Cleanup
  displayThread.detach();
  UnhookWindowsHookEx(keyboardHook);

  std::cout << "\n\nExit sequence confirmed. Goodbye!\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  return 0;
}
