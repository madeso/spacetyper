#ifndef SPACETYPER_INTERPOLATE_H
#define SPACETYPER_INTERPOLATE_H

#include <cassert>
#include <deque>

#include "aheasing/easing.h"

struct FloatTransform {
  static float Transform(float from, float v, float to);
};

// Transform should have a function Transform(T from, float zeroToOne, T to)
template<typename Type, typename Transform>
class Interpolate {
 public:
  typedef Interpolate<Type, Transform> This;
  explicit Interpolate(Type v) : value_(v), from_(v), delta_(0.0f) {}

  const Type& GetValue() const { assert(this); return value_; }
  void SetValue(const Type& t) { assert(this); value_ = t; Clear(); }
  operator const Type&() const { assert(this); return GetValue(); }
  void operator=(const Type& rhs) { assert(this); SetValue(rhs); }

  void Update(float dt) {
    assert(this);
    if( data_.empty()) return;
    const InterpolationData& d = data_.front();
    if( d.type != nullptr ) {
      const float interpolated = d.type(delta_);
      value_ = Transform::Transform(from_, interpolated, d.target);
    }
    delta_ += dt / d.time;
    if( delta_ > 1.0f) {
      delta_ -= 1.0f; // not really correct, but works for now
      data_.pop_front();
      if( !data_.empty() ) {
        from_ = value_;
      }
    }
  }

  This& Clear() {assert(this); data_.clear(); return *this; }

  This& Sleep(float time) {assert(this); AddInterpolation(nullptr, value_, time); return *this; }

#define FUN(NAME, FUNC) This& NAME(const Type& target, float time) {assert(this); AddInterpolation(FUNC, target, time); return *this; }
  // Linear interpolation (no easing)
  FUN(Linear, LinearInterpolation)

// Quadratic easing; p^2
  FUN(QuadIn, QuadraticEaseIn)
  FUN(QuadOut, QuadraticEaseOut)
  FUN(Quad, QuadraticEaseInOut)

// Cubic easing; p^3
  FUN(CubicIn, CubicEaseIn)
  FUN(CubicOut, CubicEaseOut)
  FUN(Cubic, CubicEaseInOut)

// Quartic easing; p^4
  FUN(QuartIn, QuarticEaseIn)
  FUN(QuartOut, QuarticEaseOut)
  FUN(Quart, QuarticEaseInOut)

// Quintic easing; p^5
  FUN(QuintIn, QuinticEaseIn)
  FUN(QuintOut, QuinticEaseOut)
  FUN(Quint, QuinticEaseInOut)

// Sine wave easing; sin(p * PI/2)
  FUN(SineIn, SineEaseIn)
  FUN(SineOut, SineEaseOut)
  FUN(Sine, SineEaseInOut)

// Circular easing; sqrt(1 - p^2)
  FUN(CircIn, CircularEaseIn)
  FUN(CircOut, CircularEaseOut)
  FUN(Circ, CircularEaseInOut)

// Exponential easing, base 2
  FUN(ExpIn, ExponentialEaseIn)
  FUN(ExpOut, ExponentialEaseOut)
  FUN(Exp, ExponentialEaseInOut)

// Exponentially-damped sine wave easing
  FUN(ElasticIn, ElasticEaseIn)
  FUN(ElasticOut, ElasticEaseOut)
  FUN(Elastic, ElasticEaseInOut)

// Overshooting cubic easing;
  FUN(BackIn, BackEaseIn)
  FUN(BackOut, BackEaseOut)
  FUN(Back, BackEaseInOut)

// Exponentially-decaying bounce easing
  FUN(BounceIn, BounceEaseIn)
  FUN(BounceOut, BounceEaseOut)
  FUN(Bounce, BounceEaseInOut)
#undef FUN

 private:
  struct InterpolationData {
    AHEasingFunction type; // how to interpolate
    Type target; // target value
    float time; // time to transition
  };
  Type value_; // current value

  Type from_; // starting value
  float delta_; // goes from 0 to 1
  std::deque<InterpolationData> data_;

  void AddInterpolation(AHEasingFunction type, const Type& target, float time) {
    assert(this);
    assert(time > 0.0f);

    if( data_.empty() ) {
      from_ = value_;
    }

    InterpolationData d;
    d.type = type;
    d.target = target;
    d.time = time;
    data_.push_back(d);
  }
};

typedef Interpolate<float, FloatTransform> FloatInterpolate;

#endif  // SPACETYPER_INTERPOLATE_H