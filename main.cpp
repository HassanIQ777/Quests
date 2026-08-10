#include "Globals.hpp"
#include "helpers.hpp"
#include "libutils/src/CLIParser.hpp"
#include "libutils/src/funcs.hpp"
#include "ui.hpp"
#include <csignal>

int main(int argc, char **argv) {
  std::signal(SIGINT, handleInterrupt);
  auto &globals = Globals::getInstance();
  globals.parser.setArgs(argc, argv);
  parseArgs(globals);
  createFiles(globals);
  globals.quest_manager.load(globals.paths.quests);

  funcs::alternativeTerminal();
  while (globals.running) {
    funcs::clearTerminal();
    printLogo();

    printQuests(globals);
    print("\n[H] Help\n");

    const std::string input = funcs::getKeyPress();
    handleInput(globals, input);
    globals.quest_manager.save(globals.paths.quests);
  }

  funcs::restoreTerminal();
  funcs::printCentered("Thanks for using Daily Quests!\n");
  funcs::printCentered("By HassanIQ777\n");
}