
#ifndef TEXTURE_H
#define TEXTURE_H

#include "spacetyper/gl.h"
#include "spacetyper/image.h"

struct Texture2dLoadData {
  Texture2dLoadData();

  GLuint internalFormat;
  GLuint imageFormat;

  GLuint wrapS;
  GLuint wrapT;
  GLuint filterMin;
  GLuint filterMax;
};

class TextureId {
 public:
  TextureId();
  ~TextureId();

  void Bind() const;

 private:
  GLuint id_;
};

class Texture2d : public TextureId {
 public:
  Texture2d();
  explicit Texture2d(const std::string& path);
  Texture2d(const std::string& path, AlphaLoad alpha,
            const Texture2dLoadData& data);
  ~Texture2d();

  void Load(int width, int height, unsigned char* pixelData,
            GLuint internalFormat, GLuint imageFormat,
            const Texture2dLoadData& data);
  void LoadFromFile(const std::string& path, AlphaLoad alpha,
                    const Texture2dLoadData& data);

 private:
  int width_, height_;
};

#endif
