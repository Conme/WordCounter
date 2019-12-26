set(CMAKE_C_STANDARD 11)

add_definitions("-D_DEBUG -D_STATS")

if (MSVC)
    add_compile_options(/Od /W4)
endif()
	
if(GNU)
    add_compile_options(-O0 -g3 -pg -pedantic-errors -Wall -Wextra -Wconversion)
endif()
