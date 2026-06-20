#include "rogue/MetaProgression.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace rogue {

MetaProgression::MetaProgression() {
  filePath = "hitman_save.txt";
  load();
}

void MetaProgression::load() {
  std::ifstream f(filePath);
  if (!f.is_open())
    return;
  std::string line;
  while (std::getline(f, line)) {
    if (line.find("UNLOCK:") == 0) {
      unlockedItems.push_back(line.substr(7));
    } else if (line.find("RUNS:") == 0) {
      unlockData.totalRuns = std::stoi(line.substr(5));
    } else if (line.find("KILLS:") == 0) {
      unlockData.totalKills = std::stoi(line.substr(6));
    } else if (line.find("WAVE:") == 0) {
      unlockData.highestWave = std::stoi(line.substr(5));
    } else if (line.find("FLOOR:") == 0) {
      unlockData.highestFloor = std::stoi(line.substr(6));
    }
  }
}

void MetaProgression::save() {
  std::ofstream f(filePath);
  if (!f.is_open())
    return;
  f << "RUNS:" << unlockData.totalRuns << "\n";
  f << "KILLS:" << unlockData.totalKills << "\n";
  f << "WAVE:" << unlockData.highestWave << "\n";
  f << "FLOOR:" << unlockData.highestFloor << "\n";
  for (auto &item : unlockedItems) {
    f << "UNLOCK:" << item << "\n";
  }
}

void MetaProgression::updateHighScore(int wave, int floor) {
  if (wave > unlockData.highestWave)
    unlockData.highestWave = wave;
  if (floor > unlockData.highestFloor)
    unlockData.highestFloor = floor;
  save();
}

bool MetaProgression::isUnlocked(const std::string &key) const {
  return std::find(unlockedItems.begin(), unlockedItems.end(), key) != unlockedItems.end();
}

void MetaProgression::unlock(const std::string &key) {
  if (!isUnlocked(key)) {
    unlockedItems.push_back(key);
    save();
  }
}

}  // namespace rogue
