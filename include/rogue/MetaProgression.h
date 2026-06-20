#pragma once
#include <string>
#include <vector>

namespace rogue {

struct UnlockData {
  bool hasGodSpeed = false;
  bool hasGodFire = false;
  bool hasGodChaos = false;
  bool hasGodHealth = false;
  bool hasGodGreed = false;
  bool hasGodLuck = false;

  bool hasExtraArm = false;
  bool hasLaserEyes = false;
  bool hasWallJump = false;
  bool hasThornSkin = false;
  bool hasVampire = false;
  bool hasBerserker = false;

  int totalRuns = 0;
  int totalKills = 0;
  int highestWave = 0;
  int highestFloor = 0;
};

class MetaProgression {
public:
  MetaProgression();

  void load();
  void save();

  UnlockData &data() { return unlockData; }
  const UnlockData &data() const { return unlockData; }

  void addRun() { unlockData.totalRuns++; }
  void addKill() { unlockData.totalKills++; }
  void updateHighScore(int wave, int floor);

  bool isUnlocked(const std::string &key) const;
  void unlock(const std::string &key);

private:
  UnlockData unlockData;
  std::string filePath;

  std::vector<std::string> unlockedItems;
};

}  // namespace rogue
