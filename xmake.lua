-- set minimum xmake version
set_xmakever("3.0.5")

-- includes
includes(os.getenv("CommonLibSSE-NG"))

-- set project
set_project("WrenRim")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++latest")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- add requirements
add_requires("spdlog", "fmt")

-- set policies
set_policy("package.requires_lock", true)
set_policy("check.auto_ignore_flags", false)
-- Включаем поддержку C++20 modules, если планируете использовать (опционально)
set_policy("build.c++.modules", true)

-- set configs
set_config("skyrim_vr", true)
set_config("skyrim_ae", true)
set_config("skyrim_se", true)
set_config("skse_xbyak", true)

-- Определение правила для .wren файлов (если понадобится компиляция в будущем)
rule("whren_scripts")
    set_extensions(".wren")

-- targets
-- ВАЖНО: Имя таргета определяет имя DLL. Здесь оно "zzWrenRim"
target("zzWrenRim")

    -- add dependencies to target
    add_deps("commonlibsse-ng")

    -- add commonlibsse-ng plugin info
    add_rules("commonlibsse-ng.plugin", {
        name = "WrenRim",
        author = "newrite",
        description = "Bring Wren script language to Skyrim SE."
    })

    add_rules("whren_scripts")

    -- add src files
    add_includedirs("src")
    add_includedirs("src/library/")
    
    add_includedirs("src/library/wren/src/vm")
    add_includedirs("src/library/wren/src/include")
    add_includedirs("src/library/wren/src/optional")
    add_includedirs("src/library/wrenbind17/include/")
    
    set_pcxxheader("src/pch.h")

    -- Собираем все нужные файлы
    add_headerfiles("src/**.h", "src/**.hpp", "src/**.wren")
    add_files("src/library/wren/src/vm/*.c", {warnings = "none", cxflags = "/wd4200 /wd4100 /wd4996 /wd4244 /wd4456"})
    add_files("src/library/wren/src/optional/*.c", {warnings = "none", cxflags = "/wd4200 /wd4100 /wd4996 /wd4244 /wd4456"})
    add_files("src/**.cpp")
    -- Можно добавить Wren файлы в проект, чтобы видеть их в IDE,
    -- но исключить из компиляции C++, так как у нас свое правило copy
    add_files("src/WrenRim/**.wren", {rule = "whren_scripts"})

    after_build(function(target)
        import("core.project.task")

        -- Пути
        local project_dir = os.projectdir()
        local dist_dir = path.join(project_dir, "dist")
        
        -- Источник скриптов: src/WrenRim (там лежат Std, WrenMods)
        local scripts_src = path.join(project_dir, "src", "WrenRim")

        -- Целевая структура внутри dist:
        -- dist/SKSE/Plugins/zzWrenRim.dll
        -- dist/SKSE/Plugins/WrenRim/ (Std, WrenMods)
        local plugins_dir = path.join(dist_dir, "SKSE", "Plugins")
        local mod_data_dir = path.join(plugins_dir, "WrenRim")

        -- 1. Очистка папки dist
        if os.exists(dist_dir) then
            os.tryrm(path.join(dist_dir, "*"))
        end

        -- Создаем структуру папок
        os.mkdir(mod_data_dir) -- Создаст всю цепочку: dist/SKSE/Plugins/WrenRim

        -- 2. Копирование бинарных файлов (DLL + PDB)
        -- Они идут прямо в SKSE/Plugins
        os.trycp(target:targetfile(), plugins_dir)
        os.trycp(target:symbolfile(), plugins_dir)
        print("Compiled binaries copied to: " .. plugins_dir)

        -- 2.1 Копирование конфига
        os.trycp(path.join(project_dir, "zzWrenRim.ini"), plugins_dir)

        -- 3. Копирование скриптов (WrenRim folder)
        if os.exists(scripts_src) then
            -- Копируем все содержимое рекурсивно (Std, WrenMods, etc.)
            -- Используем "*", чтобы скопировать структуру папок, а не "**", который разворачивает все файлы в плоский список
            os.trycp(path.join(scripts_src, "*"), mod_data_dir)
            print("Wren scripts copied to: " .. mod_data_dir)
        else
            print("WARNING: Source directory 'src/WrenRim' not found!")
        end
    end)
