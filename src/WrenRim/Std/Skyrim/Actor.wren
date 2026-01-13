// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Actor)
// Module: Skyrim/Actor

/**
 * Представляет актера (NPC или игрока) в игровом мире.
 */
foreign class Actor {
    /**
     * Создает новый (пустой/невалидный) объект Actor.
     */
    construct new() {}

    /**
     * Создает объект Actor по его FormID.
     * @param formId {Num} FormID актера.
     */
    construct new(formId) {}
    
    /**
     * Возвращает имя актера.
     * @return {String} Имя актера.
     */
    foreign getName()

    /**
     * Проверяет, является ли актер валидным.
     * @return {Bool} true, если актер существует.
     */
    foreign isValid()

    /**
     * Получает значение ActorValue по его числовому ID.
     * @param actorValue {Num} ID ActorValue (см. класс ActorValue).
     * @return {Num} Значение.
     */
    foreign getActorValueByEnum(actorValue)

    /**
     * Получает значение ActorValue по его имени.
     * @param actorValue {String} Имя ActorValue (например, "Health").
     * @return {Num} Значение.
     */
    foreign getActorValueByString(actorValue)

    /**
     * Изменяет значение ActorValue по его числовому ID.
     * @param actorValue {Num} ID ActorValue.
     * @param value {Num} Значение изменения (может быть отрицательным).
     */
    foreign modActorValueByEnum(actorValue, value)

    /**
     * Изменяет значение ActorValue по его имени.
     * @param actorValue {String} Имя ActorValue.
     * @param value {Num} Значение изменения.
     */
    foreign modActorValueByString(actorValue, value)

    /**
     * Восстанавливает значение ActorValue по его числовому ID.
     * @param actorValue {Num} ID ActorValue.
     * @param value {Num} Значение восстановления.
     */
    foreign restoreActorValueByEnum(actorValue, value)

    /**
     * Восстанавливает значение ActorValue по его имени.
     * @param actorValue {String} Имя ActorValue.
     * @param value {Num} Значение восстановления.
     */
    foreign restoreActorValueByString(actorValue, value)

    /**
     * Наносит урон значению ActorValue по его числовому ID.
     * @param actorValue {Num} ID ActorValue.
     * @param value {Num} Значение урона.
     */
    foreign damageActorValueByEnum(actorValue, value)

    /**
     * Наносит урон значению ActorValue по его имени.
     * @param actorValue {String} Имя ActorValue.
     * @param value {Num} Значение урона.
     */
    foreign damageActorValueByString(actorValue, value)

    /**
     * Проверяет наличие ключевого слова у актера.
     * @param keyword {Keyword} Объект Keyword.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeyword(keyword)

    /**
     * Проверяет наличие ключевого слова по EditorID.
     * @param editorId {String} EditorID ключевого слова.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeywordString(editorId)
}
