CMAKE_MINIMUM_REQUIRED(VERSION 3.20)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

project(todo-cli)

add_executable(todo)


target_sources(todo PRIVATE src/main.cpp src/calendar.h src/calendar.cpp src/date_selection.h src/date_selection.cpp src/date_display.cpp src/date_display.h src/db.h src/db.cpp src/todo_create.h src/todo_create.cpp src/todo_item.h src/todo_list.h src/todo_list.cpp src/todo_list_due.cpp src/todo_list_due.h)

#target_compile_options(todo PRIVATE -Wpedantic -Wconversion)

target_link_libraries(todo PRIVATE ftxui::screen ftxui::dom ftxui::component nlohmann_json::nlohmann_json)
