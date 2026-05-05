#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iterator>
#include <string>
#include <vector>

enum class QuestType { Main, Side };

/*
struct QuestStats {
  int main_completed = 0;
  int main_total = 0;
  int side_completed = 0;
  int side_total = 0;
  int missed_total = 0;
  int missed_main = 0;
  int missed_side = 0;

  double completion_rate = 0.0;
};
*/

class Quest {
  QuestType type;
  int completion_percentage = 0;
  std::string content, description;

public:
  Quest(const std::string &quest_content, QuestType quest_type)
      : type(quest_type), content(quest_content) {}

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
  /*
    void checkAndReset() {
      auto now = std::chrono::system_clock::now();
      auto time_t_now = std::chrono::system_clock::to_time_t(now);
      auto tm = localtime(&time_t_now);

      // Check if it's 3AM (03:00 - 03:59)
      if (tm->tm_hour == 3) {
        // Only reset once per day at 3AM
        if (last_reset_time == 0 ||
            difftime(time_t_now, last_reset_time) >= 86400) {
          resetDailyQuests();
          last_reset_time = time_t_now;
        }
      }
    }
    QuestStats getStats() const {
      QuestStats stats;

      // Count main quests
      stats.main_total = main_quests.size();
      for (const auto &q : main_quests) {
        if (q.isComplete())
          stats.main_completed++;
      }

      // Count side quests
      stats.side_total = side_quests.size();
      for (const auto &q : side_quests) {
        if (q.isComplete())
          stats.side_completed++;
      }

      // Count missed quests
      stats.missed_total = missed_quests.size();
      for (const auto &q : missed_quests) {
        if (q.getQuestType() == QuestType::Main) {
          stats.missed_main++;
        } else {
          stats.missed_side++;
        }
      }

      // Calculate completion rate
      int total = stats.main_total + stats.side_total;
      int completed = stats.main_completed + stats.side_completed;
      stats.completion_rate = (total > 0) ? (100.0 * completed / total) : 0.0;

      return stats;
    }
    */

  void addQuest(const Quest &quest) {
    auto type = quest.getQuestType();
    std::vector<Quest> &vec =
        (type == QuestType::Main) ? main_quests : side_quests;

    vec.push_back(quest);
    // sort by completion percentage
    std::sort(vec.begin(), vec.end(), [](const Quest &a, const Quest &b) {
      return a.getCompletionPercentage() > b.getCompletionPercentage();
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

  // this function will only be used for printing
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
};