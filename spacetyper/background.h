#ifndef SPACETYPER_BACKGROUND_H
#define SPACETYPER_BACKGROUND_H

#include <memory>
#include <vector>

#include "render/sprite.h"
#include "render/texture.h"

class SpriteRenderer;

class Background {
public:
  Background(int count, int width, int height,
             std::shared_ptr<Texture2d> texture, float speed, Layer *layer);

  void Update(float delta);

private:
  float width_;
  float height_;
  float speed_;
  std::vector<Sprite> positions_;
};

#endif // SPACETYPER_BACKGROUND_H
