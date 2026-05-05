#include "Globals.hpp"
#include "helpers.hpp"
#include "libutils/src/CLIParser.hpp"
#include "libutils/src/funcs.hpp"
#include "ui.hpp"

int main(int argc, char **argv) {
  Globals globals;
  globals.parser.setArgs(argc, argv);
  parseArgs(globals);
  createFiles(globals);
  readQuests(globals);

  while (globals.running) {
    funcs::clearTerminal();
    printLogo();
    // globals.quest_manager.checkAndReset(); // needed to update the stats

    printQuests(globals);
    print("\n[H] Help\n");

    const std::string input = funcs::getKeyPress();
    handleInput(globals, input);
    writeQuests(globals);
  }

  funcs::printCentered("Thanks for using Daily Quests!\n");
  funcs::printCentered("By HassanIQ777\n");
}