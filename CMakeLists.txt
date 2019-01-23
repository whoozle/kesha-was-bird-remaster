project(kesha C ASM)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --opt-code-size --peep-asm --peep-return")
set(CMAKE_EXE_LINKER_FLAGS "--code-loc 0x6000 --data-loc 0xf000 --no-xinit-opt")

set(GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_ROOT})
set(PALETTE 0 6 2 7)

include_directories(${GENERATED_ROOT} sources)

macro(generate_texture)
	cmake_parse_arguments(_GT "" "NAME;SOURCE" "PALETTE" ${ARGN})
	if ("X${_GT_PALETTE}" STREQUAL "X")
		set(_GT_PALETTE "${PALETTE}")
	endif()

	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/texture_${_GT_NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/texture_${_GT_NAME}.c ${GENERATED_ROOT}/texture_${_GT_NAME}.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py -c ${_GT_SOURCE} ${_GT_NAME} -p ${_GT_PALETTE}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${_GT_SOURCE}
	)
endmacro()

generate_texture(NAME drinking		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/drinking.png PALETTE 0 1 5 1)
generate_texture(NAME earth_1		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth.png)
generate_texture(NAME earth_2		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_2.png)
generate_texture(NAME earth_3		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_3.png)
generate_texture(NAME earth_4		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_4.png)
generate_texture(NAME fday_device	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fday_devise.png)
generate_texture(NAME fish			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish.png)
generate_texture(NAME fish_army		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish_army.png)
generate_texture(NAME frame			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/frame.png PALETTE 5 2 3 0)
generate_texture(NAME galina		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina.png)
generate_texture(NAME galina_pests	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina_pests.png)
generate_texture(NAME memory_erizer	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/memory_erizer.png)
generate_texture(NAME ninja			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja.png)
generate_texture(NAME ninja_fish	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_fish.png)
generate_texture(NAME ninja_kesha_1	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha.png)
generate_texture(NAME ninja_kesha_2	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_2.png)
generate_texture(NAME ninja_kesha_3	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_3.png)
generate_texture(NAME phone			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/phone_notepad.png PALETTE 2 1 5 4)
generate_texture(NAME prison		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/prison.png)
generate_texture(NAME professor		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/professor.png)
generate_texture(NAME room			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/room.png PALETTE 7 1 2 0)

set(SOURCES
	sources/main.c
	sources/lz4.c
	sources/fb.c
	sources/init.s
	sources/tiles.c
	sources/days.c
	sources/texture.c
#	sources/fb_fast.s
)
add_executable(kesha ${SOURCES} ${GENERATED_SOURCES})

add_custom_command(TARGET kesha POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ihx-to-z80.py $<TARGET_FILE:kesha> ${CMAKE_CURRENT_BINARY_DIR}/kesha.z80)
