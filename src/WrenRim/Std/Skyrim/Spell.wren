// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Spell)
// Module: Skyrim/Spell

/**
 * Представляет заклинание.
 */
foreign class Spell {
    /**
     * Создает новый (пустой/невалидный) объект Spell.
     */
    construct new() {}

    /**
     * Создает объект Spell по его FormID.
     * @param formId {Num} FormID заклинания.
     */
    construct new(formId) {}
    
    /**
     * Проверяет, является ли заклинание валидным.
     * @return {Bool} true, если заклинание существует.
     */
    foreign isValid()

    /**
     * Возвращает имя заклинания.
     * @return {String} Имя.
     */
    foreign getName()

    /**
     * Возвращает время каста (зарядки).
     * @return {Num} Время в секундах.
     */
    foreign getCastTime()

    /**
     * Возвращает базовую стоимость магии.
     * @return {Num} Стоимость.
     */
    foreign getMagickaCost()

    /**
     * Вычисляет эффективную стоимость магии для кастера.
     * @param caster {Actor} Кастер.
     * @return {Num} Эффективная стоимость.
     */
    foreign getEffectiveMagickaCost(caster)

    /**
     * Возвращает тип заклинания.
     * @return {Num} 0=Spell, 1=Disease, 2=Power, 3=LesserPower, 4=Ability, 5=Poison, 6=Addition, 7=Voice.
     */
    foreign getSpellType()

    /**
     * Возвращает тип каста.
     * @return {Num} 0=ConstantEffect, 1=FireAndForget, 2=Concentration, 3=Scroll.
     */
    foreign getCastingType()

    /**
     * Возвращает тип доставки (Delivery).
     * @return {Num} 0=Self, 1=Touch, 2=Aimed, 3=TargetActor, 4=TargetLocation.
     */
    foreign getDelivery()

    /**
     * Проверяет, является ли заклинание враждебным.
     * @return {Bool} true, если враждебное.
     */
    foreign isHostile()

    /**
     * Устанавливает время каста (зарядки).
     * @param time {Num} Время в секундах.
     */
    foreign setCastTime(time)

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
