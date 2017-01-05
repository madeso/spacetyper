#ifndef SPACETYPER_SPRITE_H
#define SPACETYPER_SPRITE_H

#include <vector>
#include <map>

#include "core/vec2.h"
#include "core/vec3.h"

class Texture2d;
class SpriteRenderer;

class Sprite {
 public:
  explicit Sprite(Texture2d* texture, const vec2f& position = vec2f(0.0f));

  Texture2d* GetTexture();

  const vec2f& GetPosition() const;
  void SetPosition(const vec2f& p);

  void SetRotation(float r);
  float GetRotation() const;

  float GetHeight() const;
  float GetWidth() const;

  void SetAlpha(float f);

  void Render(SpriteRenderer* render);
 private:
  Texture2d* texture_;
  vec2f position_;
  float rotate_;
  vec2f scale_;
  vec3f color_;
  float alpha_;
};

class Layer {
 public:
  explicit Layer(SpriteRenderer* render);
  void Add(Sprite* sprite);
  void Remove(Sprite* sprite);
  void Render();
 private:
  SpriteRenderer* render_;
  typedef std::vector<Sprite*> SpriteList;
  typedef std::map<Texture2d*, SpriteList> SpriteMap;
  SpriteMap sprites_;
};

#endif  // SPACETYPER_SPRITE_H