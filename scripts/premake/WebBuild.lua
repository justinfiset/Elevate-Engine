WebBuild = {}

local htmlTemplatePath = _MAIN_SCRIPT_DIR .. "/ElevateEngine/web/index.template.html"

function WebBuild.GenerateHTML(project, outputDir)
    local f = io.open(htmlTemplatePath, "r")
    if f then
        local content = f:read("*all")
        f:close()

        content = content:gsub("${PROJECT_NAME}", project.name)

        local outPath = outputDir .. "/index.html"
        local outFile = io.open(outPath, "w")
        if outFile then
            outFile:write(content)
            outFile:close()
            Logger.Info("Elevate Engine: Generated index.html for " .. project.name .. " at " .. outPath)
        end
    end
end