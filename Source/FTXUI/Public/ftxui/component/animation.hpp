// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include "HAL/Platform.h"

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

#include "ftxui/component/event.hpp"

namespace ftxui {

namespace animation {
// Components who haven't completed their animation can call this function to
// request a new frame to be drawn later.
//
// When there is no new events and no animations to complete, no new frame is
// drawn.
FTXUI_API void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Parameter of Component::OnAnimation(param).
class FTXUI_API Params {
 public:
  Params(Duration duration) : duration_(duration) {}

  /// The duration this animation step represents.
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// Linear interpolation (no easing)
FTXUI_API float Linear(float p);

// Quadratic easing; p^2
FTXUI_API float QuadraticIn(float p);
FTXUI_API float QuadraticOut(float p);
FTXUI_API float QuadraticInOut(float p);

// Cubic easing; p^3
FTXUI_API float CubicIn(float p);
FTXUI_API float CubicOut(float p);
FTXUI_API float CubicInOut(float p);

// Quartic easing; p^4
FTXUI_API float QuarticIn(float p);
FTXUI_API float QuarticOut(float p);
FTXUI_API float QuarticInOut(float p);

// Quintic easing; p^5
FTXUI_API float QuinticIn(float p);
FTXUI_API float QuinticOut(float p);
FTXUI_API float QuinticInOut(float p);

// Sine wave easing; sin(p * PI/2)
FTXUI_API float SineIn(float p);
FTXUI_API float SineOut(float p);
FTXUI_API float SineInOut(float p);

// Circular easing; sqrt(1 - p^2)
FTXUI_API float CircularIn(float p);
FTXUI_API float CircularOut(float p);
FTXUI_API float CircularInOut(float p);

// Exponential easing, base 2
FTXUI_API float ExponentialIn(float p);
FTXUI_API float ExponentialOut(float p);
FTXUI_API float ExponentialInOut(float p);

// Exponentially-damped sine wave easing
FTXUI_API float ElasticIn(float p);
FTXUI_API float ElasticOut(float p);
FTXUI_API float ElasticInOut(float p);

// Overshooting cubic easing;
FTXUI_API float BackIn(float p);
FTXUI_API float BackOut(float p);
FTXUI_API float BackInOut(float p);

// Exponentially-decaying bounce easing
FTXUI_API float BounceIn(float p);
FTXUI_API float BounceOut(float p);
FTXUI_API float BounceInOut(float p);
}  // namespace easing

class FTXUI_API Animator {
 public:
  Animator(float* from,
           float to = 0.f,
           Duration duration = std::chrono::milliseconds(250),
           easing::Function easing_function = easing::Linear,
           Duration delay = std::chrono::milliseconds(0));

  void OnAnimation(Params&);

  float to() const { return to_; }

 private:
  float* value_;
  float from_;
  float to_;
  Duration duration_;
  easing::Function easing_function_;
  Duration current_;
};

}  // namespace animation
}  // namespace ftxui

#endif /* end of include guard: FTXUI_ANIMATION_HPP */
