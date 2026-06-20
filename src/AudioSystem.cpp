#include "rogue/AudioSystem.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace rogue {

AudioSystem::AudioSystem() {}

AudioSystem::~AudioSystem() {
  destroySound(shootSound);
  destroySound(hitSound);
  destroySound(pickupSound);
  destroySound(zaWarudoSound);
  destroySound(explosionSound);
  destroySound(deathSound);
  destroySound(dashSound);
  destroySound(levelUpSound);
  destroySound(waveStartSound);
}

void AudioSystem::destroySound(Sound &s) {
  if (s.frameCount > 0)
    UnloadSound(s);
}

Sound AudioSystem::makeSine(float freq, float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0)
    samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float envelope = 1.0f;
    float fade = 0.005f;
    if (t < fade)
      envelope = t / fade;
    if (t > duration - fade)
      envelope = (duration - t) / fade;
    data[i] = (short)(amp * envelope * 32767.0f * std::sin(2.0f * PI * freq * t));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

Sound AudioSystem::makeNoise(float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0)
    samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float envelope = 1.0f;
    float fade = 0.003f;
    if (t < fade)
      envelope = t / fade;
    if (t > duration - fade)
      envelope = (duration - t) / fade;
    data[i] =
      (short)(amp * envelope * 32767.0f * ((float)std::rand() / (float)RAND_MAX * 2.0f - 1.0f));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

Sound AudioSystem::makeSweep(float freqStart, float freqEnd, float duration, float amp) {
  unsigned int sr = 44100;
  unsigned int samples = (unsigned int)(sr * duration);
  if (samples == 0)
    samples = 1;
  short *data = new short[samples];
  for (unsigned int i = 0; i < samples; ++i) {
    float t = (float)i / sr;
    float frac = t / duration;
    float freq = freqStart + (freqEnd - freqStart) * frac;
    float envelope = 1.0f;
    float fade = 0.005f;
    if (t < fade)
      envelope = t / fade;
    if (t > duration - fade)
      envelope = (duration - t) / fade;
    float phase = 2.0f * PI * freq * t;
    data[i] = (short)(amp * envelope * 32767.0f * std::sin(phase));
  }
  Wave wave;
  wave.data = data;
  wave.frameCount = samples;
  wave.sampleRate = sr;
  wave.sampleSize = 16;
  wave.channels = 1;
  Sound s = LoadSoundFromWave(wave);
  delete[] data;
  return s;
}

void AudioSystem::init() {
  shootSound = makeNoise(0.06f, 0.4f);
  hitSound = makeSine(120, 0.15f, 0.6f);
  pickupSound = makeSweep(400, 900, 0.2f, 0.4f);
  zaWarudoSound = makeSweep(80, 30, 0.6f, 0.7f);
  explosionSound = makeNoise(0.35f, 0.6f);
  deathSound = makeSweep(500, 80, 0.5f, 0.5f);
  dashSound = makeSweep(300, 600, 0.12f, 0.3f);
  levelUpSound = makeSweep(400, 1200, 0.3f, 0.4f);
  waveStartSound = makeSweep(200, 800, 0.4f, 0.4f);
}

void AudioSystem::playShoot() {
  SetSoundVolume(shootSound, volume);
  PlaySound(shootSound);
}
void AudioSystem::playHit() {
  SetSoundVolume(hitSound, volume);
  PlaySound(hitSound);
}
void AudioSystem::playPickup() {
  SetSoundVolume(pickupSound, volume);
  PlaySound(pickupSound);
}
void AudioSystem::playZaWarudo() {
  SetSoundVolume(zaWarudoSound, volume * 1.2f);
  PlaySound(zaWarudoSound);
}
void AudioSystem::playExplosion() {
  SetSoundVolume(explosionSound, volume);
  PlaySound(explosionSound);
}
void AudioSystem::playDeath() {
  SetSoundVolume(deathSound, volume);
  PlaySound(deathSound);
}
void AudioSystem::playDash() {
  SetSoundVolume(dashSound, volume);
  PlaySound(dashSound);
}
void AudioSystem::playLevelUp() {
  SetSoundVolume(levelUpSound, volume);
  PlaySound(levelUpSound);
}
void AudioSystem::playWaveStart() {
  SetSoundVolume(waveStartSound, volume * 1.2f);
  PlaySound(waveStartSound);
}

}  // namespace rogue
