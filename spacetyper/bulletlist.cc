#include "spacetyper/bulletlist.h"

#include <algorithm>

#include "render/sprite.h"
#include "spacetyper/enemyword.h"

#include "core/vec2.h"

// todo: improve resolution
const float PI = 3.14;

BulletType::~BulletType()
{
}

BulletList::BulletList(Layer* layer)
    : layer_(layer)
{
}

Angle
GetAngle(const vec2f& dn)
{
  float aa = dot(vec2f(0.0f, 1.0f), dn);
  Angle a  = Acos(aa);
  if(dn.x > 0)
    a = -a;
  return a;
}

Angle
BulletList::Add(EnemyWord* word, std::shared_ptr<Texture2d> t, const vec2f& pos)
{
  BulletType b;
  b.word = word;
  b.sprite.reset(new Sprite(t, pos));
  layer_->Add(b.sprite.get());
  bullets_.push_back(b);

  // todo: move to core
  const vec2f& p  = pos;
  const vec2f& w  = word->GetPosition();
  const vec2f  d  = w - p;
  const vec2f  dn = d.GetNormalized();

  return GetAngle(dn);
}

void
BulletList::Update(float dt)
{
  const float speed = 1000.0f;
  for(BulletType& b : bullets_)
  {
    const vec2f& p = b.sprite->GetPosition();
    const vec2f& w = b.word->GetPosition();

    const vec2f d = w - p;
    if(d.GetLength() < speed * dt)
    {
      b.word->Damage();
      b.word = nullptr;
      layer_->Remove(b.sprite.get());
    }
    else
    {
      const vec2f dn = d.GetNormalized();
      b.sprite->SetPosition(p + dn * speed * dt);
      b.sprite->rotation = GetAngle(dn);
    }
  }

  bullets_.erase(
      std::remove_if(
          bullets_.begin(),
          bullets_.end(),
          [](const BulletType& b) { return b.word == nullptr; }),
      bullets_.end());
}
