set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/O2 /W4)
endif()

if(GNU)
    add_compile_options(-O3 -march=native -pedantic-errors -Wextra -Wconversion)
endif()
