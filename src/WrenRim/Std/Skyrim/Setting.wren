// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Setting)
// Module: Skyrim/Setting

/**
 * Представляет настройку игры (INI setting).
 */
foreign class Setting {
    /**
     * Создает новый (пустой/невалидный) объект Setting.
     */
    construct new() {}
    
    /**
     * Проверяет, является ли настройка валидной (найденной).
     * @return {Bool} true, если настройка существует.
     */
    foreign isValid()
    
    /**
     * Возвращает тип настройки.
     * @return {Num} Тип: 0=Unknown, 1=Bool, 2=Float, 3=Int, 4=Color, 5=String, 6=UInt.
     */
    foreign getType()
    
    /**
     * Возвращает значение настройки как bool.
     * @return {Bool} Значение.
     */
    foreign getBool()

    /**
     * Возвращает значение настройки как float.
     * @return {Num} Значение.
     */
    foreign getFloat()

    /**
     * Возвращает значение настройки как int.
     * @return {Num} Значение.
     */
    foreign getInt()

    /**
     * Возвращает значение настройки как string.
     * @return {String} Значение.
     */
    foreign getString()

    /**
     * Возвращает значение настройки как unsigned int.
     * @return {Num} Значение.
     */
    foreign getUInt()

    static TypeUnknown { 0 }
    static TypeBool { 1 }
    static TypeFloat { 2 }
    static TypeInt { 3 }
    static TypeColor { 4 }
    static TypeString { 5 }
    static TypeUInt { 6 }
}
