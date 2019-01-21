project(kesha C ASM)
cmake_minimum_required(VERSION 3.13)

set(GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_ROOT})
set(PALETTE 2 3 5 4)

macro(generate_texture NAME SOURCE)
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/_${NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/_${NAME}.c
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${SOURCE} ${NAME} -p ${PALETTE} > ${GENERATED_ROOT}/_${NAME}.c
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${SOURCE}
	)
endmacro()

generate_texture(splash ${CMAKE_CURRENT_SOURCE_DIR}/assets/splash.png)

set(SOURCES sources/main.c sources/init.s)
add_executable(kesha ${SOURCES} ${GENERATED_SOURCES})

add_custom_command(TARGET kesha POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ihx-to-sna.py $<TARGET_FILE:kesha> ${CMAKE_CURRENT_BINARY_DIR}/kesha.sna)
