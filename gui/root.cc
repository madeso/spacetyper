#include "gui/root.h"
#include "gui/load.h"

Root::Root(const Sizef& size) : size_(size) {

}

bool Root::Load(Font* font, const std::string& path, TextureCache* cache, TextBackgroundRenderer* br) {
  const bool result = ::Load(&state_, font, &container_, path, cache, br);
  if( result ) {
    container_.DoLayout(Rectf::FromWidthHeight(size_));
  }
  return result;
}

void Root::SetInputMouse(const vec2f& pos, bool down) {
  state_.mouse = pos;
  state_.mouse_down = down;
}

void Root::Step() {
  state_.Begin();
  container_.Step();
  state_.End();
}

void Root::Render(SpriteRenderer* sp) const {
  container_.Render(sp);
}
