#include "../include/PlayerPane.hpp"
#include "../include/util/RandomUtil.hpp"
#include <string.h>

// Attempt at an ostream operator for GameResult
std::ostream& operator<<(std::ostream& os, const GameResult& gr) {
  switch (gr) {
  case GameResult::LOWEST: os << "Lowest (N/A)"; break;
  case GameResult::WIN: os << "Win"; break;
  case GameResult::LOSS: os << "Loss"; break;
  case GameResult::HIGHEST: os << "Highest"; break;
  default: os << "Unimplemented"; break;
  }
  return os;
}

PlayerPane::PlayerPane(bool isTurn)
:card_ht("tombstone", "empty"), card_selection(4), wins_losses([](GameResult a, GameResult b) { return a <= b; }, GameResult::LOWEST, GameResult::HIGHEST), player_effects(), pressed(), isActiveTurn(isTurn), stats(10, 0, 5) {
  cardTypes[0] = "Poison";
  cardTypes[1] = "Health";
  cardTypes[2] = "Shield";
  cardTypes[3] = "Sword";

  // Insert the various known card types
  card_ht.Insert(cardTypes[0], CardInfo(std::make_shared<PoisonEffect>(), 2));
  card_ht.Insert(cardTypes[1], CardInfo(std::make_shared<HealingEffect>(), 2));
  card_ht.Insert(cardTypes[2], CardInfo(std::make_shared<ShieldEffect>(2), 1));
  card_ht.Insert(cardTypes[3], CardInfo(nullptr, 1));
  
  // randomize the cards that start in your deck
  RandomizeCardsInInventory();
  
  // callback that decides what happens when a card is pressed
  card_select_callback = [&](int index) {
    if (index < 0 || index >= BUTTON_COUNT) {
      return;    // just do nothing (may never happen but its okay to have this anyways)
    }
    if (pressed[index] || !isActiveTurn)
      return;
    try {
      card_selection.Enqueue(cards.at(index));
      pressed[index] = true;
    } catch (std::string e) {
      std::cerr << "Failed to enqueue [" << cards.at(index) << "]" << std::endl; 
    }
  };
}

PlayerPane::~PlayerPane() {
  // no dynamic memory to deal with
  //  RAII takes care of everything in this class
}

Component PlayerPane::MakeEffectsContainer() {
  return Container::Horizontal({
    Renderer([&]{
      std::vector<Element> effectVec;
      int size = player_effects.Size();
      while (size > 0) {
        std::shared_ptr<PlayerEffect> effect = player_effects.Peek();
        player_effects.Pop();
        size--;
        
        #define SHIELD_CAST(effect) std::dynamic_pointer_cast<ShieldEffect>(effect)
        #define HEALTH_CAST(effect) std::dynamic_pointer_cast<HealingEffect>(effect)
        #define POISON_CAST(effect) std::dynamic_pointer_cast<PoisonEffect>(effect)
        if (HEALTH_CAST(effect) == nullptr) {
          if (SHIELD_CAST(effect) != nullptr) {
            effectVec.push_back(hbox({text(SHIELD_CAST(effect).get()->GetName())}));
          }
          else if (POISON_CAST(effect) != nullptr) {
            effectVec.push_back(hbox({text(POISON_CAST(effect).get()->GetName())}));
          }
        }
        player_effects.Push(effect);
      }
      return hbox({
        text("[Effects]  ") | color(Color::Cyan),
        vbox({effectVec})
      });
    })
  });
}

Component PlayerPane::MakeStatsContainer() {
  return Container::Horizontal({
    Renderer([&]{
      // Data for turn indicator
      Color turnIndicatorColor = isActiveTurn ? Color::Green : Color::Red;;
      std::string turnIndicatorString = isActiveTurn ? "It is your turn" : "It is not your turn";

      // Data for stats
      Element s = hbox({
        text("[Stats]  ") | color(Color::Cyan),
        text("Health ♥ " + std::to_string(stats.health) + " ") | color(Color::Pink1),
        text("Armor Θ " + std::to_string(stats.armor) + " ") | color(Color::Yellow),
        text("Mana ┼ " + std::to_string(stats.mana) + " ") | color(Color::Aquamarine1)
      });
    
      return vbox({
        hbox({
          text("[Turn]   ") | color(Color::Cyan),
          text(turnIndicatorString) | color(turnIndicatorColor),
        }),
        separator(),
        s
      });
    })
  });
}

Component PlayerPane::MakeCardGrid() {
  auto card_grid = Container::Vertical({});
  auto row1 = Container::Horizontal({
    Button(cards.at(0), [&] { card_select_callback(0); /* [&] { try { card_selection.Enqueue(cards.at(0)); pressed[0] = true; } catch (std::string& e) { } */ }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8),
    Button(cards.at(1), [&] { card_select_callback(1); /* [&] { try { card_selection.Enqueue(cards.at(1)); pressed[1] = true; } catch (std::string& e) { } */ }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8),
    Button(cards.at(2), [&] { card_select_callback(2); /* [&] { try { card_selection.Enqueue(cards.at(2)); pressed[2] = true; } catch (std::string& e) { } */ }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8)
  });
  auto row2 = Container::Horizontal({
    Button(cards.at(3), [&] { card_select_callback(3); }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8),
    Button(cards.at(4), [&] { card_select_callback(4); }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8),
    Button(cards.at(5), [&] { card_select_callback(5); }, ButtonOption::Simple()) | size(WIDTH, EQUAL, 15) | size(HEIGHT, EQUAL, 8)
  });
  auto title = Renderer([&] {
    return vbox({
      text("[Card Grid]") | color(Color::Cyan)
    });    
  });
  card_grid->Add(title);
  card_grid->Add(row1);
  card_grid->Add(row2);
  return card_grid;
}

Component PlayerPane::MakeCardQueue() {
  return Renderer([&] {
    std::vector<Element> queue;
    int queueSize = card_selection.Size();
    for (int i = 0; i < queueSize; i++) {
      std::string s = card_selection.Front();

      queue.push_back(hbox({text(s) | bold}));

      card_selection.Dequeue();
      card_selection.Enqueue(s);
    }
    return hbox({
      text("[Queued Cards] ") | color(Color::Cyan),
      vbox({
        queue
      }),
    });
  });
}

Component PlayerPane::MakeWinsLossesSortedList() {
  return Renderer([&] {
    std::vector<Element> elements;
    for (int i = 0; i < wins_losses.Length(); i++) {
      std::string s = wins_losses.At(i) == GameResult::WIN ? "Win  " : "Loss  ";
      std::cerr << s << std::endl;
      elements.push_back(hbox({text(s) | bold}));
    }
    return hbox({
      text("[Wins/Losses] ") | color(Color::Cyan),
      hbox({
        elements
      })
    });
  });
}

bool PlayerPane::IsActiveTurn() const {
  return this->isActiveTurn;
}

bool PlayerPane::IsDead() const {
  return stats.health <= 0;
}

void PlayerPane::ToggleTurn() {
  this->isActiveTurn = !this->isActiveTurn;
  RandomizeCardsInInventory();
}

void PlayerPane::PerformActions(PlayerPane& other_pane) {
  // this gets called at the start of THIS player's turn
  // this should only happen this it is this player's turn and NOT the other player's turn
  // the OTHER player is attacking THIS player
  if (IsActiveTurn() && !other_pane.IsActiveTurn()) {
    CircularQueue<std::string>& q = other_pane.GetCardSelectionQueue();
    const ClosedHashTable<std::string, CardInfo>& ht = other_pane.GetCardInfoHT();
    while (q.Size() != 0) {
      std::string effect = q.Front(); q.Dequeue();
      CardInfo info = ht.Find(effect);
      if (info.GetEffect() == nullptr) {
        
      }
      else {
        std::shared_ptr<PoisonEffect> peffect = std::dynamic_pointer_cast<PoisonEffect>(info.GetEffect());
        std::shared_ptr<HealingEffect> heffect = std::dynamic_pointer_cast<HealingEffect>(info.GetEffect());
        std::shared_ptr<ShieldEffect> seffect = std::dynamic_pointer_cast<ShieldEffect>(info.GetEffect());
        if (peffect != nullptr) {
          player_effects.Push(peffect);
        }
        if (heffect != nullptr) {
          player_effects.Push(heffect);
        }
        if (seffect != nullptr) {
          other_pane.GetPlayerEffectsStack().Push(seffect);
        }
      }
    }
    
    std::shared_ptr<PlayerEffect> e = GetPlayerEffectsStack().Peek();
    GetPlayerEffectsStack().Pop();

    std::shared_ptr<PoisonEffect> peffect = std::dynamic_pointer_cast<PoisonEffect>(e);
    std::shared_ptr<HealingEffect> heffect = std::dynamic_pointer_cast<HealingEffect>(e);
    std::shared_ptr<ShieldEffect> seffect = std::dynamic_pointer_cast<ShieldEffect>(e);
    if (peffect != nullptr) {
      stats.health -= peffect->GetPoisonDamage();
    }
    if (heffect != nullptr) {
      stats.health += heffect->GetHealAmount();
      if (stats.health >= 10) {
        stats.health = 10;
      }
    }
    if (seffect != nullptr) {
      stats.armor += seffect->GetShieldAmount();
      if (stats.armor >= 10) {
        stats.armor = 10;
      }
    }
  }
  
  if (IsDead()) {
    other_pane.GetWinsLossesList().Insert(GameResult::WIN);
    wins_losses.Insert(GameResult::LOSS);
    Reset();
    other_pane.Reset();
  }
}

void PlayerPane::Reset() {
  this->stats.armor = 0;
  this->stats.health = 10;
  this->stats.mana = 0;
  player_effects.Clear();
  // wins_losses.Clear(); // why clear the wins_losses list?
  card_selection.Clear();

  // Not sure how necessary this is
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pressed[i] = false;
  }
}

CircularQueue<std::string>& PlayerPane::GetCardSelectionQueue() {
  return this->card_selection;
}

DoubleLinkedStack<std::shared_ptr<PlayerEffect>>& PlayerPane::GetPlayerEffectsStack() {
  return this->player_effects;
}

SortedList<GameResult>& PlayerPane::GetWinsLossesList() {
  return this->wins_losses;
}

const ClosedHashTable<std::string, CardInfo>& PlayerPane::GetCardInfoHT() {
  return this->card_ht;
}

// =====================================================================================
// RandomizeCardsInInventory function for PlayerPane
//   - Notes
//     \__ 
// =====================================================================================
void PlayerPane::RandomizeCardsInInventory() {
  RandomInt random;
  cards.clear();
  for (int i = 0; i < 6; i++) {
    cards.push_back(cardTypes[random.GetRandomInt(0, CARD_TYPE_AMOUNT-1)]);
    // std::cout << cardTypes[index]; 
  }
}
