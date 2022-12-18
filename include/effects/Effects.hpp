#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include <string>
#include "../util/RandomUtil.hpp"

//============================================================================
// Base effect class
//============================================================================
class PlayerEffect {
  public:
    PlayerEffect(): PlayerEffect("NULL") {}
    PlayerEffect(const std::string& _name): name(_name) {}
    virtual ~PlayerEffect() = default;  
    std::string GetName() const { return name; }  
  
  private:
    std::string name;
};

//============================================================================
// Posion effect specialty class
//============================================================================
class PoisonEffect : public PlayerEffect {
public:
  PoisonEffect(): PoisonEffect(0) {}
  PoisonEffect(int poison_damage): PlayerEffect("Poison"), poisonDamage(poison_damage) {}
  int GetPoisonDamage() const { return poisonDamage; }

private:
  int poisonDamage;
};

//============================================================================
// Healing effect specialty class
//============================================================================
class HealingEffect : public PlayerEffect {
public:
  HealingEffect(): PlayerEffect("Healing") { srand(time(0)); }
  int GetHealAmount() const { return rand() % 10; }
};

//============================================================================
// Shield effect specialty class
//============================================================================
class ShieldEffect : public PlayerEffect {
public:
  ShieldEffect(): ShieldEffect(0) {}
  ShieldEffect(int _shieldAmount): PlayerEffect("Shield"), shieldAmount(_shieldAmount) {};
  int GetShieldAmount() const { return shieldAmount; }
  
private:
  int shieldAmount;
};

//============================================================================
// Shield effect specialty class
//============================================================================
class SwordEffect : public PlayerEffect {
public:
  SwordEffect(): SwordEffect(0) {
    RandomInt r;
    swordDamage = r.GetRandomInt(0, 3);
  }
  SwordEffect(int _swordDamage): PlayerEffect("Sword"), swordDamage(_swordDamage) {};
  int GetSwordAmount() const { return swordDamage; }
  
private:
  int swordDamage;
};
#endif