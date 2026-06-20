#pragma once
#include <raylib.h>

#include <string>
#include <unordered_map>

namespace rogue {

class TextureManager {
public:
  static TextureManager &getInstance() {
    static TextureManager instance;
    return instance;
  }

  void loadTexture(const std::string &name, const std::string &path) {
    textures[name] = LoadTexture(path.c_str());
  }

  Texture2D &get(const std::string &name) { return textures.at(name); }

  void unloadAll() {
    for (auto &[name, tex] : textures) {
      UnloadTexture(tex);
    }
    textures.clear();
  }

private:
  TextureManager() = default;
  std::unordered_map<std::string, Texture2D> textures;
};

}  // namespace rogue
