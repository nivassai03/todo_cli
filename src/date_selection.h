#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H
#include "calendar.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <unordered_map>

// using namespace ftxui;

ftxui::Component CalendarWidget(std::unordered_map<int, Year> &cal_year,
                                std::chrono::year yr, std::chrono::month mn,
                                std::chrono::day d, int *x, int *y,
                                ftxui::Decorator d_size,
                                ftxui::Decorator d_height,
                                ftxui::Decorator cal_size, YearMonthDay *ymd);
ftxui::Component CalendarWidget(std::unordered_map<int, Year> &cal_year,
                                YearMonthDay *ymd);

class Calendar : public ftxui::ComponentBase {
public:
  Calendar(std::unordered_map<int, Year> c_years, std::chrono::year cal_year,
           std::chrono::month cal_month, std::chrono::day cal_day,
           ftxui::Ref<int> vselector, ftxui::Ref<int> hselector,
           ftxui::Decorator d_size, ftxui::Decorator d_height,
           ftxui::Decorator cal_size, ftxui::Ref<YearMonthDay> ymd);
  ftxui::Element Render() override;

private:
  std::unordered_map<int, Year> m_cal_years;
  std::chrono::year m_year;
  std::chrono::month m_month;
  std::chrono::day m_day;
  ftxui::Ref<int> m_vselector;
  ftxui::Ref<int> m_hselector;
  ftxui::Ref<int> m_nav_selector;
  ftxui::Decorator m_date_size;
  ftxui::Decorator m_date_height;
  ftxui::Decorator m_cal_size;
  ftxui::Ref<YearMonthDay> m_ymd;

  void update_current_year(std::chrono::year y);
  void update_current_month(std::chrono::month m);
  void update_current_day_and_weekday(std::chrono::day d);
  void increment_month();
  void decrement_month();
  ftxui::Component generate_dates_component();
  void redraw_cal();
};
#endif
