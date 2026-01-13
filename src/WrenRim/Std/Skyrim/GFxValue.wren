// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.GFxValue)
// Module: Skyrim/GFxValue

foreign class GFxValue {
    construct new() {}
    
    static foreign fromBool(val)
    static foreign fromNumber(val)
    static foreign fromString(val)

    foreign isUndefined()
    foreign isNull()
    foreign isBool()
    foreign isNumber()
    foreign isString()
    foreign isObject()
    foreign isArray()
    foreign isDisplayObject()

    foreign getBool()
    foreign getNumber()
    foreign getString()

    foreign setBool(val)
    foreign setNumber(val)
    foreign setString(val)
    foreign setUndefined()
    foreign setNull()

    foreign hasMember(name)
    foreign getMember(name)
    foreign setMember(name, val)
    foreign invoke(name, args)
    foreign invokeResult(name, args)

    foreign getArraySize()
    foreign setArraySize(size)
    foreign getElement(index)
    foreign setElement(index, val)
    foreign pushBack(val)

    foreign setText(text)
    foreign setTextHTML(html)
}
