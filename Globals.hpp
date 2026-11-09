#pragma once

#include "Quest.hpp"
#include "libutils/CLIParser.hpp"
#include "libutils/funcs.hpp"

using funcs::print;

struct Paths {
  std::filesystem::path home_dir, quests_dir; // directories
  std::filesystem::path quests, logs;         // files
  // std::filesystem::path stats;             // will add this later...one day
};

struct Globals {
  std::string VERSION = "v26.11.9";
  bool running        = true;
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
