-- set minimum xmake version
set_xmakever("3.0.5")

-- includes
-- Убедитесь, что переменная окружения CommonLibSSE-NG установлена
includes(os.getenv("CommonLibSSE-NG"))

-- set project
set_project("WrenRim")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23") -- CommonLibSSE-NG требует свежий стандарт
set_warnings("allextra")
set_defaultmode("releasedbg")

-- Policies
set_policy("package.requires_lock", true)
set_policy("check.auto_ignore_flags", false)
set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", false)
-- Включаем Link Time Optimization (ThinLTO для Clang работает отлично)
set_policy("build.optimization.lto", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- add requirements
add_requires("spdlog", "fmt")

-- set configs
set_config("skyrim_vr", true)
set_config("skyrim_ae", true)
set_config("skyrim_se", true)
set_config("skse_xbyak", true)

-- Custom rules
rule("whren_scripts")
set_extensions(".wren")

rule("papyrus_source")
set_extensions(".psc")

-- TARGET
target("zzWrenRim")

-- add dependencies to target
add_deps("commonlibsse-ng")

-- CommonLibSSE-NG plugin info
add_rules("commonlibsse-ng.plugin", {
    name = "WrenRim",
    author = "newrite",
    description = "Bring Wren script language to Skyrim SE."
})

add_rules("whren_scripts")
add_rules("papyrus_source")

-- include dirs
add_includedirs("src")
add_includedirs("src/library/")

-- Wren includes
add_includedirs("src/library/wren/src/vm")
add_includedirs("src/library/wren/src/include")
add_includedirs("src/library/wren/src/optional")
add_includedirs("src/library/wrenbind17/include/")

-- PCH
-- set_pcxxheader("src/pch.h")

if is_mode("releasedbg") then
        set_optimize("fastest") 
elseif is_mode("debug") then
        set_optimize("none")
end

-- Специфичные настройки для Clang-CL через фильтры
-- Эти флаги применятся ТОЛЬКО если активен тулчейн clang-cl

-- Используем lld-link (быстрее и лучше для LTO)
add_ldflags("-fuse-ld=lld", { tools = "clang-cl" })

-- Принудительно включаем Computed Gotos для Wren
add_defines("WREN_COMPUTED_GOTO", { tools = "clang-cl" })

-- Подавление специфичных для Clang варнингов
-- ДОБАВЛЕНО: -Wno-invalid-noreturn (убирает ворнинг из CommonLib)
-- ДОБАВЛЕНО: -Wno-unknown-pragmas (на всякий случай, если вы решите оставить прагму)
add_cxflags(
    "-Wno-unused-command-line-argument", 
    "-Wno-microsoft-cast", 
    "-Wno-invalid-noreturn",
    { tools = "clang-cl" }
)

-- ДОБАВЛЕНО: Глобально включаем UTF-8 для исходников и исполнения
add_cxflags("/utf-8")

-- Source Files
add_headerfiles("src/**.h", "src/**.hpp", "src/**.wren", "src/**.psc")

-- C++ файлы проекта
add_files("src/**.cpp")

-- Wren C sources
-- Мы используем флаги совместимости с MSVC (/wd...), clang-cl их понимает.
-- Добавляем WREN_NAN_TAGGING для оптимизации значений (если архитектура x64)
local wren_flags = {
    warnings = "none",
    cxflags = {
        "/wd4200",             -- Zero-sized array
        "/wd4100",             -- Unreferenced formal parameter
        "/wd4996",             -- Deprecated functions
        "/wd4244",             -- Conversion loss of data
        "/wd4456",             -- Declaration hides previous local declaration
        "-DWREN_OPT_META",     -- Включаем мету оптимизаций
        "-DWREN_NAN_TAGGING=1" -- Включаем Nan Tagging (быстрее на x64)
    }
}

add_files("src/library/wren/src/vm/*.c", wren_flags)
add_files("src/library/wren/src/optional/*.c", wren_flags)

-- Non-compiled script files
add_files("src/WrenRim/**.wren", { rule = "whren_scripts" })
add_files("src/library/PapyrusSKSE/**.psc", { rule = "papyrus_source" })

-- Post-build copy
after_build(function(target)
    import("core.project.task")
    local project_dir = os.projectdir()
    local dist_dir = path.join(project_dir, "dist")
    local scripts_src = path.join(project_dir, "src", "WrenRim")
    local plugins_dir = path.join(dist_dir, "SKSE", "Plugins")
    local mod_data_dir = path.join(plugins_dir, "WrenRim")

    if os.exists(dist_dir) then os.tryrm(path.join(dist_dir, "*")) end
    os.mkdir(mod_data_dir)

    os.trycp(target:targetfile(), plugins_dir)
    os.trycp(target:symbolfile(), plugins_dir)
    print("Compiled binaries copied to: " .. plugins_dir)

    -- Копируем конфиг если он есть (проверка чтобы не падало)
    local ini_path = path.join(project_dir, "zzWrenRim.ini")
    if os.exists(ini_path) then
        os.trycp(ini_path, plugins_dir)
    end

    if os.exists(scripts_src) then
        os.trycp(path.join(scripts_src, "*"), mod_data_dir)
        print("Wren scripts copied to: " .. mod_data_dir)
    else
        print("WARNING: Source directory 'src/WrenRim' not found!")
    end
end)
