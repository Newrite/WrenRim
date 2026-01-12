export module WrenRim.Core.Hooks;

import WrenRim.Core.HooksCtx;
import WrenRim.Config;
import WrenRim.Wren.ScriptEngine;
import WrenRim.API.Wrappers;

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
      if (!character || !delta) {
        return on_update_character_original(character, delta);
      }

      auto _ = hooks_ctx::on_actor_update{character, last_player_delta};

      return on_update_character_original(character, delta);
    }

    static auto on_update_player_character(RE::PlayerCharacter* character, const float delta) -> void
    {
      wren::script_engine::engine::get_singleton()->on_frame_start();
      last_player_delta = delta;

      if (!character || !delta) {
        return on_update_player_character_original(character, delta);
      }

      auto _ = hooks_ctx::on_actor_update{character, last_player_delta};

      return on_update_player_character_original(character, delta);
    }

    static auto on_drink_potion_character(RE::Character* character, RE::AlchemyItem* potion,
                                          RE::ExtraDataList* extra_list) -> bool
    {
      if (!character || !potion || !config::manager::get_singleton()->is_enabled()) {
        return on_drink_potion_character_original(character, potion, extra_list);
      }

      api::wrappers::actor wren_actor(character);
      api::wrappers::potion wren_potion(potion);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionStart", wren_actor, wren_potion);

      bool result = on_drink_potion_character_original(character, potion, extra_list);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionEnd", wren_actor, wren_potion);

      return result;
    }

    static auto on_drink_potion_player_character(RE::PlayerCharacter* character, RE::AlchemyItem* potion,
                                                 RE::ExtraDataList* extra_list) -> bool
    {
      if (!character || !potion || !config::manager::get_singleton()->is_enabled()) {
        return on_drink_potion_player_character_original(character, potion, extra_list);
      }

      api::wrappers::actor wren_actor(character);
      api::wrappers::potion wren_potion(potion);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionStart", wren_actor, wren_potion);

      bool result = on_drink_potion_player_character_original(character, potion, extra_list);

      wren::script_engine::engine::get_singleton()->dispatch("OnDrinkPotionEnd", wren_actor, wren_potion);


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

  export auto install_hooks() -> void
  {
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(512);

    character::install_hook();
  }
}
