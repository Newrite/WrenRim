// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.AlchemyItem)
// Module: Skyrim/AlchemyItem

/**
 * Представляет алхимический предмет (зелье, яд, еда).
 */
foreign class AlchemyItem {
    /**
     * Создает новый (пустой/невалидный) объект AlchemyItem.
     */
    construct new() {}

    /**
     * Создает объект AlchemyItem по его FormID.
     * @param formId {Num} FormID предмета.
     */
    construct new(formId) {}
    
    /**
     * Проверяет, является ли предмет валидным.
     * @return {Bool} true, если предмет существует.
     */
    foreign isValid()

    /**
     * Проверяет, является ли предмет ядом.
     * @return {Bool} true, если это яд.
     */
    foreign isPoison()

    /**
     * Проверяет, является ли предмет едой.
     * @return {Bool} true, если это еда.
     */
    foreign isFood()

    /**
     * Возвращает имя предмета.
     * @return {String} Имя предмета.
     */
    foreign getName()

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
}
