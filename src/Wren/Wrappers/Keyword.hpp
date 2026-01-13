#pragma once

#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::BGSKeyword.
   * Представляет ключевое слово (тег), используемое для классификации объектов в игре.
   */
  class keyword
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::BGSKeyword.
     * @param k Указатель на RE::BGSKeyword.
     */
    keyword(RE::BGSKeyword* k)
    {
      if (k) {
        keyword_ = k;
        form_id_ = k->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы ключевого слова.
     */
    keyword(const RE::FormID form_id)
    {
      if (auto k = RE::TESForm::LookupByID<RE::BGSKeyword>(form_id)) {
        keyword_ = k;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор из EditorID (строкового идентификатора).
     * @param editor_id EditorID ключевого слова (например, "ActorTypeNPC").
     */
    keyword(const std::string& editor_id)
    {
        if (auto k = RE::TESForm::LookupByEditorID<RE::BGSKeyword>(editor_id)) {
            keyword_ = k;
            form_id_ = k->GetFormID();
        }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    keyword() = default;

    /**
     * @brief Получает указатель на RE::BGSKeyword.
     * @return Указатель на RE::BGSKeyword или nullptr.
     */
    [[nodiscard]] RE::BGSKeyword* get() const
    {
      if (keyword_) return keyword_;
      return RE::TESForm::LookupByID<RE::BGSKeyword>(form_id_);
    }

    /**
     * @brief Проверяет валидность ключевого слова.
     * @return true, если ключевое слово существует и валидно.
     */
    [[nodiscard]] bool is_valid() const
    {
      return static_cast<bool>(get());
    }

    /**
     * @brief Получает EditorID ключевого слова.
     * @return EditorID или пустая строка.
     */
    std::string get_editor_id() const
    {
      auto ptr = get();
      return ptr ? ptr->GetFormEditorID() : "";
    }

    /**
     * @brief Получает имя ключевого слова (обычно совпадает с EditorID).
     * @return Имя.
     */
    std::string get_name() const
    {
        // Keywords don't really have a "Name" in the UI sense usually, but they are TESForms.
        // Usually we care about EditorID.
        return get_editor_id();
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<keyword>("Keyword");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.ctor<const std::string&>(); // Allow creating by EditorID
      cls.func<&keyword::is_valid>("isValid");
      cls.func<&keyword::get_editor_id>("getEditorID");
      cls.func<&keyword::get_name>("getName");
    }

  private:
    RE::BGSKeyword* keyword_{nullptr};
    RE::FormID form_id_{0};
  };
}
