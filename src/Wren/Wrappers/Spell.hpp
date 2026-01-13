#pragma once

#include "Wren/Wrappers/Actor.hpp"
#include "Wren/Wrappers/Keyword.hpp"
#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::SpellItem.
   * Представляет заклинание в игре.
   */
  class spell
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::SpellItem.
     * @param spell Указатель на RE::SpellItem.
     */
    spell(RE::SpellItem* spell)
    {
      if (spell) {
        spell_ = spell;
        form_id_ = spell->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы заклинания.
     */
    spell(const RE::FormID form_id)
    {
      if (auto sp = RE::TESForm::LookupByID<RE::SpellItem>(form_id)) {
        spell_ = sp;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    spell() = default;

    /**
     * @brief Получает указатель на RE::SpellItem.
     * @return Указатель на RE::SpellItem или nullptr.
     */
    [[nodiscard]] RE::SpellItem* get() const
    {
      if (spell_) return spell_;
      return RE::TESForm::LookupByID<RE::SpellItem>(form_id_);
    }

    /**
     * @brief Проверяет валидность заклинания.
     * @return true, если заклинание существует и валидно.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(get());
    }

    /**
     * @brief Получает имя заклинания.
     * @return Имя заклинания или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    /**
     * @brief Получает время каста (зарядки) заклинания.
     * @return Время в секундах.
     */
    float get_cast_time() const
    {
      auto ptr = get();
      return ptr ? ptr->GetChargeTime() : 0.0f;
    }

    /**
     * @brief Получает базовую стоимость магии.
     * @return Стоимость магии.
     */
    int32_t get_magicka_cost() const
    {
      auto ptr = get();
      return ptr ? ptr->data.costOverride : 0;
    }

    /**
     * @brief Вычисляет эффективную стоимость магии для конкретного кастера (с учетом перков и навыков).
     * @param caster Актер, который кастует заклинание.
     * @return Эффективная стоимость.
     */
    float get_effective_magicka_cost(const actor& caster) const
    {
      auto ptr = get();
      auto caster_ptr = caster.get();
      return (ptr && caster_ptr) ? ptr->CalculateMagickaCost(caster_ptr) : 0.f;
    }

    /**
     * @brief Получает тип заклинания.
     * @return 0=Spell, 1=Disease, 2=Power, 3=LesserPower, 4=Ability, 5=Poison, 6=Addition, 7=Voice.
     */
    int32_t get_spell_type() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetSpellType()) : -1;
    }

    /**
     * @brief Получает тип каста.
     * @return 0=ConstantEffect, 1=FireAndForget, 2=Concentration, 3=Scroll.
     */
    int32_t get_casting_type() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetCastingType()) : -1;
    }

    /**
     * @brief Получает тип доставки (Delivery).
     * @return 0=Self, 1=Touch, 2=Aimed, 3=TargetActor, 4=TargetLocation.
     */
    int32_t get_delivery() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetDelivery()) : -1;
    }

    /**
     * @brief Проверяет, является ли заклинание враждебным.
     * @return true, если враждебное.
     */
    bool is_hostile() const
    {
      auto ptr = get();
      return ptr ? ptr->IsHostile() : false;
    }

    /**
     * @brief Устанавливает время каста (зарядки).
     * @param time Время в секундах.
     */
    void set_cast_time(float time)
    {
        auto ptr = get();
        if (ptr) ptr->data.chargeTime = time;
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
      auto& cls = module.klass<spell>("Spell");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&spell::is_valid>("isValid");
      cls.func<&spell::get_name>("getName");
      cls.func<&spell::get_cast_time>("getCastTime");
      cls.func<&spell::get_magicka_cost>("getMagickaCost");
      cls.func<&spell::get_effective_magicka_cost>("getEffectiveMagickaCost");
      cls.func<&spell::get_spell_type>("getSpellType");
      cls.func<&spell::get_casting_type>("getCastingType");
      cls.func<&spell::get_delivery>("getDelivery");
      cls.func<&spell::is_hostile>("isHostile");
      cls.func<&spell::set_cast_time>("setCastTime");
      cls.func<&spell::has_keyword>("hasKeyword");
      cls.func<&spell::has_keyword_id>("hasKeywordID");
      cls.func<&spell::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::SpellItem* spell_{nullptr};
    RE::FormID form_id_{0};
  };
}
