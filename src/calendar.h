#ifndef CAL_H
#define CAL_H
#include <chrono>
#include <ftxui/component/component_base.hpp>
#include <unordered_map>
#include <vector>

struct Day {
  std::chrono::day m_day;
  std::chrono::weekday m_weekday;
};

struct Month {
  std::chrono::month m_month;
  std::vector<Day> m_days_in_month;
};

struct Year {
  std::chrono::year m_year;
  std::vector<Month> m_months_in_year;
};

struct YearMonthDay {
  std::chrono::year c_year;
  std::chrono::month c_month;
  std::chrono::day c_day;
  std::chrono::weekday c_weekday;
};

class Cal {
public:
  Cal();
  Cal(int s_year, int e_year);
  void init_cal();
  std::unordered_map<int, Year> get_cal();

private:
  std::unordered_map<int, Year> cal_year;
  int start_year;
  int end_year;
};

void populate_with_days(Year &y);
void populate_with_months(Year &y);
std::string get_month_strings(unsigned m);
std::string get_weekday_strings(unsigned w);
YearMonthDay get_current_date();
std::string get_date_string_format(const YearMonthDay &ymd);
bool isDateLessThanToday(const YearMonthDay &lhs, const YearMonthDay &rhs);
YearMonthDay Chrono_To_YMD(std::chrono::year_month_day ymd);
std::chrono::year_month_day YMD_To_Chrono(YearMonthDay ymd);
YearMonthDay get_date_ymd_format(const std::string &d);

#endif
