#pragma once

#include "Quest.hpp"
#include "libutils/src/CLIParser.hpp"
#include "libutils/src/funcs.hpp"
#include <filesystem>

using funcs::print;

struct Paths {
  std::filesystem::path home_dir, quests_dir; // directories
  std::filesystem::path quests;               // files
  std::filesystem::path logs, stats;          // interesting stuff
};

struct Globals {
  std::string VERSION = "1.4";
  bool running = true;
  CLIParser parser;
  QuestManager quest_manager;
  Paths paths;

  static Globals &getInstance() {
    static Globals globals;
    return globals;
  }

  Globals(const Globals &) = delete;
  Globals(Globals &&) = delete;
  Globals &operator=(const Globals &) = delete;
  Globals &operator=(Globals &&) = delete;

private:
  Globals() {}
};
