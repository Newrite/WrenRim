import "Events" for Events
import "Skyrim/ActorValue" for ActorValue

System.print("[ReflectPhysicalDamage] Mod loaded!")

Events.on("OnWeaponHitStart", Fn.new { |hitData|
      var attacker = hitData.getAttacker()
      var target = hitData.getTarget()
      var damage = hitData.getTotalDamage()
      System.print("[ReflectPhysicalDamage] Attacker: %(attacker), Target: %(target), Damage: %(damage)")
      attacker.damageActorValueByEnum(ActorValue.Health, damage)
})
