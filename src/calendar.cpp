#include "calendar.h"
#include <chrono>
#include <iomanip>
#include <unordered_map>
#include <utility>

using namespace std::chrono;

void populate_with_days(Year &y) {
  for (Month &m : y.m_months_in_year) {
    year_month_day_last ymdl{last / m.m_month / y.m_year};
    for (day d{1}; d <= ymdl.day(); d++) {
      Day d_day;
      d_day.m_day = d;
      d_day.m_weekday = weekday{d / m.m_month / y.m_year};
      m.m_days_in_month.push_back(d_day);
    }
  }
}

void populate_with_months(Year &y) {
  for (unsigned i = 1; i <= 12; i++) {
    Month m;
    m.m_month = month{i};
    y.m_months_in_year.push_back(m);
  }
}

Year init_year(const year &y) {
  Year cal_year;
  cal_year.m_year = y;
  populate_with_months(cal_year);
  populate_with_days(cal_year);
  return cal_year;
}

Cal::Cal() : start_year(2000), end_year(2050) {
}
Cal::Cal(int s_year, int e_year) : start_year(s_year), end_year(e_year) {
}

void Cal::init_cal() {
  for (int i = start_year; i <= end_year; i++) {
    Year y = init_year(year{i});
    cal_year.insert(std::make_pair(i, y));
  }
}

std::string get_month_strings(unsigned m) {
  switch (m) {
  case 1:
    return std::string("Jan");
    break;

  case 2:
    return std::string("Feb");
    break;

  case 3:
    return std::string("Mar");
    break;

  case 4:
    return std::string("Apr");
    break;

  case 5:
    return std::string("May");
    break;

  case 6:
    return std::string("Jun");
    break;

  case 7:
    return std::string("Jul");
    break;

  case 8:
    return std::string("Aug");
    break;

  case 9:
    return std::string("Sep");
    break;

  case 10:
    return std::string("Oct");
    break;

  case 11:
    return std::string("Nov");
    break;

  case 12:
    return std::string("Dec");
    break;
  default:
    return std::string("");
  }
}

std::string get_weekday_strings(unsigned w) {
  switch (w) {
  case 0:
    return std::string("Sun");
    break;

  case 1:
    return std::string("Mon");
    break;

  case 2:
    return std::string("Tue");
    break;

  case 3:
    return std::string("Wed");
    break;

  case 4:
    return std::string("Thu");
    break;

  case 5:
    return std::string("Fri");
    break;

  case 6:
    return std::string("Sat");
    break;
  default:
    return std::string("");
  }
}

std::unordered_map<int, Year> Cal::get_cal() { return cal_year; }

YearMonthDay get_current_date() {

  auto now = system_clock::now();
  sys_days no_of_days = floor<days>(now);
  year_month_day y_m_d{no_of_days};

  YearMonthDay ymd;
  ymd.c_day = y_m_d.day();
  ymd.c_month = y_m_d.month();
  ymd.c_year = y_m_d.year();
  ymd.c_weekday = weekday{no_of_days};
  return ymd;
}

std::string get_date_string_format(const YearMonthDay &ymd) {

  std::string date = std::to_string(static_cast<int>(ymd.c_year)) + "/" +
                     std::to_string(static_cast<unsigned>(ymd.c_month)) + "/" +
                     std::to_string(static_cast<unsigned>(ymd.c_day));

  return date;
}

bool isDateLessThanToday(const YearMonthDay &lhs, const YearMonthDay &rhs) {
  if (lhs.c_year < rhs.c_year && lhs.c_month < rhs.c_month &&
      lhs.c_day < rhs.c_day) {
    return true;
  } else {
    return true;
  }
}

YearMonthDay Chrono_To_YMD(std::chrono::year_month_day ymd) {
  YearMonthDay temp_date;
  temp_date.c_year = ymd.year();
  temp_date.c_month = ymd.month();
  temp_date.c_day = ymd.day();
  temp_date.c_weekday = weekday{ymd};
  return temp_date;
}

year_month_day YMD_To_Chrono(YearMonthDay ymd) {
  return year_month_day{ymd.c_year / ymd.c_month / ymd.c_day};
}

YearMonthDay get_date_ymd_format(const std::string &d) {
  std::istringstream ss{d};
  std::tm tm = {};
  ss >> std::get_time(&tm, "%Y/%m/%d");
  int date_y = static_cast<int>(tm.tm_year + 1900);
  unsigned date_m = static_cast<unsigned>(tm.tm_mon + 1);
  unsigned date_d = static_cast<unsigned>(tm.tm_mday);

  year_month_day ymd{year{date_y} / month{date_m} / day{date_d}};
  return Chrono_To_YMD(ymd);
}
