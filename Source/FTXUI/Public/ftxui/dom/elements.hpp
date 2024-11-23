// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include "HAL/Platform.h"

#include <functional>
#include <memory>

#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/ref.hpp"

namespace ftxui {
class Node;
using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;
using Decorator = std::function<Element(Element)>;
using GraphFunction = std::function<std::vector<int>(int, int)>;

enum BorderStyle {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// Pipe elements into decorator togethers.
// For instance the next lines are equivalents:
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))
FTXUI_API Element operator|(Element, Decorator);
FTXUI_API Element& operator|=(Element&, Decorator);
FTXUI_API Elements operator|(Elements, Decorator);
FTXUI_API Decorator operator|(Decorator, Decorator);

// --- Widget ---
FTXUI_API Element text(std::string text);
FTXUI_API Element vtext(std::string text);
FTXUI_API Element separator();
FTXUI_API Element separatorLight();
FTXUI_API Element separatorDashed();
FTXUI_API Element separatorHeavy();
FTXUI_API Element separatorDouble();
FTXUI_API Element separatorEmpty();
FTXUI_API Element separatorStyled(BorderStyle);
FTXUI_API Element separator(Pixel);
FTXUI_API Element separatorCharacter(std::string);
FTXUI_API Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
FTXUI_API Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
FTXUI_API Element gauge(float progress);
FTXUI_API Element gaugeLeft(float progress);
FTXUI_API Element gaugeRight(float progress);
FTXUI_API Element gaugeUp(float progress);
FTXUI_API Element gaugeDown(float progress);
FTXUI_API Element gaugeDirection(float progress, Direction direction);
FTXUI_API Element border(Element);
FTXUI_API Element borderLight(Element);
FTXUI_API Element borderDashed(Element);
FTXUI_API Element borderHeavy(Element);
FTXUI_API Element borderDouble(Element);
FTXUI_API Element borderRounded(Element);
FTXUI_API Element borderEmpty(Element);
FTXUI_API Decorator borderStyled(BorderStyle);
FTXUI_API Decorator borderStyled(BorderStyle, Color);
FTXUI_API Decorator borderStyled(Color);
FTXUI_API Decorator borderWith(const Pixel&);
FTXUI_API Element window(Element title, Element content);
FTXUI_API Element spinner(int charset_index, size_t image_index);
FTXUI_API Element paragraph(const std::string& text);
FTXUI_API Element paragraphAlignLeft(const std::string& text);
FTXUI_API Element paragraphAlignRight(const std::string& text);
FTXUI_API Element paragraphAlignCenter(const std::string& text);
FTXUI_API Element paragraphAlignJustify(const std::string& text);
FTXUI_API Element graph(GraphFunction);
FTXUI_API Element emptyElement();
FTXUI_API Element canvas(ConstRef<Canvas>);
FTXUI_API Element canvas(int width, int height, std::function<void(Canvas&)>);
FTXUI_API Element canvas(std::function<void(Canvas&)>);

// -- Decorator ---
FTXUI_API Element bold(Element);
FTXUI_API Element dim(Element);
FTXUI_API Element inverted(Element);
FTXUI_API Element underlined(Element);
FTXUI_API Element underlinedDouble(Element);
FTXUI_API Element blink(Element);
FTXUI_API Element strikethrough(Element);
FTXUI_API Decorator color(Color);
FTXUI_API Decorator bgcolor(Color);
FTXUI_API Decorator color(const LinearGradient&);
FTXUI_API Decorator bgcolor(const LinearGradient&);
FTXUI_API Element color(Color, Element);
FTXUI_API Element bgcolor(Color, Element);
FTXUI_API Element color(const LinearGradient&, Element);
FTXUI_API Element bgcolor(const LinearGradient&, Element);
FTXUI_API Decorator focusPosition(int x, int y);
FTXUI_API Decorator focusPositionRelative(float x, float y);
FTXUI_API Element automerge(Element child);
FTXUI_API Decorator hyperlink(std::string link);
FTXUI_API Element hyperlink(std::string link, Element child);

// --- Layout is
// Horizontal, Vertical or stacked set of elements.
FTXUI_API Element hbox(Elements);
FTXUI_API Element vbox(Elements);
FTXUI_API Element dbox(Elements);
FTXUI_API Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
FTXUI_API Element gridbox(std::vector<Elements> lines);

FTXUI_API Element hflow(Elements);  // Helper: default flexbox with row direction.
FTXUI_API Element vflow(Elements);  // Helper: default flexbox with column direction.

// -- Flexibility ---
// Define how to share the remaining space when not all of it is used inside a
// container.
FTXUI_API Element flex(Element);         // Expand/Minimize if possible/needed.
FTXUI_API Element flex_grow(Element);    // Expand element if possible.
FTXUI_API Element flex_shrink(Element);  // Minimize element if needed.

FTXUI_API Element xflex(Element);         // Expand/Minimize if possible/needed on X axis.
FTXUI_API Element xflex_grow(Element);    // Expand element if possible on X axis.
FTXUI_API Element xflex_shrink(Element);  // Minimize element if needed on X axis.

FTXUI_API Element yflex(Element);         // Expand/Minimize if possible/needed on Y axis.
FTXUI_API Element yflex_grow(Element);    // Expand element if possible on Y axis.
FTXUI_API Element yflex_shrink(Element);  // Minimize element if needed on Y axis.

FTXUI_API Element notflex(Element);  // Reset the flex attribute.
FTXUI_API Element filler();          // A blank expandable element.

// -- Size override;
enum WidthOrHeight { WIDTH, HEIGHT };
enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };
FTXUI_API Decorator size(WidthOrHeight, Constraint, int value);

// --- Frame ---
// A frame is a scrollable area. The internal area is potentially larger than
// the external one. The internal area is scrolled in order to make visible the
// focused element.
FTXUI_API Element frame(Element);
FTXUI_API Element xframe(Element);
FTXUI_API Element yframe(Element);
FTXUI_API Element focus(Element);
FTXUI_API Element select(Element);

// --- Cursor ---
// Those are similar to `focus`, but also change the shape of the cursor.
FTXUI_API Element focusCursorBlock(Element);
FTXUI_API Element focusCursorBlockBlinking(Element);
FTXUI_API Element focusCursorBar(Element);
FTXUI_API Element focusCursorBarBlinking(Element);
FTXUI_API Element focusCursorUnderline(Element);
FTXUI_API Element focusCursorUnderlineBlinking(Element);

// --- Misc ---
FTXUI_API Element vscroll_indicator(Element);
FTXUI_API Decorator reflect(Box& box);
// Before drawing the |element| clear the pixel below. This is useful in
// combinaison with dbox.
FTXUI_API Element clear_under(Element element);

// --- Util --------------------------------------------------------------------
FTXUI_API Element hcenter(Element);
FTXUI_API Element vcenter(Element);
FTXUI_API Element center(Element);
FTXUI_API Element align_right(Element);
FTXUI_API Element nothing(Element element);

namespace Dimension {
FTXUI_API Dimensions Fit(Element&);
}  // namespace Dimension

}  // namespace ftxui

// Make container able to take any number of children as input.
#include "ftxui/dom/take_any_args.hpp"

// Include old definitions using wstring.
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP
