# Project Context: WrenRim

## 1. Project Overview
**WrenRim** is an SKSE64 plugin that embeds the **Wren** scripting language into the Skyrim SE/AE/VR engine.
The goal is to provide a high-performance alternative to Papyrus (comparable to LuaJIT speeds) featuring modern syntax (classes, fibers) and type safety, while maintaining strict game stability.

The plugin implements a "Bridge" architecture, allowing game logic to be written in Wren, interacting with native C++ classes via safe Proxy Objects.

## 2. Technology Stack
- **Language:** C++ (C++20/23 Standard).
- **Build System:** xmake (v3.0.5+).
- **Scripting Engine:** [Wren](https://wren.io) (C source, embedded).
- **Binding Library:** [wrenbind17](https://github.com/matusnovak/wrenbind17) (Header-only C++17 library).
- **Game Engine:** Skyrim Special Edition (SE), Anniversary Edition (AE), and Skyrim VR.
- **Core Dependencies:**
    - **CommonLibSSE-NG:** Next-generation reverse engineered library supporting multi-targeting.
    - **SKSE64:** Script Extender.
    - **SimpleIni:** For parsing `zzWrenRim.ini`.

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
  REL::RelocateVirtual<decltype(&Derived::Func)>(0x03, 0x04, this, args);
  ```

### C. Hook Priority & DLL Naming
- **Strategy:** We use a **"Last Loaded"** strategy to ensure our hooks execute *after* other combat mods (Valhalla, MCO), giving us control over the final data processing (e.g., modifying `OnHit` damage).
- **Naming:** The DLL is named `zzWrenRim.dll` to ensure it loads last alphabetically.

---

## 4. Wren Integration Strategy (The Bridge)

This layer resolves the conflict between C++17 (WrenBind17) and the raw Skyrim engine.

### A. Exception Firewall
- **Problem:** `wrenbind17` throws C++ exceptions on errors (e.g., type mismatch). Skyrim disables exception handling; an escaping exception causes an immediate Crash To Desktop (CTD).
- **Solution:** All calls from C++ to Wren must be wrapped in a **SafeWrenCall** barrier.
  ```cpp
  template<typename Func>
  void SafeWrenCall(Func&& f) {
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
- **Problem:** `wrenbind17` uses `std::shared_ptr`. Skyrim uses intrusive `RE::BSTSmartPointer`. Mixing them or holding raw pointers in scripts leads to **Double Frees** or access violations (CTD) if the object is deleted by the game.
- **Solution (Facade Pattern):**
    1.  Wren interacts only with **Wrapper Classes**.
    2.  Wrappers store a safe **Handle** (`RE::ActorHandle`, `RE::FormID`), not a pointer.
    3.  Wrappers resolve the pointer instantly via `.get()` on every method call.

  ```cpp
  // C++ Wrapper Example
  class WrenActor {
      RE::ActorHandle _handle;
  public:
      WrenActor(RE::Actor* a) : _handle(a ? a->GetHandle() : RE::ActorHandle()) {}
      
      float getHealth() {
          auto ptr = _handle.get(); // Resolve safely
          return ptr ? ptr->GetActorValue(RE::ActorValue::kHealth) : 0.0f;
      }
  };
  ```

### C. Threading Model
- **Constraint:** The Wren VM is **NOT** thread-safe.
- **Rule:** All VM execution (`vm.call`, `vm.interpret`) must occur on the **Main Thread**.
- **Implementation:** If a game event occurs on a background thread, queue it to a `TaskQueue` to be processed in the next Main Frame update.

---

## 5. Event System (Observer Pattern)

To avoid hardcoding specific scripts in C++, we use a centralized Event Bus.

1.  **C++ Side:** Calls `Dispatch("EventName", args...)`.
2.  **Wren Core (`core/events.wren`):** Receives the call and iterates through registered listeners.
3.  **Wren Scripts:** Register via `Events.on("EventName", callback)`.

**Arity Overloading:**
The C++ engine automatically selects the correct cached handle based on argument count:
* `Dispatch("OnStart")` -> calls Wren `dispatch(_)`
* `Dispatch("OnHit", target)` -> calls Wren `dispatch(_,_)`

---

## 6. Configuration (`zzWrenRim.ini`)

Configuration allows tuning VM constraints to prevent script-induced lag. Using `SimpleIni` library.

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
- **Indentation:** 4 Spaces (standard for C++ projects, distinct from the 2 spaces in TrueFlasks).
- **Config:** Follow `.clang-format`.

### Safety & Pointers
- **Volatile Data:** All pointers from `RE::` are volatile. Always check for `nullptr`.
- **Casting:** Use `skyrim_cast<Target*>(source)` for RTTI safety across game versions.

### Error Handling
- **General C++:** No `try-catch` in general game logic (performance overhead), except for the specific **SafeWrenCall** boundary.
- **Return Values:** Check `[[nodiscard]]` return values.

---

## 8. Deployment & File Structure

The project uses `xmake` to generate a MO2-ready structure in `dist/`.

```text
/
├── src/
│   ├── WrenRimStd/        # Standard Library (.wren)
│   │   ├── core/
│   │   │   └── events.wren
│   │   └── ...
├── xmake.lua              # Build script (Copies scripts to dist)
└── dist/ (Generated)
    └── SKSE/
        └── Plugins/
            ├── zzWrenRim.dll
            ├── zzWrenRim.ini
            └── WrenRim/
                └── WrenRimStd/  # Scripts deploy here
```

## 9. Wren Scripting Guidelines (For AI Generation)

When generating `.wren` scripts for this project:

1.  **Imports:** Always `import "core/events" for Events`.
2.  **Static Logic:** Use static class methods for logic.
3.  **Registration:** Register events at the bottom of the file.

**Template:**
```wren
import "core/events" for Events
import "Skyrim" for Actor, Game

class MyModLogic {
    static onHit(target, source) {
        if (target.isDead) return
        System.print(target.getName() + " was hit!")
    }
}

// Register
Events.on("OnHit", Fn.new { |t, s| MyModLogic.onHit(t, s) })
```# Project Context: WrenRim

## 1. Project Overview
**WrenRim** is an SKSE64 plugin that embeds the **Wren** scripting language into the Skyrim SE/AE/VR engine.
The goal is to provide a high-performance alternative to Papyrus (comparable to LuaJIT speeds) featuring modern syntax (classes, fibers) and type safety, while maintaining strict game stability.

The plugin implements a "Bridge" architecture, allowing game logic to be written in Wren, interacting with native C++ classes via safe Proxy Objects.

## 2. Technology Stack
- **Language:** C++ (C++20/23 Standard).
- **Build System:** xmake (v3.0.5+).
- **Scripting Engine:** [Wren](https://wren.io) (C source, embedded).
- **Binding Library:** [wrenbind17](https://github.com/matusnovak/wrenbind17) (Header-only C++17 library).
- **Game Engine:** Skyrim Special Edition (SE), Anniversary Edition (AE), and Skyrim VR.
- **Core Dependencies:**
    - **CommonLibSSE-NG:** Next-generation reverse engineered library supporting multi-targeting.
    - **SKSE64:** Script Extender.
    - **SimpleIni:** For parsing `zzWrenRim.ini`.

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
  REL::RelocateVirtual<decltype(&Derived::Func)>(0x03, 0x04, this, args);
  ```

### C. Hook Priority & DLL Naming
- **Strategy:** We use a **"Last Loaded"** strategy to ensure our hooks execute *after* other combat mods (Valhalla, MCO), giving us control over the final data processing (e.g., modifying `OnHit` damage).
- **Naming:** The DLL is named `zzWrenRim.dll` to ensure it loads last alphabetically.

---

## 4. Wren Integration Strategy (The Bridge)

This layer resolves the conflict between C++17 (WrenBind17) and the raw Skyrim engine.

### A. Exception Firewall
- **Problem:** `wrenbind17` throws C++ exceptions on errors (e.g., type mismatch). Skyrim disables exception handling; an escaping exception causes an immediate Crash To Desktop (CTD).
- **Solution:** All calls from C++ to Wren must be wrapped in a **SafeWrenCall** barrier.
  ```cpp
  template<typename Func>
  void SafeWrenCall(Func&& f) {
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
- **Problem:** `wrenbind17` uses `std::shared_ptr`. Skyrim uses intrusive `RE::BSTSmartPointer`. Mixing them or holding raw pointers in scripts leads to **Double Frees** or access violations (CTD) if the object is deleted by the game.
- **Solution (Facade Pattern):**
    1.  Wren interacts only with **Wrapper Classes**.
    2.  Wrappers store a safe **Handle** (`RE::ActorHandle`, `RE::FormID`), not a pointer.
    3.  Wrappers resolve the pointer instantly via `.get()` on every method call.

  ```cpp
  // C++ Wrapper Example
  class WrenActor {
      RE::ActorHandle _handle;
  public:
      WrenActor(RE::Actor* a) : _handle(a ? a->GetHandle() : RE::ActorHandle()) {}
      
      float getHealth() {
          auto ptr = _handle.get(); // Resolve safely
          return ptr ? ptr->GetActorValue(RE::ActorValue::kHealth) : 0.0f;
      }
  };
  ```

### C. Threading Model
- **Constraint:** The Wren VM is **NOT** thread-safe.
- **Rule:** All VM execution (`vm.call`, `vm.interpret`) must occur on the **Main Thread**.
- **Implementation:** If a game event occurs on a background thread, queue it to a `TaskQueue` to be processed in the next Main Frame update.

---

## 5. Event System (Observer Pattern)

To avoid hardcoding specific scripts in C++, we use a centralized Event Bus.

1.  **C++ Side:** Calls `Dispatch("EventName", args...)`.
2.  **Wren Core (`core/events.wren`):** Receives the call and iterates through registered listeners.
3.  **Wren Scripts:** Register via `Events.on("EventName", callback)`.

**Arity Overloading:**
The C++ engine automatically selects the correct cached handle based on argument count:
* `Dispatch("OnStart")` -> calls Wren `dispatch(_)`
* `Dispatch("OnHit", target)` -> calls Wren `dispatch(_,_)`

---

## 6. Configuration (`zzWrenRim.ini`)

Configuration allows tuning VM constraints to prevent script-induced lag. Using `SimpleIni` library.

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
- **Indentation:** 4 Spaces (standard for C++ projects, distinct from the 2 spaces in TrueFlasks).
- **Config:** Follow `.clang-format`.

### Safety & Pointers
- **Volatile Data:** All pointers from `RE::` are volatile. Always check for `nullptr`.
- **Casting:** Use `skyrim_cast<Target*>(source)` for RTTI safety across game versions.

### Error Handling
- **General C++:** No `try-catch` in general game logic (performance overhead), except for the specific **SafeWrenCall** boundary.
- **Return Values:** Check `[[nodiscard]]` return values.

---

## 8. Deployment & File Structure

The project uses `xmake` to generate a MO2-ready structure in `dist/`.

```text
/
├── src/
│   ├── WrenRimStd/        # Standard Library (.wren)
│   │   ├── core/
│   │   │   └── events.wren
│   │   └── ...
├── xmake.lua              # Build script (Copies scripts to dist)
└── dist/ (Generated)
    └── SKSE/
        └── Plugins/
            ├── zzWrenRim.dll
            ├── zzWrenRim.ini
            └── WrenRim/
                └── WrenRimStd/  # Scripts deploy here
```

## 9. Wren Scripting Guidelines (For AI Generation)

When generating `.wren` scripts for this project:

1.  **Imports:** Always `import "core/events" for Events`.
2.  **Static Logic:** Use static class methods for logic.
3.  **Registration:** Register events at the bottom of the file.

**Template:**
```wren
import "core/events" for Events
import "Skyrim" for Actor, Game

class MyModLogic {
    static onHit(target, source) {
        if (target.isDead) return
        System.print(target.getName() + " was hit!")
    }
}

// Register
Events.on("OnHit", Fn.new { |t, s| MyModLogic.onHit(t, s) })
```# Project Context: WrenRim

## 1. Project Overview
**WrenRim** is an SKSE64 plugin that embeds the **Wren** scripting language into the Skyrim SE/AE/VR engine.
The goal is to provide a high-performance alternative to Papyrus (comparable to LuaJIT speeds) featuring modern syntax (classes, fibers) and type safety, while maintaining strict game stability.

The plugin implements a "Bridge" architecture, allowing game logic to be written in Wren, interacting with native C++ classes via safe Proxy Objects.

## 2. Technology Stack
- **Language:** C++ (C++20/23 Standard).
- **Build System:** xmake (v3.0.5+).
- **Scripting Engine:** [Wren](https://wren.io) (C source, embedded).
- **Binding Library:** [wrenbind17](https://github.com/matusnovak/wrenbind17) (Header-only C++17 library).
- **Game Engine:** Skyrim Special Edition (SE), Anniversary Edition (AE), and Skyrim VR.
- **Core Dependencies:**
    - **CommonLibSSE-NG:** Next-generation reverse engineered library supporting multi-targeting.
    - **SKSE64:** Script Extender.
    - **SimpleIni:** For parsing `zzWrenRim.ini`.

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
  REL::RelocateVirtual<decltype(&Derived::Func)>(0x03, 0x04, this, args);
  ```

### C. Hook Priority & DLL Naming
- **Strategy:** We use a **"Last Loaded"** strategy to ensure our hooks execute *after* other combat mods (Valhalla, MCO), giving us control over the final data processing (e.g., modifying `OnHit` damage).
- **Naming:** The DLL is named `zzWrenRim.dll` to ensure it loads last alphabetically.

---

## 4. Wren Integration Strategy (The Bridge)

This layer resolves the conflict between C++17 (WrenBind17) and the raw Skyrim engine.

### A. Exception Firewall
- **Problem:** `wrenbind17` throws C++ exceptions on errors (e.g., type mismatch). Skyrim disables exception handling; an escaping exception causes an immediate Crash To Desktop (CTD).
- **Solution:** All calls from C++ to Wren must be wrapped in a **SafeWrenCall** barrier.
  ```cpp
  template<typename Func>
  void SafeWrenCall(Func&& f) {
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
- **Problem:** `wrenbind17` uses `std::shared_ptr`. Skyrim uses intrusive `RE::BSTSmartPointer`. Mixing them or holding raw pointers in scripts leads to **Double Frees** or access violations (CTD) if the object is deleted by the game.
- **Solution (Facade Pattern):**
    1.  Wren interacts only with **Wrapper Classes**.
    2.  Wrappers store a safe **Handle** (`RE::ActorHandle`, `RE::FormID`), not a pointer.
    3.  Wrappers resolve the pointer instantly via `.get()` on every method call.

  ```cpp
  // C++ Wrapper Example
  class WrenActor {
      RE::ActorHandle _handle;
  public:
      WrenActor(RE::Actor* a) : _handle(a ? a->GetHandle() : RE::ActorHandle()) {}
      
      float getHealth() {
          auto ptr = _handle.get(); // Resolve safely
          return ptr ? ptr->GetActorValue(RE::ActorValue::kHealth) : 0.0f;
      }
  };
  ```

### C. Threading Model
- **Constraint:** The Wren VM is **NOT** thread-safe.
- **Rule:** All VM execution (`vm.call`, `vm.interpret`) must occur on the **Main Thread**.
- **Implementation:** If a game event occurs on a background thread, queue it to a `TaskQueue` to be processed in the next Main Frame update.

---

## 5. Event System (Observer Pattern)

To avoid hardcoding specific scripts in C++, we use a centralized Event Bus.

1.  **C++ Side:** Calls `Dispatch("EventName", args...)`.
2.  **Wren Core (`core/events.wren`):** Receives the call and iterates through registered listeners.
3.  **Wren Scripts:** Register via `Events.on("EventName", callback)`.

**Arity Overloading:**
The C++ engine automatically selects the correct cached handle based on argument count:
* `Dispatch("OnStart")` -> calls Wren `dispatch(_)`
* `Dispatch("OnHit", target)` -> calls Wren `dispatch(_,_)`

---

## 6. Configuration (`zzWrenRim.ini`)

Configuration allows tuning VM constraints to prevent script-induced lag. Using `SimpleIni` library.

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
- **Indentation:** 4 Spaces (standard for C++ projects, distinct from the 2 spaces in TrueFlasks).
- **Config:** Follow `.clang-format`.

### Safety & Pointers
- **Volatile Data:** All pointers from `RE::` are volatile. Always check for `nullptr`.
- **Casting:** Use `skyrim_cast<Target*>(source)` for RTTI safety across game versions.

### Error Handling
- **General C++:** No `try-catch` in general game logic (performance overhead), except for the specific **SafeWrenCall** boundary.
- **Return Values:** Check `[[nodiscard]]` return values.

---

## 8. Deployment & File Structure

The project uses `xmake` to generate a MO2-ready structure in `dist/`.

```text
/
├── src/
│   ├── WrenRimStd/        # Standard Library (.wren)
│   │   ├── core/
│   │   │   └── events.wren
│   │   └── ...
├── xmake.lua              # Build script (Copies scripts to dist)
└── dist/ (Generated)
    └── SKSE/
        └── Plugins/
            ├── zzWrenRim.dll
            ├── zzWrenRim.ini
            └── WrenRim/
                └── WrenRimStd/  # Scripts deploy here
```

## 9. Wren Scripting Guidelines (For AI Generation)

When generating `.wren` scripts for this project:

1.  **Imports:** Always `import "core/events" for Events`.
2.  **Static Logic:** Use static class methods for logic.
3.  **Registration:** Register events at the bottom of the file.

**Template:**
```wren
import "core/events" for Events
import "Skyrim" for Actor, Game

class MyModLogic {
    static onHit(target, source) {
        if (target.isDead) return
        System.print(target.getName() + " was hit!")
    }
}

// Register
Events.on("OnHit", Fn.new { |t, s| MyModLogic.onHit(t, s) })
```
