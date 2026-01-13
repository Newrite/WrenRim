#pragma once

#include "Wren/Wrappers/Actor.hpp"
#include "Wren/Wrappers/Setting.hpp"
#include "pch.h"

namespace wren::wrappers
{
  /**
   * @brief Статический класс с глобальными функциями игры.
   */
  class game
  {
  public:
    /**
     * @brief Получает объект игрока.
     * @return Объект actor, представляющий игрока.
     */
    static actor get_player()
    {
      auto player = RE::PlayerCharacter::GetSingleton();
      return actor(player);
    }

    /**
     * @brief Выводит уведомление в левом верхнем углу экрана.
     * @param notification Текст уведомления.
     */
    static void debug_notification(const std::string& notification)
    {
      RE::DebugNotification(notification.c_str());
    }

    /**
     * @brief Выводит модальное окно с сообщением (MessageBox).
     * @param message Текст сообщения.
     */
    static void debug_message_box(const std::string& message)
    {
        RE::DebugMessageBox(message.c_str());
    }

    #undef PlaySound
    /**
     * @brief Проигрывает звук по его EditorID.
     * @param editor_id EditorID звука.
     */
    static void play_sound(const std::string& editor_id)
    {
        RE::PlaySound(editor_id.c_str());
    }

    /**
     * @brief Трясет камеру.
     * @param strength Сила тряски (0.0 - 1.0).
     * @param position Позиция источника тряски (массив из 3 float: [x, y, z]). Если меньше 3 элементов, используется (0,0,0).
     * @param duration Длительность тряски в секундах.
     */
    static void shake_camera(float strength, const std::vector<float>& position, float duration)
    {
        RE::NiPoint3 pos(0.f, 0.f, 0.f);
        if (position.size() >= 3) {
            pos.x = position[0];
            pos.y = position[1];
            pos.z = position[2];
        }
        RE::ShakeCamera(strength, pos, duration);
    }

    /**
     * @brief Возвращает время работы приложения в миллисекундах.
     * @return Время в мс.
     */
    static uint32_t get_duration_of_application_run_time()
    {
        return RE::GetDurationOfApplicationRunTime();
    }

    /**
     * @brief Возвращает время, прошедшее с последнего кадра (дельта времени).
     * @return Время в секундах.
     */
    static float get_seconds_since_last_frame()
    {
        return RE::GetSecondsSinceLastFrame();
    }

    /**
     * @brief Получает настройку INI по имени.
     * @param name Имя настройки (например, "fJumpHeightMin:GamePlay").
     * @return Объект setting.
     */
    static setting get_ini_setting(const std::string& name)
    {
        auto s = RE::GetINISetting(name.c_str());
        return setting(s);
    }

    static void bind(wrenbind17::ForeignModule& module)
    {
      auto& cls = module.klass<game>("Game");
      cls.funcStatic<&game::get_player>("getPlayer");
      cls.funcStatic<&game::debug_notification>("debugNotification");
      cls.funcStatic<&game::debug_message_box>("debugMessageBox");
      cls.funcStatic<&game::play_sound>("playSound");
      cls.funcStatic<&game::shake_camera>("shakeCamera");
      cls.funcStatic<&game::get_duration_of_application_run_time>("getDurationOfApplicationRunTime");
      cls.funcStatic<&game::get_seconds_since_last_frame>("getSecondsSinceLastFrame");
      cls.funcStatic<&game::get_ini_setting>("getINISetting");
    }
  };
}
