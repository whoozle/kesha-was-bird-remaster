project(kesha C ASM)
cmake_minimum_required(VERSION 3.13)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --opt-code-speed")
set(CMAKE_EXE_LINKER_FLAGS "--code-loc 0x6000 --data-loc 0xf000 --no-xinit-opt --opt-code-speed --peep-asm --peep-return")

set(GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_ROOT})
set(PALETTE 0 6 2 7)

include_directories(${GENERATED_ROOT})

macro(generate_texture NAME SOURCE)
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/texture_${NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/texture_${NAME}.c ${GENERATED_ROOT}/texture_${NAME}.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py -c ${SOURCE} ${NAME}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${SOURCE}
	)
endmacro()

generate_texture(drinking		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/drinking.png)
generate_texture(earth_1		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth.png)
generate_texture(earth_2		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_2.png)
generate_texture(earth_3		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_3.png)
generate_texture(earth_4		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_4.png)
generate_texture(fday_device	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fday_devise.png)
generate_texture(fish			${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish.png)
generate_texture(fish_army		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish_army.png)
generate_texture(galina			${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina.png)
generate_texture(galina_pests	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina_pests.png)
generate_texture(memory_erizer	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/memory_erizer.png)
generate_texture(ninja			${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja.png)
generate_texture(ninja_fish		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_fish.png)
generate_texture(ninja_kesha_1	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha.png)
generate_texture(ninja_kesha_2	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_2.png)
generate_texture(ninja_kesha_3	${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_3.png)
generate_texture(phone			${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/phone_notepad.png)
generate_texture(prison			${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/prison.png)
generate_texture(professor		${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/professor.png)

set(SOURCES
	sources/main.c
	sources/lz4.c
	sources/fb.c
	sources/init.s
#	sources/fb_fast.s
)
add_executable(kesha ${SOURCES} ${GENERATED_SOURCES})

add_custom_command(TARGET kesha POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ihx-to-z80.py $<TARGET_FILE:kesha> ${CMAKE_CURRENT_BINARY_DIR}/kesha.z80)
