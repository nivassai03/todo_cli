#include "calendar.h"
#include "date_display.h"
#include "date_selection.h"
#include "todo_create.h"
#include "todo_list.h"
#include "todo_list_due.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std::chrono;
using namespace ftxui;

struct DateLocation {
  int x=0;
  int y=0;
};

int main() {


  auto screen = ScreenInteractive::Fullscreen();


  // DateLocation
  DateLocation dxy;


  // intialize db
  TodoDB db;
  db.init("todo.json");

  // Setup date object(state)
  YearMonthDay ymd;
  ymd = get_current_date();

  // Calendar Widget
  std::unordered_map<int, Year> cal_years = get_cal(2000,2050);
  auto cal_widget =
      CalendarWidget(cal_years, ymd.c_year, ymd.c_month, ymd.c_day, &dxy.x, &dxy.y,
                     size(WIDTH, EQUAL, 10), size(HEIGHT, EQUAL, 2),
                     size(WIDTH, EQUAL, 45), &ymd);

  // setup Calendar Display Widget
  auto cal_disp = CalendarDisplay(&ymd, size(WIDTH, EQUAL, 45));

  // setup Todo List Widget
  auto todo_list = TListWidget(&ymd, &db, size(WIDTH, EQUAL, 65));

  // // setup create Todo Widget
  auto create_todo = CreateTodoWidget(&ymd, &db, size(WIDTH, EQUAL, 70));

  auto incomplete_tasks_header = text("Due Tasks") | bold | center |
                                 bgcolor(Color::Red) | border |
                                 size(WIDTH, EQUAL, 70);

  auto incomplete_tasks = TDListWidget(&ymd, &db, size(WIDTH, EQUAL, 70));

  auto todo_cli_component = Container::Horizontal({
      Container::Vertical({cal_disp, cal_widget}),
      Container::Vertical({create_todo, todo_list}),
      Container::Vertical({incomplete_tasks}),
  });

  auto renderer = Renderer(todo_cli_component, [&] {
    return hbox(
        {vbox({
             cal_disp->Render(),
             cal_widget->Render(),
         }),
         emptyElement() | size(WIDTH, EQUAL, 2),
         vbox({
             create_todo->Render(),
             todo_list->Render() | size(HEIGHT, EQUAL, Terminal::Size().dimy),
         }),
         emptyElement() | size(WIDTH, EQUAL, 2),
         vbox({
             incomplete_tasks_header,
             incomplete_tasks->Render() |
                 size(HEIGHT, EQUAL, Terminal::Size().dimy),
         })});
      });
  // });
  screen.Loop(renderer);
}
