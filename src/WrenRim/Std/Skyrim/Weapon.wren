// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Weapon)
// Module: Skyrim/Weapon

/**
 * Представляет оружие.
 */
foreign class Weapon {
  /**
   * Создает новый (пустой/невалидный) объект Weapon.
   */
  construct new() {}

  /**
   * Создает объект Weapon по его FormID.
   * @param formId {Num} FormID оружия.
   */
  construct new(formId) {}
  
  /**
   * Возвращает имя оружия.
   * @return {String} Имя.
   */
  foreign getName()

  /**
   * Проверяет, является ли оружие валидным.
   * @return {Bool} true, если оружие существует.
   */
  foreign isValid()

  /**
   * Возвращает базовый урон оружия.
   * @return {Num} Урон.
   */
  foreign getAttackDamage()

  /**
   * Возвращает дальность атаки.
   * @return {Num} Дальность.
   */
  foreign getReach()

  /**
   * Возвращает скорость атаки.
   * @return {Num} Скорость.
   */
  foreign getSpeed()

  /**
   * Возвращает вес оружия.
   * @return {Num} Вес.
   */
  foreign getWeight()

  /**
   * Возвращает стоимость оружия в золоте.
   * @return {Num} Стоимость.
   */
  foreign getValue()

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
}
