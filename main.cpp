#include "keymap.h"
#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

// === cek key yang di ignore ===
bool ignoredKey(int k) {
  return (k == VK_APPS || k == 255 || k == 183 || k == VK_LWIN || k == VK_RWIN);
}

// === cek key modifier ===
bool isModifier(int k) {
  return k == VK_CONTROL || k == VK_MENU || k == VK_SHIFT;
}

// === format kombinasi key ===
std::string formatCombo(const std::vector<int> &codes) {
  bool c = false, a = false, s = false;
  std::string last = "";

  for (int k : codes) {
    if (ignoredKey(k))
      continue;

    if (k == VK_CONTROL)
      c = true;
    else if (k == VK_MENU)
      a = true;
    else if (k == VK_SHIFT)
      s = true;
    else {
      if (keyMap.count(k))
        last = keyMap.at(k);
      else if (k >= 'A' && k <= 'Z')
        last = std::string(1, (char)k);
      else if (k >= '0' && k <= '9')
        last = std::string(1, (char)k);
    }
  }

  // jika tidak ada
  if (last == "")
    last = " ";

  std::string out = "<";
  if (c)
    out += "C-";
  if (a)
    out += "A-";
  if (s)
    out += "S-";
  out += last + ">";
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

// === main program ===
int main() {
  bool prevState[256] = {0};
  bool currState[256] = {0};

  std::vector<std::string> history;
  std::string liveText = "";
  std::string lastFinalKey = "";
  bool wantToExit = false;

  while (true) {

    // === scan key ===
    for (int i = 0; i < 256; i++) {
      if (ignoredKey(i))
        currState[i] = false;
      else
        currState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    // === keys yang aktif ===
    std::vector<int> activeKeys;
    for (int k = 0; k < 256; k++) {
      if (currState[k] && !ignoredKey(k))
        activeKeys.push_back(k);
    }

    // === key kombinasi ===
    if (!activeKeys.empty()) {
      bool ctrl = currState[VK_CONTROL];
      bool alt = currState[VK_MENU];
      bool shift = currState[VK_SHIFT];
      bool hasMod = ctrl || alt || shift;

      if (hasMod && activeKeys.size() >= 2) {
        liveText = formatCombo(activeKeys);
        lastFinalKey = liveText;
      } else {
        int k = activeKeys.back();
        if (keyMap.count(k))
          liveText = keyMap.at(k);
        else if (k >= 'A' && k <= 'Z')
          liveText = std::string(1, (char)k);
        else if (k >= '0' && k <= '9')
          liveText = std::string(1, (char)k);
        lastFinalKey = liveText;
      }
    }

    // === pengolahan penyimpanan key ===
    bool anyPrev = false;
    bool anyNow = false;

    for (int i = 0; i < 256; i++)
      if (!ignoredKey(i) && prevState[i])
        anyPrev = true;

    for (int i = 0; i < 256; i++)
      if (!ignoredKey(i) && currState[i])
        anyNow = true;

    // Jika sebelumnya ada key, sekarang tidak ada key → simpan history
    if (anyPrev && !anyNow) {
      if (lastFinalKey != "") {
        history.push_back(lastFinalKey);
        lastFinalKey = "";

        // Check exit sequence: tab -> end
        if (history.size() >= 2) {
          std::string prev = history[history.size() - 2];
          std::string curr = history[history.size() - 1];

          if (prev == "tab" && curr == "end" || curr == "end") {

            if (wantToExit) {
              // Second sequence detected, exit
              std::cout << "\n\nExit sequence confirmed. Goodbye!\n";
              std::this_thread::sleep_for(std::chrono::milliseconds(500));
              break;
            } else {
              // First sequence detected, activate exit mode
              wantToExit = true;
            }
          } else {
            // Reset if different sequence
            wantToExit = false;
          }
        }
      }
      if (history.size() > 2)
        history.erase(history.begin());
    }

    // === tampilan ===
    clearFullScreen();
    std::cout << "=== KEY OVERLAY (LIVE) ===\n";

    if (wantToExit) {
      std::cout << "[ EXIT MODE ACTIVE - Press end again to exit ]\n";
    } else {
      std::cout << "save in history { tab and end } to exit mode\n";
    }
    std::cout << "\n";

    std::cout << "[ LIVE ]\n";
    printBox(liveText == "" ? " " : liveText, 0);

    std::cout << "\n[ HISTORY ]\n";
    for (size_t i = 0; i < history.size(); i++) {
      printBox(history[i], i + 1);
    }

    // copy state
    for (int i = 0; i < 256; i++)
      prevState[i] = currState[i];

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}
