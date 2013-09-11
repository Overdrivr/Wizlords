if (not _OPTIONS["united"]) then
	project "NazaraVoxelEngine"
end

files
{
	"../include/Nazara/VoxelEngine/**.hpp",
	"../include/Nazara/VoxelEngine/**.inl",
	"../src/Nazara/VoxelEngine/**.hpp",
	"../src/Nazara/VoxelEngine/**.cpp"
}

if (os.is("windows")) then
	excludes { "../src/Nazara/VoxelEngine/Posix/*.hpp", "../src/Nazara/VoxelEngine/Posix/*.cpp" }
else
	excludes { "../src/Nazara/VoxelEngine/Win32/*.hpp", "../src/Nazara/VoxelEngine/Win32/*.cpp" }
end

if (_OPTIONS["united"]) then
	excludes "../src/Nazara/VoxelEngine/Debug/Leaks.cpp"
end

configuration "Debug*"
	links "libNazaraEngine-d"

configuration "Release*"
	links "libNazaraEngine"