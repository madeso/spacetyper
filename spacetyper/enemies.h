#ifndef SPACETYPER_ENEMIES_H
#define SPACETYPER_ENEMIES_H

#include <vector>
#include <memory>
#include <random>
#include <string>

#include "core/vec2.h"
#include "core/angle.h"

class EnemyWord;
class TextureCache;
class Font;
class Layer;
class Dictionary;
class Sprite;
class BulletList;
class SpriteFader;
class SpriteRenderer;

class Enemies
{
 public:
  Enemies(
      SpriteFader*  fader,
      TextureCache* cache,
      Font*         font,
      Layer*        layer,
      Dictionary*   dictionary,
      float         width,
      float         height,
      BulletList*   bullets);
  ~Enemies();

  void
  SpawnEnemies(int count);

  void
  AddEnemy();
  int
  EnemyCount();

  void
  Update(float delta);

  void
  Render(SpriteRenderer* renderer);

  EnemyWord*
  DetectWord(const std::string& input);
  void
  Remove(EnemyWord* word);

  Angle
  FireAt(const vec2f& pos, EnemyWord* word);

 private:
  SpriteFader*         fader_;
  mutable std::mt19937 generator_;
  TextureCache*        cache_;
  Font*                font_;
  Layer*               layer_;
  Dictionary*          dictionary_;
  float                width_;
  float                height_;

  int   spawn_count_;
  float spawn_time_;


  typedef std::shared_ptr<EnemyWord> EnemyPtr;
  typedef std::vector<EnemyPtr>      EnemyList;
  EnemyList                          enemies_;
  EnemyList                          destroyed_;

  BulletList* bullets_;
};

#endif  // SPACETYPER_ENEMIES_H
