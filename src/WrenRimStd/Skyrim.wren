import "core/events" for Events

// Foreign classes defined in C++ (WrenRim.API.Wrappers)
foreign class Actor {
    foreign getName()
}

foreign class Potion {
    foreign isPoison()
    foreign isFood()
    foreign getName()
}

// Example Logic: Prevent drinking poisons
class PotionLogic {
    static onDrinkStart(actor, potion) {
        System.print("[Wren] Drinking: " + potion.getName())
    }

    static onDrinkEnd(actor, potion) {
        System.print("[Wren] Finished drinking: " + potion.getName())
    }
}

// Register Listeners
Events.on("OnDrinkPotionStart", Fn.new { |a, p| PotionLogic.onDrinkStart(a, p) })
Events.on("OnDrinkPotionEnd", Fn.new { |a, p| PotionLogic.onDrinkEnd(a, p) })
