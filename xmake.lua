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
    set_pcxxheader("src/pch.h")

    -- Собираем все нужные файлы
    add_headerfiles("src/**.h", "src/**.hpp")
    add_files("src/**.cpp")
    -- Можно добавить Wren файлы в проект, чтобы видеть их в IDE,
    -- но исключить из компиляции C++, так как у нас свое правило copy
    add_files("src/WrenRimStd/**.wren", {rule = "whren_scripts"})

    after_build(function(target)
        import("core.project.task")

        -- Пути
        local project_dir = os.projectdir()
        local dist_dir = path.join(project_dir, "dist")
        local scripts_src = path.join(project_dir, "src", "WrenRimStd")

        -- Целевая структура внутри dist:
        -- dist/SKSE/Plugins/zzWrenRim.dll
        -- dist/SKSE/Plugins/WrenRim/WrenRimStd/
        local plugins_dir = path.join(dist_dir, "SKSE", "Plugins")
        local mod_data_dir = path.join(plugins_dir, "WrenRim")
        local scripts_dist = path.join(mod_data_dir, "WrenRimStd")

        -- 1. Очистка и создание папки dist
        if os.exists(dist_dir) then
            os.rm(path.join(dist_dir, "*"))
        end

        -- Создаем структуру папок
        os.mkdir(scripts_dist) -- Создаст всю цепочку: dist/SKSE/Plugins/WrenRim/WrenRimStd

        -- 2. Копирование бинарных файлов (DLL + PDB)
        -- Они идут прямо в SKSE/Plugins
        os.trycp(target:targetfile(), plugins_dir)
        os.trycp(target:symbolfile(), plugins_dir)
        print("Compiled binaries copied to: " .. plugins_dir)

        -- 3. Копирование стандартной библиотеки Wren (WrenRimStd)
        if os.exists(scripts_src) then
            -- Копируем все содержимое рекурсивно
            os.trycp(path.join(scripts_src, "**"), scripts_dist)
            print("WrenStd scripts copied to: " .. scripts_dist)
        else
            print("WARNING: Source directory 'src/WrenRimStd' not found!")
        end

        -- (Опционально) Копирование в папку игры для быстрой отладки
        -- Если у вас настроена переменная окружения SkyrimPath или аналогичная
        -- local game_path = "C:/Path/To/Skyrim/Data"
        -- os.trycp(dist_dir .. "/*", game_path)
    end)
