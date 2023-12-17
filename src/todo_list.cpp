#include "todo_list.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;
using json = nlohmann::json;

TList::TList(Ref<YearMonthDay> ymd, Ref<TodoDB> db, Decorator list_size)
    : m_ymd{ymd}, m_tododb(db), m_list_size(list_size) {
  m_prev_ymd = m_ymd();
  m_todo_list = json::array();
  m_map_todo.reserve(1000);
  Add(generate_todolist());
}

bool TList::date_changed() {
  if (m_prev_ymd.c_day == m_ymd().c_day &&
      m_prev_ymd.c_month == m_ymd().c_month &&
      m_prev_ymd.c_year == m_ymd().c_year &&
      m_prev_ymd.c_weekday == m_ymd().c_weekday) {
    return false;
  } else {
    return true;
  }
}

Component TList::generate_todolist() {
  m_map_todo.clear();
  m_todo_list = m_tododb->getTodos(get_date_string_format(m_ymd()));
  Components cur_todos;
  for (auto j_object : m_todo_list) {
    Todo t;
    t.m_label = j_object["label"];
    t.m_is_done = j_object["is_done"];
    t.m_deadline = m_ymd();
    m_map_todo.insert(std::make_pair(t.m_label, t));
    CheckboxOption opt = CheckboxOption();
    opt.transform = [&](const EntryState &s) {
      auto prefix = text(s.state ? "[X]" : "[ ]");
      auto ts = text(s.label);
      if (s.active) {
        ts |= bold;
      }
      if (s.state) {
        prefix |= color(Color::Green);
        ts |= strikethrough | color(Color::Green);
      }
      if (!s.state) {
        prefix = text(s.state ? "[X]" : "[ ]");
        ts = text(s.label);
      }
      return hbox({emptyElement() | size(WIDTH, EQUAL, 1), prefix,
                   emptyElement() | size(WIDTH, EQUAL, 3), ts,
                   emptyElement() | size(WIDTH, EQUAL, 35)}) |
             size(HEIGHT, EQUAL, 1) | border;
    };
    opt.on_change = [=, this] {
      updateTodo(get_date_string_format(m_map_todo.at(t.m_label).m_deadline),
                 m_map_todo.at(t.m_label).m_label,
                 m_map_todo.at(t.m_label).m_is_done);
    };
    cur_todos.push_back(Checkbox(&m_map_todo.at(t.m_label).m_label,
                                 &m_map_todo.at(t.m_label).m_is_done, opt));
  }
  auto todo_list_component = Container::Vertical(cur_todos);
  return todo_list_component;
}
void TList::updateTodo(const std::string &ymd, const std::string &label,
                       const bool &b) {
  m_tododb->updateTodo(ymd, label, b);
}

void TList::redraw_todolist() {
  ComponentBase::DetachAllChildren();
  Add(generate_todolist());
}
Element TList::Render() {

  if (date_changed()) {
    redraw_todolist();
  }
  if (m_map_todo.size() !=
      m_tododb->getTodos(get_date_string_format(m_ymd())).size()) {
    redraw_todolist();
  }
  auto currentTime =
      std::chrono::floor<std::chrono::seconds>(std::chrono::file_clock::now());
  if (m_tododb->isDBUpdated(currentTime)) {
    redraw_todolist();
  }
  m_prev_ymd = m_ymd();
  return ComponentBase::Render() | m_list_size | vscroll_indicator | yframe |
         border;
}

Component TListWidget(YearMonthDay *ymd, TodoDB *db, Decorator list_size) {
  return Make<TList>(ymd, db, list_size);
}
