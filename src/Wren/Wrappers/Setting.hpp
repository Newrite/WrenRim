#pragma once

#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::Setting.
   * Представляет настройку игры (INI setting).
   */
  class setting
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::Setting.
     * @param s Указатель на RE::Setting.
     */
    setting(RE::Setting* s) : setting_(s) {}

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    setting() = default;

    /**
     * @brief Проверяет валидность настройки.
     * @return true, если настройка найдена.
     */
    [[nodiscard]] bool is_valid() const { return setting_ != nullptr; }

    /**
     * @brief Получает тип настройки.
     * @return Тип настройки: 0=Unknown, 1=Bool, 2=Float, 3=Int, 4=Color, 5=String, 6=UInt.
     */
    int get_type() const {
        return setting_ ? static_cast<int>(setting_->GetType()) : 0;
    }

    /**
     * @brief Получает значение настройки как bool.
     * @return Значение или false, если настройка невалидна.
     */
    bool get_bool() const { return setting_ ? setting_->GetBool() : false; }

    /**
     * @brief Получает значение настройки как float.
     * @return Значение или 0.0f.
     */
    float get_float() const { return setting_ ? setting_->GetFloat() : 0.0f; }

    /**
     * @brief Получает значение настройки как int.
     * @return Значение или 0.
     */
    int32_t get_int() const { return setting_ ? setting_->GetSInt() : 0; }

    /**
     * @brief Получает значение настройки как string.
     * @return Значение или пустая строка.
     */
    std::string get_string() const { return setting_ ? setting_->GetString() : ""; }

    /**
     * @brief Получает значение настройки как unsigned int.
     * @return Значение или 0.
     */
    uint32_t get_uint() const { return setting_ ? setting_->GetUInt() : 0; }

    static void bind(wrenbind17::ForeignModule& module) {
        auto& cls = module.klass<setting>("Setting");
        cls.ctor<>();
        cls.func<&setting::is_valid>("isValid");
        cls.func<&setting::get_type>("getType");
        cls.func<&setting::get_bool>("getBool");
        cls.func<&setting::get_float>("getFloat");
        cls.func<&setting::get_int>("getInt");
        cls.func<&setting::get_string>("getString");
        cls.func<&setting::get_uint>("getUInt");
    }

  private:
    RE::Setting* setting_{nullptr};
  };
}
