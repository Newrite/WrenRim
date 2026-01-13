export module WrenRim.Core.Hooks;

import WrenRim.Core.HooksCtx;
import WrenRim.Config;
import WrenRim.Wren.ScriptEngine;
import WrenRim.Wren.Wrappers.Actor;
import WrenRim.Wren.Wrappers.AlchemyItem;
import WrenRim.Wren.Wrappers.ActiveEffect;
import WrenRim.Wren.Wrappers.HitData;

namespace core::hooks
{
  template<typename T>
  auto write_call(SKSE::Trampoline& trampoline, const uintptr_t src, T& func, REL::Relocation<T>& func_original,
                  const char* label) -> void
  {
    logger::info("Hook write_call: {}", label);
    func_original = trampoline.write_call<5>(src, func);
  }

  template<typename T>
  auto write_vfunc(REL::Relocation<> addr,
                   const uintptr_t offset,
                   T& func,
                   REL::Relocation<T>& func_original, const char* label) -> void
  {
    logger::info("Hook write_vfunc: {}", label);
    func_original = addr.write_vfunc(offset, func);
  }

  float last_player_delta = 0.1f;

  STATIC_STRUCT(character)

    static constexpr auto addr_vtable_character = RE::Character::VTABLE[0];
    static constexpr auto addr_vtable_player_character = RE::PlayerCharacter::VTABLE[0];
    static constexpr auto offset_vtable_update = RELOCATION_OFFSET(0xAD, 0xAD);
    static constexpr auto offset_vtable_drink_potion = RELOCATION_OFFSET(0x10F, 0x10F);
    static constexpr auto offset_vtable_remove_item = RELOCATION_OFFSET(0x56, 0x56);

    static auto on_update_character(RE::Character* character, const float delta) -> void
    {
      if (!character) {
        return on_update_character_original(character, delta);
      }

      wren::wrappers::actor wren_actor(character);

      wren::script_engine::engine::get_singleton()->dispatch("OnUpdateCharacterStart", wren_actor, last_player_delta);

      auto _ = hooks_ctx::on_actor_update{character, last_player_delta};
      on_update_character_original(character, delta);

      wren::script_engine::engine::get_singleton()->dispatch("OnUpdateCharacterEnd", wren_actor, last_player_delta);
    }

    static auto on_update_player_character(RE::PlayerCharacter* character, const float delta) -> void
    {
      wren::script_engine::engine::get_singleton()->on_frame_start();
      last_player_delta = delta;

      if (!character) {
        return on_update_player_character_original(character, delta);
      }

      wren::wrappers::actor wren_actor(character);

      wren::script_engine::engine::get_singleton()->dispatch("OnUpdatePlayerStart", wren_actor,
                                                             last_player_delta);

      auto _ = hooks_ctx::on_actor_update{character, last_player_delta};

      on_update_player_character_original(character, delta);

      wren::script_engine::engine::get_singleton()->dispatch("OnUpdatePlayerEnd", wren_actor,
                                                             last_player_delta);
    }

    static auto on_drink_potion_character(RE::Character* character, RE::AlchemyItem* potion,
                                          RE::ExtraDataList* extra_list) -> bool
    {
      if (!character || !potion || !config::manager::get_singleton()->is_enabled()) {
        return on_drink_potion_character_original(character, potion, extra_list);
      }

      wren::wrappers::actor wren_actor(character);
      wren::wrappers::alchemy_item wren_alchemy_item(potion);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionCharacterStart", wren_actor, wren_alchemy_item);

      bool result = on_drink_potion_character_original(character, potion, extra_list);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionCharacterEnd", wren_actor, wren_alchemy_item);

      return result;
    }

    static auto on_drink_potion_player_character(RE::PlayerCharacter* character, RE::AlchemyItem* potion,
                                                 RE::ExtraDataList* extra_list) -> bool
    {
      if (!character || !potion || !config::manager::get_singleton()->is_enabled()) {
        return on_drink_potion_player_character_original(character, potion, extra_list);
      }

      wren::wrappers::actor wren_actor(character);
      wren::wrappers::alchemy_item wren_alchemy_item(potion);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionPlayerStart", wren_actor, wren_alchemy_item);

      bool result = on_drink_potion_player_character_original(character, potion, extra_list);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionPlayerEnd", wren_actor, wren_alchemy_item);


      return result;
    }

    static auto on_remove_item_character(RE::Character* actor, RE::TESBoundObject* item, std::int32_t count,
                                         RE::ITEM_REMOVE_REASON reason, RE::ExtraDataList* extra_list,
                                         RE::TESObjectREFR* move_to_ref, const RE::NiPoint3* drop_loc,
                                         const RE::NiPoint3* rotate) -> RE::ObjectRefHandle
    {
      auto _ = hooks_ctx::on_actor_remove_item{actor, item, count, reason, move_to_ref, drop_loc, rotate};
      return on_remove_item_character_original(actor, item, count, reason, extra_list, move_to_ref, drop_loc, rotate);
    }

    static auto on_remove_item_player_character(RE::PlayerCharacter* actor, RE::TESBoundObject* item,
                                                std::int32_t count, RE::ITEM_REMOVE_REASON reason,
                                                RE::ExtraDataList* extra_list, RE::TESObjectREFR* move_to_ref,
                                                const RE::NiPoint3* drop_loc,
                                                const RE::NiPoint3* rotate) -> RE::ObjectRefHandle
    {
      auto _ = hooks_ctx::on_actor_remove_item{actor, item, count, reason, move_to_ref, drop_loc, rotate};
      return on_remove_item_player_character_original(actor, item, count, reason, extra_list, move_to_ref, drop_loc,
                                                      rotate);
    }

    static inline REL::Relocation<decltype(on_update_character)> on_update_character_original;
    static inline REL::Relocation<decltype(on_update_player_character)> on_update_player_character_original;
    static inline REL::Relocation<decltype(on_drink_potion_character)> on_drink_potion_character_original;
    static inline REL::Relocation<decltype(on_drink_potion_player_character)> on_drink_potion_player_character_original;
    static inline REL::Relocation<decltype(on_remove_item_character)> on_remove_item_character_original;
    static inline REL::Relocation<decltype(on_remove_item_player_character)> on_remove_item_player_character_original;

    static auto install_hook() -> void
    {
      write_vfunc(REL::Relocation<>{addr_vtable_character},
                  offset_vtable_update,
                  on_update_character,
                  on_update_character_original, "on_update_character");
      write_vfunc(REL::Relocation<>{addr_vtable_player_character},
                  offset_vtable_update,
                  on_update_player_character,
                  on_update_player_character_original, "on_update_player_character");
      write_vfunc(REL::Relocation<>{addr_vtable_character},
                  offset_vtable_drink_potion,
                  on_drink_potion_character,
                  on_drink_potion_character_original, "on_drink_potion_character");
      write_vfunc(REL::Relocation<>{addr_vtable_player_character},
                  offset_vtable_drink_potion,
                  on_drink_potion_player_character,
                  on_drink_potion_player_character_original, "on_drink_potion_player_character");
      write_vfunc(REL::Relocation<>{addr_vtable_character},
                  offset_vtable_remove_item,
                  on_remove_item_character,
                  on_remove_item_character_original, "on_remove_item_character");
      write_vfunc(REL::Relocation<>{addr_vtable_player_character},
                  offset_vtable_remove_item,
                  on_remove_item_player_character,
                  on_remove_item_player_character_original, "on_remove_item_player_character");
    }
  };


  STATIC_STRUCT(magic_target)

    static constexpr auto addr_vtable_actor_owner_character = RE::Character::VTABLE[4];
    static constexpr auto addr_vtable_actor_owner_player_character = RE::PlayerCharacter::VTABLE[4];
    static constexpr auto offset_vtable_effect_added = RELOCATION_OFFSET(0x08, 0x08);

    static auto on_effect_added_character(RE::MagicTarget* magic_target, RE::ActiveEffect* active_effect) -> void
    {
      if (!magic_target || !active_effect) {
        return on_effect_added_character_original(magic_target, active_effect);
      }
      
      wren::wrappers::active_effect wren_active_effect(active_effect);

      wren::script_engine::engine::get_singleton()->dispatch("OnEffectAddedCharacterStart", wren_active_effect);

      on_effect_added_character_original(magic_target, active_effect);
      
      wren::script_engine::engine::get_singleton()->dispatch("OnEffectAddedCharacterEnd", wren_active_effect);
    }

    static auto on_effect_added_player_character(RE::MagicTarget* magic_target,
                                                 RE::ActiveEffect* active_effect) -> void
    {
      if (!magic_target || !active_effect) {
        return on_effect_added_player_character_original(magic_target, active_effect);
      }
      
      wren::wrappers::active_effect wren_active_effect(active_effect);

      wren::script_engine::engine::get_singleton()->dispatch("OnEffectAddedPlayerStart", wren_active_effect);

      on_effect_added_player_character_original(magic_target, active_effect);
      
      wren::script_engine::engine::get_singleton()->dispatch("OnEffectAddedPlayerEnd", wren_active_effect);
    }

    static inline REL::Relocation<decltype(on_effect_added_character)> on_effect_added_character_original;
    static inline REL::Relocation<decltype(on_effect_added_player_character)>
    on_effect_added_player_character_original;

    static auto install_hook() -> void
    {
      write_vfunc(REL::Relocation<>{addr_vtable_actor_owner_character},
                  offset_vtable_effect_added,
                  on_effect_added_character,
                  on_effect_added_character_original, "on_effect_added_character");
      write_vfunc(REL::Relocation<>{addr_vtable_actor_owner_player_character},
                  offset_vtable_effect_added,
                  on_effect_added_player_character,
                  on_effect_added_player_character_original, "on_effect_added_player_character");
    }
  };


  STATIC_STRUCT(weapon_hit)

    static constexpr auto addr = RELOCATION_ID(37673, 38627);
    static constexpr auto offset = RELOCATION_OFFSET(0x3C0, 0x4A8);

    static auto on_weapon_hit(RE::Actor* target, RE::HitData* hit_data) -> void
    {
      if (!target || !hit_data) {
        return on_weapon_hit_original(target, hit_data);
      }

      wren::wrappers::hit_data wren_hit_data(hit_data);

      wren::script_engine::engine::get_singleton()->dispatch("OnWeaponHitStart", wren_hit_data);

      on_weapon_hit_original(target, hit_data);

      wren::script_engine::engine::get_singleton()->dispatch("OnWeaponHitEnd", wren_hit_data);
    }

    static inline REL::Relocation<decltype(on_weapon_hit)> on_weapon_hit_original;

    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      write_call<decltype(on_weapon_hit)>(trampoline, addr.address() + offset, on_weapon_hit,
                                          on_weapon_hit_original, "on_weapon_hit");
    }
  };

  export auto install_hooks() -> void
  {
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(2048);

    character::install_hook();
    weapon_hit::install_hook(trampoline);
    magic_target::install_hook();
  }
}
