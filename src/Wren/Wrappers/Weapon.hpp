#pragma once

#include "pch.h"
#include "Wren/Wrappers/Keyword.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::TESObjectWEAP.
   * Представляет оружие в игре.
   */
  class weapon
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::TESObjectWEAP.
     * @param w Указатель на RE::TESObjectWEAP.
     */
    weapon(RE::TESObjectWEAP* w)
    {
      if (w) {
        weapon_ = w;
        form_id_ = w->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы оружия.
     */
    weapon(const RE::FormID form_id)
    {
      if (auto w = RE::TESForm::LookupByID<RE::TESObjectWEAP>(form_id)) {
        weapon_ = w;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    weapon() = default;

    /**
     * @brief Получает указатель на RE::TESObjectWEAP.
     * @return Указатель на RE::TESObjectWEAP или nullptr.
     */
    [[nodiscard]] RE::TESObjectWEAP* get() const
    {
      if (weapon_) return weapon_;
      return RE::TESForm::LookupByID<RE::TESObjectWEAP>(form_id_);
    }

    /**
     * @brief Проверяет валидность оружия.
     * @return true, если оружие существует и валидно.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(weapon_);
    }

    /**
     * @brief Получает имя оружия.
     * @return Имя оружия или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    /**
     * @brief Получает базовый урон оружия.
     * @return Урон.
     */
    float get_attack_damage() const
    {
      auto ptr = get();
      return ptr ? ptr->GetAttackDamage() : 0.0f;
    }

    /**
     * @brief Получает дальность атаки оружия.
     * @return Дальность.
     */
    float get_reach() const
    {
      auto ptr = get();
      return ptr ? ptr->GetReach() : 0.0f;
    }

    /**
     * @brief Получает скорость атаки оружия.
     * @return Скорость.
     */
    float get_speed() const
    {
      auto ptr = get();
      return ptr ? ptr->GetSpeed() : 0.0f;
    }

    /**
     * @brief Получает вес оружия.
     * @return Вес.
     */
    float get_weight() const
    {
      auto ptr = get();
      return ptr ? ptr->GetWeight() : 0.0f;
    }

    /**
     * @brief Получает стоимость оружия в золоте.
     * @return Стоимость.
     */
    float get_value() const
    {
      auto ptr = get();
      return ptr ? static_cast<float>(ptr->GetGoldValue()) : 0.0f;
    }

    /**
     * @brief Проверяет наличие ключевого слова (Keyword).
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
     * @brief Проверяет наличие ключевого слова по FormID.
     * @param form_id FormID ключевого слова.
     * @return true, если ключевое слово есть.
     */
    bool has_keyword_id(const uint32_t form_id) const
    {
        auto ptr = get();
        if (ptr) {
            return ptr->HasKeywordID(form_id);
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
      auto& cls = module.klass<weapon>("Weapon");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&weapon::get_name>("getName");
      cls.func<&weapon::is_valid>("isValid");
      cls.func<&weapon::get_attack_damage>("getAttackDamage");
      cls.func<&weapon::get_reach>("getReach");
      cls.func<&weapon::get_speed>("getSpeed");
      cls.func<&weapon::get_weight>("getWeight");
      cls.func<&weapon::get_value>("getValue");
      cls.func<&weapon::has_keyword>("hasKeyword");
      cls.func<&weapon::has_keyword_id>("hasKeywordID");
      cls.func<&weapon::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::TESObjectWEAP* weapon_{nullptr};
    RE::FormID form_id_{0};
  };
}
