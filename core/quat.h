#ifndef CORE_QUAT_H
#define CORE_QUAT_H

#include "core/vec3.h"
#include "core/numeric.h"
#include "core/axisangle.h"

template <typename T>
class quat {
 public:
  typedef quat<T> Q;
  typedef vec3<T> Vec;

  T x;
  T y;
  T z;
  T w;

  Vec vec() const { return Vec(x, y , z); }

  quat(T w, const Vec& v) : w(w), x(v.x), y(v.y), z(v.z) {}

  quat(const AxisAngle& aa) {
    const T sin_a = Sin( aa.angle / 2 );
    const T cos_a = Cos( aa.angle / 2 );
    Q r(cos_a, aa.axis*sin_a);
    r.Normalize();
    *this = r;
  }

  AxisAngle ToAxisAngle() const {
    const T cos_a = w;
    const auto angle = Acos( cos_a ) * 2;
    const T sin_a = DefaultIfCloseToZero<T>(Sqrt(1.0 - cos_a * cos_a), 1, 0.0005);
    return AxisAngle::RightHandAround(vec()/sin_a, angle);
  }

  // static Q FromAngles(T x, T y, T z);

  static Q Identity() {
    return Q(1, Vec(0, 0, 0));
  }
  static Q LookAt(const Vec& from, const Vec& to, const typename Vec::Unit up) {
    return LookInDirection(Vec::FromTo(from, to).GetNormalized(), up);
  }
  static Q LookInDirection(const Vec& dir, const typename Vec::Unit& up) {
    float dot = ::dot(Vec::In(), dir);

    if (Abs(dot - (-1.0f)) < 0.000001f)
    {
      return Q(3.1415926535897932f, up);
    }
    if (Abs(dot - (1.0f)) < 0.000001f)
    {
      return Identity();
    }

    const auto rotAngle = Acos(dot);
    const typename Vec::Unit rotAxis = cross(Vec::In(), dir).GetNormalized();
    return Q(AxisAngle::RightHandAround(rotAxis, rotAngle));
  }

  Q GetConjugate() const {
    return quat(w, -vec());
  }
  Q GetIdentity() const {
    return quat(w/GetLength(), -vec());
  }
  T GetLength() const {
    return Sqrt(GetLengthSquared());
  }
  T GetLengthSquared() const {
    return x*x + y*y + z*z + w*w;
  }

  void Normalize() {
    const T l = GetLength();
    if( IsZero(l) ) {
      *this = Identity();
    }
    else {
      x /= l;
      y /= l;
      z /= l;
      w /= l;
    }
  }

  Q GetNormalized() const {
    Q r = *this;
    r.Normalize();
    return r;
  }

  Vec In() const {
    return RotateAroundOrigo(-Vec::ZAxis());
  }
  Vec Out() const {
    return RotateAroundOrigo(Vec::ZAxis());
  }
  Vec Right() const {
    return RotateAroundOrigo(Vec::XAxis());
  }
  Vec Left() const {
    return RotateAroundOrigo(-Vec::XAxis());
  }
  Vec Up() const {
    return RotateAroundOrigo(Vec::YAxis());
  }
  Vec Down() const {
    return RotateAroundOrigo(-Vec::YAxis());
  }

  // In*Z + Right*X + Up*Y
  Vec RightUpIn(const Vec v) const {
    return   In()*v.z
           + Right()*v.x
           + Up()*v.y;
  }

  Vec RotateAroundOrigo(const Vec v) const {
    // http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
    const Q pure(0, v);
    const Q a = *this * pure;
    const Q ret = a * GetConjugate();
    return ret.vec();
  }

  static Q Lerp(const Q& f, const T scale, const Q& t) {
    return f * (1 - scale) + t * scale;
  }

  static Q Slerp(const Q& qa, const T t, const Q& qb) {
    // from: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
    // Calculate angle between them.
    const T cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
    // if qa=qb or qa=-qb then theta = 0 and we can return qa
    if (abs(cosHalfTheta) >= 1.0){
      return qa;
    }
    // Calculate temporary values.
    const T halfTheta = acos(cosHalfTheta);
    const T sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
    if (Abs(sinHalfTheta) < 0.001) {
      // if theta = 180 degrees then result is not fully defined
      // we could rotate around any axis normal to qa or qb
      return (qa + qb) * 0.5;
    }
    const T ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
    const T ratioB = sin(t * halfTheta) / sinHalfTheta;
    return qa * ratioA + qb * ratioB;
  }

  static Q SlerpShortway(const Q& f, const T scale, const Q& t) {

    if(dot(f, t) < 0)
    {
      return Slerp(f.GetConjugate(), scale, t);
    }
    else
    {
      return Slerp(f, scale, t);
    }
  }

  void operator+=(const Q& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
  }

  void operator-=(const Q& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
  }

  void operator*=(const T& rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
  }
  void operator*=(const Q& rhs) {
#define VAR(a,b) const T a##1##b##2 = a*rhs.b
    VAR(w,w);
    VAR(w,x);
    VAR(w,y);
    VAR(w,z);

    VAR(x,w);
    VAR(x,x);
    VAR(x,y);
    VAR(x,z);

    VAR(y,w);
    VAR(y,x);
    VAR(y,y);
    VAR(y,z);

    VAR(z,w);
    VAR(z,x);
    VAR(z,y);
    VAR(z,z);
#undef VAR

    w = w1w2 - x1x2 - y1y2 - z1z2;
    x = w1x2 + x1w2 + y1z2 - z1y2;
    y = w1y2 + y1w2 + z1x2 - x1z2;
    z = w1z2 + z1w2 + x1y2 - y1x2;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const quat<T>& v)
{
  return stream << "(" << v.w << " (" << v.x << ", " << v.y << ", " << v.z << "))";
}

template<typename T>
T dot(const quat<T>& lhs, const quat<T>& rhs) {
  return   lhs.x * rhs.x
         + lhs.y * rhs.y
         + lhs.z * rhs.z
         + lhs.w * rhs.w;
}

template<typename T>
quat<T> operator*(const quat<T>& lhs, const quat<T>& rhs) {
  quat<T> r = lhs;
  r *= rhs;
  return r;
}

template<typename T>
quat<T> operator*(T scale, const quat<T>& q) {
  quat<T> r = q;
  r *= scale;
  return r;
}

template<typename T>
quat<T> operator*(const quat<T>& q, T scale) {
  quat<T> r = q;
  r *= scale;
  return r;
}


template<typename T>
quat<T> operator+(const quat<T>& lhs, const quat<T>& rhs) {
  quat<T> r = rhs;
  r += lhs;
  return r;
}
template<typename T>
quat<T> operator-(const quat<T>& lhs, const quat<T>& rhs) {
  quat<T> r = rhs;
  r -= lhs;
  return r;
}

template <typename T>
bool operator==(const quat<T>& lhs, const quat<T>& rhs) {
  return
      lhs.x == rhs.x
      && lhs.y==rhs.y
      && lhs.z==rhs.z
      && lhs.w==rhs.w
      ;
}

typedef quat<float> quatf;
typedef quat<int> quati;

#endif  // CORE_QUAT_H