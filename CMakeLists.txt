cmake_minimum_required(VERSION 3.30)
project(pirgpu C)

set(CMAKE_C_STANDARD 11)
include_directories(src src/include)

#add_compile_definitions(USE_BIGNUM)

link_libraries(ssl crypto m)


add_executable(pirgoldberg
        test/test_pirgb.c
        src/lib/pirgb.c
        src/lib/poly.c
)

add_executable(polynomial
        test/test_polynomial.c
        src/lib/poly.c
)
