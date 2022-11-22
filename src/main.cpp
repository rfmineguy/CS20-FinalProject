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

  auto card = [&](const char* t) {
    return Button(t, []{});
  };
  
  auto name = Container::Horizontal({

  }) | border;
  
  auto effects = Container::Horizontal({
    
  }) | border;
  
  auto stats = Container::Horizontal({

  }) | border;
  
  auto card_row1 = [&](const char* but1, const char* but2, const char* but3, const char* but4) {
    auto container = Container::Horizontal({});
    container->Add(Button(but1, [&]() {}, ButtonOption::Simple()) | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 8));
    container->Add(Button(but2, [&]() {}, ButtonOption::Simple()) | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 8));
    container->Add(Button(but3, [&]() {}, ButtonOption::Simple()) | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 8));
    container->Add(Button(but4, [&]() {}, ButtonOption::Simple()) | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 8));
    return container;
  };

  auto cards = Container::Vertical({
    card_row1("0",  "1",  "2",  "3"),
    card_row1("4",  "5",  "6",  "7"),
  });
   
  auto winsLosses = Container::Horizontal({
  
  }) | border;
  
  auto controls = Container::Horizontal({
    Button("Forfeit", screen.ExitLoopClosure()),
    Button("Next Turn", [] {})
  }) | border;
  
  auto cardRenderer = Renderer(cards, [&] {
    return vbox({
      cards->Render() | vscroll_indicator | frame | border
    });
  });
  
  screen.Loop(cardRenderer);

  //auto renderer = Renderer(layout, [&] {
  //  return vbox({
  //    name->Render(),
  //    effects->Render(),
  //    stats->Render(),
  //    winsLosses->Render(),
  //    controls->Render(),
  //  }) | border,
  //});
  //
  //screen.Loop(renderer);
}