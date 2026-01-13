module;

#include "library/mINI.h"
#include "pch.h"

export module WrenRim.Config;

namespace config
{
	export class manager
	{
	public:
		static manager* get_singleton()
		{
			static manager singleton;
			return &singleton;
		}

		void load()
		{
			auto path = std::filesystem::path("Data/SKSE/Plugins/zzWrenRim.ini");
			mINI::INIFile file(path.string());
			mINI::INIStructure ini;

			if (!std::filesystem::exists(path)) {
				logger::info("Config file not found, generating defaults.");
				generate_default(file, ini);
			}
			else {
				file.read(ini);
			}

			enabled = parse_bool(ini["General"]["bEnabled"], true);
			enable_logging = parse_bool(ini["General"]["bEnableLogging"], true);

			// Memory
			// Convert MB to Bytes
			max_heap_size = parse_size_t(ini["Memory"]["iMaxHeapSizeMB"], 64) * 1024 * 1024;
			initial_heap_size = parse_size_t(ini["Memory"]["iInitialHeapSizeMB"], 8) * 1024 * 1024;

			// Performance
			max_frame_time_budget_us = parse_size_t(ini["Performance"]["iMaxFrameTimeBudgetUs"], 2000);

			logger::info("Config loaded: Enabled={}, Heap={}MB, Budget={}us",
				enabled, max_heap_size / (1024 * 1024), max_frame_time_budget_us);
		}

		[[nodiscard]] bool is_enabled() const { return enabled; }
		[[nodiscard]] bool is_logging_enabled() const { return enable_logging; }
		[[nodiscard]] size_t get_max_heap_size() const { return max_heap_size; }
		[[nodiscard]] size_t get_initial_heap_size() const { return initial_heap_size; }
		[[nodiscard]] size_t get_max_frame_time_budget_us() const { return max_frame_time_budget_us; }

	private:
		manager() = default;
		~manager() = default;
		manager(const manager&) = delete;
		manager(manager&&) = delete;
		manager& operator=(const manager&) = delete;
		manager& operator=(manager&&) = delete;

		bool enabled{ true };
		bool enable_logging{ true };
		size_t max_heap_size{ 64 * 1024 * 1024 };
		size_t initial_heap_size{ 8 * 1024 * 1024 };
		size_t max_frame_time_budget_us{ 2000 };

		void generate_default(const mINI::INIFile& file, mINI::INIStructure& ini)
		{
			ini["General"]["bEnabled"] = "true";
			ini["General"]["bEnableLogging"] = "true";
			ini["Memory"]["iMaxHeapSizeMB"] = "64";
			ini["Memory"]["iInitialHeapSizeMB"] = "8";
			ini["Performance"]["iMaxFrameTimeBudgetUs"] = "2000";
			if (!file.generate(ini, true)) {
				logger::warn("Failed to generate default config file.");
			}
		}

		bool parse_bool(const std::string& val, bool def)
		{
			if (val.empty()) return def;
			std::string v = val;
			std::transform(v.begin(), v.end(), v.begin(), ::tolower);
			return v == "true" || v == "1" || v == "on";
		}

		size_t parse_size_t(const std::string& val, size_t def)
		{
			if (val.empty()) return def;
			try {
				return std::stoull(val);
			}
			catch (...) {
				return def;
			}
		}
	};
}
