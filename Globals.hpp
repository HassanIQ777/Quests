#pragma once

#include "Quest.hpp"
#include "libutils/src/CLIParser.hpp"
#include "libutils/src/funcs.hpp"
#include <filesystem>

using funcs::print;

struct Paths {
  std::filesystem::path home_dir, quests_dir;     // directories
  std::filesystem::path main_quests, side_quests; // files
  std::filesystem::path logs, stats;              // interesting stuff
};

struct Globals {
  bool running = true;
  CLIParser parser;
  std::string version = "1.2";
  QuestManager quest_manager;
  Paths paths;
};
