#ifndef TODO_CREATE_H
#define TODO_CREATE_H
#include "calendar.h"
#include "db.h"
#include <ftxui/component/component_base.hpp>

class TodoCreate : public ftxui::ComponentBase {
public:
	TodoCreate(ftxui::Ref<YearMonthDay> ymd, ftxui::Ref<TodoDB> db,
		ftxui::Decorator c_size, bool init_state);
	ftxui::Element Render() override;

private:
	std::string m_todolabel;
	bool m_todoinitstate;
	ftxui::Ref<YearMonthDay> m_ymd;
	ftxui::Decorator m_size;
	ftxui::Ref<TodoDB> m_tododb;
	void redraw_create_todo();
	ftxui::Component m_create_todo_component;
	ftxui::Component generate_create_todo();
	void addNewTodo();
};

ftxui::Component CreateTodoWidget(YearMonthDay* ymd, TodoDB* db,
	ftxui::Decorator c_size);

#endif
