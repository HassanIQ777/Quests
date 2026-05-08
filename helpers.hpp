#pragma once

#include "Globals.hpp"
#include "Quest.hpp"
#include "libutils/src/File.hpp"
#include "libutils/src/Log.hpp"
#include "libutils/src/color.hpp"
#include "libutils/src/funcs.hpp"
#include "libutils/src/strutils.hpp"
// #include "ui.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

inline void LOG(Globals &globals, const std::string &msg) {
  std::string date = funcs::currentTime();
  std::string output = date + " -> " + msg;
  File::insertline(globals.paths.logs, output, 0);
}

inline std::string trimStr(std::string text, size_t WIDTH) {
  if (text.length() <= WIDTH) {
    return text;
  }
  return text.substr(0, WIDTH - 2) + "··";
}

inline void printHelp(Globals &globals) {
  const std::string program_name = globals.parser.getArg(0);
  print("Usage:\n");
  print("  ", program_name, " <HOME_DIR>\n");
  print("  ", program_name, " -h    print this help message\n");
  print("  ", program_name, " -v    print version\n");
}

inline void assignPaths(Globals &globals) {
  globals.paths.quests_dir = globals.paths.home_dir / "Quests";
  globals.paths.quests = globals.paths.quests_dir / "quests.json";
  globals.paths.logs = globals.paths.quests_dir / "logs.txt";
  globals.paths.stats = globals.paths.quests_dir / "stats.ini";
}

inline void parseArgs(Globals &globals) {
  if (int argc = globals.parser.getArgc(); argc != 2) {
    if (argc == 1) {
      Log::error("One argument is required but nothing was provided.", false);
    } else {

      Log::error("One argument is required but " +
                     funcs::str(globals.parser.getArgc() - 1) +
                     " arguments were provided.",
                 false);
    }
    printHelp(globals);
    exit(-1);
  }

  const std::string first_arg = globals.parser.getArg(1);
  if (first_arg == "-h") {
    printHelp(globals);
    exit(0);
  } else if (first_arg == "-v") {
    print("dailyquests version ", globals.VERSION, "\n");
    exit(0);
  }

  if (File::isdirectory(first_arg)) {
    globals.paths.home_dir = first_arg;
    assignPaths(globals);
  } else {
    Log::error("The provided path is not a directory.", true);
  }
}

inline QuestType selectQuestType() {
  bool isMain = true;
  print("Select Quest Type:\n");
  print(">[Main]  [Side]  ");
  while (true) {
    std::string inp = funcs::getKeyPress();
    if (inp == "\n") {
      break;
    } else {
      isMain = !isMain;
    }
    if (isMain) {
      print("\r>[Main]  [Side]  ");
    } else {
      print("\r [Main] >[Side]  ");
    }
  }
  return (isMain) ? QuestType::Main : QuestType::Side;
}

inline void handleInput(Globals &globals, std::string input) {
  // Quit
  input = funcs::uppercase(input);
  if (input == "Q") {
    globals.running = false;
    return;
  }

  // Help
  else if (input == "H") {
    print("[A] Add Quest\n");
    print("[D] Delete Quest\n");
    print("[R] Remove completed Quests\n");
    print("[S] Set Quest Progress\n");
    print("[V] View A Quest's Info\n");
    print("[Q] Quit\n");
    funcs::getKeyPress();
  }

  // Add Quest
  else if (input == "A") {
    print("Quest: ", color::_ITALIC);
    std::string quest_content;
    std::getline(std::cin, quest_content);
    print(color::_RESET);
    quest_content = strutils::trim(quest_content);
    if (quest_content.size() < 2) {
      return;
    }
    print("Description: ", color::_ITALIC);
    std::string description;
    std::getline(std::cin, description);
    print(color::_RESET);
    description = strutils::trim(description);
    if (description.size() < 2) {
      description.clear();
    }

    QuestType type = selectQuestType();
    Quest quest(quest_content, type);
    if (!description.empty())
      quest.setDescription(description);
    globals.quest_manager.addQuest(quest);

    std::string msg = "Added Quest -> " + quest_content + ":" +
                      (type == QuestType::Main ? "Main" : "Side");
    LOG(globals, msg);
  }

  // Delete Quest
  else if (input == "D") {
    QuestType type = selectQuestType();
    print("\n");
    std::vector<Quest> &vec = (type == QuestType::Main)
                                  ? globals.quest_manager.getMainQuests()
                                  : globals.quest_manager.getSideQuests();
    if (vec.size() == 0) {
      std::string msg;
      msg = (type == QuestType::Main) ? "You have no main quests."
                                      : "You have no side quests.";
      print(msg, "\n");
      funcs::getKeyPress();
      return;
    }

    print("Quest Number: ");
    int num;
    std::cin >> num;
    std::cin.ignore();
    size_t index = num - 1;

    if (index >= vec.size()) {
      Log::warn("No quest of this index exists.");
      funcs::getKeyPress();
      return;
    }

    Quest &quest = vec[index];
    std::string msg = "Deleted Quest -> " + quest.getContent() + ":" +
                      (type == QuestType::Main ? "Main" : "Side");
    LOG(globals, msg);
    globals.quest_manager.removeQuestByIndex(type, index);
  }

  // Remove completed Quests
  else if (input == "R") {
    size_t removed = globals.quest_manager.removeCompletedQuests();
    std::string msg;
    if (removed == 0) {
      msg = "No quests are completed...";
      print(msg, "\n");
    } else if (removed == 1) {
      msg = "One quest was removed.";
      print(msg, "\n");
    } else {
      msg = funcs::str(removed) + " quests were removed.";
      print(msg, "\n");
    }
    LOG(globals, msg);
    funcs::getKeyPress();
  }

  // Set Quest Progress
  else if (input == "S") {
    QuestType type = selectQuestType();
    print("\n");
    std::vector<Quest> &vec = (type == QuestType::Main)
                                  ? globals.quest_manager.getMainQuests()
                                  : globals.quest_manager.getSideQuests();
    if (vec.size() == 0) {
      std::string msg;
      msg = (type == QuestType::Main) ? "You have no main quests."
                                      : "You have no side quests.";
      print(msg, "\n");
      funcs::getKeyPress();
      return;
    }

    print("Quest Number: ");
    int num;
    std::cin >> num;
    std::cin.ignore();
    size_t index = num - 1;

    if (index >= vec.size()) {
      Log::warn("No quest of this index exists.");
      funcs::getKeyPress();
      return;
    }
    int completion_percentage;
    print("Percentage: ");
    std::cin >> completion_percentage;
    std::cin.ignore();
    completion_percentage = funcs::clamp(completion_percentage, 0, 100);
    globals.quest_manager.getQuestByIndex(index, type)
        .setCompletionPercentage(completion_percentage);

    Quest quest = globals.quest_manager.getQuestByIndex(index, type);
    std::string msg = "Set Quest Progress -> " + quest.getContent() + ":" +
                      (type == QuestType::Main ? "Main" : "Side") + ":" +
                      funcs::str(completion_percentage) + "%";
    LOG(globals, msg);
  }

  // View A Quest's Info
  else if (input == "V") {
    QuestType type = selectQuestType();
    print("\n");
    std::vector<Quest> &vec = (type == QuestType::Main)
                                  ? globals.quest_manager.getMainQuests()
                                  : globals.quest_manager.getSideQuests();
    if (vec.size() == 0) {
      std::string msg;
      msg = (type == QuestType::Main) ? "You have no main quests."
                                      : "You have no side quests.";
      print(msg, "\n");
      funcs::getKeyPress();
      return;
    }

    print("Quest Number: ");
    int num;
    std::cin >> num;
    std::cin.ignore();
    size_t index = num - 1;

    if (index >= vec.size()) {
      Log::warn("No quest of this index exists.");
      funcs::getKeyPress();
      return;
    }

    Quest &quest = vec[index];
    std::string msg = "Viewed Quest -> " + quest.getContent() + ":" +
                      (type == QuestType::Main ? "Main" : "Side");
    LOG(globals, msg);
    const std::string content = quest.getContent(),
                      description = quest.getDescription();
    int completion_percentage = quest.getCompletionPercentage();
    print("\n", content, "\n");
    if (!description.empty()) {
      print("Description: ", color::_ITALIC, description, color::_RESET, "\n");
    }
    print("This quest is ", completion_percentage, "% complete.\n");
    funcs::getKeyPress();
  }
}

inline bool createFile(Globals &globals, const std::string &fp) {
  if (!File::isfile(fp)) {
    if (File::createfile(fp)) {
      LOG(globals, "Successfully created '" + fp + "'");
      return true; // we newly created this
    } else {
      LOG(globals, "Failed to create '" + fp + "'");
      exit(-3);
    }
  }
  return false; // already created
}

inline void createFiles(Globals &globals) {
  if (std::string dir = globals.paths.quests_dir; !File::isdirectory(dir)) {
    if (!File::createdir(dir)) {
      Log::error("Failed to created '" + dir + "'");
      // we can't really log this to logs.txt since it doesn't exist yet
    }
  }

  if (createFile(globals, globals.paths.quests)) {
    File::appendline(globals.paths.quests, "[]");
  }
  createFile(globals, globals.paths.logs);
  // createFile(globals, globals.paths.stats); // will add this later...one day
}

inline std::vector<Quest>
loadQuests(const std::vector<std::string> &quests_file, QuestType type) {
  static const char delimiter = 0x1F;
  std::vector<Quest> result;

  for (const auto &line : quests_file) {
    auto v = funcs::split(line, delimiter);
    // v either has 2 elements or 3
    std::string quest_content = v[0];
    int completion_percentage = stoi(v[1]);
    Quest q(quest_content, type);
    q.setCompletionPercentage(completion_percentage);
    if (v.size() == 3) {
      q.setDescription(v[2]);
    }
    result.push_back(q);
  }

  return result;
}

inline std::vector<std::string> createVector(std::vector<Quest> &quests) {
  static const char delimiter = 0x1F;
  static const std::string d(1, delimiter);
  std::vector<std::string> result;

  for (const auto &quest : quests) {
    std::string content = quest.getContent();
    int completion_percentage = quest.getCompletionPercentage();
    std::string line = content + d + funcs::str(completion_percentage);
    if (!quest.getDescription().empty()) {
      line += d + quest.getDescription();
    }
    result.push_back(line);
  }

  return result;
}