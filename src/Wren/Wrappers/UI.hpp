#pragma once

#include "Wren/Wrappers/GFxValue.hpp"
#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Статический класс для работы с пользовательским интерфейсом (UI) Skyrim.
   * Позволяет взаимодействовать с Scaleform (Flash) меню.
   */
  class ui
  {
  public:
    /**
     * @brief Проверяет, открыто ли меню с указанным именем.
     * @param menu_name Имя меню (например, "InventoryMenu").
     * @return true, если меню открыто.
     */
    static bool is_menu_open(const std::string& menu_name)
    {
      auto ui = RE::UI::GetSingleton();
      return ui ? ui->IsMenuOpen(menu_name) : false;
    }

    /**
     * @brief Получает корневой объект (MovieView) меню.
     * @param menu_name Имя меню.
     * @return Объект GFxValue, представляющий _root меню, или undefined, если меню не найдено.
     */
    static gfx_value get_movie_view(const std::string& menu_name)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return gfx_value();

        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return gfx_value();

        // Return the root of the movie view as a GFxValue
        RE::GFxValue root;
        movie->GetVariable(&root, "_root");
        return gfx_value(root);
    }

    /**
     * @brief Вызывает функцию ActionScript в указанном меню.
     * @param menu_name Имя меню.
     * @param target Путь к функции (например, "_root.MyFunction").
     * @param args Список аргументов для функции.
     */
    static void invoke(const std::string& menu_name, const std::string& target, const std::vector<gfx_value>& args)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return;

        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return;

        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) {
            gfx_args.push_back(arg.get());
        }

        movie->Invoke(target.c_str(), nullptr, gfx_args.data(), static_cast<uint32_t>(gfx_args.size()));
    }

    /**
     * @brief Вызывает функцию ActionScript в указанном меню и возвращает результат.
     * @param menu_name Имя меню.
     * @param target Путь к функции.
     * @param args Список аргументов.
     * @return Результат выполнения функции как GFxValue.
     */
    static gfx_value invoke_result(const std::string& menu_name, const std::string& target, const std::vector<gfx_value>& args)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return gfx_value();

        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return gfx_value();

        std::vector<RE::GFxValue> gfx_args;
        gfx_args.reserve(args.size());
        for (const auto& arg : args) {
            gfx_args.push_back(arg.get());
        }

        RE::GFxValue result;
        movie->Invoke(target.c_str(), &result, gfx_args.data(), static_cast<uint32_t>(gfx_args.size()));
        return gfx_value(result);
    }

    /**
     * @brief Устанавливает булеву переменную в меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @param value Значение.
     */
    static void set_bool(const std::string& menu_name, const std::string& target, bool value)
    {
         auto ui = RE::UI::GetSingleton();
        if (!ui) return;
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return;

        RE::GFxValue val(value);
        movie->SetVariable(target.c_str(), val);
    }

    /**
     * @brief Устанавливает числовую переменную в меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @param value Значение.
     */
    static void set_number(const std::string& menu_name, const std::string& target, double value)
    {
         auto ui = RE::UI::GetSingleton();
        if (!ui) return;
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return;

        RE::GFxValue val(value);
        movie->SetVariable(target.c_str(), val);
    }

    /**
     * @brief Устанавливает строковую переменную в меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @param value Значение.
     */
    static void set_string(const std::string& menu_name, const std::string& target, const std::string& value)
    {
         auto ui = RE::UI::GetSingleton();
        if (!ui) return;
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return;

        RE::GFxValue val(value.c_str());
        movie->SetVariable(target.c_str(), val);
    }

    /**
     * @brief Получает булеву переменную из меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @return Значение переменной или false, если не найдена.
     */
    static bool get_bool(const std::string& menu_name, const std::string& target)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return false;
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return false;

        RE::GFxValue val;
        if (movie->GetVariable(&val, target.c_str())) {
            return val.GetBool();
        }
        return false;
    }

    /**
     * @brief Получает числовую переменную из меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @return Значение переменной или 0.0.
     */
    static double get_number(const std::string& menu_name, const std::string& target)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return 0.0;
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return 0.0;

        RE::GFxValue val;
        if (movie->GetVariable(&val, target.c_str())) {
            return val.GetNumber();
        }
        return 0.0;
    }

    /**
     * @brief Получает строковую переменную из меню.
     * @param menu_name Имя меню.
     * @param target Путь к переменной.
     * @return Значение переменной или пустая строка.
     */
    static std::string get_string(const std::string& menu_name, const std::string& target)
    {
        auto ui = RE::UI::GetSingleton();
        if (!ui) return "";
        auto movie = ui->GetMovieView(menu_name);
        if (!movie) return "";

        RE::GFxValue val;
        if (movie->GetVariable(&val, target.c_str())) {
            return val.GetString();
        }
        return "";
    }


    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<ui>("UI");
      // Static methods only for UI singleton access
      cls.funcStatic<&ui::is_menu_open>("isMenuOpen");
      cls.funcStatic<&ui::get_movie_view>("getMovieView"); // Returns root GFxValue
      cls.funcStatic<&ui::invoke>("invoke");
      cls.funcStatic<&ui::invoke_result>("invokeResult");

      cls.funcStatic<&ui::set_bool>("setBool");
      cls.funcStatic<&ui::set_number>("setNumber");
      cls.funcStatic<&ui::set_string>("setString");

      cls.funcStatic<&ui::get_bool>("getBool");
      cls.funcStatic<&ui::get_number>("getNumber");
      cls.funcStatic<&ui::get_string>("getString");
    }
  };
}
