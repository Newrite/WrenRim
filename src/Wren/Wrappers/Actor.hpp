#pragma once

#include "pch.h"
#include "Wren/Wrappers/Keyword.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::Actor.
   * Представляет NPC или игрока в игровом мире.
   */
  class actor
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::Actor.
     * @param a Указатель на RE::Actor.
     */
    actor(RE::Actor* a)
    {
      if (a) {
        handle_ = a->GetHandle();
      }
    }

    /**
     * @brief Конструктор из RE::ActorHandle.
     * @param handle Хендл актера.
     */
    actor(const RE::ActorHandle& handle)
    {
      if (handle) {
        handle_ = handle;
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы актера.
     */
    actor(const RE::FormID form_id)
    {
      if (auto a = RE::TESForm::LookupByID<RE::Actor>(form_id)) {
        handle_ = a->GetHandle();
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    actor() = default;

    /**
     * @brief Получает указатель на RE::Actor.
     * @return Указатель на RE::Actor или nullptr, если актер невалиден.
     */
    [[nodiscard]] RE::Actor* get() const
    {
      return handle_.get().get();
    }

    /**
     * @brief Проверяет валидность актера.
     * @return true, если актер существует и валиден.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(handle_);
    }

    /**
     * @brief Получает имя актера.
     * @return Имя актера или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    /**
     * @brief Получает значение ActorValue по его числовому ID.
     * @param av_raw ID ActorValue (см. RE::ActorValue).
     * @return Значение ActorValue.
     */
    auto get_actor_value_by_enum(const uint32_t av_raw) const -> float
    {
      if (!is_valid()) return 0.f;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return 0.f;

      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->GetActorValue(av);
    }

    /**
     * @brief Получает значение ActorValue по его имени (строке).
     * @param str_av Имя ActorValue (например, "Health").
     * @return Значение ActorValue.
     */
    auto get_actor_value_by_string(const std::string& str_av) const -> float
    {
      if (!is_valid()) return 0.f;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return 0.f;

      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      if (av == RE::ActorValue::kNone) return 0.f;

      return owner->GetActorValue(av);
    }

    /**
     * @brief Модифицирует (изменяет) значение ActorValue по его числовому ID.
     * @param av_raw ID ActorValue.
     * @param value Значение, на которое нужно изменить (может быть отрицательным).
     */
    auto mod_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, value);
    }

    /**
     * @brief Модифицирует (изменяет) значение ActorValue по его имени.
     * @param str_av Имя ActorValue.
     * @param value Значение, на которое нужно изменить.
     */
    auto mod_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, value);
    }

    /**
     * @brief Восстанавливает значение ActorValue по его числовому ID.
     * @param av_raw ID ActorValue.
     * @param value Значение для восстановления.
     */
    auto restore_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->RestoreActorValue(av, value);
    }

    /**
     * @brief Восстанавливает значение ActorValue по его имени.
     * @param str_av Имя ActorValue.
     * @param value Значение для восстановления.
     */
    auto restore_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->RestoreActorValue(av, value);
    }

    /**
     * @brief Наносит урон значению ActorValue по его числовому ID.
     * @param av_raw ID ActorValue.
     * @param value Значение урона.
     */
    auto damage_actor_value_by_enum(const uint32_t av_raw, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = static_cast<RE::ActorValue>(av_raw);

      return owner->DamageActorValue(av, value);
    }

    /**
     * @brief Наносит урон значению ActorValue по его имени.
     * @param str_av Имя ActorValue.
     * @param value Значение урона.
     */
    auto damage_actor_value_by_string(const std::string& str_av, const float value) const -> void
    {
      if (!is_valid()) return;

      const auto owner = get()->AsActorValueOwner();
      if (!owner) return;

      const auto av = RE::ActorValueList::LookupActorValueByName(str_av.c_str());

      return owner->DamageActorValue(av, value);
    }

    /**
     * @brief Проверяет наличие ключевого слова (Keyword) у актера.
     * @param kw Объект keyword.
     * @return true, если ключевое слово есть.
     */
    bool has_keyword(const keyword& kw) const
    {
        auto ptr = get();
        auto kw_ptr = kw.get();
        if (ptr && kw_ptr) {
            return ptr->HasKeyword(kw_ptr);
        }
        return false;
    }

    /**
     * @brief Проверяет наличие ключевого слова по EditorID.
     * @param editor_id EditorID ключевого слова.
     * @return true, если ключевое слово есть.
     */
    bool has_keyword_string(const std::string& editor_id) const
    {
        auto ptr = get();
        if (ptr) {
            return ptr->HasKeywordString(editor_id);
        }
        return false;
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<actor>("Actor");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&actor::get_name>("getName");
      cls.func<&actor::is_valid>("isValid");
      cls.func<&actor::get_actor_value_by_enum>("getActorValueByEnum");
      cls.func<&actor::get_actor_value_by_string>("getActorValueByString");
      cls.func<&actor::mod_actor_value_by_enum>("modActorValueByEnum");
      cls.func<&actor::mod_actor_value_by_string>("modActorValueByString");
      cls.func<&actor::restore_actor_value_by_enum>("restoreActorValueByEnum");
      cls.func<&actor::restore_actor_value_by_string>("restoreActorValueByString");
      cls.func<&actor::damage_actor_value_by_enum>("damageActorValueByEnum");
      cls.func<&actor::damage_actor_value_by_string>("damageActorValueByString");
      cls.func<&actor::has_keyword>("hasKeyword");
      cls.func<&actor::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::ActorHandle handle_;
  };
}
