module;

#include "pch.h"

export module WrenRim.Events;

import WrenRim.Events.MenuEvent;

namespace events
{
  export auto register_events() -> void
  {
    menu_event::menu_event_handler::register_handler();
  }
}
