// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_TERMINAL_HPP
#define FTXUI_SCREEN_TERMINAL_HPP

#include "HAL/Platform.h"

namespace ftxui {
struct Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
FTXUI_API Dimensions Size();
FTXUI_API void SetFallbackSize(const Dimensions& fallbackSize);

enum Color {
  Palette1,
  Palette16,
  Palette256,
  TrueColor,
};
FTXUI_API Color ColorSupport();
FTXUI_API void SetColorSupport(Color color);

}  // namespace Terminal

}  // namespace ftxui

#endif  // FTXUI_SCREEN_TERMINAL_HPP
