#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"

#include "../include/CirclularQueue.hpp"
#include "../include/ClosedHashTable.hpp"
#include "../include/DoubleLinkedStack.hpp"

using namespace ftxui;

Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

class CardInfo {
  public:
    CardInfo(): CardInfo("NULL", 0) {}
    CardInfo(const std::string& s, int _duration): name(s), durationInTurns(_duration) {}
    friend std::ostream& operator<<(std::ostream& os, const CardInfo& ci) {
      os << "CardInfo : " << ci.name << std::endl;
      return os;
    }
  private:
    std::string name;
    int durationInTurns;
};

class PlayerEffect {
  public:
    PlayerEffect(): PlayerEffect("NULL") {}
    PlayerEffect(const std::string& _name): name(_name) {}
    virtual ~PlayerEffect() = default;  
    std::string GetName() const { return name; }  
  
  private:
    std::string name;
};

class PoisonEffect : public PlayerEffect {
public:
  PoisonEffect(): PlayerEffect("Poison") {}
};

class HealingEffect : public PlayerEffect {
public:
  HealingEffect(): PlayerEffect("Healing") { srand(time(0)); }
  int GetHealAmount() { return rand() % 10; }
};

int main() {
  //==================================================================
  // Use this to actually render the screen
  //==================================================================
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  
  //==================================================================
  // Use this to look up the stats for each card type
  //==================================================================  
  ClosedHashTable<std::string, CardInfo> card_ht("tombstone", "empty");
  card_ht.Insert("Poison", CardInfo("Poison", 2));
  card_ht.Insert("Shield", CardInfo("Shield", 1));
  card_ht.Insert("Sword", CardInfo("Sword", 1));
  
  //==================================================================
  // Use this to manage the card selection order
  //==================================================================
  CircularQueue<std::string> card_selection(4);
  std::string selected = "<selection>";
  
  //==================================================================
  // Use this to manage the effects that are applied to the player
  //==================================================================
  DoubleLinkedStack<std::shared_ptr<PlayerEffect>> player_effects;
  player_effects.Push(std::make_shared<PoisonEffect>());
  player_effects.Push(std::make_shared<HealingEffect>());
  
  //==================================================================
  // Create a container for storing the current player effects (FTXUI)
  //==================================================================
  auto effects = Container::Horizontal({
    Renderer([&]{
      std::vector<Element> effectVec;
      int size = player_effects.Size();
      while (size > 0) {
        std::shared_ptr<PlayerEffect> effect = player_effects.Peek();
        player_effects.Pop();
        size--;
        if (std::dynamic_pointer_cast<PoisonEffect>(effect) != nullptr) {
          // poison effect
        }
        if (std::dynamic_pointer_cast<PoisonEffect>(effect) != nullptr) {
          // healing effect
        }
        effectVec.push_back(hbox({ text(effect->GetName()) }));
        player_effects.Push(effect);
      }
      return hbox({
        text("Effects: "),
        vbox({effectVec})
      });
    })
  });
  
  //==================================================================
  // Create a container for storing the current player stats (FTXUI)
  //==================================================================
  auto stats = Container::Horizontal({
    Renderer([&]{
      std::stringstream ss;
      ss << "Stats ||| Health ♥ " << 10 << " | Armor Θ "<< 10 << " | Mana ┼ " << 10;
      return vbox({
        text(ss.str()),
      });
    })
  });

  //==================================================================
  // Create a container for storing the current player's cards (FTXUI)
  //==================================================================
  std::vector<std::string> cards = { "Poison", "Sword", "Poison", "Shield", "Sword", "Shield" };
  bool pressed[6] = {false, false, false, false, false, false};
  auto card_select_callback = [&](int index) {
    if (pressed[index])
      return;
    try {
      card_selection.Enqueue(cards.at(index));
      pressed[index] = true;
    } catch (std::string e) {
      
    }
  };
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

  //============================================================================
  // Create a renderer for displaying the current player's move queue (FTXUI)
  //============================================================================
  auto card_queue = Renderer([&] {
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
  
  //============================================================================
  // Create a container for displaying the game controls (FTXUI)
  //============================================================================
  auto controls = Container::Horizontal({
    Button("Forfeit", screen.ExitLoopClosure()),
    Button("End Turn", [] { })
  }) | border;
  
  //============================================================================
  // Create a container for displaying the player's cards (FTXUI)
  //============================================================================
  auto cardGrid = Renderer(card_grid, [&] {
    return vbox({ 
      card_grid->Render(),
    }); //card_grid->Render() | vscroll_indicator | frame | border;
  });
  
  auto layout = Container::Vertical({
    cardGrid,
    controls
  });
  
  auto cardRenderer = Renderer(layout, [&] {
    return vbox({
      stats->Render() | border,
      effects->Render() | border,
      card_queue->Render() | border,
      cardGrid->Render() | border,
      separator(),
      controls->Render()
    });
  });
  
  screen.Loop(cardRenderer);
}