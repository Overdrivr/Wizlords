project "stb_image" -- Bug de premake si � le m�me nom que la biblio link�e dans Utility
language "C++" -- On compile en C++ car le C99 n'est pas support� partout
kind "StaticLib"
targetname "stb_image-s"

defines "STBI_NO_STDIO"

files
{
	"../include/stb_image/stb_image.h",
	"../src/stb_image/stb_image.c"
}
