// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Armor)
// Module: Skyrim/Armor

foreign class Armor {
    construct new() {}
    
    foreign isValid()
    foreign getName()
    foreign getWeight()
    foreign getValue()
    foreign getArmorRating()
    
    // Returns: 0 = Light, 1 = Heavy, 2 = Clothing
    foreign getArmorType()
    
    static Light { 0 }
    static Heavy { 1 }
    static Clothing { 2 }
}