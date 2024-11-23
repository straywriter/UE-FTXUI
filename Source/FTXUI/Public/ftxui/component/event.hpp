// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_EVENT_HPP
#define FTXUI_COMPONENT_EVENT_HPP

#include "HAL/Platform.h"

#include <ftxui/component/mouse.hpp>  // for Mouse
#include <functional>
#include <string>  // for string, operator==
#include <vector>

namespace ftxui {

class ScreenInteractive;
class ComponentBase;

/// @brief Represent an event. It can be key press event, a terminal resize, or
/// more ...
///
/// For example:
/// - Printable character can be created using Event::Character('a').
/// - Some special are predefined, like Event::ArrowLeft.
/// - One can find arbitrary code for special Events using:
///   ./example/util/print_key_press
///  For instance, CTLR+A maps to Event::Special({1});
///
/// Useful documentation about xterm specification:
/// https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
struct Event {
  // --- Constructor section ---------------------------------------------------
  FTXUI_API static Event Character(std::string);
  FTXUI_API static Event Character(char);
  FTXUI_API static Event Character(wchar_t);
  FTXUI_API static Event Special(std::string);
  FTXUI_API static Event Mouse(std::string, Mouse mouse);
  FTXUI_API static Event CursorReporting(std::string, int x, int y);

  // --- Arrow ---
FTXUI_API static const Event ArrowLeft;
FTXUI_API static const Event ArrowRight;
FTXUI_API static const Event ArrowUp;
FTXUI_API static const Event ArrowDown;

FTXUI_API static const Event ArrowLeftCtrl;
FTXUI_API static const Event ArrowRightCtrl;
FTXUI_API static const Event ArrowUpCtrl;
FTXUI_API static const Event ArrowDownCtrl;

  // --- Other ---
FTXUI_API static const Event Backspace;
FTXUI_API static const Event Delete;
FTXUI_API static const Event Return;
FTXUI_API static const Event Escape;
FTXUI_API static const Event Tab;
FTXUI_API static const Event TabReverse;
FTXUI_API static const Event F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

FTXUI_API static const Event Home;
FTXUI_API static const Event End;

FTXUI_API static const Event PageUp;
FTXUI_API static const Event PageDown;

  // --- Custom ---
FTXUI_API static const Event Custom;

  //--- Method section ---------------------------------------------------------
  FTXUI_API bool is_character() const { return type_ == Type::Character; }
  FTXUI_API std::string character() const { return input_; }

  FTXUI_API bool is_mouse() const { return type_ == Type::Mouse; }
  FTXUI_API struct Mouse& mouse() { return data_.mouse; }

  FTXUI_API bool is_cursor_reporting() const { return type_ == Type::CursorReporting; }
  FTXUI_API int cursor_x() const { return data_.cursor.x; }
  FTXUI_API int cursor_y() const { return data_.cursor.y; }

  FTXUI_API const std::string& input() const { return input_; }

  FTXUI_API bool operator==(const Event& other) const { return input_ == other.input_; }
  FTXUI_API bool operator!=(const Event& other) const { return !operator==(other); }

  //--- State section ----------------------------------------------------------
  ScreenInteractive* screen_ = nullptr;

 private:
  friend ComponentBase;
  friend ScreenInteractive;
  enum class Type {
    Unknown,
    Character,
    Mouse,
    CursorReporting,
  };
  Type type_ = Type::Unknown;

  struct Cursor {
    int x = 0;
    int y = 0;
  };

  union {
    struct Mouse mouse;
    struct Cursor cursor;
  } data_ = {};

  std::string input_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_EVENT_HPP */
