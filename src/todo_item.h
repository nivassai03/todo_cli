#ifndef TODO_ITEM_H
#define TODO_ITEM_H

#include "calendar.h"
#include <string>

struct Todo {
  std::string m_label;
  bool m_is_done;
  YearMonthDay m_deadline;
};

#endif
