#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"

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
int main() {
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  
  std::vector<std::vector<Element>> table_elements = {
    {"hi", "bye"},
    {}
  };
  auto table = ftxui::Table(table_elements);
  
  auto card = [&](const char* t) {
    return Button(t, []{});
  };
  
  auto name = Container::Horizontal({

  }) | border;
  
  auto effects = Container::Horizontal({
    
  }) | border;
  
  auto stats = Container::Horizontal({

  }) | border;
  
  auto cards = Container::Horizontal({
    Button("X", [&] {}) | border,
    Button("Y", [&] {}) | border,
    Button("Z", [&] {}) | border,
  });
  
  auto component = Renderer(cards, [&] {
    return vbox({
        cards->Render(),
        cards->Render(),
    });
  });
 
//  for (int i = 0; i < 3; i++) {
//    cards->Add(Container::Vertical({
//      card("card A"),
//      card("card B"),
//      card("card C")
//    }));
//  }
  
  auto winsLosses = Container::Horizontal({
  
  }) | border;
  
  auto controls = Container::Horizontal({
    Button("Forfeit", screen.ExitLoopClosure()),
    Button("Next Turn", [] {})
  }) | border;
  
  auto cardRenderer = Renderer([](Element e) {
    return e;
  });
  
  screen.Loop(component);

  // auto renderer = Renderer(layout, [&] {
  //   return vbox({
  //     name->Render(),
  //     effects->Render(),
  //     stats->Render(),
  //     winsLosses->Render(),
  //     controls->Render(),
  //   }) | border,
  // });
  // 
  // screen.Loop(renderer);
}