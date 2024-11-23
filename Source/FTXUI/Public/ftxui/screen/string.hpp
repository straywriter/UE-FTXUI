// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_STRING_HPP
#define FTXUI_SCREEN_STRING_HPP

#include "HAL/Platform.h"

#include <stddef.h>  // for size_t
#include <cstdint>   // for uint8_t
#include <string>    // for string, wstring, to_string
#include <vector>    // for vector

namespace ftxui {
FTXUI_API std::string to_string(const std::wstring& s);
FTXUI_API std::wstring to_wstring(const std::string& s);

template <typename T>
std::wstring to_wstring(T s) {
  return to_wstring(std::to_string(s));
}

FTXUI_API int string_width(const std::string&);

// Split the string into a its glyphs. An empty one is inserted ater fullwidth
// ones.
FTXUI_API std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// Map every cells drawn by |input| to their corresponding Glyphs. Half-size
// Glyphs takes one cell, full-size Glyphs take two cells.
FTXUI_API std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#include "ftxui/screen/deprecated.hpp"

#endif /* end of include guard: FTXUI_SCREEN_STRING_HPP */
