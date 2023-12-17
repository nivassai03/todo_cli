#ifndef CAL_DISPLAY_H
#define CAL_DISPLAY_H

#include "calendar.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/util/ref.hpp>

class CalDisplay : public ftxui::ComponentBase {
public:
  CalDisplay(ftxui::Ref<YearMonthDay> ymd, ftxui::Decorator display_size);
  ftxui::Element Render() override;

private:
  ftxui::Ref<YearMonthDay> m_ymd;
  ftxui::Decorator m_display_size;
  std::string generate_date_string();
};
ftxui::Component CalendarDisplay(YearMonthDay *ymd,
                                 ftxui::Decorator display_size);

#endif
