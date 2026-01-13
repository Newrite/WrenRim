export module WrenRim.Wren.BindingManager;

import WrenRim.Wren.Wrappers.Actor;
import WrenRim.Wren.Wrappers.AlchemyItem;
import WrenRim.Wren.Wrappers.HitData;
import WrenRim.Wren.Wrappers.Weapon;
import WrenRim.Wren.Wrappers.ActiveEffect;
import WrenRim.Wren.Wrappers.Armor;
import WrenRim.Wren.Wrappers.UI;
import WrenRim.Wren.Wrappers.GFxValue;
import WrenRim.Wren.Wrappers.Spell;

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
    }

}
