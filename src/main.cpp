#include "pch.h"

import WrenRim.Core.LoggerSetup;
import WrenRim.UI.SKSEMenu;
import WrenRim.Events;
import WrenRim.Core.Hooks;
import WrenRim.Config;
import WrenRim.Wren.ScriptEngine;

auto skse_message_handle(SKSE::MessagingInterface::Message* message) -> void
{
  switch (message->type) {
  case SKSE::MessagingInterface::kPostLoad: {
    break;
  }
  case SKSE::MessagingInterface::kPostPostLoad: {
    break;
  }
  case SKSE::MessagingInterface::kInputLoaded: {
    break;
  }
  case SKSE::MessagingInterface::kDataLoaded: {
    // 1. Load Config
    config::manager::get_singleton()->load();

    // 2. Init Script Engine
    wren::script_engine::engine::get_singleton()->initialize();

    // 3. Install Hooks
    core::hooks::install_hooks();

    // 4. Register Events
    events::register_events();
    break;
  }
  case SKSE::MessagingInterface::kNewGame:
  case SKSE::MessagingInterface::kPreLoadGame:
  case SKSE::MessagingInterface::kPostLoadGame:
  case SKSE::MessagingInterface::kSaveGame:
  case SKSE::MessagingInterface::kDeleteGame:
  default:
    break;
  }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
  core::logger_setup::setup_log();

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  logger::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());


  SKSE::Init(skse);

  SKSE::GetMessagingInterface()->RegisterListener(skse_message_handle);
  ui::skse_menu::register_skse_menu();

  const auto serialization = SKSE::GetSerializationInterface();
  if (!serialization) {
    logger::info("Serialization interface is null"sv);
    return false;
  }

  logger::info("{} has finished loading.", plugin->GetName());

  return true;
}
