// Foreign classes defined in C++ (WrenRim.Wren.Wrappers.ActiveEffect)
// Module: Skyrim/ActiveEffect

foreign class ActiveEffect {
    construct new() {}
    foreign isValid()
    foreign getDuration()
    foreign getMagnitude()
    foreign getElapsed()
    foreign setMagnitude(value)
    foreign getCaster()
    foreign getTarget()
}