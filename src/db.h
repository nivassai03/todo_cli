#ifndef CAL_DB_H
#define CAL_DB_H

#include <chrono>
#include <nlohmann/json.hpp>

class TodoDB {

public:
	TodoDB() = default;
	void addTodo(const std::string& date, const std::string& label, bool isDone);
	nlohmann::json getTodos(const std::string& date);
	void init(const std::string& filename);
	void updateTodos(const std::string& date, const nlohmann::json& updatedList);
	void updateTodo(const std::string& date, const std::string& label,
		const bool& b);
	void saveToFile(nlohmann::json todos);
	bool isDBUpdated(std::chrono::file_clock::time_point currentTime);
	bool isDBUpdatedNSecAgo(std::chrono::file_clock::time_point currentTime,
		std::chrono::seconds n);
	bool
		isDBUpdatedNMilliSecAgo(std::chrono::system_clock::time_point currentTime,
			std::chrono::milliseconds n);
	nlohmann::json getIncompleteTaskForPastWeek();
	nlohmann::json getIncompleteTasksAll();
	nlohmann::json parse_db();

private:
	std::string m_filename;
	std::chrono::file_clock::time_point lastModifiedTime;
	std::chrono::system_clock::time_point lastModifiedTimeMs;
	std::string getHomeDirectory();
};

#endif
