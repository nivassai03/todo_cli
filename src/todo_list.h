#ifndef TOL_H_
#define TOL_H_
#include "calendar.h"
#include "db.h"
#include "todo_item.h"
#include <ftxui/component/component_base.hpp>
#include <unordered_map>

class TList : public ftxui::ComponentBase {
public:
  TList(ftxui::Ref<YearMonthDay> ymd, ftxui::Ref<TodoDB> db,
        ftxui::Decorator list_size);
  ftxui::Element Render() override;

private:
  nlohmann::json m_todo_list;
  ftxui::Ref<YearMonthDay> m_ymd;
  std::unordered_map<std::string, Todo> m_map_todo;
  YearMonthDay m_prev_ymd;
  ftxui::Decorator m_list_size;
  ftxui::Component generate_todolist();
  ftxui::Components generate_todolists();
  //ftxui::Component m_todo_container;
  ftxui::Ref<TodoDB> m_tododb;
  void updateTodo(const std::string &ymd, const std::string &label,
                  const bool &b);
  void update_and_save();
  void redraw_todolist();
  bool date_changed();
};

ftxui::Component TListWidget(YearMonthDay *ymd, TodoDB *db,
                             ftxui::Decorator list_size);

#endif // TOL_H_
