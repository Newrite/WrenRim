module;
#include "pch.h"
#include <iostream>

export module WrenRim.Wren.ScriptEngine;

import WrenRim.Config;
import WrenRim.API.Wrappers;

namespace wren::script_engine
{
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

            logger::info("Initializing Wren ScriptEngine...");

            api::wrappers::bind_wrappers(vm_);
            
            logger::info("Wren ScriptEngine initialized.");
        }

        template<typename... Args>
        bool dispatch(const std::string& event_name, Args&&... args)
        {
            if (!config::manager::get_singleton()->is_enabled()) return true;

            // TODO: Implement actual Wren call using wrenbind17
            // auto events = vm_.find("core/events", "Events");
            // auto result = events.func("dispatch")(event_name, std::forward<Args>(args)...);
            
            return true; 
        }
        
        void run_string(const std::string& code) {
             try {
                // Use runFromSource instead of run
                vm_.runFromSource("main", code);
             } catch (const std::exception& e) {
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

        wrenbind17::VM vm_;
    };
}
