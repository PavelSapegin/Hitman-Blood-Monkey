#pragma once
#include <raylib.h>

#include <string>
#include <vector>

namespace rogue {

enum class DynamicEvent {
  None,
  Eclipse,          // dim light, all monsters get +50% damage
  MeteorRain,       // random fireballs fall from sky
  MonsterInvasion,  // extra wave of monsters spawns
  Blackout,         // vision radius shrinks to 5 tiles
  GoldRush,         // all drops are doubled
  ZombieHorde,      // killed monsters revive once
  Earthquake,       // screen shakes, walls crack
  TimeWarp          // everything slows down 50% for player too
};

struct ActiveEvent {
  DynamicEvent type = DynamicEvent::None;
  float timer = 0.0f;
  float duration = 8.0f;
  float warningTimer = 2.0f;
};

struct CommentatorLine {
  std::string text;
  float timer = 0.0f;
  float duration = 3.0f;
};

class EventSystem {
public:
  EventSystem();

  void update(float dt, int waveNumber, int floorNumber, float timeOnFloor, float playerHp,
              float playerMaxHp);
  void forceEvent(DynamicEvent type);
  DynamicEvent getCurrentEvent() const { return currentEvent.type; }
  float getEventTimer() const { return currentEvent.timer; }
  float getEventDuration() const { return currentEvent.duration; }
  float getEventWarning() const { return currentEvent.warningTimer; }
  bool isEventActive() const {
    return currentEvent.type != DynamicEvent::None && currentEvent.timer > 0.0f;
  }

  // Commentator
  void triggerComment(const std::string &text);
  void updateComments(float dt);
  const std::vector<CommentatorLine> &getComments() const { return comments; }
  void maybeComment(const std::string &text, float chance = 0.3f);

  // Risk system
  float getTimeOnFloor() const { return timeOnFloor; }
  float getRiskMultiplier() const;
  int getBonusXp() const;

  static DynamicEvent randomEvent();

private:
  ActiveEvent currentEvent;
  float eventCooldown = 0.0f;
  float timeOnFloor = 0.0f;
  std::vector<CommentatorLine> comments;
  float commentCooldown = 0.0f;

  static const std::vector<std::string> commentatorLines;
};

}  // namespace rogue
