module;

#include "library/SKSEMenuFramework.h"
#include <string>

export module WrenRim.UI.SKSEMenu;

import WrenRim.Config;
import WrenRim.Wren.ScriptEngine;

namespace ui::skse_menu
{
  using namespace ImGuiMCP;
  
  auto __stdcall render_main() -> void
  {
      auto cfg = config::manager::get_singleton();
      auto engine = wren::script_engine::engine::get_singleton();

      if (cfg) {
          ImGui::Text("Status: %s", cfg->is_enabled() ? "Enabled" : "Disabled");
          ImGui::Text("Logging: %s", cfg->is_logging_enabled() ? "On" : "Off");
          ImGui::Text("Heap: %zu MB (Initial: %zu MB)", cfg->get_max_heap_size() / (1024 * 1024), cfg->get_initial_heap_size() / (1024 * 1024));
          ImGui::Text("Frame Budget: %zu us", cfg->get_max_frame_time_budget_us());
      }

      if (ImGui::Button("Hot Reload User Scripts")) {
          if (engine) {
              engine->reload_user_mods();
          }
      }
  }

  export auto register_skse_menu() -> void
  {
    if (!SKSEMenuFramework::IsInstalled()) {
      logger::warn("SKSEMenuFramework not installed");
      return;
    }

    static constexpr auto main_title = "WrenRim";
    SKSEMenuFramework::SetSection(main_title);
    
    static constexpr auto config_title = "Config";
    SKSEMenuFramework::AddSectionItem(config_title, render_main);
    
  }
}
