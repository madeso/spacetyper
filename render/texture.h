#ifndef TEXTURE_H
#define TEXTURE_H

#include "render/gl.h"
#include "render/image.h"
#include "core/noncopyable.h"

struct Texture2dLoadData {
  Texture2dLoadData();

  GLuint wrapS;
  GLuint wrapT;
  GLuint filterMin;
  GLuint filterMax;
};

class TextureId : Noncopyable {
 public:
  TextureId();
  ~TextureId();

  bool IsCurrentlyBound() const;
  GLuint id() const;

 private:
  GLuint id_;
};

void Use(const TextureId* texture);

class Texture2d : public TextureId {
 public:
  Texture2d();
  explicit Texture2d(const std::string& path);
  Texture2d(const std::string& path, AlphaLoad alpha,
            const Texture2dLoadData& data);

  void Load(int width, int height, unsigned char* pixelData,
            GLuint internalFormat, GLuint imageFormat,
            const Texture2dLoadData& data);
  void LoadFromFile(const std::string& path, AlphaLoad alpha,
                    const Texture2dLoadData& data);

  int width() const;
  int height() const;

 private:
  int width_, height_;
};

#endif
