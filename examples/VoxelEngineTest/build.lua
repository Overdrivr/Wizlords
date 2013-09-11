kind "ConsoleApp"

files "main.cpp"

if (_OPTIONS["united"]) then
	configuration "DebugStatic"
		links "NazaraEngine-s-d"
		links "NazaraEngineWizlordsAddons-s-d"

	configuration "ReleaseStatic"
		links "NazaraEngine-s"
		links "NazaraEngineWizlordsAddons-s"

	configuration "DebugDLL"
		links "NazaraEngine-d"
		links "NazaraEngineWizlordsAddons-d"

	configuration "ReleaseDLL"
		links "NazaraEngine"
		links "NazaraEngineWizlordsAddons"
else
	configuration "DebugStatic"
		links "NazaraEngine-s-d"
		links "NazaraVoxelEngine-s-d"

	configuration "ReleaseStatic"
		links "NazaraEngine-s"
		links "NazaraVoxelEngine-s"

	configuration "DebugDLL"
		links "NazaraEngine-d"
		links "NazaraVoxelEngine-d"

	configuration "ReleaseDLL"
		links "NazaraEngine"
		links "NazaraVoxelEngine"
end
