newoption {
	trigger     = "x64",
	description = "Setup build project for x64 arch"
}

newoption {
	trigger     = "united",
	description = "Builds all the modules as one united library"
}

newoption {
	trigger     = "with-extlibs",
	description = "Builds the extern libraries"
}

newoption {
	trigger     = "with-examples",
	description = "Builds the examples"
}

solution "NazaraEngineWizlordsAddons"
-- Hack: loadfile doesn't change current directory, as does premake-overloaded dofile
loadfile("scripts/common.lua")()
configuration {} -- Désactivation du filtre

local modules

if (_OPTIONS["united"]) then
	project "NazaraEngineWizlordsAddons"
end

modules = os.matchfiles("scripts/module/*.lua")
for k,v in pairs(modules) do
	local moduleName = v:match(".*/(.*).lua")

	if (moduleName ~= "core") then
		newoption {
			trigger     = "exclude-" .. moduleName,
			description = "Exclude the " .. moduleName .. " module from build system"
		}
	end

	if (not _OPTIONS["exclude-" .. moduleName]) then
		local f, err = loadfile(v)
		if (f) then
			f()
			configuration {} -- Désactivation du filtre
		else
			error("Unable to load module: " .. err)
		end
	end
end

modules = os.matchfiles("scripts/actions/*.lua")
for k,v in pairs(modules) do
	local f, err = loadfile(v)
	if (f) then
		f()
		configuration {} -- Désactivation du filtre
	else
		print("Unable to load action: " .. err)
	end
end

if (_OPTIONS["with-extlibs"]) then
	solution "NazaraExtlibs"
	loadfile("scripts/common_extlibs.lua")()
	configuration {} -- Désactivation du filtre

	local extlibs = os.matchfiles("../extlibs/build/*.lua")
	for k,v in pairs(extlibs) do
		dofile(v)
		configuration {} -- Désactivation du filtre
	end
end

if (_OPTIONS["with-examples"]) then
	solution "NazaraWizlordsExamples"
	loadfile("scripts/common_examples.lua")()

	local examples = os.matchdirs("../examples/*")
	for k,v in pairs(examples) do
		local dirName = v:match(".*/(.*)")
		if (dirName ~= "bin" and
		    dirName ~= "build") then
			project(dirName)
			dofile(v .. "/build.lua")
			configuration {} -- Désactivation du filtre
		end
	end
end
