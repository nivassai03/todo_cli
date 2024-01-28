#include "todo_list_due.h"
#include "calendar.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

using namespace ftxui;
using json = nlohmann::json;

TDList::TDList(Ref<YearMonthDay> ymd, Ref<TodoDB> db, Decorator list_size)
	: m_ymd{ ymd }, m_tododb(db), m_list_size(list_size), m_tab_selected(0) {
	m_prev_ymd = m_ymd();
	m_todo_list = json::object();
	m_map_todo.reserve(1000);
	Add(generate_due_todolist());
}

bool TDList::date_changed() {
	if (m_prev_ymd.c_day == m_ymd().c_day &&
		m_prev_ymd.c_month == m_ymd().c_month &&
		m_prev_ymd.c_year == m_ymd().c_year &&
		m_prev_ymd.c_weekday == m_ymd().c_weekday) {
		return false;
	}
	else {
		return true;
	}
}
Component TDList::generate_due_todolist() {
	m_map_todo.clear();
	m_todo_list = m_tododb->getIncompleteTaskForPastWeek();
	Components cur_todos;
	if (m_todo_list.empty()) {
		auto empty_tasks_label = text("No Due Tasks") | center | bold |
			bgcolor(Color::Black) | color(Color::Green);
		cur_todos.push_back(Renderer([=, this] { return empty_tasks_label; }));
	}
	else {
		for (auto& entry : m_todo_list.items()) {
			std::string date = entry.key();
			auto date_label = text(date) | center | bold | bgcolor(Color::White) |
				color(Color::Black) | border;
			cur_todos.push_back(Renderer([=, this] { return date_label; }));
			json tasks_array = entry.value();
			for (auto j_object : tasks_array) {
				Todo t;
				t.m_label = j_object["label"];
				t.m_is_done = j_object["is_done"];
				t.m_deadline = get_date_ymd_format(date);
				m_map_todo.insert(std::make_pair(t.m_label, t));
				CheckboxOption opt = CheckboxOption();
				opt.transform = [&](const EntryState& s) {
					auto prefix = text(s.state ? "[X]" : "[ ]");
					auto ts = text(s.label);
					if (s.active) {
						ts |= bold;
					}
					if (s.state) {
						prefix |= color(Color::Green);
						ts |= color(Color::Green);
					}
					return hbox({ emptyElement() | size(WIDTH, EQUAL, 1), prefix,
								 emptyElement() | size(WIDTH, EQUAL, 3), ts,
								 emptyElement() | size(WIDTH, EQUAL, 35) }) |
						size(HEIGHT, EQUAL, 1) | border;
					};
				opt.on_change = [=, this] {
					updateTodo(
						get_date_string_format(m_map_todo.at(t.m_label).m_deadline),
						m_map_todo.at(t.m_label).m_label,
						m_map_todo.at(t.m_label).m_is_done);
					};
				cur_todos.push_back(Checkbox(&m_map_todo.at(t.m_label).m_label,
					&m_map_todo.at(t.m_label).m_is_done, opt));
			}
		}
	}
	auto todo_list_component = Container::Vertical(cur_todos);
	return todo_list_component;
}

void TDList::updateTodo(const std::string& ymd, const std::string& label,
	const bool& b) {
	m_tododb->updateTodo(ymd, label, b);
}

void TDList::redraw_todolist() {
	ComponentBase::DetachAllChildren();
	Add(generate_due_todolist());
}

Element TDList::Render() {
	m_prev_ymd = m_ymd();

	// delay to update due list.
	// auto currentTimeMs = std::chrono::floor<std::chrono::milliseconds>(
	//     std::chrono::system_clock::now());
	// if (m_tododb->isDBUpdatedNMilliSecAgo(currentTimeMs,
	//                                       std::chrono::milliseconds(200))) {
	//   redraw_todolist();
	// }

	// instant update of due list.
	auto currentTime =
		std::chrono::floor<std::chrono::seconds>(std::chrono::file_clock::now());
	if (m_tododb->isDBUpdated(currentTime)) {
		redraw_todolist();
	}
	return ComponentBase::Render() | m_list_size | vscroll_indicator | yframe |
		border;
}

Component TDListWidget(YearMonthDay* ymd, TodoDB* db, Decorator list_size) {
	return Make<TDList>(ymd, db, list_size);
}
