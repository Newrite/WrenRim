// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.Actor)
// Module: Skyrim/Actor

foreign class Actor {
    construct new() {}
    foreign getName()
    foreign isValid()
    foreign getActorValueByEnum(actorValue)
    foreign getActorValueByString(actorValue)
    foreign modActorValueByEnum(actorValue, value)
    foreign modActorValueByString(actorValue, value)
    foreign restoreActorValueByEnum(actorValue, value)
    foreign restoreActorValueByString(actorValue, value)
    foreign damageActorValueByEnum(actorValue, value)
    foreign damageActorValueByString(actorValue, value)
}
