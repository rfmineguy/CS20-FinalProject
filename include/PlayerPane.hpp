#ifndef PLAYER_PANE_HPP
#define PLAYER_PANE_HPP

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"

#include "../include/CirclularQueue.hpp"
#include "../include/ClosedHashTable.hpp"
#include "../include/DoubleLinkedStack.hpp"
#include "../include/SortedList.hpp"
#include "../include/effects/Effects.hpp"

#include <random>

using namespace ftxui;

//============================================================================
// Holds information about a specific card type and what it should do
//============================================================================
class CardInfo {
  public:
    CardInfo(): CardInfo(nullptr, 0) {}
    CardInfo(std::shared_ptr<PlayerEffect> effect, int _duration): effect(effect), durationInTurns(_duration) {}
    friend std::ostream& operator<<(std::ostream& os, const CardInfo& ci) {
      os << "CardInfo : " << ci.name << std::endl;
      return os;
    }

    std::shared_ptr<PlayerEffect> GetEffect() {
      return effect;
    }

  private:
    std::string name;
    int durationInTurns;
    std::shared_ptr<PlayerEffect> effect;
};

struct PlayerStats {
  int health, armor, mana;
  PlayerStats(): PlayerStats(0, 0, 0) {}
  PlayerStats(int _h, int _a, int _m): health(_h), armor(_a), mana(_m) {}
};

enum class GameResult : int {
  LOWEST = -999999, WIN = 5, LOSS = 6, HIGHEST = 999999
};

std::ostream& operator<<(std::ostream& os, const GameResult& gr);

// =====================
#define CARD_TYPE_AMOUNT 4
#define BUTTON_COUNT 6

class PlayerPane {
public:
  PlayerPane(bool);
  ~PlayerPane();

// These functions return FTXUI components for rendering
public:
  /* Create an ftxui component to store the visuals for player effects */
  Component MakeEffectsContainer();
  Component MakeStatsContainer();
  Component MakeCardGrid();
  Component MakeCardQueue();
  Component MakeWinsLossesSortedList();

public:
  bool IsActiveTurn() const;
  bool IsDead() const;
  void ToggleTurn();
  void PerformActions(PlayerPane&);
  
public:
  void Reset();

// NOTE: Should these be public?
public:
  // returns a mutable reference
  CircularQueue<std::string>& GetCardSelectionQueue();
  DoubleLinkedStack<std::shared_ptr<PlayerEffect>>& GetPlayerEffectsStack();
  SortedList<GameResult>& GetWinsLossesList();
  const ClosedHashTable<std::string, CardInfo>& GetCardInfoHT();

private:
  void RandomizeCardsInInventory();
  
private:
  std::string cardTypes[CARD_TYPE_AMOUNT];
  std::vector<std::string> cards;
  ClosedHashTable<std::string, CardInfo> card_ht;
  CircularQueue<std::string> card_selection;
  SortedList<GameResult> wins_losses;
  DoubleLinkedStack<std::shared_ptr<PlayerEffect>> player_effects;
  std::function<void(int)> card_select_callback;
  PlayerStats stats;
  bool pressed[BUTTON_COUNT];
  bool isActiveTurn;
};

#endif