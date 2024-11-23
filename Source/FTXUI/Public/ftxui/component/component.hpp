// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_HPP
#define FTXUI_COMPONENT_HPP

#include "HAL/Platform.h"

#include <functional>  // for function
#include <memory>      // for make_shared, shared_ptr
#include <string>      // for wstring
#include <utility>     // for forward
#include <vector>      // for vector

#include "ftxui/component/component_base.hpp"  // for Component, Components
#include "ftxui/component/component_options.hpp"  // for ButtonOption, CheckboxOption, MenuOption
#include "ftxui/dom/elements.hpp"  // for Element
#include "ftxui/util/ref.hpp"  // for ConstRef, Ref, ConstStringRef, ConstStringListRef, StringRef
#include "ftxui/screen/util.hpp"   // for clamp

namespace ftxui {
struct ButtonOption;
struct CheckboxOption;
struct Event;
struct InputOption;
struct MenuOption;
struct RadioboxOption;
struct MenuEntryOption;

template <class T, class... Args>
std::shared_ptr<T> Make(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

// Pipe operator to decorate components.
using ComponentDecorator = std::function<Component(Component)>;
using ElementDecorator = std::function<Element(Element)>;
FTXUI_API Component operator|(Component component, ComponentDecorator decorator);
FTXUI_API Component operator|(Component component, ElementDecorator decorator);
FTXUI_API Component& operator|=(Component& component, ComponentDecorator decorator);
FTXUI_API Component& operator|=(Component& component, ElementDecorator decorator);

namespace Container {
FTXUI_API Component Vertical(Components children);
FTXUI_API Component Vertical(Components children, int* selector);
FTXUI_API Component Horizontal(Components children);
FTXUI_API Component Horizontal(Components children, int* selector);
FTXUI_API Component Tab(Components children, int* selector);
FTXUI_API Component Stacked(Components children);
}  // namespace Container

FTXUI_API Component Button(ButtonOption options);
FTXUI_API Component Button(ConstStringRef label,
                 std::function<void()> on_click,
                 ButtonOption options = ButtonOption::Simple());

FTXUI_API Component Checkbox(CheckboxOption options);
FTXUI_API Component Checkbox(ConstStringRef label,
                   bool* checked,
                   CheckboxOption options = CheckboxOption::Simple());

FTXUI_API Component Input(InputOption options = {});
FTXUI_API Component Input(StringRef content, InputOption options = {});
FTXUI_API Component Input(StringRef content,
                StringRef placeholder,
                InputOption options = {});

FTXUI_API Component Menu(MenuOption options);
FTXUI_API Component Menu(ConstStringListRef entries,
               int* selected_,
               MenuOption options = MenuOption::Vertical());
FTXUI_API Component MenuEntry(MenuEntryOption options);
FTXUI_API Component MenuEntry(ConstStringRef label, MenuEntryOption options = {});

FTXUI_API Component Radiobox(RadioboxOption options);
FTXUI_API Component Radiobox(ConstStringListRef entries,
                   int* selected_,
                   RadioboxOption options = {});

FTXUI_API Component Dropdown(ConstStringListRef entries, int* selected);
FTXUI_API Component Toggle(ConstStringListRef entries, int* selected);

FTXUI_API Decorator flexDirection(Direction direction);

template <class T>
class SliderBase : public ComponentBase {
 public:
  explicit SliderBase(SliderOption<T> options)
      : value_(options.value),
        min_(options.min),
        max_(options.max),
        increment_(options.increment),
        options_(options) {}

  Element Render() override {
    auto gauge_color = Focused() ? color(options_.color_active)
                                 : color(options_.color_inactive);
    const float percent = float(value_() - min_()) / float(max_() - min_());
    return gaugeDirection(percent, options_.direction) |
           flexDirection(options_.direction) | reflect(gauge_box_) |
           gauge_color;
  }

  void OnLeft() {
    switch (options_.direction) {
      case Direction::Right:
        value_() -= increment_();
        break;
      case Direction::Left:
        value_() += increment_();
        break;
      case Direction::Up:
      case Direction::Down:
        break;
    }
  }

  void OnRight() {
    switch (options_.direction) {
      case Direction::Right:
        value_() += increment_();
        break;
      case Direction::Left:
        value_() -= increment_();
        break;
      case Direction::Up:
      case Direction::Down:
        break;
    }
  }

  void OnUp() {
    switch (options_.direction) {
      case Direction::Up:
        value_() -= increment_();
        break;
      case Direction::Down:
        value_() += increment_();
        break;
      case Direction::Left:
      case Direction::Right:
        break;
    }
  }

  void OnDown() {
    switch (options_.direction) {
      case Direction::Down:
        value_() -= increment_();
        break;
      case Direction::Up:
        value_() += increment_();
        break;
      case Direction::Left:
      case Direction::Right:
        break;
    }
  }

  bool OnEvent(Event event) final {
    using namespace ftxui;
    if (event.is_mouse()) {
      return OnMouseEvent(event);
    }

    T old_value = value_();
    if (event == Event::ArrowLeft || event == Event::Character('h')) {
      OnLeft();
    }
    if (event == Event::ArrowRight || event == Event::Character('l')) {
      OnRight();
    }
    if (event == Event::ArrowUp || event == Event::Character('k')) {
      OnDown();
    }
    if (event == Event::ArrowDown || event == Event::Character('j')) {
      OnUp();
    }

    value_() = util::clamp(value_(), min_(), max_());
    if (old_value != value_()) {
      return true;
    }

    return ComponentBase::OnEvent(event);
  }

  bool OnMouseEvent(Event event) {
    if (captured_mouse_) {
      if (event.mouse().motion == Mouse::Released) {
        captured_mouse_ = nullptr;
        return true;
      }

      switch (options_.direction) {
        case Direction::Right: {
          value_() = min_() + (event.mouse().x - gauge_box_.x_min) *
                                  (max_() - min_()) /
                                  (gauge_box_.x_max - gauge_box_.x_min);
          break;
        }
        case Direction::Left: {
          value_() = max_() - (event.mouse().x - gauge_box_.x_min) *
                                  (max_() - min_()) /
                                  (gauge_box_.x_max - gauge_box_.x_min);
          break;
        }
        case Direction::Down: {
          value_() = min_() + (event.mouse().y - gauge_box_.y_min) *
                                  (max_() - min_()) /
                                  (gauge_box_.y_max - gauge_box_.y_min);
          break;
        }
        case Direction::Up: {
          value_() = max_() - (event.mouse().y - gauge_box_.y_min) *
                                  (max_() - min_()) /
                                  (gauge_box_.y_max - gauge_box_.y_min);
          break;
        }
      }
      value_() = std::max(min_(), std::min(max_(), value_()));
      return true;
    }

    if (event.mouse().button != Mouse::Left ||
        event.mouse().motion != Mouse::Pressed) {
      return false;
    }

    if (!gauge_box_.Contain(event.mouse().x, event.mouse().y)) {
      return false;
    }

    captured_mouse_ = CaptureMouse(event);

    if (captured_mouse_) {
      TakeFocus();
      return true;
    }

    return false;
  }

  bool Focusable() const final { return true; }

 private:
  Ref<T> value_;
  ConstRef<T> min_;
  ConstRef<T> max_;
  ConstRef<T> increment_;
  SliderOption<T> options_;
  Box gauge_box_;
  CapturedMouse captured_mouse_;
};


// General slider constructor:
template <typename T>
Component Slider(SliderOption<T> options) {
    return Make<SliderBase<T>>(options);
  }

// Shorthand without the `SliderOption` constructor:
FTXUI_API Component Slider(ConstStringRef label,
                 Ref<int> value,
                 ConstRef<int> min = 0,
                 ConstRef<int> max = 100,
                 ConstRef<int> increment = 5);
FTXUI_API Component Slider(ConstStringRef label,
                 Ref<float> value,
                 ConstRef<float> min = 0.f,
                 ConstRef<float> max = 100.f,
                 ConstRef<float> increment = 5.f);
FTXUI_API Component Slider(ConstStringRef label,
                 Ref<long> value,
                 ConstRef<long> min = 0l,
                 ConstRef<long> max = 100l,
                 ConstRef<long> increment = 5l);

FTXUI_API Component ResizableSplit(ResizableSplitOption options);
FTXUI_API Component ResizableSplitLeft(Component main, Component back, int* main_size);
FTXUI_API Component ResizableSplitRight(Component main, Component back, int* main_size);
FTXUI_API Component ResizableSplitTop(Component main, Component back, int* main_size);
FTXUI_API Component ResizableSplitBottom(Component main, Component back, int* main_size);

FTXUI_API Component Renderer(Component child, std::function<Element()>);
FTXUI_API Component Renderer(std::function<Element()>);
FTXUI_API Component Renderer(std::function<Element(bool /* focused */)>);
FTXUI_API ComponentDecorator Renderer(ElementDecorator);

FTXUI_API Component CatchEvent(Component child, std::function<bool(Event)>);
FTXUI_API ComponentDecorator CatchEvent(std::function<bool(Event)> on_event);

FTXUI_API Component Maybe(Component, const bool* show);
FTXUI_API Component Maybe(Component, std::function<bool()>);
FTXUI_API ComponentDecorator Maybe(const bool* show);
FTXUI_API ComponentDecorator Maybe(std::function<bool()>);

FTXUI_API Component Modal(Component main, Component modal, const bool* show_modal);
FTXUI_API ComponentDecorator Modal(Component modal, const bool* show_modal);

FTXUI_API Component Collapsible(ConstStringRef label,
                      Component child,
                      Ref<bool> show = false);

FTXUI_API Component Hoverable(Component component, bool* hover);
FTXUI_API Component Hoverable(Component component,
                    std::function<void()> on_enter,
                    std::function<void()> on_leave);
FTXUI_API Component Hoverable(Component component,  //
                    std::function<void(bool)> on_change);
FTXUI_API ComponentDecorator Hoverable(bool* hover);
FTXUI_API ComponentDecorator Hoverable(std::function<void()> on_enter,
                             std::function<void()> on_leave);
FTXUI_API ComponentDecorator Hoverable(std::function<void(bool)> on_change);

FTXUI_API Component Window(WindowOptions option);

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_HPP */
