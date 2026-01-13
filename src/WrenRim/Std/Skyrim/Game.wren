// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Game)
// Module: Skyrim/Game

/**
 * Глобальные функции игры.
 */
foreign class Game {
    /**
     * Возвращает объект игрока.
     * @return {Actor} Игрок.
     */
    static foreign getPlayer()

    /**
     * Выводит уведомление в левом верхнем углу экрана.
     * @param message {String} Текст уведомления.
     */
    static foreign debugNotification(message)

    /**
     * Выводит модальное окно с сообщением.
     * @param message {String} Текст сообщения.
     */
    static foreign debugMessageBox(message)

    /**
     * Проигрывает звук по его EditorID.
     * @param editorId {String} EditorID звука.
     */
    static foreign playSound(editorId)

    /**
     * Трясет камеру.
     * @param strength {Num} Сила тряски (0.0 - 1.0).
     * @param position {List} Позиция источника [x, y, z].
     * @param duration {Num} Длительность в секундах.
     */
    static foreign shakeCamera(strength, position, duration)

    /**
     * Возвращает время работы приложения в миллисекундах.
     * @return {Num} Время в мс.
     */
    static foreign getDurationOfApplicationRunTime()

    /**
     * Возвращает время, прошедшее с последнего кадра.
     * @return {Num} Время в секундах.
     */
    static foreign getSecondsSinceLastFrame()

    /**
     * Получает настройку INI по имени.
     * @param name {String} Имя настройки (например, "fJumpHeightMin:GamePlay").
     * @return {Setting} Объект настройки.
     */
    static foreign getINISetting(name)
}
