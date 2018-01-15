#ifndef SPACETYPER_ENEMYWORD_H
#define SPACETYPER_ENEMYWORD_H

#include <random>

#include "render/sprite.h"
#include "render/fonts.h"

#include "core/vec2.h"
#include "core/size.h"

class TextureCache;
class SpriteFader;

class EnemyWord
{
 public:
  EnemyWord(
      SpriteFader*       fader,
      TextureCache*      cache,
      Font*              font,
      const std::string& word);
  ~EnemyWord();

  void
  Setup(std::mt19937* generator, float screen_width, float screen_height);

  void
  Update(float delta);

  void
  AddSprite(Layer* layer);
  void
  RemoveSprite();

  void
  Render(SpriteRenderer* renderer);

  bool
  Type(const std::string& input);
  bool
  IsAlive() const;

  const std::string&
  GetWord() const;
  const vec2f&
  GetPosition() const;
  const Sizef
  GetSize() const;

  void
  Damage();
  bool
  IsDestroyed() const;

 private:
  SpriteFader* fader_;
  Sprite       sprite_;
  Text         text_;
  vec2f        position_;
  Layer*       layer_;
  float        speed_;
  unsigned int index_;
  int          health_;
  float        explisiontimer_;
  int          explosions_;
  float        knockback_;
};

#endif  // SPACETYPER_ENEMYWORD_H
