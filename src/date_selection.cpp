#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include "date_selection.h"
#include "ftxui/component/screen_interactive.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

using namespace ftxui;
using namespace std::chrono;

Calendar::Calendar(std::unordered_map<int, Year> c_years, year cal_year,
	month cal_month, day cal_day, Ref<int> vselector,
	Ref<int> hselector, Decorator d_size, Decorator d_height,
	Decorator cal_size, Ref<YearMonthDay> ymd)
	: m_cal_years(c_years), m_year(cal_year), m_month(cal_month),
	m_day(cal_day), m_vselector(vselector), m_hselector(hselector),
	m_date_size(d_size), m_date_height(d_height),
	m_cal_size(cal_size), m_ymd(ymd) {
	m_nav_selector = 0;
	update_current_year(m_year);
	update_current_month(m_month);
	update_current_day_and_weekday(m_day);
	auto weekdays_label = generate_weekdays_label();
	m_date_disp.push_back(generate_nav_component());
	m_date_disp.push_back(Renderer([=] { return emptyElement() | size(HEIGHT, EQUAL, 1); }));
	m_date_disp.push_back(Renderer([=] { return weekdays_label; }));
	m_date_disp.push_back(Renderer([=] { return emptyElement() | size(HEIGHT, EQUAL, 2); }));
	m_date_disp.push_back(generate_dates_component());
	Add(Container::Vertical(m_date_disp));

}

Element Calendar::generate_weekdays_label() {
	auto label = hbox({ text("Sun") | m_date_size, text("Mon") | m_date_size,
		  text("Tue") | m_date_size, text("Wed") | m_date_size,
		  text("Thu") | m_date_size, text("Fri") | m_date_size,
		  text("Sat") | m_date_size });
	return label;
}

Element Calendar::Render() {
	std::string year_month_str =
		std::to_string(static_cast<int>(m_year)) + " " +
		get_month_strings(static_cast<unsigned>(m_month));
	auto year_month_header = text(year_month_str) | center | bold | bgcolor(Color::Blue) | color(Color::White) | border;
	return window(text("Calendar") | vcenter | bold | size(HEIGHT, EQUAL, 20),
		{ vbox({
			ComponentBase::Render(),
   }) }) | m_cal_size | bold;
}

void Calendar::update_current_year(year y) { m_ymd->c_year = y; }
void Calendar::update_current_month(month m) { m_ymd->c_month = m; }
void Calendar::update_current_day_and_weekday(day d) {

	m_ymd->c_day = d;
	m_ymd->c_weekday = weekday{ year_month_day{m_year / m_month / d} };
}

void Calendar::increment_month() {
	if (m_month == month{ 12 }) {
		m_month++;
		m_year++;
	}
	else {
		m_month++;
	}
	update_current_month(m_month);
	update_current_year(m_year);
	update_current_day_and_weekday(day{ 1 });
}

void Calendar::decrement_month() {
	if (m_month == month{ 1 }) {
		m_month--;
		m_year--;
	}
	else {
		m_month--;
	}
	update_current_month(m_month);
	update_current_year(m_year);
	update_current_day_and_weekday(day{ 1 });
}

Component Calendar::generate_nav_component() {
	Components nav_bar_comp;
	nav_bar_comp.push_back(Button(
		"◄",
		[&] {
			decrement_month();
			redraw_cal();
		},
		ButtonOption::Simple()));
	nav_bar_comp.push_back(Renderer(filler));
	nav_bar_comp.push_back(Button(
		"►",
		[&] {
			increment_month();
			redraw_cal();
		},
		ButtonOption::Simple()));
	auto nav_bar = Container::Horizontal(nav_bar_comp, &m_nav_selector());
	return nav_bar;
}

Component Calendar::generate_dates_component() {
	auto yr_itr = m_cal_years.find(static_cast<int>(m_year));
	Year yr;
	int year_str;
	if (yr_itr != m_cal_years.end()) {
		yr = (*yr_itr).second;
		year_str = (*yr_itr).first;
	}
	for (const Month& m : yr.m_months_in_year) {
		if (m.m_month == m_month) {
			Components DaysRow;
			Components weekRow;
			for (const Day& d : m.m_days_in_month) {
				if (d.m_weekday == Sunday && static_cast<unsigned>(d.m_day) != 1u) {
					DaysRow.push_back(Container::Horizontal(weekRow, &m_hselector()));
					weekRow.clear();
				}
				unsigned idx = static_cast<unsigned>(d.m_weekday.c_encoding());
				if (static_cast<unsigned>(d.m_day) == 1u) {
					for (unsigned i = 0; i < idx; i++) {
						weekRow.push_back(Renderer(emptyElement) | m_date_size |
							m_date_height);
					}
				}
				auto btn = Button(
					std::to_string(static_cast<unsigned>(d.m_day)),
					[=, this] { update_current_day_and_weekday(d.m_day); },
					ButtonOption::Ascii());
				weekRow.push_back(btn | m_date_size | m_date_height);
			}
			for (std::size_t i = weekRow.size(); i < 7; i++) {
				weekRow.push_back(Renderer(emptyElement) | m_date_size | m_date_height);
			}

			DaysRow.push_back(Container::Horizontal(weekRow, &m_hselector()));
			for (std::size_t i = DaysRow.size(); i < 7; i++) {
				Components filler_row;
				for (int i = 0; i < 7; i++) {
					filler_row.push_back(Renderer(emptyElement) | m_date_size |
						m_date_height);
				}
				DaysRow.push_back(Container::Horizontal(filler_row));
			}
			auto days_component = Container::Vertical(DaysRow, &m_vselector());
			return days_component;
		}
	}
	return Renderer([=] {return text("Date Not Found" + std::to_string(year_str)); });
}
void Calendar::redraw_cal() {
	m_date_disp.pop_back();
	m_date_disp.push_back(generate_dates_component());
	DetachAllChildren();
	Add(Container::Vertical(m_date_disp));
}

Component CalendarWidget(std::unordered_map<int, Year>& cal_year, year yr,
	month mn, day d, int* x, int* y, Decorator d_size,
	Decorator d_height, Decorator cal_size,
	YearMonthDay* ymd) {

	return Make<Calendar>(cal_year, yr, mn, d, x, y, d_size, d_height, cal_size,
		ymd);
}

Component CalendarWidget(std::unordered_map<int, Year>& cal_year,
	YearMonthDay* ymd) {
	return Make<Calendar>(cal_year, year{ 2023 }, month{ 1 }, day{ 1 }, 0, 0,
		size(WIDTH, EQUAL, 5), size(HEIGHT, EQUAL, 1),
		size(WIDTH, EQUAL, 35), ymd);
}
