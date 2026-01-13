// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.HitData)
// Module: Skyrim/HitData

/**
 * Содержит информацию о попадании (ударе).
 */
foreign class HitData {
    /**
     * Создает новый (пустой/невалидный) объект HitData.
     */
    construct new() {}

    /**
     * Проверяет, являются ли данные валидными.
     * @return {Bool} true, если данные существуют.
     */
    foreign isValid()

    /**
     * Возвращает атакующего (агрессора).
     * @return {Actor} Атакующий.
     */
    foreign getAttacker()

    /**
     * Возвращает цель атаки.
     * @return {Actor} Цель.
     */
    foreign getTarget()

    /**
     * Возвращает общий урон от попадания.
     * @return {Num} Урон.
     */
    foreign getTotalDamage()

    /**
     * Устанавливает общий урон от попадания.
     * @param value {Num} Новое значение урона.
     */
    foreign setTotalDamage(value)

    /**
     * Возвращает оружие, которым был нанесен удар.
     * @return {Weapon} Оружие.
     */
    foreign getWeapon()
}
