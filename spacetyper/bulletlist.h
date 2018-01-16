#ifndef SPACETYPER_BULLETLIST_H
#define SPACETYPER_BULLETLIST_H

#include <memory>
#include <vector>

#include "core/vec2.h"
#include "core/angle.h"

#include "render/sprite.h"

class EnemyWord;
class Layer;

class BulletType
{
 public:
  typedef std::shared_ptr<Sprite> SpritePtr;

  ~BulletType();

  EnemyWord* word;
  SpritePtr  sprite;
};

class BulletList
{
 public:
  explicit BulletList(Layer* layer);
  Angle
  Add(EnemyWord* word, std::shared_ptr<Texture2d> t, const vec2f& pos);
  void
  Update(float d);

 private:
  Layer*                          layer_;
  typedef std::vector<BulletType> Bullets;
  Bullets                         bullets_;
};

#endif  // SPACETYPER_BULLETLIST_H
