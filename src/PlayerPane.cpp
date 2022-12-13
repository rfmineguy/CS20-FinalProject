#include "../include/PlayerPane.hpp"
#include <string.h>

PlayerPane::PlayerPane(bool isTurn)
:card_ht("tombstone", "empty"), card_selection(4), wins_losses([](GameResult a, GameResult b) { return a <= b; }), player_effects(), pressed(), isActiveTurn(isTurn), stats(10, 0, 5) {
  auto func = [](int a, int b) { return a <= b; };
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
        text("Effects: "),
        vbox({effectVec})
      });
    })
  
  });
}

Component PlayerPane::MakeStatsContainer() {
  return Container::Horizontal({
    Renderer([&]{
      std::stringstream ss;
      ss << "Stats ||| Health ♥ " << stats.health << " | Armor Θ "<< stats.armor << " | Mana ┼ " << stats.mana;
      return vbox({
        text(isActiveTurn ? "It is your turn" : "It is not your turn"),
        text(ss.str()),
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
      text("Queued Cards: "),
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
      std::string s = wins_losses.At(i) == WIN ? "Win" : "Loss";
      elements.push_back(hbox({text(s) | bold}));
    }
    return hbox({
      text("Wins/Losses"),
      vbox({
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
          player_effects.Push(seffect);
        }
      }
    }
  }
}

void PlayerPane::Reset() {
  this->stats.armor = 0;
  this->stats.health = 10;
  this->stats.mana = 0;
  player_effects.Clear();
  wins_losses.Clear();
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

const ClosedHashTable<std::string, CardInfo>& PlayerPane::GetCardInfoHT() {
  return this->card_ht;
}

void PlayerPane::RandomizeCardsInInventory() {
  // NOTE: Figure out this random number generator
  srand(time(NULL));
  cards.clear();
  for (int i = 0; i < 6; i++) {
    int index = rand() % 4; // from RandomUtil.hpp
    cards.push_back(cardTypes[index]);
    std::cout << cardTypes[index]; 
  }
}
