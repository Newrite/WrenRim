import "Events" for Events
import "Skyrim/ActorValue" for ActorValue

System.print("[RegenerateHealth] Mod loaded!")

Events.on("OnUpdatePlayer", Fn.new { |actor, delta|
  actor.restoreActorValueByEnum(ActorValue.Health, 1 * delta)
})
