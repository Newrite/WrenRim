#pragma once

#include "pch.h"
#include "Wren/Wrappers/Actor.hpp"
#include "Wren/Wrappers/Weapon.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::HitData.
   * Содержит информацию о попадании (ударе): атакующий, цель, урон, оружие и т.д.
   */
  class hit_data
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::HitData.
     * @param data Указатель на RE::HitData.
     */
    hit_data(RE::HitData* data)
    {
      if (data) {
        data_ = data;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    hit_data() = default;

    /**
     * @brief Получает указатель на RE::HitData.
     * @return Указатель на RE::HitData или nullptr.
     */
    [[nodiscard]] RE::HitData* get() const
    {
      return data_;
    }

    /**
     * @brief Проверяет валидность данных о попадании.
     * @return true, если данные существуют.
     */
    auto is_valid() const -> bool
    {
      return data_ ? true : false;
    }

    /**
     * @brief Получает атакующего (агрессора).
     * @return Объект actor, представляющий атакующего.
     */
    auto get_attacker() const -> actor
    {
      if (!is_valid()) return actor();

      if (data_->aggressor) {
        return actor(data_->aggressor);
      }

      return actor();
    }

    /**
     * @brief Получает цель атаки.
     * @return Объект actor, представляющий цель.
     */
    auto get_target() const -> actor
    {
      if (!is_valid()) return actor();

      if (data_->target) {
        return actor(data_->target);
      }

      return actor();
    }

    /**
     * @brief Получает общий урон от попадания.
     * @return Значение урона.
     */
    auto get_total_damage() const -> float
    {
      if (!is_valid()) return 0.f;

      return data_->totalDamage;
    }

    /**
     * @brief Устанавливает общий урон от попадания.
     * @param value Новое значение урона.
     */
    auto set_total_damage(const float value) const -> void
    {
      if (!is_valid()) return;

      data_->totalDamage = value;
    }

    /**
     * @brief Получает оружие, которым был нанесен удар.
     * @return Объект weapon.
     */
    auto get_weapon() const -> weapon
    {
      if (!is_valid() || !data_->weapon) return weapon();
      return weapon{data_->weapon};
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<hit_data>("HitData");
      cls.ctor<>();
      cls.func<&hit_data::is_valid>("isValid");
      cls.func<&hit_data::get_attacker>("getAttacker");
      cls.func<&hit_data::get_target>("getTarget");
      cls.func<&hit_data::get_total_damage>("getTotalDamage");
      cls.func<&hit_data::set_total_damage>("setTotalDamage");
      cls.func<&hit_data::get_weapon>("getWeapon");
    }

  private:
    RE::HitData* data_{nullptr};
  };
}
