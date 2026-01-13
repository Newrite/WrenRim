// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Armor)
// Module: Skyrim/Armor

/**
 * Представляет броню или одежду.
 */
foreign class Armor {
    /**
     * Создает новый (пустой/невалидный) объект Armor.
     */
    construct new() {}

    /**
     * Создает объект Armor по его FormID.
     * @param formId {Num} FormID брони.
     */
    construct new(formId) {}
    
    /**
     * Проверяет, является ли броня валидной.
     * @return {Bool} true, если броня существует.
     */
    foreign isValid()

    /**
     * Возвращает имя брони.
     * @return {String} Имя брони.
     */
    foreign getName()

    /**
     * Возвращает вес брони.
     * @return {Num} Вес.
     */
    foreign getWeight()

    /**
     * Возвращает стоимость брони в золоте.
     * @return {Num} Стоимость.
     */
    foreign getValue()

    /**
     * Возвращает рейтинг брони (защиту).
     * @return {Num} Рейтинг брони.
     */
    foreign getArmorRating()
    
    /**
     * Возвращает тип брони.
     * @return {Num} 0 = Легкая, 1 = Тяжелая, 2 = Одежда, -1 = Ошибка.
     */
    foreign getArmorType()
    
    /**
     * Проверяет наличие ключевого слова.
     * @param keyword {Keyword} Объект Keyword.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeyword(keyword)

    /**
     * Проверяет наличие ключевого слова по FormID.
     * @param formId {Num} FormID ключевого слова.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeywordID(formId)

    /**
     * Проверяет наличие ключевого слова по EditorID.
     * @param editorId {String} EditorID ключевого слова.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeywordString(editorId)

    static Light { 0 }
    static Heavy { 1 }
    static Clothing { 2 }
}
