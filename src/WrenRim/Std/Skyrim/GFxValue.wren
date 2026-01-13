// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.GFxValue)
// Module: Skyrim/GFxValue

/**
 * Представляет значение в Scaleform (Flash) интерфейсе.
 * Может быть числом, строкой, булевым значением, объектом или массивом.
 */
foreign class GFxValue {
    /**
     * Создает новое undefined значение.
     */
    construct new() {}
    
    /**
     * Создает GFxValue из булева значения.
     * @param val {Bool} Значение.
     * @return {GFxValue} Новый объект.
     */
    static foreign fromBool(val)

    /**
     * Создает GFxValue из числа.
     * @param val {Num} Значение.
     * @return {GFxValue} Новый объект.
     */
    static foreign fromNumber(val)

    /**
     * Создает GFxValue из строки.
     * @param val {String} Значение.
     * @return {GFxValue} Новый объект.
     */
    static foreign fromString(val)

    /**
     * Проверяет, является ли значение undefined.
     * @return {Bool} true, если undefined.
     */
    foreign isUndefined()

    /**
     * Проверяет, является ли значение null.
     * @return {Bool} true, если null.
     */
    foreign isNull()

    /**
     * Проверяет, является ли значение булевым.
     * @return {Bool} true, если boolean.
     */
    foreign isBool()

    /**
     * Проверяет, является ли значение числом.
     * @return {Bool} true, если number.
     */
    foreign isNumber()

    /**
     * Проверяет, является ли значение строкой.
     * @return {Bool} true, если string.
     */
    foreign isString()

    /**
     * Проверяет, является ли значение объектом.
     * @return {Bool} true, если object.
     */
    foreign isObject()

    /**
     * Проверяет, является ли значение массивом.
     * @return {Bool} true, если array.
     */
    foreign isArray()

    /**
     * Проверяет, является ли значение DisplayObject (элементом интерфейса).
     * @return {Bool} true, если DisplayObject.
     */
    foreign isDisplayObject()

    /**
     * Получает булево значение.
     * @return {Bool} Значение.
     */
    foreign getBool()

    /**
     * Получает числовое значение.
     * @return {Num} Значение.
     */
    foreign getNumber()

    /**
     * Получает строковое значение.
     * @return {String} Значение.
     */
    foreign getString()

    /**
     * Устанавливает булево значение.
     * @param val {Bool} Новое значение.
     */
    foreign setBool(val)

    /**
     * Устанавливает числовое значение.
     * @param val {Num} Новое значение.
     */
    foreign setNumber(val)

    /**
     * Устанавливает строковое значение.
     * @param val {String} Новое значение.
     */
    foreign setString(val)

    /**
     * Устанавливает значение в undefined.
     */
    foreign setUndefined()

    /**
     * Устанавливает значение в null.
     */
    foreign setNull()

    /**
     * Проверяет наличие члена объекта по имени.
     * @param name {String} Имя члена.
     * @return {Bool} true, если член существует.
     */
    foreign hasMember(name)

    /**
     * Получает член объекта по имени.
     * @param name {String} Имя члена.
     * @return {GFxValue} Значение члена.
     */
    foreign getMember(name)

    /**
     * Устанавливает член объекта.
     * @param name {String} Имя члена.
     * @param val {GFxValue} Значение для установки.
     */
    foreign setMember(name, val)

    /**
     * Вызывает метод объекта.
     * @param name {String} Имя метода.
     * @param args {List} Список аргументов (GFxValue).
     */
    foreign invoke(name, args)

    /**
     * Вызывает метод объекта и возвращает результат.
     * @param name {String} Имя метода.
     * @param args {List} Список аргументов (GFxValue).
     * @return {GFxValue} Результат вызова.
     */
    foreign invokeResult(name, args)

    /**
     * Получает размер массива.
     * @return {Num} Размер массива.
     */
    foreign getArraySize()

    /**
     * Устанавливает размер массива.
     * @param size {Num} Новый размер.
     */
    foreign setArraySize(size)

    /**
     * Получает элемент массива по индексу.
     * @param index {Num} Индекс элемента.
     * @return {GFxValue} Значение элемента.
     */
    foreign getElement(index)

    /**
     * Устанавливает элемент массива по индексу.
     * @param index {Num} Индекс элемента.
     * @param val {GFxValue} Новое значение.
     */
    foreign setElement(index, val)

    /**
     * Добавляет элемент в конец массива.
     * @param val {GFxValue} Значение для добавления.
     */
    foreign pushBack(val)

    /**
     * Устанавливает текст (для текстовых полей).
     * @param text {String} Текст.
     */
    foreign setText(text)

    /**
     * Устанавливает HTML текст (для текстовых полей).
     * @param html {String} HTML текст.
     */
    foreign setTextHTML(html)
}
