// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Spell)
// Module: Skyrim/Spell

foreign class Spell {
    construct new() {}
    
    foreign isValid()
    foreign getName()
    foreign getCastTime()
    foreign getMagickaCost()
    foreign getEffectiveMagickaCost(caster)
    foreign getSpellType()
    foreign getCastingType()
    foreign getDelivery()
    foreign isHostile()
    foreign setCastTime(time)

    // Spell Types
    static TypeSpell { 0 }
    static TypeDisease { 1 }
    static TypePower { 2 }
    static TypeLesserPower { 3 }
    static TypeAbility { 4 }
    static TypePoison { 5 }
    static TypeAddition { 6 }
    static TypeVoice { 7 }

    // Casting Types
    static CastingConstant { 0 }
    static CastingFireAndForget { 1 }
    static CastingConcentration { 2 }
    static CastingScroll { 3 }

    // Delivery Types
    static DeliverySelf { 0 }
    static DeliveryTouch { 1 }
    static DeliveryAimed { 2 }
    static DeliveryTargetActor { 3 }
    static DeliveryTargetLocation { 4 }
}
