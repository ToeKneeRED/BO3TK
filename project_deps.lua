import("core.project.project")

local sln_path = os.curdir().."\\vsxmake2022\\"..project.name()..".sln"
local f = assert(io.open(sln_path, "r"))
local content = f:read("*a")
f:close()

local projects = {}
for name, guid in string.gmatch(content, 'Project%b() = "([^"]+)",.-{([0-9A-Fa-f%-]+)}') do
    projects[name] = guid
end

local dependencies = {
    ["Dashboard"] = {"BO3TK", "Tools"},
    ["BO3TK"]    = {"Tools"},
}

for proj, deps in pairs(dependencies) do
    local proj_guid = projects[proj]
    if not proj_guid then
        print("Warning: project "..proj.." not found")
    else
        local dep_block = "\tProjectSection(ProjectDependencies) = postProject\n"
        for _, dep_name in ipairs(deps) do
            local dep_guid = projects[dep_name]
            if dep_guid then
                dep_block = dep_block .. "\t\t{"..dep_guid.."} = {"..dep_guid.."}\n"
            else
                print("Warning: dependency "..dep_name.." not found")
            end
        end
        dep_block = dep_block .. "\tEndProjectSection\n"

        local pattern = "(Project%b() = \""..proj.."\".-)\nEndProject"
        content = content:gsub(pattern, "%1\n"..dep_block.."EndProject")
    end
end

local out = assert(io.open(sln_path, "w"))
out:write(content)
out:close()