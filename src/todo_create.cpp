#include "todo_create.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

using namespace ftxui;

TodoCreate::TodoCreate(Ref<YearMonthDay> ymd, Ref<TodoDB> db, Decorator c_size,
                       bool init_state = false)
    : m_ymd(ymd), m_tododb(db), m_todoinitstate(false), m_size(c_size) {
  Add(generate_create_todo());
}

Component TodoCreate::generate_create_todo() {
  InputOption opt = InputOption();
  opt.multiline = false;
  opt.placeholder = "create todo...";
  opt.transform = [](InputState state) {
    auto temp_elem = state.element;
    state.element |= border;
    if (state.is_placeholder) {
      state.element |= dim;
    }
    if (state.hovered) {
      state.element = temp_elem;
      state.element |= borderStyled(Color::Blue);
    }
    return state.element;
  };
  opt.on_enter = [&] {
    addNewTodo();
    redraw_create_todo();
    m_todolabel = "";
  };
  return Input(&m_todolabel, opt);
}

void TodoCreate::redraw_create_todo() {
  ComponentBase::DetachAllChildren();
  Add(generate_create_todo());
}

Element TodoCreate::Render() { return ComponentBase::Render() | m_size; }
void TodoCreate::addNewTodo() {

  m_tododb->addTodo(get_date_string_format(m_ymd()), m_todolabel,
                    m_todoinitstate);
}

Component CreateTodoWidget(YearMonthDay *ymd, TodoDB *db, Decorator c_size) {
  return Make<TodoCreate>(ymd, db, c_size);
}
