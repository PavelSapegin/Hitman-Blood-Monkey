#include "rogue/EventSystem.h"

#include <algorithm>
#include <cstdlib>

namespace rogue {

const std::vector<std::string> EventSystem::commentatorLines = {
  "Nice shot! ...wait, that was an ally.",
  "You call THAT a dodge?",
  "My grandmother fights better than you.",
  "BOSS FIGHT! ...just kidding, it's a regular rat.",
  "Wave " + std::to_string(999) + "? You're screwed.",
  "Have you tried pressing buttons randomly?",
  "LEEEEROOOOY JEEENKIIINS!",
  "That monster has a family, you know.",
  "Critical hit! ...for the enemy.",
  "You're doing great! ...relatively speaking.",
  "Is that a new weapon or did you just drop lunch?",
  "50% of players die before this point. Congrats!",
  "The gods are watching... and laughing.",
  "This would be a great time to panic.",
  "I've seen better moves in a nursing home.",
  "Did you just STEP on that monster?",
  "THE BEES! NOT THE BEES!",
  "Omae wa mou shindeiru.",
  "NANI?!",
  "GET GOOD SCRUB.",
  "Is that a mimic? Yes. Yes it is.",
  "Your mother was a hamster and your father smelt of elderberries!",
  "PRESS ANY KEY... oh wait, you already did.",
  "Insert coin to continue... oh wait, it's free.",
  "The hunter becomes the hunted! ...or not.",
  "Floor " + std::to_string(50) + "? In THIS economy?",
};

EventSystem::EventSystem() {}

void EventSystem::update(float dt, int /*waveNumber*/, int /*floorNumber*/, float timeOnFloor,
                         float /*playerHp*/, float /*playerMaxHp*/) {
  this->timeOnFloor += dt;

  // Risk timer
  if (eventCooldown > 0.0f)
    eventCooldown -= dt;

  // Random events (every 15-30 seconds if no event active)
  if (!isEventActive() && eventCooldown <= 0.0f) {
    float chance = dt * 0.08f * (1.0f + timeOnFloor / 60.0f);
    if ((float)rand() / (float)RAND_MAX < chance) {
      int r2 = rand() % 100;
      if (r2 < 18)
        currentEvent.type = DynamicEvent::MeteorRain;
      else if (r2 < 33)
        currentEvent.type = DynamicEvent::MonsterInvasion;
      else if (r2 < 46)
        currentEvent.type = DynamicEvent::Eclipse;
      else if (r2 < 58)
        currentEvent.type = DynamicEvent::Blackout;
      else if (r2 < 68)
        currentEvent.type = DynamicEvent::GoldRush;
      else if (r2 < 78)
        currentEvent.type = DynamicEvent::ZombieHorde;
      else if (r2 < 89)
        currentEvent.type = DynamicEvent::Earthquake;
      else
        currentEvent.type = DynamicEvent::TimeWarp;
      currentEvent.timer = currentEvent.duration;
      currentEvent.warningTimer = 2.0f;
      eventCooldown = 15.0f;
      maybeComment("EVENT INCOMING!", 1.0f);
    }
  }

  if (isEventActive()) {
    currentEvent.warningTimer -= dt;
    if (currentEvent.warningTimer <= 0.0f) {
      currentEvent.timer -= dt;
      // Earthquake: continuous shake
      if (currentEvent.type == DynamicEvent::Earthquake) {
        float chance = dt * 5.0f;
        if ((float)rand() / (float)RAND_MAX < chance) {
          maybeComment("EARTHQUAKE!", 0.8f);
        }
      }
      if (currentEvent.timer <= 0.0f) {
        currentEvent.type = DynamicEvent::None;
      }
    }
  }

  // Commentator
  if (commentCooldown > 0.0f)
    commentCooldown -= dt;
  updateComments(dt);
}

void EventSystem::forceEvent(DynamicEvent type) {
  currentEvent.type = type;
  currentEvent.timer = currentEvent.duration;
  currentEvent.warningTimer = 2.0f;
  eventCooldown = 20.0f;
}

DynamicEvent EventSystem::randomEvent() {
  int r = rand() % 8;
  switch (r) {
    case 0:
      return DynamicEvent::Eclipse;
    case 1:
      return DynamicEvent::MeteorRain;
    case 2:
      return DynamicEvent::MonsterInvasion;
    case 3:
      return DynamicEvent::Blackout;
    case 4:
      return DynamicEvent::GoldRush;
    case 5:
      return DynamicEvent::ZombieHorde;
    case 6:
      return DynamicEvent::Earthquake;
    default:
      return DynamicEvent::TimeWarp;
  }
}

void EventSystem::triggerComment(const std::string &text) {
  CommentatorLine cl;
  cl.text = text;
  cl.timer = cl.duration;
  comments.push_back(cl);
  if (comments.size() > 5)
    comments.erase(comments.begin());
}

void EventSystem::updateComments(float dt) {
  for (auto &c : comments)
    c.timer -= dt;
  comments.erase(std::remove_if(comments.begin(), comments.end(),
                                [](const CommentatorLine &c) { return c.timer <= 0; }),
                 comments.end());
}

void EventSystem::maybeComment(const std::string & /*text*/, float chance) {
  if (commentCooldown > 0.0f)
    return;
  if ((float)rand() / (float)RAND_MAX < chance) {
    if (!commentatorLines.empty()) {
      int idx = rand() % commentatorLines.size();
      triggerComment(commentatorLines[idx]);
    }
    commentCooldown = 4.0f;
  }
}

float EventSystem::getRiskMultiplier() const {
  float t = timeOnFloor;
  if (t < 30.0f)
    return 1.0f;
  if (t < 60.0f)
    return 1.2f;
  if (t < 90.0f)
    return 1.5f;
  if (t < 120.0f)
    return 2.0f;
  return 3.0f;
}

int EventSystem::getBonusXp() const {
  float t = timeOnFloor;
  if (t < 30.0f)
    return 0;
  if (t < 60.0f)
    return 1;
  if (t < 90.0f)
    return 2;
  if (t < 120.0f)
    return 3;
  return 5;
}

}  // namespace rogue
