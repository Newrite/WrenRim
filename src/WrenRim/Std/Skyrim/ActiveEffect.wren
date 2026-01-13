// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.ActiveEffect)
// Module: Skyrim/ActiveEffect

/**
 * Представляет активный магический эффект в игре.
 */
foreign class ActiveEffect {
    /**
     * Создает новый (пустой/невалидный) объект ActiveEffect.
     */
    construct new() {}

    /**
     * Проверяет, является ли эффект валидным.
     * @return {Bool} true, если эффект существует.
     */
    foreign isValid()

    /**
     * Возвращает длительность эффекта в секундах.
     * @return {Num} Длительность.
     */
    foreign getDuration()

    /**
     * Возвращает величину (силу) эффекта.
     * @return {Num} Величина.
     */
    foreign getMagnitude()

    /**
     * Возвращает время, прошедшее с начала действия эффекта.
     * @return {Num} Прошедшее время в секундах.
     */
    foreign getElapsed()

    /**
     * Устанавливает величину (силу) эффекта.
     * @param value {Num} Новое значение величины.
     */
    foreign setMagnitude(value)

    /**
     * Возвращает актера, наложившего эффект (кастера).
     * @return {Actor} Кастер.
     */
    foreign getCaster()

    /**
     * Возвращает актера, на которого наложен эффект (цель).
     * @return {Actor} Цель.
     */
    foreign getTarget()
}
