// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Effect)
// Module: Skyrim/Effect

/**
 * Представляет базовый эффект (Magic Effect) в игре.
 */
foreign class Effect {
    /**
     * Создает новый (пустой/невалидный) объект Effect.
     */
    construct new() {}

    /**
     * Создает объект Effect по его FormID.
     * @param formId {Num} FormID эффекта.
     */
    construct new(formId) {}

    /**
     * Проверяет валидность эффекта.
     * @return {Bool} true, если эффект существует и валиден.
     */
    foreign isValid()

    /**
     * Получает имя эффекта.
     * @return {String} Имя эффекта или пустая строка.
     */
    foreign getName()

    /**
     * Проверяет, является ли эффект враждебным.
     * @return {Bool} true, если враждебный.
     */
    foreign isHostile()

    /**
     * Проверяет, является ли эффект вредным (Detrimental).
     * @return {Bool} true, если вредный.
     */
    foreign isDetrimental()

    /**
     * Получает базовую стоимость эффекта.
     * @return {Num} Стоимость.
     */
    foreign getBaseCost()

    /**
     * Устанавливает базовую стоимость эффекта.
     * @param cost {Num} Новая стоимость.
     */
    foreign setBaseCost(cost)

    /**
     * Получает архетип эффекта.
     * @return {Num} ID архетипа.
     */
    foreign getArchetype()

    /**
     * Устанавливает архетип эффекта.
     * @param type {Num} ID архетипа.
     */
    foreign setArchetype(type)

    /**
     * Получает связанный навык (Magic Skill).
     * @return {Num} ID навыка (ActorValue).
     */
    foreign getMagicSkill()

    /**
     * Устанавливает связанный навык.
     * @param skill {Num} ID навыка (ActorValue).
     */
    foreign setMagicSkill(skill)

    /**
     * Получает минимальный уровень навыка для использования.
     * @return {Num} Уровень навыка.
     */
    foreign getMinimumSkillLevel()

    /**
     * Устанавливает минимальный уровень навыка.
     * @param level {Num} Уровень навыка.
     */
    foreign setMinimumSkillLevel(level)

    /**
     * Получает тип каста (Casting Type).
     * @return {Num} 0=ConstantEffect, 1=FireAndForget, 2=Concentration, 3=Scroll.
     */
    foreign getCastingType()

    /**
     * Устанавливает тип каста.
     * @param type {Num} Тип каста.
     */
    foreign setCastingType(type)

    /**
     * Получает тип доставки (Delivery).
     * @return {Num} 0=Self, 1=Touch, 2=Aimed, 3=TargetActor, 4=TargetLocation.
     */
    foreign getDelivery()

    /**
     * Устанавливает тип доставки.
     * @param type {Num} Тип доставки.
     */
    foreign setDelivery(type)

    /**
     * Получает множитель использования навыка.
     * @return {Num} Множитель.
     */
    foreign getSkillUsageMult()

    /**
     * Устанавливает множитель использования навыка.
     * @param mult {Num} Новый множитель.
     */
    foreign setSkillUsageMult(mult)

    /**
     * Получает масштаб двойного каста (Dual Cast Scale).
     * @return {Num} Масштаб.
     */
    foreign getDualCastScale()

    /**
     * Устанавливает масштаб двойного каста.
     * @param scale {Num} Новый масштаб.
     */
    foreign setDualCastScale(scale)

    /**
     * Получает переменную сопротивления (Resist Variable).
     * @return {Num} ID ActorValue.
     */
    foreign getResistVariable()

    /**
     * Устанавливает переменную сопротивления.
     * @param var {Num} ID ActorValue.
     */
    foreign setResistVariable(var)

    /**
     * Получает количество противодействующих эффектов.
     * @return {Num} Количество.
     */
    foreign getNumCounterEffects()

    /**
     * Устанавливает количество противодействующих эффектов.
     * @param num {Num} Количество.
     */
    foreign setNumCounterEffects(num)

    /**
     * Получает вес затухания (Taper Weight).
     * @return {Num} Вес.
     */
    foreign getTaperWeight()

    /**
     * Устанавливает вес затухания.
     * @param weight {Num} Вес.
     */
    foreign setTaperWeight(weight)

    /**
     * Получает область создания заклинания (Spellmaking Area).
     * @return {Num} Область.
     */
    foreign getSpellmakingArea()

    /**
     * Устанавливает область создания заклинания.
     * @param area {Num} Область.
     */
    foreign setSpellmakingArea(area)

    /**
     * Получает время зарядки при создании заклинания.
     * @return {Num} Время.
     */
    foreign getSpellmakingChargeTime()

    /**
     * Устанавливает время зарядки при создании заклинания.
     * @param time {Num} Время.
     */
    foreign setSpellmakingChargeTime(time)

    /**
     * Получает кривую затухания (Taper Curve).
     * @return {Num} Кривая.
     */
    foreign getTaperCurve()

    /**
     * Устанавливает кривую затухания.
     * @param curve {Num} Кривая.
     */
    foreign setTaperCurve(curve)

    /**
     * Получает длительность затухания (Taper Duration).
     * @return {Num} Длительность.
     */
    foreign getTaperDuration()

    /**
     * Устанавливает длительность затухания.
     * @param duration {Num} Длительность.
     */
    foreign setTaperDuration(duration)

    /**
     * Получает вес второго атрибута (Second AV Weight).
     * @return {Num} Вес.
     */
    foreign getSecondAVWeight()

    /**
     * Устанавливает вес второго атрибута.
     * @param weight {Num} Вес.
     */
    foreign setSecondAVWeight(weight)

    /**
     * Получает первичный атрибут (Primary AV).
     * @return {Num} ID ActorValue.
     */
    foreign getPrimaryAV()

    /**
     * Устанавливает первичный атрибут.
     * @param av {Num} ID ActorValue.
     */
    foreign setPrimaryAV(av)

    /**
     * Получает вторичный атрибут (Secondary AV).
     * @return {Num} ID ActorValue.
     */
    foreign getSecondaryAV()

    /**
     * Устанавливает вторичный атрибут.
     * @param av {Num} ID ActorValue.
     */
    foreign setSecondaryAV(av)

    /**
     * Получает способность экипировки (Equip Ability).
     * @return {Spell} Объект spell.
     */
    foreign getEquipAbility()

    /**
     * Устанавливает способность экипировки.
     * @param ability {Spell} Объект spell.
     */
    foreign setEquipAbility(ability)

    /**
     * Получает уровень звука каста.
     * @return {Num} Уровень звука (0=Loud, 1=Normal, 2=Silent).
     */
    foreign getCastingSoundLevel()

    /**
     * Устанавливает уровень звука каста.
     * @param level {Num} Уровень звука.
     */
    foreign setCastingSoundLevel(level)

    /**
     * Получает оценку AI (AI Score).
     * @return {Num} Оценка.
     */
    foreign getAIScore()

    /**
     * Устанавливает оценку AI.
     * @param score {Num} Оценка.
     */
    foreign setAIScore(score)

    /**
     * Получает таймер задержки AI.
     * @return {Num} Таймер.
     */
    foreign getAIDelayTimer()

    /**
     * Устанавливает таймер задержки AI.
     * @param timer {Num} Таймер.
     */
    foreign setAIDelayTimer(timer)

    /**
     * Проверяет наличие флага.
     * @param flagBit {Num} Битовая маска флага.
     * @return {Bool} true, если флаг установлен.
     */
    foreign hasFlag(flagBit)

    /**
     * Проверяет наличие ключевого слова (Keyword).
     * @param kw {Keyword} Объект keyword.
     * @return {Bool} true, если ключевое слово есть.
     */
    foreign hasKeyword(kw)

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

    // Sound Levels
    static SoundLoud { 0 }
    static SoundNormal { 1 }
    static SoundSilent { 2 }
}
