// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Keyword)
// Module: Skyrim/Keyword

/**
 * Представляет ключевое слово (тег) для классификации объектов.
 */
foreign class Keyword {
    /**
     * Создает новый (пустой/невалидный) объект Keyword.
     */
    construct new() {}

    /**
     * Создает объект Keyword по FormID или EditorID.
     * @param arg {Num|String} FormID (число) или EditorID (строка).
     */
    construct new(arg) {} 
    
    /**
     * Проверяет, является ли ключевое слово валидным.
     * @return {Bool} true, если ключевое слово существует.
     */
    foreign isValid()

    /**
     * Возвращает EditorID ключевого слова.
     * @return {String} EditorID.
     */
    foreign getEditorID()

    /**
     * Возвращает имя ключевого слова (обычно совпадает с EditorID).
     * @return {String} Имя.
     */
    foreign getName()
}
