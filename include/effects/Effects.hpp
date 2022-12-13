#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include <string>

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
#endif