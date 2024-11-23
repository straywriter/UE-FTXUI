// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for max, min
#include <cstdint>    // for uint8_t, uint16_t, uint32_t, uint64_t
#include <ftxui/component/component_options.hpp>  // for SliderOption
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Down, Direction::Left, Direction::Right, Direction::Up
#include <string>                   // for allocator
#include <utility>                  // for move

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/component.hpp"       // for Make, Slider
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/event.hpp"  // for Event, Event::ArrowDown, Event::ArrowLeft, Event::ArrowRight, Event::ArrowUp
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed, Mouse::Released
#include "ftxui/component/screen_interactive.hpp"  // for Component
#include "ftxui/dom/elements.hpp"  // for operator|, text, Element, xflex, hbox, color, underlined, reflect, Decorator, dim, vcenter, focus, nothing, select, yflex, gaugeDirection
#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/color.hpp"  // for Color, Color::GrayDark, Color::White
#include "ftxui/screen/util.hpp"   // for clamp
#include "ftxui/util/ref.hpp"      // for ConstRef, Ref, ConstStringRef

namespace ftxui {
  
Decorator flexDirection(Direction direction) {
  switch (direction) {
  case Direction::Up:
  case Direction::Down:
    return yflex;
  case Direction::Left:
  case Direction::Right:
    return xflex;
  }
  return xflex;  // NOT_REACHED()
}
  
namespace {

class SliderWithLabel : public ComponentBase {
 public:
  SliderWithLabel(ConstStringRef label, Component inner)
      : label_(std::move(label)) {
    Add(std::move(inner));
    SetActiveChild(ChildAt(0));
  }

 private:
  bool OnEvent(Event event) final {
    if (ComponentBase::OnEvent(event)) {
      return true;
    }

    if (!event.is_mouse()) {
      return false;
    }

    mouse_hover_ = box_.Contain(event.mouse().x, event.mouse().y);

    if (!mouse_hover_) {
      return false;
    }

    if (!CaptureMouse(event)) {
      return false;
    }

    return true;
  }

  Element Render() override {
    auto focus_management = Focused() ? focus : Active() ? select : nothing;
    auto gauge_color = (Focused() || mouse_hover_) ? color(Color::White)
                                                   : color(Color::GrayDark);
    return hbox({
               text(label_()) | dim | vcenter,
               hbox({
                   text("["),
                   ComponentBase::Render() | underlined,
                   text("]"),
               }) | xflex,
           }) |
           gauge_color | xflex | reflect(box_) | focus_management;
  }

  ConstStringRef label_;
  Box box_;
  bool mouse_hover_ = false;
};
}  // namespace

/// @brief An horizontal slider.
/// @param label The name of the slider.
/// @param value The current value of the slider.
/// @param min The minimum value.
/// @param max The maximum value.
/// @param increment The increment when used by the cursor.
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// int value = 50;
/// auto slider = Slider("Value:", &value, 0, 100, 1);
/// screen.Loop(slider);
/// ```
///
/// ### Output
///
/// ```bash
/// Value:[██████████████████████████                          ]
/// ```
Component Slider(ConstStringRef label,
                 Ref<int> value,
                 ConstRef<int> min,
                 ConstRef<int> max,
                 ConstRef<int> increment) {
  SliderOption<int> option;
  option.value = value;
  option.min = min;
  option.max = max;
  option.increment = increment;
  auto slider = Make<SliderBase<int>>(option);
  return Make<SliderWithLabel>(std::move(label), slider);
}

Component Slider(ConstStringRef label,
                 Ref<float> value,
                 ConstRef<float> min,
                 ConstRef<float> max,
                 ConstRef<float> increment) {
  SliderOption<float> option;
  option.value = value;
  option.min = min;
  option.max = max;
  option.increment = increment;
  auto slider = Make<SliderBase<float>>(option);
  return Make<SliderWithLabel>(std::move(label), slider);
}
Component Slider(ConstStringRef label,
                 Ref<long> value,
                 ConstRef<long> min,
                 ConstRef<long> max,
                 ConstRef<long> increment) {
  SliderOption<long> option;
  option.value = value;
  option.min = min;
  option.max = max;
  option.increment = increment;
  auto slider = Make<SliderBase<long>>(option);
  return Make<SliderWithLabel>(std::move(label), slider);
}

/// @brief A slider in any direction.
/// @param options The options
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// int value = 50;
/// auto slider = Slider({
///   .value = &value,
///   .min = 0,
///   .max = 100,
///   .increment= 20,
/// });
/// screen.Loop(slider);
/// ```

template Component Slider(SliderOption<int8_t>);
template Component Slider(SliderOption<int16_t>);
template Component Slider(SliderOption<int32_t>);
template Component Slider(SliderOption<int64_t>);

template Component Slider(SliderOption<uint8_t>);
template Component Slider(SliderOption<uint16_t>);
template Component Slider(SliderOption<uint32_t>);
template Component Slider(SliderOption<uint64_t>);

template Component Slider(SliderOption<float>);
template Component Slider(SliderOption<double>);

}  // namespace ftxui
