#include "date_display.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>

using namespace ftxui;

CalDisplay::CalDisplay(Ref<YearMonthDay> ymd, Decorator display_size)
	: m_ymd(ymd), m_display_size(display_size) {
	Add(Renderer([&] { return text(generate_date_string()); }));
}
Element CalDisplay::Render() {
	return hbox({
			   ComponentBase::Render(),
		}) |
		center | bold | border | m_display_size;
}

std::string CalDisplay::generate_date_string() {
	std::string cur_date =
		std::to_string(static_cast<unsigned>(m_ymd->c_day)) + " " +
		get_month_strings(static_cast<unsigned>(m_ymd->c_month)) + " " +
		std::to_string(static_cast<int>(m_ymd->c_year)) + " (" +
		get_weekday_strings(
			static_cast<unsigned>(m_ymd->c_weekday.c_encoding())) +
		")";
	return cur_date;
}
Component CalendarDisplay(YearMonthDay* ymd, Decorator display_size) {
	return Make<CalDisplay>(ymd, display_size);
}
