#pragma once

#include "pch.h"
#include "Wren/Wrappers/Keyword.hpp"
#include "Wren/Wrappers/Spell.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::EffectSetting.
   * Представляет базовый эффект (Magic Effect) в игре.
   */
  class effect
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::EffectSetting.
     * @param e Указатель на RE::EffectSetting.
     */
    effect(RE::EffectSetting* e)
    {
      if (e) {
        effect_ = e;
        form_id_ = e->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы эффекта.
     */
    effect(const RE::FormID form_id)
    {
      if (auto e = RE::TESForm::LookupByID<RE::EffectSetting>(form_id)) {
        effect_ = e;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    effect() = default;

    /**
     * @brief Получает указатель на RE::EffectSetting.
     * @return Указатель на RE::EffectSetting или nullptr.
     */
    [[nodiscard]] RE::EffectSetting* get() const
    {
      if (effect_) return effect_;
      return RE::TESForm::LookupByID<RE::EffectSetting>(form_id_);
    }

    /**
     * @brief Проверяет валидность эффекта.
     * @return true, если эффект существует и валиден.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(get());
    }

    /**
     * @brief Получает имя эффекта.
     * @return Имя эффекта или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
    }

    /**
     * @brief Проверяет, является ли эффект враждебным.
     * @return true, если враждебный.
     */
    bool is_hostile() const
    {
      auto ptr = get();
      return ptr ? ptr->IsHostile() : false;
    }

    /**
     * @brief Проверяет, является ли эффект вредным (Detrimental).
     * @return true, если вредный.
     */
    bool is_detrimental() const
    {
      auto ptr = get();
      return ptr ? ptr->IsDetrimental() : false;
    }

    /**
     * @brief Получает базовую стоимость эффекта.
     * @return Стоимость.
     */
    float get_base_cost() const
    {
      auto ptr = get();
      return ptr ? ptr->data.baseCost : 0.0f;
    }

    /**
     * @brief Устанавливает базовую стоимость эффекта.
     * @param cost Новая стоимость.
     */
    void set_base_cost(float cost)
    {
        auto ptr = get();
        if (ptr) ptr->data.baseCost = cost;
    }

    /**
     * @brief Получает архетип эффекта.
     * @return ID архетипа (см. RE::EffectArchetypes::ArchetypeID).
     */
    int32_t get_archetype() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetArchetype()) : -1;
    }

    /**
     * @brief Устанавливает архетип эффекта.
     * @param type ID архетипа.
     */
    void set_archetype(int32_t type)
    {
        auto ptr = get();
        if (ptr) ptr->data.archetype = static_cast<RE::EffectArchetypes::ArchetypeID>(type);
    }

    /**
     * @brief Получает связанный навык (Magic Skill).
     * @return ID навыка (ActorValue).
     */
    int32_t get_magic_skill() const
    {
      auto ptr = get();
      return ptr ? static_cast<int32_t>(ptr->GetMagickSkill()) : -1;
    }

    /**
     * @brief Устанавливает связанный навык.
     * @param skill ID навыка (ActorValue).
     */
    void set_magic_skill(int32_t skill)
    {
        auto ptr = get();
        if (ptr) ptr->data.associatedSkill = static_cast<RE::ActorValue>(skill);
    }

    /**
     * @brief Получает минимальный уровень навыка для использования.
     * @return Уровень навыка.
     */
    int32_t get_minimum_skill_level() const
    {
      auto ptr = get();
      return ptr ? ptr->GetMinimumSkillLevel() : 0;
    }

    /**
     * @brief Устанавливает минимальный уровень навыка.
     * @param level Уровень навыка.
     */
    void set_minimum_skill_level(int32_t level)
    {
        auto ptr = get();
        if (ptr) ptr->data.minimumSkill = level;
    }

    /**
     * @brief Получает тип каста (Casting Type).
     * @return 0=ConstantEffect, 1=FireAndForget, 2=Concentration, 3=Scroll.
     */
    int32_t get_casting_type() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.castingType) : -1;
    }

    /**
     * @brief Устанавливает тип каста.
     * @param type Тип каста.
     */
    void set_casting_type(int32_t type)
    {
        auto ptr = get();
        if (ptr) ptr->data.castingType = static_cast<RE::MagicSystem::CastingType>(type);
    }

    /**
     * @brief Получает тип доставки (Delivery).
     * @return 0=Self, 1=Touch, 2=Aimed, 3=TargetActor, 4=TargetLocation.
     */
    int32_t get_delivery() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.delivery) : -1;
    }

    /**
     * @brief Устанавливает тип доставки.
     * @param type Тип доставки.
     */
    void set_delivery(int32_t type)
    {
        auto ptr = get();
        if (ptr) ptr->data.delivery = static_cast<RE::MagicSystem::Delivery>(type);
    }

    /**
     * @brief Получает множитель использования навыка.
     * @return Множитель.
     */
    float get_skill_usage_mult() const
    {
        auto ptr = get();
        return ptr ? ptr->data.skillUsageMult : 0.0f;
    }

    /**
     * @brief Устанавливает множитель использования навыка.
     * @param mult Новый множитель.
     */
    void set_skill_usage_mult(float mult)
    {
        auto ptr = get();
        if (ptr) ptr->data.skillUsageMult = mult;
    }

    /**
     * @brief Получает масштаб двойного каста (Dual Cast Scale).
     * @return Масштаб.
     */
    float get_dual_cast_scale() const
    {
        auto ptr = get();
        return ptr ? ptr->data.dualCastScale : 0.0f;
    }

    /**
     * @brief Устанавливает масштаб двойного каста.
     * @param scale Новый масштаб.
     */
    void set_dual_cast_scale(float scale)
    {
        auto ptr = get();
        if (ptr) ptr->data.dualCastScale = scale;
    }

    /**
     * @brief Получает переменную сопротивления (Resist Variable).
     * @return ID ActorValue.
     */
    int32_t get_resist_variable() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.resistVariable) : -1;
    }

    /**
     * @brief Устанавливает переменную сопротивления.
     * @param var ID ActorValue.
     */
    void set_resist_variable(int32_t var)
    {
        auto ptr = get();
        if (ptr) ptr->data.resistVariable = static_cast<RE::ActorValue>(var);
    }

    /**
     * @brief Получает количество противодействующих эффектов.
     * @return Количество.
     */
    int16_t get_num_counter_effects() const
    {
        auto ptr = get();
        return ptr ? ptr->data.numCounterEffects : 0;
    }

    /**
     * @brief Устанавливает количество противодействующих эффектов.
     * @param num Количество.
     */
    void set_num_counter_effects(int16_t num)
    {
        auto ptr = get();
        if (ptr) ptr->data.numCounterEffects = num;
    }

    /**
     * @brief Получает вес затухания (Taper Weight).
     * @return Вес.
     */
    float get_taper_weight() const
    {
        auto ptr = get();
        return ptr ? ptr->data.taperWeight : 0.0f;
    }

    /**
     * @brief Устанавливает вес затухания.
     * @param weight Вес.
     */
    void set_taper_weight(float weight)
    {
        auto ptr = get();
        if (ptr) ptr->data.taperWeight = weight;
    }

    /**
     * @brief Получает область создания заклинания (Spellmaking Area).
     * @return Область.
     */
    int32_t get_spellmaking_area() const
    {
        auto ptr = get();
        return ptr ? ptr->data.spellmakingArea : 0;
    }

    /**
     * @brief Устанавливает область создания заклинания.
     * @param area Область.
     */
    void set_spellmaking_area(int32_t area)
    {
        auto ptr = get();
        if (ptr) ptr->data.spellmakingArea = area;
    }

    /**
     * @brief Получает время зарядки при создании заклинания.
     * @return Время.
     */
    float get_spellmaking_charge_time() const
    {
        auto ptr = get();
        return ptr ? ptr->data.spellmakingChargeTime : 0.0f;
    }

    /**
     * @brief Устанавливает время зарядки при создании заклинания.
     * @param time Время.
     */
    void set_spellmaking_charge_time(float time)
    {
        auto ptr = get();
        if (ptr) ptr->data.spellmakingChargeTime = time;
    }

    /**
     * @brief Получает кривую затухания (Taper Curve).
     * @return Кривая.
     */
    float get_taper_curve() const
    {
        auto ptr = get();
        return ptr ? ptr->data.taperCurve : 0.0f;
    }

    /**
     * @brief Устанавливает кривую затухания.
     * @param curve Кривая.
     */
    void set_taper_curve(float curve)
    {
        auto ptr = get();
        if (ptr) ptr->data.taperCurve = curve;
    }

    /**
     * @brief Получает длительность затухания (Taper Duration).
     * @return Длительность.
     */
    float get_taper_duration() const
    {
        auto ptr = get();
        return ptr ? ptr->data.taperDuration : 0.0f;
    }

    /**
     * @brief Устанавливает длительность затухания.
     * @param duration Длительность.
     */
    void set_taper_duration(float duration)
    {
        auto ptr = get();
        if (ptr) ptr->data.taperDuration = duration;
    }

    /**
     * @brief Получает вес второго атрибута (Second AV Weight).
     * @return Вес.
     */
    float get_second_av_weight() const
    {
        auto ptr = get();
        return ptr ? ptr->data.secondAVWeight : 0.0f;
    }

    /**
     * @brief Устанавливает вес второго атрибута.
     * @param weight Вес.
     */
    void set_second_av_weight(float weight)
    {
        auto ptr = get();
        if (ptr) ptr->data.secondAVWeight = weight;
    }

    /**
     * @brief Получает первичный атрибут (Primary AV).
     * @return ID ActorValue.
     */
    int32_t get_primary_av() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.primaryAV) : -1;
    }

    /**
     * @brief Устанавливает первичный атрибут.
     * @param av ID ActorValue.
     */
    void set_primary_av(int32_t av)
    {
        auto ptr = get();
        if (ptr) ptr->data.primaryAV = static_cast<RE::ActorValue>(av);
    }

    /**
     * @brief Получает вторичный атрибут (Secondary AV).
     * @return ID ActorValue.
     */
    int32_t get_secondary_av() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.secondaryAV) : -1;
    }

    /**
     * @brief Устанавливает вторичный атрибут.
     * @param av ID ActorValue.
     */
    void set_secondary_av(int32_t av)
    {
        auto ptr = get();
        if (ptr) ptr->data.secondaryAV = static_cast<RE::ActorValue>(av);
    }

    /**
     * @brief Получает способность экипировки (Equip Ability).
     * @return Объект spell.
     */
    spell get_equip_ability() const
    {
        auto ptr = get();
        if (ptr && ptr->data.equipAbility) {
            return spell(ptr->data.equipAbility);
        }
        return spell();
    }

    /**
     * @brief Устанавливает способность экипировки.
     * @param ability Объект spell.
     */
    void set_equip_ability(const spell& ability)
    {
        auto ptr = get();
        if (ptr) {
            ptr->data.equipAbility = ability.get();
        }
    }

    /**
     * @brief Получает уровень звука каста.
     * @return Уровень звука (0=Loud, 1=Normal, 2=Silent).
     */
    int32_t get_casting_sound_level() const
    {
        auto ptr = get();
        return ptr ? static_cast<int32_t>(ptr->data.castingSoundLevel) : 0;
    }

    /**
     * @brief Устанавливает уровень звука каста.
     * @param level Уровень звука.
     */
    void set_casting_sound_level(int32_t level)
    {
        auto ptr = get();
        if (ptr) ptr->data.castingSoundLevel = static_cast<RE::SOUND_LEVEL>(level);
    }

    /**
     * @brief Получает оценку AI (AI Score).
     * @return Оценка.
     */
    float get_ai_score() const
    {
        auto ptr = get();
        return ptr ? ptr->data.aiScore : 0.0f;
    }

    /**
     * @brief Устанавливает оценку AI.
     * @param score Оценка.
     */
    void set_ai_score(float score)
    {
        auto ptr = get();
        if (ptr) ptr->data.aiScore = score;
    }

    /**
     * @brief Получает таймер задержки AI.
     * @return Таймер.
     */
    float get_ai_delay_timer() const
    {
        auto ptr = get();
        return ptr ? ptr->data.aiDelayTimer : 0.0f;
    }

    /**
     * @brief Устанавливает таймер задержки AI.
     * @param timer Таймер.
     */
    void set_ai_delay_timer(float timer)
    {
        auto ptr = get();
        if (ptr) ptr->data.aiDelayTimer = timer;
    }

    /**
     * @brief Проверяет наличие флага.
     * @param flag_bit Битовая маска флага (см. RE::EffectSetting::EffectSettingData::Flag).
     * @return true, если флаг установлен.
     */
    bool has_flag(uint32_t flag_bit) const
    {
        auto ptr = get();
        if (!ptr) return false;
        return (ptr->data.flags.underlying() & flag_bit) != 0;
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
      auto& cls = module.klass<effect>("Effect");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&effect::is_valid>("isValid");
      cls.func<&effect::get_name>("getName");
      cls.func<&effect::is_hostile>("isHostile");
      cls.func<&effect::is_detrimental>("isDetrimental");

      cls.func<&effect::get_base_cost>("getBaseCost");
      cls.func<&effect::set_base_cost>("setBaseCost");

      cls.func<&effect::get_archetype>("getArchetype");
      cls.func<&effect::set_archetype>("setArchetype");

      cls.func<&effect::get_magic_skill>("getMagicSkill");
      cls.func<&effect::set_magic_skill>("setMagicSkill");

      cls.func<&effect::get_minimum_skill_level>("getMinimumSkillLevel");
      cls.func<&effect::set_minimum_skill_level>("setMinimumSkillLevel");

      cls.func<&effect::get_casting_type>("getCastingType");
      cls.func<&effect::set_casting_type>("setCastingType");

      cls.func<&effect::get_delivery>("getDelivery");
      cls.func<&effect::set_delivery>("setDelivery");

      cls.func<&effect::get_skill_usage_mult>("getSkillUsageMult");
      cls.func<&effect::set_skill_usage_mult>("setSkillUsageMult");

      cls.func<&effect::get_dual_cast_scale>("getDualCastScale");
      cls.func<&effect::set_dual_cast_scale>("setDualCastScale");

      cls.func<&effect::get_resist_variable>("getResistVariable");
      cls.func<&effect::set_resist_variable>("setResistVariable");

      cls.func<&effect::get_num_counter_effects>("getNumCounterEffects");
      cls.func<&effect::set_num_counter_effects>("setNumCounterEffects");

      cls.func<&effect::get_taper_weight>("getTaperWeight");
      cls.func<&effect::set_taper_weight>("setTaperWeight");

      cls.func<&effect::get_spellmaking_area>("getSpellmakingArea");
      cls.func<&effect::set_spellmaking_area>("setSpellmakingArea");

      cls.func<&effect::get_spellmaking_charge_time>("getSpellmakingChargeTime");
      cls.func<&effect::set_spellmaking_charge_time>("setSpellmakingChargeTime");

      cls.func<&effect::get_taper_curve>("getTaperCurve");
      cls.func<&effect::set_taper_curve>("setTaperCurve");

      cls.func<&effect::get_taper_duration>("getTaperDuration");
      cls.func<&effect::set_taper_duration>("setTaperDuration");

      cls.func<&effect::get_second_av_weight>("getSecondAVWeight");
      cls.func<&effect::set_second_av_weight>("setSecondAVWeight");

      cls.func<&effect::get_primary_av>("getPrimaryAV");
      cls.func<&effect::set_primary_av>("setPrimaryAV");

      cls.func<&effect::get_secondary_av>("getSecondaryAV");
      cls.func<&effect::set_secondary_av>("setSecondaryAV");

      cls.func<&effect::get_equip_ability>("getEquipAbility");
      cls.func<&effect::set_equip_ability>("setEquipAbility");

      cls.func<&effect::get_casting_sound_level>("getCastingSoundLevel");
      cls.func<&effect::set_casting_sound_level>("setCastingSoundLevel");

      cls.func<&effect::get_ai_score>("getAIScore");
      cls.func<&effect::set_ai_score>("setAIScore");

      cls.func<&effect::get_ai_delay_timer>("getAIDelayTimer");
      cls.func<&effect::set_ai_delay_timer>("setAIDelayTimer");

      cls.func<&effect::has_flag>("hasFlag");
      cls.func<&effect::has_keyword>("hasKeyword");
      cls.func<&effect::has_keyword_id>("hasKeywordID");
      cls.func<&effect::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::EffectSetting* effect_{nullptr};
    RE::FormID form_id_{0};
  };
}
