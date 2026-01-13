# Project Context: WrenRim

## 1. Project Overview
**WrenRim** is an SKSE64 plugin that embeds the **Wren** scripting language into the Skyrim SE/AE/VR engine.
The goal is to provide a high-performance alternative to Papyrus (comparable to LuaJIT speeds) featuring modern syntax (classes, fibers) and type safety, while maintaining strict game stability.

The plugin implements a "Bridge" architecture, allowing game logic to be written in Wren, interacting with native C++ classes via safe Proxy Objects.

## 2. Technology Stack
- **Language:** C++ (C++20/23 Standard) mixed with C (Wren VM).
- **Module System:** **C++20 Modules** are enabled and preferred for new internal subsystems.
- **Build System:** xmake (v3.0.5+).
- **Scripting Engine:** [Wren](https://wren.io) (Vendored in `src/library/wren`).
- **Binding Library:** [wrenbind17](https://github.com/matusnovak/wrenbind17) (Vendored in `src/library/wrenbind17`).
- **Game Engine:** Skyrim Special Edition (SE), Anniversary Edition (AE), and Skyrim VR.
- **Core Dependencies:**
    - **CommonLibSSE-NG:** Next-generation reverse engineered library supporting multi-targeting.
    - **SKSE64:** Script Extender.
    - **mINI:** Tiny, header-only C++ library for parsing `.ini` configuration (`src/library/mINI.h`).
    - **SKSEMenuFramework:** For in-game UI configuration (`src/library/SKSEMenuFramework.h`).

---

## 3. Architecture: Unified Binary & Multi-Targeting
**CRITICAL:** The code must support SE (1.5.x), AE (1.6.x), and VR simultaneously from a single binary.

### A. Address Library & REL Namespace
- **NO Magic Numbers:** Never hardcode memory addresses (e.g., `0x140001000`).
- **Use REL::ID:** Always use `REL::ID` with IDs from the Address Library to resolve functions.
- **Pattern:**
  ```cpp
  // CORRECT: Using Address Library ID
  REL::Relocation<void(RE::Actor*)> func{ REL::ID(12345) };
  func(actor);
  ```

### B. VR Compatibility (VTable Shifts)
- **Problem:** Skyrim VR often has different VTable indices (shifted by +1 due to `Unk_03` in some classes).
- **Solution:** Use `REL::RelocateVirtual` for calling virtual functions to handle the index shift automatically.
  ```cpp
  // CORRECT: Handles SE/AE vs VR index differences
  // 0x03 = SE index, 0x04 = VR index
  REL::RelocateVirtual<decltype(&Derived::func)>(0x03, 0x04, this, args);
  ```

### C. Hook Priority & DLL Naming
- **Strategy:** We use a **"Last Loaded"** strategy to ensure our hooks wrap the entire execution chain.
- **Naming:** The DLL is named `zzWrenRim.dll` to ensure it loads last alphabetically.
- **Execution Flow:** Because we load last, our hook executes **first**. When we call `_original_func`, we pass control to other mods (Valhalla, MCO) and finally the Vanilla game. When they return, control comes back to us.

---

## 4. Wren Integration Strategy (The Bridge)

This layer resolves the conflict between C++17 (WrenBind17) and the raw Skyrim engine.

### A. Exception Firewall
- **Problem:** `wrenbind17` throws C++ exceptions on errors. Skyrim disables exception handling; escaping exceptions cause CTD.
- **Solution:** All calls from C++ to Wren must be wrapped in a **safe_wren_call** barrier (or try-catch blocks in hook handlers).

### B. Memory Management: Proxy Objects (Handles)
- **CRITICAL:** Never pass raw `RE::Actor*` or `RE::TESForm*` to Wren.
- **Problem:** `wrenbind17` uses `std::shared_ptr`. Skyrim uses intrusive `RE::BSTSmartPointer`. Holding raw pointers in scripts leads to **Double Frees** or CTD.
- **Solution (Facade Pattern):**
    1.  Wren interacts only with **Wrapper Classes** (defined in `src/Wren/Wrappers/`).
    2.  Wrappers store a safe **Handle** (`RE::ActorHandle`, `RE::FormID`), not a pointer.
    3.  Wrappers resolve the pointer instantly via `.get()` on every method call.
    4.  Wrappers are exposed to Wren via `BindingManager.cpp`.

### C. Threading Model
- **Constraint:** The Wren VM is **NOT** thread-safe.
- **Rule:** All VM execution (`vm.call`, `vm.interpret`) must occur on the **Main Thread**.

---

## 5. Event System (Observer Pattern) & Hook Lifecycle

To avoid hardcoding specific scripts in C++, we use a centralized Event Bus with a strict lifecycle pattern.

### 5.1 Hook Lifecycle (The Sandwich Pattern)
For critical hooks (e.g., `OnWeaponHit`, `OnDrinkPotion`), we implement a **Start/End** pattern.

**Implementation in C++ Hook (`src/Core/Hooks.cpp`):**
```cpp
void hooked_function(Args... args) {
    // 1. Prepare Wrappers
    wren::wrappers::actor wren_actor(character);

    // 2. START Event (Pre-Hook)
    wren::script_engine::engine::get_singleton()->dispatch("OnEventStart", wren_actor);

    // 3. Original Call
    _original_func(args...);

    // 4. END Event (Post-Hook)
    wren::script_engine::engine::get_singleton()->dispatch("OnEventEnd", wren_actor);
}
```

### 5.2 Dispatch Logic
1.  **C++ Side:** Calls `wren::script_engine::engine::get_singleton()->dispatch("EventName", args...)`.
2.  **Wren Core (`Std/Events.wren`):** Receives the call and iterates through registered listeners.
3.  **Wren Scripts:** Register via `Events.on("EventName", callback)`.

---

## 6. Configuration

**Path:** `Data\SKSE\Plugins\zzWrenRim.ini`
Managed by `src/Config.cpp` using `mINI`.

```ini
[General]
bEnabled = true
bEnableLogging = true

[Memory]
iMaxHeapSizeMB = 64
iInitialHeapSizeMB = 8

[Performance]
iMaxFrameTimeBudgetUs = 2000
```

---

## 7. C++ Coding Standards

### Formatting & Tooling
- **Indentation:** 4 Spaces.
- **Config:** Follow `.clang-format`.

### Naming Conventions
- **Modules:** `PascalCase.DotNotation` (e.g., `WrenRim.Wren.Wrappers.Actor`).
- **Universal snake_case:** All C++ identifiers MUST use `snake_case`.
    - **Namespaces:** `wren::wrappers` (mapped from `WrenRim.Wren.Wrappers`).
    - **Classes/Structs:** `class config_manager`, `struct actor_data`, `class actor` (wrapper).
    - **Functions/Methods:** `void do_something()`, `bool is_enabled()`.
- **Exception:** External libraries (`RE::`, `SKSE::`, `ImGui::`) retain their original naming style.

### C++ Modules
- **Policy:** The project uses **C++20 Modules**.
- **Location:** Source files in `src/` serve as module interfaces/implementations.
- **Structure:**
    - `WrenRim.Core.*`: Core logic (Hooks, Utils, Log).
    - `WrenRim.Wren.*`: Wren VM integration and Wrappers.
    - `WrenRim.UI.*`: UI/Menu logic.
    - `WrenRim.Events.*`: Native event handling.

### Precompiled Headers (PCH)
- **Global Includes:** `wren.hpp` and `wrenbind17/wrenbind17.hpp` are included globally in `src/pch.h`.
- **Rule:** DO NOT `#include "pch.h"` manually in `.cpp` files. xmake handles this.

### 7.4 Binding with wrenbind17 (Gotchas)
- **Static vs Instance Methods:** When binding C++ methods to Wren using `wrenbind17`, you MUST use the correct binding function based on the method type:
    - Use **`cls.func<&Class::method>("name")`** ONLY for non-static instance methods.
    - Use **`cls.funcStatic<&Class::static_method>("name")`** for static methods or standalone functions.
- **Symptom:** Using `func` for a static method causes `Error C2027: use of undefined type 'wrenbind17::detail::ForeignMethodDetails...'`. This happens because the template specialization for instance methods fails to match the signature of a static function pointer.
- **Example Fix:** In static wrapper classes like `UI` or static factory methods in `GFxValue`, ensure `funcStatic` is used for all exported members.

---

## 8. Deployment & File Structure

The project uses `xmake` to generate a MO2-ready structure in `dist/`.

```text
/
├── src/
│   ├── library/           # Vendored dependencies (wren, wrenbind17, mINI, SKSEMenuFramework)
│   ├── Core/              # Config, Utility, Hooks, Logger
│   ├── Events/            # Native Event Sinks (e.g., MenuEvent)
│   ├── UI/                # ImGui/SKSEMenu logic
│   ├── Wren/              # Wren VM Integration
│   │   ├── Wrappers/      # C++ Wrappers for Skyrim types (Actor, Weapon, etc.)
│   │   ├── BindingManager.cpp # Binds wrappers to Wren VM
│   │   └── ScriptEngine.cpp   # VM management and event dispatch
│   ├── WrenRim/           # Wren Source Files (deployed to Data/...)
│   │   ├── Std/           # Standard Library (Events.wren, Skyrim/*.wren)
│   │   └── WrenMods/      # User Scripts / Example Mods
├── xmake.lua              # Build script
└── dist/ (Generated)
    └── SKSE/
        └── Plugins/
            ├── zzWrenRim.dll
            ├── zzWrenRim.ini
            └── WrenRim/
                ├── Std/       # Standard Library scripts
                └── WrenMods/  # User scripts
```

## 9. Wren Scripting & Standard Library

### 9.1 Scripting Guidelines
1.  **Imports:** `import "Events" for Events` (Standard Library modules are in `Std/`).
2.  **Skyrim Types:** `import "Skyrim/Actor" for Actor`.
3.  **Lifecycle:** Use `On...Start` (pre) and `On...End` (post) logic.

### 9.2 Extending WrenRimStd
- **New Types:**
    1.  Create C++ Wrapper in `src/Wren/Wrappers/`.
    2.  Bind it in `src/Wren/BindingManager.cpp`.
    3.  Create Wren definition in `src/WrenRim/Std/Skyrim/`.

**Example Script (`src/WrenRim/WrenMods/MyMod.wren`):**
```wren
import "Events" for Events
import "Skyrim/Actor" for Actor

Events.on("OnDrinkPotionStart", Fn.new { |actor, potion|
    if (actor.getName() == "Prisoner") {
        System.print("Prisoner is drinking!")
    }
})
```

### 9.3 Wren Language Constraints & Gotchas
- **Static Fields:** Must start with `__` (e.g., `static __listeners`).
- **Module Resolution:** Imports resolve to logical names (e.g., "Skyrim/Actor"), managed by `ScriptEngine.cpp`.
- **Variable Names:** Module-level variables cannot start with underscore.