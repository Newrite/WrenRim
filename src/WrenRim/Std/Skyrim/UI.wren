// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.UI)
// Module: Skyrim/UI

foreign class UI {
    static foreign isMenuOpen(menuName)
    
    // Returns the _root GFxValue of the menu
    static foreign getMovieView(menuName)

    // Invokes a function on the menu's root MovieClip
    // args must be a list of GFxValue or compatible types (bool, num, string)
    static foreign invoke(menuName, target, args)
    static foreign invokeResult(menuName, target, args)

    static foreign setBool(menuName, target, value)
    static foreign setNumber(menuName, target, value)
    static foreign setString(menuName, target, value)

    static foreign getBool(menuName, target)
    static foreign getNumber(menuName, target)
    static foreign getString(menuName, target)
}
