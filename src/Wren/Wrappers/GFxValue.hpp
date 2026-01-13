#pragma once

#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::GFxValue.
   * Представляет значение в Scaleform (Flash) интерфейсе: число, строку, булево значение, объект или массив.
   */
  class gfx_value
  {
  public:
    /**
     * @brief Конструктор по умолчанию. Создает undefined значение.
     */
    gfx_value() : value_() {}

    /**
     * @brief Конструктор из RE::GFxValue.
     * @param val Значение RE::GFxValue.
     */
    gfx_value(const RE::GFxValue& val) : value_(val) {}

    /**
     * @brief Конструктор из числа.
     * @param val Числовое значение.
     */
    gfx_value(double val) : value_(val) {}

    /**
     * @brief Конструктор из булева значения.
     * @param val Булево значение.
     */
    gfx_value(bool val) : value_(val) {}

    /**
     * @brief Конструктор из строки.
     * @param val Строковое значение.
     */
    gfx_value(const std::string& val) : value_(val) {}

    // Copy/Move
    gfx_value(const gfx_value& other) : value_(other.value_) {}
    gfx_value(gfx_value&& other) noexcept : value_(std::move(other.value_)) {}
    gfx_value& operator=(const gfx_value& other) { value_ = other.value_; return *this; }
    gfx_value& operator=(gfx_value&& other) noexcept { value_ = std::move(other.value_); return *this; }

    /**
     * @brief Получает константную ссылку на RE::GFxValue.
     * @return Ссылка на RE::GFxValue.
     */
    [[nodiscard]] const RE::GFxValue& get() const { return value_; }

    /**
     * @brief Получает ссылку на RE::GFxValue.
     * @return Ссылка на RE::GFxValue.
     */
    [[nodiscard]] RE::GFxValue& get() { return value_; }

    // Factory methods to avoid constructor ambiguity in Wren

    /**
     * @brief Создает GFxValue из булева значения.
     * @param val Булево значение.
     * @return Новый объект GFxValue.
     */
    static gfx_value from_bool(bool val) { return gfx_value(val); }

    /**
     * @brief Создает GFxValue из числа.
     * @param val Числовое значение.
     * @return Новый объект GFxValue.
     */
    static gfx_value from_number(double val) { return gfx_value(val); }

    /**
     * @brief Создает GFxValue из строки.
     * @param val Строковое значение.
     * @return Новый объект GFxValue.
     */
    static gfx_value from_string(const std::string& val) { return gfx_value(val); }

    // Type checks

    /**
     * @brief Проверяет, является ли значение undefined.
     * @return true, если undefined.
     */
    bool is_undefined() const { return value_.IsUndefined(); }

    /**
     * @brief Проверяет, является ли значение null.
     * @return true, если null.
     */
    bool is_null() const { return value_.IsNull(); }

    /**
     * @brief Проверяет, является ли значение булевым.
     * @return true, если boolean.
     */
    bool is_bool() const { return value_.IsBool(); }

    /**
     * @brief Проверяет, является ли значение числом.
     * @return true, если number.
     */
    bool is_number() const { return value_.IsNumber(); }

    /**
     * @brief Проверяет, является ли значение строкой.
     * @return true, если string.
     */
    bool is_string() const { return value_.IsString(); }

    /**
     * @brief Проверяет, является ли значение объектом.
     * @return true, если object.
     */
    bool is_object() const { return value_.IsObject(); }

    /**
     * @brief Проверяет, является ли значение массивом.
     * @return true, если array.
     */
    bool is_array() const { return value_.IsArray(); }

    /**
     * @brief Проверяет, является ли значение DisplayObject (элементом интерфейса).
     * @return true, если DisplayObject.
     */
    bool is_display_object() const { return value_.IsDisplayObject(); }

    // Getters

    /**
     * @brief Получает булево значение.
     * @return Булево значение.
     */
    bool get_bool() const { return value_.GetBool(); }

    /**
     * @brief Получает числовое значение.
     * @return Числовое значение.
     */
    double get_number() const { return value_.GetNumber(); }

    /**
     * @brief Получает строковое значение.
     * @return Строковое значение.
     */
    std::string get_string() const { return value_.GetString(); }

    // Setters

    /**
     * @brief Устанавливает булево значение.
     * @param val Новое значение.
     */
    void set_bool(bool val) { value_ = val; }

    /**
     * @brief Устанавливает числовое значение.
     * @param val Новое значение.
     */
    void set_number(double val) { value_ = val; }

    /**
     * @brief Устанавливает строковое значение.
     * @param val Новое значение.
     */
    void set_string(const std::string& val) { value_ = val; }

    /**
     * @brief Устанавливает значение в undefined.
     */
    void set_undefined() { value_.SetUndefined(); }

    /**
     * @brief Устанавливает значение в null.
     */
    void set_null() { value_.SetNull(); }

    // Object/Array operations

    /**
     * @brief Проверяет наличие члена объекта по имени.
     * @param name Имя члена.
     * @return true, если член существует.
     */
    bool has_member(const std::string& name) const { return value_.HasMember(name.c_str()); }

    /**
     * @brief Получает член объекта по имени.
     * @param name Имя члена.
     * @return Значение члена как GFxValue.
     */
    gfx_value get_member(const std::string& name) const {
        RE::GFxValue result;
        value_.GetMember(name.c_str(), &result);
        return gfx_value(result);
    }

    /**
     * @brief Устанавливает член объекта.
     * @param name Имя члена.
     * @param val Значение для установки.
     */
    void set_member(const std::string& name, const gfx_value& val) {
        value_.SetMember(name.c_str(), val.get());
    }

    /**
     * @brief Вызывает метод объекта.
     * @param name Имя метода.
     * @param args Аргументы вызова.
     */
    void invoke(const std::string& name, const std::vector<gfx_value>& args) {
        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) gfx_args.push_back(arg.get());
        value_.Invoke(name.c_str(), nullptr, gfx_args.data(), gfx_args.size());
    }

    /**
     * @brief Вызывает метод объекта и возвращает результат.
     * @param name Имя метода.
     * @param args Аргументы вызова.
     * @return Результат вызова как GFxValue.
     */
    gfx_value invoke_result(const std::string& name, const std::vector<gfx_value>& args) {
        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) gfx_args.push_back(arg.get());
        RE::GFxValue result;
        value_.Invoke(name.c_str(), &result, gfx_args.data(), gfx_args.size());
        return gfx_value(result);
    }

    /**
     * @brief Получает размер массива.
     * @return Размер массива.
     */
    uint32_t get_array_size() const { return value_.GetArraySize(); }

    /**
     * @brief Устанавливает размер массива.
     * @param size Новый размер.
     */
    void set_array_size(uint32_t size) { value_.SetArraySize(size); }

    /**
     * @brief Получает элемент массива по индексу.
     * @param index Индекс элемента.
     * @return Значение элемента.
     */
    gfx_value get_element(uint32_t index) const {
        RE::GFxValue result;
        value_.GetElement(index, &result);
        return gfx_value(result);
    }

    /**
     * @brief Устанавливает элемент массива по индексу.
     * @param index Индекс элемента.
     * @param val Новое значение.
     */
    void set_element(uint32_t index, const gfx_value& val) {
        value_.SetElement(index, val.get());
    }

    /**
     * @brief Добавляет элемент в конец массива.
     * @param val Значение для добавления.
     */
    void push_back(const gfx_value& val) {
        value_.PushBack(val.get());
    }

    // Text

    /**
     * @brief Устанавливает текст (для текстовых полей).
     * @param text Текст.
     */
    void set_text(const std::string& text) { value_.SetText(text.c_str()); }

    /**
     * @brief Устанавливает HTML текст (для текстовых полей).
     * @param html HTML текст.
     */
    void set_text_html(const std::string& html) { value_.SetTextHTML(html.c_str()); }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<gfx_value>("GFxValue");
      cls.ctor<>();

      // Use funcStatic for static methods
      cls.funcStatic<&gfx_value::from_bool>("fromBool");
      cls.funcStatic<&gfx_value::from_number>("fromNumber");
      cls.funcStatic<&gfx_value::from_string>("fromString");

      cls.func<&gfx_value::is_undefined>("isUndefined");
      cls.func<&gfx_value::is_null>("isNull");
      cls.func<&gfx_value::is_bool>("isBool");
      cls.func<&gfx_value::is_number>("isNumber");
      cls.func<&gfx_value::is_string>("isString");
      cls.func<&gfx_value::is_object>("isObject");
      cls.func<&gfx_value::is_array>("isArray");
      cls.func<&gfx_value::is_display_object>("isDisplayObject");

      cls.func<&gfx_value::get_bool>("getBool");
      cls.func<&gfx_value::get_number>("getNumber");
      cls.func<&gfx_value::get_string>("getString");

      cls.func<&gfx_value::set_bool>("setBool");
      cls.func<&gfx_value::set_number>("setNumber");
      cls.func<&gfx_value::set_string>("setString");
      cls.func<&gfx_value::set_undefined>("setUndefined");
      cls.func<&gfx_value::set_null>("setNull");

      cls.func<&gfx_value::has_member>("hasMember");
      cls.func<&gfx_value::get_member>("getMember");
      cls.func<&gfx_value::set_member>("setMember");
      cls.func<&gfx_value::invoke>("invoke");
      cls.func<&gfx_value::invoke_result>("invokeResult");

      cls.func<&gfx_value::get_array_size>("getArraySize");
      cls.func<&gfx_value::set_array_size>("setArraySize");
      cls.func<&gfx_value::get_element>("getElement");
      cls.func<&gfx_value::set_element>("setElement");
      cls.func<&gfx_value::push_back>("pushBack");

      cls.func<&gfx_value::set_text>("setText");
      cls.func<&gfx_value::set_text_html>("setTextHTML");
    }

  private:
    RE::GFxValue value_;
  };
}
