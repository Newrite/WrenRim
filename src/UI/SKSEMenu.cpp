module;

#include "library/SKSEMenuFramework.h"
#include <string>

export module WrenRim.UI.SKSEMenu;

namespace ui::skse_menu
{
  using namespace ImGuiMCP;

  export auto register_skse_menu() -> void
  {
    if (!SKSEMenuFramework::IsInstalled()) {
      logger::warn("SKSEMenuFramework not installed");
      return;
    }

    static constexpr auto main_title = "WrenRim";
    SKSEMenuFramework::SetSection(main_title);
    
  }
}
