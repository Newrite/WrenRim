#pragma once

#include "pch.h"
#include "Wren/Wrappers/Actor.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::ActiveEffect.
   * Предоставляет доступ к свойствам активных магических эффектов в игре.
   */
  class active_effect
  {
  public:
    /**
     * @brief Конструктор, инициализирующий обертку с указателем на RE::ActiveEffect.
     * @param effect Указатель на RE::ActiveEffect.
     */
    active_effect(RE::ActiveEffect* effect) : effect_(effect) {}

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    active_effect() = default;

    /**
     * @brief Возвращает указатель на базовый RE::ActiveEffect.
     * @return Указатель на RE::ActiveEffect.
     */
    [[nodiscard]] RE::ActiveEffect* get() const { return effect_; }

    /**
     * @brief Проверяет, является ли обертка действительной (т.е. содержит ли непустой указатель на эффект).
     * @return true, если эффект действителен, иначе false.
     */
    [[nodiscard]] bool is_valid() const { return effect_ != nullptr; }

    /**
     * @brief Возвращает длительность активного эффекта.
     * @return Длительность эффекта в секундах или 0.0f, если эффект недействителен.
     */
    float get_duration() const { return is_valid() ? effect_->duration : 0.0f; }

    /**
     * @brief Возвращает величину (силу) активного эффекта.
     * @return Величина эффекта или 0.0f, если эффект недействителен.
     */
    float get_magnitude() const { return is_valid() ? effect_->magnitude : 0.0f; }

    /**
     * @brief Возвращает прошедшее время с момента начала действия эффекта.
     * @return Прошедшее время в секундах или 0.0f, если эффект недействителен.
     */
    float get_elapsed() const { return is_valid() ? effect_->elapsedSeconds : 0.0f; }

    /**
     * @brief Устанавливает величину (силу) активного эффекта.
     * @param value Новое значение величины.
     */
    void set_magnitude(const float value) const
    {
        if (is_valid()) {
            effect_->magnitude = value;
        }
    }

    /**
     * @brief Возвращает актера, наложившего эффект (кастера).
     * @return Объект actor, представляющий кастера, или невалидный actor, если кастер не найден.
     */
    actor get_caster() const
    {
        if (!is_valid() || !effect_->caster) return actor();
        return actor(effect_->caster);
    }

    /**
     * @brief Возвращает актера, на которого наложен эффект (цель).
     * @return Объект actor, представляющий цель, или невалидный actor, если цель не найдена.
     */
    actor get_target() const
    {
        if (!is_valid() || !effect_->target) return actor();
        // MagicTarget обычно является Actor в контексте ActiveEffects, которые нас интересуют
        const auto actor_ptr = effect_->target->GetTargetAsActor();
        if (actor_ptr) {
            return actor(actor_ptr);
        }
        return actor();
    }

    /**
     * @brief Привязывает методы класса к модулю Wren.
     * @param module Модуль Wren, к которому будут привязаны методы.
     */
    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<active_effect>("ActiveEffect");
      cls.ctor<>();
      cls.func<&active_effect::is_valid>("isValid");
      cls.func<&active_effect::get_duration>("getDuration");
      cls.func<&active_effect::get_magnitude>("getMagnitude");
      cls.func<&active_effect::get_elapsed>("getElapsed");
      cls.func<&active_effect::set_magnitude>("setMagnitude");
      cls.func<&active_effect::get_caster>("getCaster");
      cls.func<&active_effect::get_target>("getTarget");
    }

  private:
    RE::ActiveEffect* effect_{ nullptr };
  };
}
