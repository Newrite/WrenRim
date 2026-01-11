import WrenRim.Core.LoggerSetup;
import WrenRim.UI.Prisma;
import WrenRim.UI.SKSEMenu;
import WrenRim.Events;
import WrenRim.Core.Hooks;
import WrenRim.Core.ActorsCache;
import WrenRim.Config;
import WrenRim.Papyrus;

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
    config::config_manager::get_singleton()->initialize();
    core::hooks::install_hooks();
    ui::prisma::initialize();
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
  
  const auto papyrus_vm = SKSE::GetPapyrusInterface();
  if (!papyrus_vm || !papyrus_vm->Register(papyrus::Register)) {
    logger::info("papyrus_vm is null or can't register papyrus functions"sv);
    return false;
  }

  const auto serialization = SKSE::GetSerializationInterface();
  if (!serialization) {
    logger::info("Serialization interface is null"sv);
    return false;
  }

  serialization->SetUniqueID('TFNG');
  serialization->SetSaveCallback(core::actors_cache::cache_data::skse_save_callback);
  serialization->SetLoadCallback(core::actors_cache::cache_data::skse_load_callback);

  logger::info("{} has finished loading.", plugin->GetName());

  return true;
}
