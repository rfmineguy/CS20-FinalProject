#include "../include/PlayerPane.hpp"

int main() {
  //==================================================================
  // Use this to actually render the screen
  //==================================================================
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  
  //============================================================================
  // Create two panes
  //============================================================================
  srand(time(nullptr));
  int turn = rand() % 2;
  PlayerPane pane1(turn);      // param determines weather this player starts
  PlayerPane pane2(!turn);
  
  //============================================================================
  // Retrieve references to the various components of pane1
  //============================================================================
  auto pane1_card_grid = pane1.MakeCardGrid();
  auto pane1_card_queue = pane1.MakeCardQueue();
  auto pane1_stats = pane1.MakeStatsContainer();
  auto pane1_effects = pane1.MakeEffectsContainer();
  auto pane1_wins_losses_sorted_list = pane1.MakeWinsLossesSortedList();
  auto pane1_debug_action_stack = pane1.MakeDebugActionStack();
  auto pane1_renderer = Renderer(Container::Vertical({pane1_card_grid}), [&] {
    return vbox({
      pane1_stats->Render() | borderRounded,
      pane1_effects->Render() | borderRounded,
      pane1_card_queue->Render() | borderRounded,
      pane1_card_grid->Render() | borderRounded,
      pane1_wins_losses_sorted_list->Render() | borderRounded,
      pane1_debug_action_stack->Render() | borderRounded
    });
  });
  
  //============================================================================
  // Retrieve references to the various components of pane2
  //============================================================================
  auto pane2_card_grid = pane2.MakeCardGrid();
  auto pane2_card_queue = pane2.MakeCardQueue();
  auto pane2_stats = pane2.MakeStatsContainer();
  auto pane2_effects = pane2.MakeEffectsContainer();
  auto pane2_wins_losses_sorted_list = pane2.MakeWinsLossesSortedList();
  auto pane2_debug_action_stack = pane1.MakeDebugActionStack();
  auto pane2_renderer = Renderer(Container::Vertical({pane2_card_grid}), [&] {
    return vbox({
      pane2_stats->Render() | borderRounded,
      pane2_effects->Render() | borderRounded,
      pane2_card_queue->Render() | borderRounded,
      pane2_card_grid->Render() | borderRounded,
      pane2_wins_losses_sorted_list->Render() | borderRounded,
      pane2_debug_action_stack->Render() | borderRounded
    });
  });
  
  //============================================================================
  // Create a container for displaying the game controls (FTXUI)
  //============================================================================
  auto controls = Container::Horizontal({
    Button("Forfeit", screen.ExitLoopClosure()),
    Button("End Turn", [&] {
      // Also conditionally randomizes the card selection
      pane1.ToggleTurn();
      pane2.ToggleTurn();
      
      // Run the game for each pane (player)
      //   These have conditions for whether they should actually happen, so calling both here is OK
      pane1.PerformActions(pane2);
      pane2.PerformActions(pane1);
      
      // Attempt to refresh the FTXUI screen when the turn is ended
      screen.PostEvent(Event::Custom);
    })
  }) | border;
  
  //============================================================================
  // Setup the splits (makes each pane display on either half of the terminal, roughly)
  //============================================================================
  int left_size = 40;
  int bottom_size = 10;
  Component mainLayout = pane1_renderer;
  mainLayout = ResizableSplitLeft(pane2_renderer, mainLayout, &left_size);
  mainLayout = ResizableSplitBottom(controls, mainLayout, &bottom_size);

  //============================================================================
  // Loop the 'mainLayout' - ftxui
  //============================================================================
  screen.Loop(mainLayout);
}