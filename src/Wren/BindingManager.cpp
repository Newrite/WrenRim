module;

#include "pch.h"
#include "Wren/Wrappers/Wrappers.hpp"

export module WrenRim.Wren.BindingManager;


namespace wren::binding_manager {

 export void bind_wrappers(wrenbind17::VM& vm) {
        // Bind Actor to "Skyrim/Actor"
        auto& mActor = vm.module("Skyrim/Actor");
        wrappers::actor::bind(mActor);

        // Bind Potion to "Skyrim/AlchemyItem"
        auto& mAlchemyItem = vm.module("Skyrim/AlchemyItem");
        wrappers::alchemy_item::bind(mAlchemyItem);

        // Bind HitData to "Skyrim/HitData"
        auto& mHitData = vm.module("Skyrim/HitData");
        wrappers::hit_data::bind(mHitData);

        // Bind Weapon to "Skyrim/Weapon"
        auto& mWeapon = vm.module("Skyrim/Weapon");
        wrappers::weapon::bind(mWeapon);

        // Bind Armor to "Skyrim/Armor"
        auto& mArmor = vm.module("Skyrim/Armor");
        wrappers::armor::bind(mArmor);

        // Bind ActiveEffect to "Skyrim/ActiveEffect"
        auto& mActiveEffect = vm.module("Skyrim/ActiveEffect");
        wrappers::active_effect::bind(mActiveEffect);

        // Bind UI to "Skyrim/UI"
        auto& mUI = vm.module("Skyrim/UI");
        wrappers::ui::bind(mUI);

        // Bind GFxValue to "Skyrim/GFxValue"
        auto& mGFxValue = vm.module("Skyrim/GFxValue");
        wrappers::gfx_value::bind(mGFxValue);

        // Bind Spell to "Skyrim/Spell"
        auto& mSpell = vm.module("Skyrim/Spell");
        wrappers::spell::bind(mSpell);

        // Bind Keyword to "Skyrim/Keyword"
        auto& mKeyword = vm.module("Skyrim/Keyword");
        wrappers::keyword::bind(mKeyword);

        // Bind Game to "Skyrim/Game"
        auto& mGame = vm.module("Skyrim/Game");
        wrappers::game::bind(mGame);

        // Bind Setting to "Skyrim/Setting"
        auto& mSetting = vm.module("Skyrim/Setting");
        wrappers::setting::bind(mSetting);

        // Bind Effect to "Skyrim/Effect"
        auto& mEffect = vm.module("Skyrim/Effect");
        wrappers::effect::bind(mEffect);
    }

    export void load_core_modules(wrenbind17::VM& vm) {
        // Core Events
        vm.runFromModule("Events");

        // Skyrim Types (Essential for Hooks)
        // We load them so their methods (foreign class ...) are registered
        // before any event passes these objects to scripts.
        SKSE::log::info("Loading Skyrim Types...");
        vm.runFromModule("Skyrim/Actor");
        vm.runFromModule("Skyrim/AlchemyItem");
        vm.runFromModule("Skyrim/HitData");
        vm.runFromModule("Skyrim/Weapon");
        vm.runFromModule("Skyrim/Armor");
        vm.runFromModule("Skyrim/ActiveEffect");
        vm.runFromModule("Skyrim/UI");
        vm.runFromModule("Skyrim/GFxValue");
        vm.runFromModule("Skyrim/Spell");
        vm.runFromModule("Skyrim/Keyword");
        vm.runFromModule("Skyrim/Game");
        vm.runFromModule("Skyrim/Setting");
        vm.runFromModule("Skyrim/Effect");
    }

}
