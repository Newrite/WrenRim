#pragma once

#include "pch.h"
#include "Wren/Wrappers/Keyword.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::TESObjectARMO.
   * Представляет броню и одежду в игре.
   */
  class armor
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::TESObjectARMO.
     * @param armo Указатель на RE::TESObjectARMO.
     */
    armor(RE::TESObjectARMO* armo)
    {
      if (armo) {
        armor_ = armo;
        form_id_ = armo->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы брони.
     */
    armor(const RE::FormID form_id)
    {
      if (auto armo = RE::TESForm::LookupByID<RE::TESObjectARMO>(form_id)) {
        armor_ = armo;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    armor() = default;

    /**
     * @brief Получает указатель на RE::TESObjectARMO.
     * @return Указатель на RE::TESObjectARMO или nullptr.
     */
    [[nodiscard]] RE::TESObjectARMO* get() const
    {
      if (armor_) return armor_;
      return RE::TESForm::LookupByID<RE::TESObjectARMO>(form_id_);
    }

    /**
     * @brief Проверяет валидность брони.
     * @return true, если броня существует и валидна.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(get());
    }

    /**
     * @brief Получает имя брони.
     * @return Имя брони или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    /**
     * @brief Получает вес брони.
     * @return Вес брони.
     */
    float get_weight() const
    {
      auto ptr = get();
      return ptr ? ptr->GetWeight() : 0.0f;
    }

    /**
     * @brief Получает стоимость брони в золоте.
     * @return Стоимость.
     */
    int32_t get_value() const
    {
      auto ptr = get();
      return ptr ? ptr->GetGoldValue() : 0;
    }

    /**
     * @brief Получает рейтинг брони (защиту).
     * @return Рейтинг брони.
     */
    float get_armor_rating() const
    {
      auto ptr = get();
      return ptr ? ptr->GetArmorRating() : 0.0f;
    }

    /**
     * @brief Получает тип брони.
     * @return 0 = Легкая, 1 = Тяжелая, 2 = Одежда, -1 = Ошибка.
     */
    int32_t get_armor_type() const
    {
      auto ptr = get();
      if (!ptr) return -1;
      return static_cast<int32_t>(ptr->GetArmorType());
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
      auto& cls = module.klass<armor>("Armor");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&armor::is_valid>("isValid");
      cls.func<&armor::get_name>("getName");
      cls.func<&armor::get_weight>("getWeight");
      cls.func<&armor::get_value>("getValue");
      cls.func<&armor::get_armor_rating>("getArmorRating");
      cls.func<&armor::get_armor_type>("getArmorType");
      cls.func<&armor::has_keyword>("hasKeyword");
      cls.func<&armor::has_keyword_id>("hasKeywordID");
      cls.func<&armor::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::TESObjectARMO* armor_{nullptr};
    RE::FormID form_id_{0};
  };
}
