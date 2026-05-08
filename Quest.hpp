#pragma once

#include "json.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using json = nlohmann::json;

enum class QuestType { Main, Side };

class Quest {
  QuestType type;
  int completion_percentage = 0;
  std::string content, description;

public:
  Quest(const std::string &quest_content, QuestType quest_type)
      : type(quest_type), content(quest_content) {}

  Quest(const std::string &quest_content, QuestType quest_type,
        int completion_percentage, std::string desc)
      : type(quest_type), completion_percentage(completion_percentage),
        content(quest_content), description(desc) {}

  int getCompletionPercentage() const { return completion_percentage; }
  std::string getContent() const { return content; }
  QuestType getQuestType() const { return type; }
  std::string getDescription() const { return description; }

  void setCompletionPercentage(int new_value) {
    completion_percentage = new_value;
  }
  void setDescription(const std::string &desc) { description = desc; }

  bool isComplete() const { return completion_percentage >= 100; }

  bool operator==(const Quest &other) const { return content == other.content; }

  json toJSON() const {
    return json{
        {"content", content},
        {"type", type},
        {"completion", completion_percentage},
        {"description", description},
    };
  }

  static Quest fromJSON(const json &j) {
    return Quest(j["content"], j["type"], j["completion"], j["description"]);
  };
};

class QuestManager {
  time_t last_reset_time = 0;

  std::vector<Quest> main_quests, side_quests, missed_quests;

  void resetDailyQuests() {
    // Move incomplete quests to "missed"
    for (auto &quest : main_quests) {
      if (!quest.isComplete()) {
        missed_quests.push_back(quest);
      }
    }
    for (auto &quest : side_quests) {
      if (!quest.isComplete()) {
        missed_quests.push_back(quest);
      }
    }

    // Clear the vectors
    main_quests.clear();
    side_quests.clear();
  }

public:
  void addQuest(const Quest &quest) {
    auto type = quest.getQuestType();
    std::vector<Quest> &vec =
        (type == QuestType::Main) ? main_quests : side_quests;

    vec.push_back(quest);
    // sort by completion percentage
    std::sort(vec.begin(), vec.end(), [](const Quest &a, const Quest &b) {
      return a.getContent().size() > b.getContent().size();
    });
  }

  bool removeQuest(Quest &quest) {
    auto type = quest.getQuestType();
    std::vector<Quest> &vec =
        (type == QuestType::Main) ? main_quests : side_quests;
    auto it = std::find(vec.begin(), vec.end(), quest);
    if (it == vec.end()) {
      return false;
      // this means the quest doesn't exist in the first place
    }

    vec.erase(it);
    return true;
  }

  // sincerely, this function is trash
  void removeQuestByIndex(QuestType quest_type, size_t index) {
    std::vector<Quest> &vec =
        (quest_type == QuestType::Main) ? main_quests : side_quests;
    vec.erase(vec.begin() + index);
  }

  // this function "should" only be used for printing
  size_t getQuestIndex(Quest &quest) {
    auto type = quest.getQuestType();
    std::vector<Quest> &vec =
        (type == QuestType::Main) ? main_quests : side_quests;

    auto it = std::find(vec.begin(), vec.end(), quest);
    if (it != vec.end()) {
      return std::distance(vec.begin(), it);
    }
    return -1;
  }

  Quest &getQuestByIndex(size_t index, QuestType type) {
    std::vector<Quest> &vec =
        (type == QuestType::Main) ? main_quests : side_quests;

    return vec.at(index);
  }

  std::vector<Quest> &getMainQuests() { return main_quests; }
  std::vector<Quest> &getSideQuests() { return side_quests; }

  void setMainQuests(const std::vector<Quest> &quests) { main_quests = quests; }
  void setSideQuests(const std::vector<Quest> &quests) { side_quests = quests; }

  // returns how many quests were removed, 0 if none
  size_t removeCompletedQuests() {
    size_t removed = 0;
    for (auto &main_quest : main_quests) {
      if (main_quest.isComplete()) {
        if (removeQuest(main_quest))
          removed++;
      }
    }
    for (auto &side_quest : side_quests) {
      if (side_quest.isComplete()) {
        if (removeQuest(side_quest))
          removed++;
      }
    }
    return removed;
  }

  void save(const std::string &filename) {
    std::vector<Quest> quests;
    json data;
    quests.reserve(main_quests.size() + side_quests.size());
    quests.insert(quests.end(), main_quests.begin(), main_quests.end());
    quests.insert(quests.end(), side_quests.begin(), side_quests.end());

    for (const auto &q : quests) {
      data.push_back(q.toJSON());
    }
    std::ofstream file(filename);
    file << data.dump(2);
  }

  void load(const std::string &filename) {
    std::vector<Quest> quests;
    std::ifstream file(filename);
    json data;
    file >> data;

    quests.clear();
    for (const auto &j : data) {
      quests.push_back(Quest::fromJSON(j));
    }

    main_quests.clear();
    side_quests.clear();
    for (const auto &q : quests) {
      addQuest(q);
    }
  }
};