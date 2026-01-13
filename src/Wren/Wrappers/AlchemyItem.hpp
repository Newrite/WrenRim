#pragma once

#include "pch.h"
#include "Wren/Wrappers/Keyword.hpp"

namespace wren::wrappers
{
  /**
   * @brief Обертка для класса RE::AlchemyItem.
   * Представляет алхимические предметы: зелья, яды и еду.
   */
  class alchemy_item
  {
  public:
    /**
     * @brief Конструктор из указателя на RE::AlchemyItem.
     * @param p Указатель на RE::AlchemyItem.
     */
    alchemy_item(RE::AlchemyItem* p)
    {
      if (p) {
        form_id_ = p->GetFormID();
      }
    }

    /**
     * @brief Конструктор из FormID.
     * @param form_id ID формы алхимического предмета.
     */
    alchemy_item(const RE::FormID form_id)
    {
      if (auto p = RE::TESForm::LookupByID<RE::AlchemyItem>(form_id)) {
        item_ = p;
        form_id_ = form_id;
      }
    }

    /**
     * @brief Конструктор по умолчанию. Создает невалидную обертку.
     */
    alchemy_item() = default;

    /**
     * @brief Проверяет валидность предмета.
     * @return true, если предмет существует и валиден.
     */
    auto is_valid() const -> bool
    {
      const auto potion = get();
      return static_cast<bool>(potion);
    }

    /**
     * @brief Получает указатель на RE::AlchemyItem.
     * @return Указатель на RE::AlchemyItem или nullptr.
     */
    [[nodiscard]] RE::AlchemyItem* get() const
    {
      if (item_) return item_;
      return RE::TESForm::LookupByID<RE::AlchemyItem>(form_id_);
    }

    /**
     * @brief Проверяет, является ли предмет ядом.
     * @return true, если это яд.
     */
    bool is_poison() const
    {
      auto ptr = get();
      return ptr ? ptr->IsPoison() : false;
    }

    /**
     * @brief Проверяет, является ли предмет едой.
     * @return true, если это еда.
     */
    bool is_food() const
    {
      auto ptr = get();
      return ptr ? ptr->IsFood() : false;
    }

    /**
     * @brief Получает имя предмета.
     * @return Имя предмета или пустая строка.
     */
    std::string get_name() const
    {
      auto ptr = get();
      return ptr ? ptr->GetName() : "";
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
      auto& cls = module.klass<alchemy_item>("AlchemyItem");
      cls.ctor<>();
      cls.ctor<const RE::FormID>();
      cls.func<&alchemy_item::is_valid>("isValid");
      cls.func<&alchemy_item::is_poison>("isPoison");
      cls.func<&alchemy_item::is_food>("isFood");
      cls.func<&alchemy_item::get_name>("getName");
      cls.func<&alchemy_item::has_keyword>("hasKeyword");
      cls.func<&alchemy_item::has_keyword_id>("hasKeywordID");
      cls.func<&alchemy_item::has_keyword_string>("hasKeywordString");
    }

  private:
    RE::AlchemyItem* item_{nullptr};
    RE::FormID form_id_{0};
  };
}
