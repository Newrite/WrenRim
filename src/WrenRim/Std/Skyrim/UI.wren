// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.UI)
// Module: Skyrim/UI

/**
 * Статический класс для работы с пользовательским интерфейсом (UI).
 */
foreign class UI {
    /**
     * Проверяет, открыто ли меню.
     * @param menuName {String} Имя меню (например, "InventoryMenu").
     * @return {Bool} true, если меню открыто.
     */
    static foreign isMenuOpen(menuName)
    
    /**
     * Получает корневой объект (MovieView) меню.
     * @param menuName {String} Имя меню.
     * @return {GFxValue} Объект _root меню.
     */
    static foreign getMovieView(menuName)

    /**
     * Вызывает функцию ActionScript в меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к функции (например, "_root.MyFunction").
     * @param args {List} Список аргументов (GFxValue).
     */
    static foreign invoke(menuName, target, args)

    /**
     * Вызывает функцию ActionScript в меню и возвращает результат.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к функции.
     * @param args {List} Список аргументов (GFxValue).
     * @return {GFxValue} Результат вызова.
     */
    static foreign invokeResult(menuName, target, args)

    /**
     * Устанавливает булеву переменную в меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @param value {Bool} Значение.
     */
    static foreign setBool(menuName, target, value)

    /**
     * Устанавливает числовую переменную в меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @param value {Num} Значение.
     */
    static foreign setNumber(menuName, target, value)

    /**
     * Устанавливает строковую переменную в меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @param value {String} Значение.
     */
    static foreign setString(menuName, target, value)

    /**
     * Получает булеву переменную из меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @return {Bool} Значение.
     */
    static foreign getBool(menuName, target)

    /**
     * Получает числовую переменную из меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @return {Num} Значение.
     */
    static foreign getNumber(menuName, target)

    /**
     * Получает строковую переменную из меню.
     * @param menuName {String} Имя меню.
     * @param target {String} Путь к переменной.
     * @return {String} Значение.
     */
    static foreign getString(menuName, target)
}
