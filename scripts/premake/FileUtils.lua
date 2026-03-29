FileUtils = {}

function FileUtils.SafeDelete(path)
    if not path or path == "" then return end
    local winPath = path:gsub("/", "\\")
    if os.host() == "windows" then
        os.execute("if exist \"" .. winPath .. "\" rd /s /q \"" .. winPath .. "\"")
    else
        os.execute("rm -rf \"" .. path .. "\"")
    end
end

function FileUtils.SafeCopy(src, dest)
    if not os.isdir(src) then 
        print("Warning: Source directory does not exist: " .. src)
        return 
    end

    local winSrc = src:gsub("/", "\\")
    local winDest = dest:gsub("/", "\\")

    if os.host() == "windows" then
        os.execute("xcopy \"" .. winSrc .. "\" \"" .. winDest .. "\" /E /I /Y > nul")
    else
        os.execute("mkdir -p \"" .. dest .. "\"")
        os.execute("cp -r \"" .. src .. "/.\" \"" .. dest .. "\"")
    end
end

return FileUtils