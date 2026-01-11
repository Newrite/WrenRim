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
- **Solution:** All calls from C++ to Wren must be wrapped in a **safe_wren_call** barrier.
  ```cpp
  template<typename Func>
  void safe_wren_call(Func&& f) {
      try {
          f();
      } catch (const std::exception& e) {
          SKSE::log::error("[Wren Error] {}", e.what());
      } catch (...) {
          SKSE::log::error("[Wren Error] Unknown exception");
      }
  }
  ```

### B. Memory Management: Proxy Objects (Handles)
- **CRITICAL:** Never pass raw `RE::Actor*` or `RE::TESForm*` to Wren.
- **Problem:** `wrenbind17` uses `std::shared_ptr`. Skyrim uses intrusive `RE::BSTSmartPointer`. Holding raw pointers in scripts leads to **Double Frees** or CTD.
- **Solution (Facade Pattern):**
    1.  Wren interacts only with **Wrapper Classes** (e.g., `class wren_actor`).
    2.  Wrappers store a safe **Handle** (`RE::ActorHandle`, `RE::FormID`), not a pointer.
    3.  Wrappers resolve the pointer instantly via `.get()` on every method call.

### C. Threading Model
- **Constraint:** The Wren VM is **NOT** thread-safe.
- **Rule:** All VM execution (`vm.call`, `vm.interpret`) must occur on the **Main Thread**.

---

## 5. Event System (Observer Pattern) & Hook Lifecycle

To avoid hardcoding specific scripts in C++, we use a centralized Event Bus with a strict lifecycle pattern.

### 5.1 Hook Lifecycle (The Sandwich Pattern)
For critical hooks (e.g., `OnWeaponHit`, `OnDrinkPotion`), we implement a **Start/End** pattern to allow scripts to intervene both *before* and *after* the game logic.

**Implementation in C++ Hook:**
```cpp
void hooked_function(Args... args) {
    // 1. START Event (Pre-Hook)
    // Executes FIRST, before Valhalla/MCO or Vanilla logic.
    dispatch("OnEventStart", args...);

    // 2. Original Call (Chain to other mods/Vanilla)
    _original_func(args...);

    // 3. END Event (Post-Hook)
    // Executes LAST, after the game and all other mods have finished.
    dispatch("OnEventEnd", args...);
}
```

### 5.2 Dispatch Logic
1.  **C++ Side:** Calls `dispatch("EventName", args...)`.
2.  **Wren Core (`core/events.wren`):** Receives the call and iterates through registered listeners.
3.  **Wren Scripts:** Register via `Events.on("EventName", callback)`.

---

## 6. Configuration

**Path:** `Data\SKSE\Plugins\zzWrenRim.ini`

Configuration allows tuning VM constraints to prevent script-induced lag. Uses the **mINI** library.

```ini
[General]
bEnabled = true
bEnableLogging = true

[Memory]
; Hard limit for Wren Heap (MB). Prevents OOM crashes.
iMaxHeapSizeMB = 64
iInitialHeapSizeMB = 8

[Performance]
; Max time budget for scripts per frame (microseconds).
; 2000 us = 2 ms. If exceeded, subsequent events in the frame are dropped/deferred.
iMaxFrameTimeBudgetUs = 2000
```

---

## 7. C++ Coding Standards

### Formatting & Tooling
- **Indentation:** 4 Spaces.
- **Config:** Follow `.clang-format`.

### Naming Conventions
- **Modules:** `PascalCase.DotNotation` (e.g., `WrenRim.Core.Hooks`).
- **Universal snake_case:** All C++ identifiers MUST use `snake_case`.
    - **Namespaces:** `core::hooks` (mapped from `WrenRim.Core.Hooks`).
    - **Classes/Structs:** `class config_manager`, `struct actor_data`, `class wren_actor`.
    - **Functions/Methods:** `void do_something()`, `bool is_enabled()`.
    - **Variables/Members:** `int heap_size`, `auto* actor`.
- **No Hungarian Notation:** Never use prefixes like `bEnabled`, `fValue`, `iCount`. Use `enabled`, `value`, `count`.
- **Exception:** External libraries (`RE::`, `SKSE::`) retain their original naming style.

### C++ Modules
- **Policy:** The project uses **C++20 Modules**.
- **Implementation:** New subsystems should be implemented as modules (`export module WrenRim.MyModule;`) where possible.
- **Interop:** Legacy headers (CommonLibSSE, Wren) are handled via PCH or Global Module Fragments (`module; #include ...`).
- **Referencing:** See existing project files for module syntax examples.

### Precompiled Headers (PCH)
- **Global Includes:** `wren.hpp` and `wrenbind17/wrenbind17.hpp` are included globally in `src/pch.h`.
- **Rule:** DO NOT `#include <wrenbind17/wrenbind17.hpp>` in individual `.cpp` files. It is redundant and slows down compilation. Assume `wrenbind17` namespace is always available.

### Safety & Pointers
- **Volatile Data:** All pointers from `RE::` are volatile. Always check for `nullptr`.
- **Casting:** Use `skyrim_cast<Target*>(source)` for RTTI safety across game versions.

### Utilities & Helpers
- **Location:** Global helper functions (string manipulation, math, generic logging) reside in **`src/Core/Utility.cpp`** (and `.h`).

### Error Handling
- **General C++:** No `try-catch` in general game logic.
- **Wren Boundary:** Use `safe_wren_call` explicitly.

---

## 8. Deployment & File Structure

The project uses `xmake` to generate a MO2-ready structure in `dist/`.

```text
/
├── src/
│   ├── library/           # Vendored dependencies
│   │   ├── wren/          # Wren C Source (VM & Optional)
│   │   ├── wrenbind17/    # C++17 Bindings
│   │   └── mINI.h         # mINI header (Single file)
│   ├── Core/              # Config, Utility, Hooks
│   ├── WrenRimStd/        # Standard Library (.wren)
│   │   ├── core/
│   │   │   └── events.wren
│   │   └── ...
├── xmake.lua              # Build script (Compiles Wren VM + C++ Plugin)
└── dist/ (Generated)
    └── SKSE/
        └── Plugins/
            ├── zzWrenRim.dll
            ├── zzWrenRim.ini
            └── WrenRim/
                └── WrenRimStd/  # Scripts deploy here
```

## 9. Wren Scripting & Standard Library

### 9.1 Scripting Guidelines
1.  **Imports:** Always `import "core/events" for Events`.
2.  **Lifecycle:** Use `On...Start` (pre) and `On...End` (post) logic correctly.
3.  **Registration:** Register events at the bottom of the file.

### 9.2 Extending WrenRimStd
- **Mandate:** You are authorized and encouraged to populate `src/WrenRimStd` with standard bindings.
- **New Types:** If the project needs a new Skyrim type (e.g., `Potion`, `Spell`, `Enchantment`) available in Wren, create the corresponding `.wren` wrapper in `WrenRimStd` and the C++ binding logic in `src/API`.

**Template:**
```wren
import "core/events" for Events

class PotionLogic {
    static onDrinkStart(actor, potion) {
        if (potion.isPoison) System.print("Don't drink that!")
    }
}
Events.on("OnDrinkPotionStart", Fn.new { |a, p| PotionLogic.onDrinkStart(a, p) })
```
