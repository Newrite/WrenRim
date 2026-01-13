import("core.base.json")

function main()
    local path = "compile_commands.json"
    local data = json.loadfile(path)
    if not data then
        print("Failed to load " .. path)
        return
    end

    local new_data = {}
    local removed_count = 0

    for _, entry in ipairs(data) do
        local new_args = {}
        local skip_next = 0
        for _, arg in ipairs(entry.arguments) do
            if skip_next > 0 then
                skip_next = skip_next - 1
                removed_count = removed_count + 1
            elseif arg == "/interface" then
                removed_count = removed_count + 1
            elseif arg == "/ifcOutput" then
                skip_next = 1
                removed_count = removed_count + 1
            elseif arg == "/reference" then
                skip_next = 1
                removed_count = removed_count + 1
            elseif arg == "/headerUnit" then
                skip_next = 1
                removed_count = removed_count + 1
            elseif arg == "/module:interface" then
                removed_count = removed_count + 1
            elseif arg == "/module:output" then
                skip_next = 1
                removed_count = removed_count + 1
            elseif arg:startswith("/reference:") then -- Handle joined case just in case
                removed_count = removed_count + 1
            elseif arg:startswith("/Yu") then -- Remove "Use Precompiled Header"
                removed_count = removed_count + 1
            elseif arg:startswith("/Fp") then -- Remove PCH output path
                removed_count = removed_count + 1
            elseif arg:startswith("/Fd") then -- Remove PDB output path
                removed_count = removed_count + 1
            elseif arg:startswith("/Zi") or arg:startswith("/ZI") then -- Remove Debug Info format
                removed_count = removed_count + 1
            elseif arg == "/FS" then -- Remove Force Synchronous PDB writes
                removed_count = removed_count + 1
            else
                table.insert(new_args, arg)
            end
        end
        entry.arguments = new_args
        table.insert(new_data, entry)
    end

    json.savefile("compile_commands.json", new_data)
    print("Cleaned compile_commands.json for clangd. Removed " .. removed_count .. " problematic flags/args.")
end
