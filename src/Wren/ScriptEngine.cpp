export module WrenRim.Wren.ScriptEngine;

import WrenRim.Config;
import WrenRim.API.Wrappers;

namespace wren::script_engine
{
	namespace fs = std::filesystem;

	export class engine
	{
	public:
		static engine* get_singleton()
		{
			static engine singleton;
			return &singleton;
		}

		void initialize()
		{
			auto cfg = config::manager::get_singleton();
			if (!cfg->is_enabled()) return;

			if (vm_) {
				logger::warn("Wren ScriptEngine already initialized. Use reload_user_mods() for a full reset.");
				return;
			}

			logger::info("Initializing Wren ScriptEngine...");

			// Create new VM instance
			vm_ = std::make_unique<wrenbind17::VM>(std::vector<std::string>{ 
				"Data/SKSE/Plugins/WrenRim/Std", 
				"Data/SKSE/Plugins/WrenRim/WrenMods" 
			});

			// Configure Module Resolution
			vm_->setPathResolveFunc([](const std::vector<std::string>&, const std::string&, const std::string& name) -> std::string {
				return name;
			});

			vm_->setLoadFileFunc([](const std::string& name) -> std::string {
				const std::vector<std::string> search_paths = {
					"Data/SKSE/Plugins/WrenRim/Std",
					"Data/SKSE/Plugins/WrenRim/WrenMods"
				};
				
				for (const auto& base_path : search_paths) {
					auto full_path = fs::path(base_path) / (name + ".wren");
					if (fs::exists(full_path)) {
						std::ifstream t(full_path);
						return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
					}
				}
				
				throw std::runtime_error("Module not found: " + name);
			});

			// Register Bindings (C++ -> Wren)
			api::wrappers::bind_wrappers(*vm_);
		  

			// 1. Pre-load Standard Library
			try {
				logger::info("Loading Standard Library...");
			  
			  // 1. Настройка логгера (чтобы System.print писал в лог SKSE)
			  vm_->setPrintFunc([](const char* text) {
				if (text[0] == '\n' && text[1] == '\0') return;
                SKSE::log::info("[Wren] {}", text);
              });
				
				// Core Events
				vm_->runFromModule("Events");

				// Skyrim Types (Essential for Hooks)
				// We load them so their methods (foreign class ...) are registered 
				// before any event passes these objects to scripts.
				vm_->runFromModule("Skyrim/Actor");
				vm_->runFromModule("Skyrim/Potion");
			}
			catch (const std::exception& e) {
				logger::error("Failed to load Standard Library: {}", e.what());
				return; // Critical failure
			}

			// 2. Load User Scripts from "Data/SKSE/Plugins/WrenRim/WrenMods"
			try {
				const fs::path mods_path = "Data/SKSE/Plugins/WrenRim/WrenMods";
				if (fs::exists(mods_path) && fs::is_directory(mods_path)) {
					logger::info("Loading User Mods from: {}", mods_path.string());
					
					for (const auto& entry : fs::directory_iterator(mods_path)) {
						if (entry.is_regular_file() && entry.path().extension() == ".wren") {
							logger::info("  Running script: {}", entry.path().filename().string());
							try {
								std::string modName = entry.path().stem().string();
								vm_->runFromFile(modName, entry.path().string());
							}
							catch (const std::exception& e) {
								logger::error("    Error running script {}: {}", entry.path().filename().string(), e.what());
							}
						}
					}
				}
				else {
					logger::warn("User mods directory not found: {}", mods_path.string());
				}
			}
			catch (const std::exception& e) {
				logger::error("Error scanning user mods directory: {}", e.what());
			}

			logger::info("Wren ScriptEngine initialized.");
		}

		void shutdown()
		{
			if (vm_) {
				logger::info("Shutting down Wren ScriptEngine...");
				vm_.reset();
				logger::info("Wren ScriptEngine shut down.");
			}
		}

		void reload_user_mods()
		{
			logger::info("Performing Nuclear Reload of Wren VM...");
			shutdown();
			initialize();
			logger::info("Nuclear Reload complete.");
		}
		
		void on_frame_start()
		{
			accumulated_time_us_ = 0;
		}

		template<typename... Args>
		void dispatch(const std::string& event_name, Args&&... args)
		{
			auto cfg = config::manager::get_singleton();
			if (!cfg->is_enabled()) return;
			if (!vm_) return;

			// Time Budget Check
			if (accumulated_time_us_ > cfg->get_max_frame_time_budget_us()) {
				// Budget exceeded, dropping event
				return;
			}

			auto start = std::chrono::high_resolution_clock::now();

			try {
				// Look for "Events" class in "Events" module
				auto events_class = vm_->find("Events", "Events");
				
				// Generate signature: dispatch(_,_,...)
				std::string signature = "dispatch(";
				constexpr size_t arg_count = 1 + sizeof...(Args);
				for (size_t i = 0; i < arg_count; ++i) {
					if (i > 0) signature += ",";
					signature += "_";
				}
				signature += ")";

				auto dispatch_func = events_class.func(signature);
				dispatch_func(event_name, std::forward<Args>(args)...);
			}
			catch (const std::exception& e) {
				logger::error("Wren Error in {}: {}", event_name, e.what());
			}
			catch (...) {
				logger::error("Unknown Wren Error in {}", event_name);
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			accumulated_time_us_ += duration;

			//if (duration > 10) {
				logger::info("[Wren] Event {} took {}us", event_name, duration);
			//}
		}

		void run_string(const std::string& code) {
			if (!vm_) return;
			try {
				vm_->runFromSource("main", code);
			}
			catch (const std::exception& e) {
				logger::error("Wren Error: {}", e.what());
			}
		}

	private:
		engine() = default;
		~engine() = default;
		engine(const engine&) = delete;
		engine(engine&&) = delete;
		engine& operator=(const engine&) = delete;
		engine& operator=(engine&&) = delete;

		std::unique_ptr<wrenbind17::VM> vm_;
		size_t accumulated_time_us_{ 0 };
	};
}
