#include "db.h"
#include "calendar.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ostream>

using json = nlohmann::json;

void TodoDB::addTodo(const std::string& date, const std::string& label,
	bool isDone) {
	json m_todos = parse_db();
	if (!m_todos.count(date)) {
		m_todos[date] = json::array();
	}

	json cur_todos = getTodos(date);
	if (cur_todos.empty()) {
		m_todos[date].push_back({ {"label", label}, {"is_done", isDone} });
	}
	else {
		bool is_present = false;
		for (const auto& t : cur_todos) {
			if (t["label"] == label || t["label"].empty()) {
				is_present = true;
				break;
			}
		}
		if (!is_present) {
			m_todos[date].push_back({ {"label", label}, {"is_done", isDone} });
		}
	}
	saveToFile(m_todos);
}

std::string TodoDB::getHomeDirectory() {
	const char* homeEnv = getenv("HOME");
	if (homeEnv != nullptr) {
		return std::string(homeEnv);
	}
	else {
		const char* userProfile = getenv("userprofile");
		if (userProfile != nullptr) {
			return std::string(userProfile);
		}
	}
	return "";
}

void TodoDB::init(const std::string& filename) {
	std::string homeDir(getHomeDirectory());
	if (homeDir.empty()) {
		return;
	}
	std::filesystem::path fullPath(homeDir);
	fullPath /= ".todo-cli";
	fullPath /= filename;
	m_filename = fullPath.string();
	std::filesystem::create_directories(fullPath.parent_path());
	if (!std::filesystem::exists(m_filename)) {
		std::ofstream createFile(m_filename);
		createFile << "{}";
		createFile.close();
	}
	parse_db();
}

json TodoDB::parse_db() {
	json m_todos;
	std::fstream f(m_filename);
	if (f.is_open() && f.peek() != std::ifstream::traits_type::eof()) {
		m_todos = json::parse(f);
	}
	f.close();
	return m_todos;
}

void TodoDB::saveToFile(json todos) {
	std::ofstream file(m_filename);
	file << todos.dump(2);
	file.close();
	lastModifiedTime =
		std::chrono::floor<std::chrono::seconds>(std::chrono::file_clock::now());
	lastModifiedTimeMs = std::chrono::floor<std::chrono::milliseconds>(
		std::chrono::system_clock::now());
}

json TodoDB::getTodos(const std::string& date) {
	json m_todos = parse_db();
	return m_todos.value(date, json::array());
}

json TodoDB::getIncompleteTaskForPastWeek() {
	json incomplete_tasks_list;
	json m_todos = parse_db();
	std::chrono::sys_days ymd =
		std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
	for (int i = 1; i < 7; i++) {
		std::chrono::year_month_day temp_ymd =
			ymd - std::chrono::days{ 7 } + std::chrono::days{ i };
		const std::string cur_date_str =
			get_date_string_format(Chrono_To_YMD(temp_ymd));
		if (m_todos.find(cur_date_str) != m_todos.end()) {
			json incompleteTasks;
			for (const auto& task : m_todos[cur_date_str]) {
				if (task.value("is_done", true) == false) {
					incompleteTasks.push_back(task);
				}
			}
			if (!incompleteTasks.empty()) {
				incomplete_tasks_list[cur_date_str] = incompleteTasks;
			}
		}
	}
	return incomplete_tasks_list;
}

json TodoDB::getIncompleteTasksAll() {
	json incomplete_tasks_list;
	json m_todos = parse_db();
	for (const auto& date_entry : m_todos.items()) {
		const std::string& cur_date_str = date_entry.key();
		const json& tasks = date_entry.value();
		json incompleteTasks;
		for (const auto& task : tasks) {
			if (task.value("is_done", true) == false) {
				incompleteTasks.push_back(task);
			}
		}
		if (!incompleteTasks.empty()) {
			incomplete_tasks_list[cur_date_str] = incompleteTasks;
		}
	}
	return incomplete_tasks_list;
}

void TodoDB::updateTodos(const std::string& date, const json& updatedList) {
	json m_todos = parse_db();
	m_todos[date] = updatedList;
}

void TodoDB::updateTodo(const std::string& date, const std::string& label,
	const bool& b) {
	json m_todos = parse_db();
	for (auto& it : m_todos[date]) {
		if (it["label"] == label) {
			it["is_done"] = b;
		}
	}
	saveToFile(m_todos);
}

bool TodoDB::isDBUpdated(std::chrono::file_clock::time_point currentTime) {
	if (currentTime != lastModifiedTime) {
		return false;
	}
	else {
		return true;
	}
}

bool TodoDB::isDBUpdatedNSecAgo(std::chrono::file_clock::time_point currentTime,
	std::chrono::seconds n) {
	if (currentTime - lastModifiedTime >= n) {
		return true;
	}
	else {
		return false;
	}
}

bool TodoDB::isDBUpdatedNMilliSecAgo(
	std::chrono::system_clock::time_point currentTime,
	std::chrono::milliseconds n) {
	std::chrono::milliseconds time_diff =
		std::chrono::round<std::chrono::milliseconds>(currentTime -
			lastModifiedTimeMs);
	std::chrono::milliseconds lower_limit = n - std::chrono::milliseconds(100);
	std::chrono::milliseconds upper_limit = n + std::chrono::milliseconds(100);
	if (time_diff > lower_limit && time_diff < upper_limit) {
		return true;
	}
	else {
		return false;
	}
}
