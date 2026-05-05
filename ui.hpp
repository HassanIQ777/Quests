#pragma once

#include "Globals.hpp"
#include "Quest.hpp"
#include "libutils/src/color.hpp"
#include "libutils/src/funcs.hpp"
#include "libutils/src/strutils.hpp"
#include <cstddef>
#include <string>

inline std::string repeatString(int count, const std::string &str) {
  std::string result;
  for (int i = 0; i < count; ++i) {
    result += str;
  }
  return result;
}

inline std::string coloredProgressBar(int percent, int width = 20) {
  int filled = (percent * width) / 100;

  // Choose color based on progress
  const char *color = (percent < 30)   ? color::TXT_RED
                      : (percent < 70) ? color::TXT_YELLOW
                                       : color::TXT_GREEN;

  std::string bar = "[";
  bar += color;
  bar += repeatString(filled, "█");
  bar += color::_RESET;
  bar += repeatString(width - filled, "░");
  bar += "] " + strutils::pad_left(std::to_string(percent) + "%", 4);

  return bar;
}

inline std::string colorizeQuest(const Quest &quest) {
  std::string content = quest.getContent();
  auto type = quest.getQuestType();

  const std::string color =
      (type == QuestType::Main) ? color::TXT_YELLOW : color::TXT_CYAN;
  return color::_BOLD + color + content;
}

inline void printQuest(QuestManager &quest_manager, Quest &quest) {
  std::string colorized_quest = colorizeQuest(quest);
  std::string progress_bar =
      coloredProgressBar(quest.getCompletionPercentage(), 10);
  size_t index = quest_manager.getQuestIndex(quest);

  const size_t width = funcs::getTerminalWidth();

  // Build the line
  std::string index_str = funcs::str(index + 1) + "  ";
  std::string quest_str = colorized_quest + color::_RESET;

  // Calculate visible lengths
  size_t index_visible = funcs::visibleLength(index_str);
  size_t quest_visible = funcs::visibleLength(quest_str);
  size_t progress_visible = funcs::visibleLength(progress_bar);

  size_t total_visible = index_visible + quest_visible + progress_visible;
  size_t padding = (total_visible < width) ? (width - total_visible) : 1;

  std::cout << index_str << quest_str << std::string(padding, ' ')
            << progress_bar << "\n";
}

inline void printQuests(Globals &globals) {
  auto &quest_manager = globals.quest_manager;
  auto &main_quests = globals.quest_manager.getMainQuests();
  auto &side_quests = globals.quest_manager.getSideQuests();

  if (!main_quests.empty()) {
    print("Main Quests:\n");
  }
  for (auto &main_quest : main_quests) {
    printQuest(quest_manager, main_quest);
  }

  if (!side_quests.empty()) {
    print("\nSide Quests:\n");
  }
  for (auto &side_quest : side_quests) {
    printQuest(quest_manager, side_quest);
  }

  // if there are absolutely no quests
  if (quest_manager.getMainQuests().size() +
          quest_manager.getSideQuests().size() ==
      0) {
    print(color::_ITALIC, "There are no quests, try adding some...\n",
          color::_RESET);
  }
}

inline size_t visibleLength(const std::string &str) {
  size_t count = 0;
  bool in_escape = false;

  for (char c : str) {
    if (c == '\033') {
      in_escape = true;
    } else if (in_escape && c == 'm') {
      in_escape = false;
    } else if (!in_escape) {
      count++;
    }
  }

  return count;
}

inline std::string padRight(const std::string &str, size_t total_width) {
  size_t visible = visibleLength(str);
  size_t padding = (visible < total_width) ? (total_width - visible) : 1;
  return str + std::string(padding, ' ');
}

inline void printLogo() {
  std::string date = funcs::currentTime().substr(11, 5);
  // funcs::printLeftMiddleRight("", "", date);
  print(strutils::pad_left(date, funcs::getTerminalWidth()), "\n");
  print(color::TXT_GREEN, color::_BOLD);
  print(R"( 
     _____        _ _          ____                  _       
    |  __ \      (_) |        / __ \                | |      
    | |  | | __ _ _| |_   _  | |  | |_   _  ___  ___| |_ ___ 
    | |  | |/ _` | | | | | | | |  | | | | |/ _ \/ __| __/ __|
    | |__| | (_| | | | |_| | | |__| | |_| |  __/\__ \ |_\__ \
    |_____/ \__,_|_|_|\__, |  \___\_\\__,_|\___||___/\__|___/
                       __/ |                                 
                      |___/                                  
)");
  print(color::_RESET);
}