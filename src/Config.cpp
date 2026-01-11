module;
#include <filesystem>
#include <string>
#include <algorithm>
#include <mINI.h>
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
            mINI::INIFile file(path);
            mINI::INIStructure ini;

            if (!std::filesystem::exists(path)) {
                logger::info("Config file not found, generating defaults.");
                generate_default(file, ini);
            } else {
                file.read(ini);
            }

            enabled = parse_bool(ini["General"]["bEnabled"], true);
            enable_logging = parse_bool(ini["General"]["bEnableLogging"], true);
            max_heap_size_mb = parse_int(ini["Memory"]["iMaxHeapSizeMB"], 64);
            initial_heap_size_mb = parse_int(ini["Memory"]["iInitialHeapSizeMB"], 8);
            max_frame_time_budget_us = parse_int(ini["Performance"]["iMaxFrameTimeBudgetUs"], 2000);

            logger::info("Config loaded: Enabled={}, Heap={}MB, Budget={}us", 
                enabled, max_heap_size_mb, max_frame_time_budget_us);
        }

        [[nodiscard]] bool is_enabled() const { return enabled; }
        [[nodiscard]] bool is_logging_enabled() const { return enable_logging; }
        [[nodiscard]] int get_max_heap_size_mb() const { return max_heap_size_mb; }
        [[nodiscard]] int get_initial_heap_size_mb() const { return initial_heap_size_mb; }
        [[nodiscard]] int get_max_frame_time_budget_us() const { return max_frame_time_budget_us; }

    private:
        manager() = default;
        ~manager() = default;
        manager(const manager&) = delete;
        manager(manager&&) = delete;
        manager& operator=(const manager&) = delete;
        manager& operator=(manager&&) = delete;

        bool enabled{ true };
        bool enable_logging{ true };
        int max_heap_size_mb{ 64 };
        int initial_heap_size_mb{ 8 };
        int max_frame_time_budget_us{ 2000 };

        void generate_default(const mINI::INIFile& file, mINI::INIStructure& ini)
        {
            ini["General"]["bEnabled"] = "true";
            ini["General"]["bEnableLogging"] = "true";
            ini["Memory"]["iMaxHeapSizeMB"] = "64";
            ini["Memory"]["iInitialHeapSizeMB"] = "8";
            ini["Performance"]["iMaxFrameTimeBudgetUs"] = "2000";
            file.generate(ini, true);
        }

        bool parse_bool(const std::string& val, bool def)
        {
            if (val.empty()) return def;
            std::string v = val;
            std::transform(v.begin(), v.end(), v.begin(), ::tolower);
            return v == "true" || v == "1" || v == "on";
        }

        int parse_int(const std::string& val, int def)
        {
            if (val.empty()) return def;
            try {
                return std::stoi(val);
            } catch (...) {
                return def;
            }
        }
    };
}
