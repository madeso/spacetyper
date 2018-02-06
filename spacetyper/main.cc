#include <SDL2/SDL.h>
#include <iostream>

#include "render/shader.h"
#include "render/spriterender.h"
#include "spacetyper/spritefader.h"

#include "core/interpolate.h"
#include "core/os.h"

#include "core/filesystemimagegenerator.h"
#include "core/filesystemdefaultshaders.h"

#include "render/debuggl.h"
#include "render/fonts.h"
#include "render/init.h"
#include "render/fontcache.h"
#include "render/scalablesprite.h"
#include "render/shaderattribute2d.h"
#include "render/texturecache.h"
#include "render/viewport.h"
#include "spacetyper/background.h"
#include "spacetyper/bulletlist.h"
#include "spacetyper/dictionary.h"
#include "spacetyper/enemies.h"
#include "spacetyper/enemyword.h"

#include "gui/root.h"

int
main(int argc, char** argv)
{
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0)
  {
    std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  int width  = 800;
  int height = 600;

  SDL_Window* window = SDL_CreateWindow(
      "Space Typer",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      width,
      height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if(window == NULL)
  {
    std::cerr << "Failed to create window " << SDL_GetError() << "\n";
    return -1;
  }

  SDL_GL_CreateContext(window);
  Init init{SDL_GL_GetProcAddress, Init::BlendHack::EnableHack};

  if(init.ok == false)
  {
    return -4;
  }

  SetupOpenglDebug();

  const auto current_directory = GetCurrentDirectory();
  FileSystem file_system;
  file_system.SetWrite(
      std::make_shared<FileSystemWriteFolder>(current_directory));
  FileSystemRootFolder::AddRoot(&file_system, current_directory);
  FileSystemImageGenerator::AddRoot(&file_system, "img-plain");
  FileSystemDefaultShaders::AddRoot(&file_system, "shaders");

  TextureCache cache{&file_system};
  Shader       shader;
  attributes2d::PrebindShader(&shader);
  shader.Load(&file_system, "shaders/sprite");
  FontCache font_cache{&file_system, &cache};
  auto      font = font_cache.GetFont("gamefont.json");
  // (cache.GetTexture("metalPanel_blueCorner.png"), 62, 14, 33, 14, vec2f(240,
  // 240));
  ScalableSprite target(
      "crosshair.png", Sizef::FromWidthHeight(100, 100), &cache);
  SpriteRenderer renderer(&shader);

  Dictionary dictionary;

  Layer      background(&renderer);
  Layer      objects(&renderer);
  Layer      foreground(&renderer);
  Background smallStars(
      25, width, height, cache.GetTexture("starSmall.png"), 20, &background);
  Background bigStars(
      15, width, height, cache.GetTexture("starBig.png"), 50, &background);

  SpriteFader fader(&foreground);

  // smoke effects
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_008.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_009.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_010.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_011.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_012.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_013.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_014.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_015.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/spaceEffects_016.png"));

  // "laser"/explosion effects
  fader.RegisterTexture(cache.GetTexture("explosion/laserBlue08.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/laserBlue10.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/laserGreen14.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/laserGreen16.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/laserRed08.png"));
  fader.RegisterTexture(cache.GetTexture("explosion/laserRed10.png"));

  Sprite player(cache.GetTexture("player.png"));
  objects.Add(&player);

  vec2f shipPos(width / 2, player.GetHeight() / 2 + 10);
  player.SetPosition(shipPos);

  const mat4f projection = init.GetOrthoProjection(width, height);
  Use(&shader);
  shader.SetUniform(shader.GetUniform("image"), 0);
  shader.SetUniform(shader.GetUniform("projection"), projection);

  Viewport viewport{
      Recti::FromWidthHeight(width, height).SetBottomLeftToCopy(0, 0)};
  viewport.Activate();

  Root       gui(Sizef::FromWidthHeight(width, height));
  const bool gui_loaded =
      gui.Load(&file_system, &font_cache, "gui.json", &cache);

  if(gui_loaded == false)
  {
    std::cerr << "Failed to load gui\n";
  }

  Uint64 NOW  = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;

  SDL_StartTextInput();

  BulletList bullets(&objects);
  Enemies    enemies(
      &fader,
      &cache,
      font.get(),
      &objects,
      &dictionary,
      width,
      height,
      &bullets);
  EnemyWord* current_word = nullptr;

  enemies.SpawnEnemies(5);

  Interpolate<Angle, AngleTransform> player_rotation(Angle::Zero());
  const float ROTATION_TIME = 0.5f;
  const float SCALE_TIME    = 0.6f;

  FloatInterpolate target_scale(1.0f);

  bool gui_running = gui_loaded;
  bool running     = true;

  int window_mouse_x = 0;
  int window_mouse_y = 0;
  SDL_GetMouseState(&window_mouse_x, &window_mouse_y);
  bool mouse_lmb_down = false;

  while(running)
  {
    LAST           = NOW;
    NOW            = SDL_GetPerformanceCounter();
    const float dt = (NOW - LAST) * 1.0f / SDL_GetPerformanceFrequency();
    SDL_Event   e;

    while(SDL_PollEvent(&e) != 0)
    {
      if(e.type == SDL_QUIT)
      {
        running = false;
      }
      else if(e.type == SDL_MOUSEMOTION)
      {
        window_mouse_x = e.motion.x;
        window_mouse_y = e.motion.y;
      }
      else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
      {
        const bool down = e.type == SDL_MOUSEBUTTONDOWN;
        window_mouse_x  = e.button.x;
        window_mouse_y  = e.button.y;
        if(e.button.button == SDL_BUTTON_LEFT)
        {
          mouse_lmb_down = down;
        }
      }
      else if(e.type == SDL_TEXTINPUT)
      {
        const std::string& input = e.text.text;
        if(gui_running)
        {
        }
        else
        {
          if(current_word == nullptr)
          {
            current_word = enemies.DetectWord(input);
            if(current_word != nullptr)
            {
              const Angle target_rotation =
                  enemies.FireAt(shipPos, current_word);
              player_rotation.Clear().BackOut(target_rotation, ROTATION_TIME);
              target_scale.SetValue(19.0f).Clear().CircOut(1.0f, SCALE_TIME);
            }
          }
          else
          {
            const bool hit = current_word->Type(input);
            if(hit)
            {
              const Angle target_rotation =
                  enemies.FireAt(shipPos, current_word);
              player_rotation.Clear().BackOut(target_rotation, ROTATION_TIME);
            }
            if(current_word->IsAlive() == false)
            {
              enemies.Remove(current_word);
              current_word               = nullptr;
              const auto target_rotation = Angle::Zero();
              player_rotation.Clear().BackOut(target_rotation, ROTATION_TIME);
            }
          }
        }
      }
    }

    if(gui_running)
    {
      // Transform mouse position to the the euphoria coordinate system
      const vec2f mouse_position{static_cast<float>(window_mouse_x),
                                 static_cast<float>(height - window_mouse_y)};
      gui.SetInputMouse(mouse_position, mouse_lmb_down);
      gui.Step(dt);
    }
    else
    {
      smallStars.Update(dt);
      bigStars.Update(dt);
      enemies.Update(dt);
      bullets.Update(dt);
      fader.Update(dt);
      player_rotation.Update(dt);
      target_scale.Update(dt);
      player.rotation = player_rotation;
    }

    /*
    // uncomment for infinite enemies!!
    if(enemies.EnemyCount() == 0 ) {
      enemies.SpawnEnemies(5);
    }
    */

    init.ClearScreen(Color::DarkslateGray);

    background.Render();
    objects.Render();
    enemies.Render(&renderer);
    foreground.Render();

    if(current_word != nullptr)
    {
      const Sizef extra_size     = Sizef::FromWidthHeight(40, 40);
      const Sizef size           = current_word->GetSize();
      const Sizef size_and_extra = size + extra_size;
      const Sizef scaled_size    = size_and_extra * target_scale.GetValue();
      renderer.DrawNinepatch(
          target,
          Rectf::FromPositionAnchorWidthAndHeight(
              current_word->GetPosition(),
              vec2f{0.5f, 0.5f},
              scaled_size.GetWidth(),
              scaled_size.GetHeight()),
          Rgba{Color::White});
    }

    if(gui_running)
    {
      gui.Render(&renderer);
    }

    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
