// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_DEPRECATED_HPP
#define FTXUI_DOM_DEPRECATED_HPP

#include "HAL/Platform.h"

#include "ftxui/dom/elements.hpp"

namespace ftxui {
FTXUI_API Element text(std::wstring text);
FTXUI_API Element vtext(std::wstring text);
FTXUI_API Elements paragraph(std::wstring text);
}  // namespace ftxui

#endif  // FTXUI_DOM_DEPRECATED_HPP
