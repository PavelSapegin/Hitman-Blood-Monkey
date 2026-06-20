#pragma once
#include <raylib.h>

namespace rogue {

class AudioSystem {
public:
  AudioSystem();
  ~AudioSystem();
  void init();
  void playShoot();
  void playHit();
  void playPickup();
  void playZaWarudo();
  void playExplosion();
  void playDeath();
  void playDash();
  void playLevelUp();
  void playWaveStart();
  void setVolume(float vol) { volume = vol; }

private:
  Sound shootSound;
  Sound hitSound;
  Sound pickupSound;
  Sound zaWarudoSound;
  Sound explosionSound;
  Sound deathSound;
  Sound dashSound;
  Sound levelUpSound;
  Sound waveStartSound;
  float volume = 0.4f;

  Sound makeSine(float freq, float duration, float amp = 0.5f);
  Sound makeNoise(float duration, float amp = 0.3f);
  Sound makeSweep(float freqStart, float freqEnd, float duration, float amp = 0.5f);
  void destroySound(Sound &s);
};

}  // namespace rogue
