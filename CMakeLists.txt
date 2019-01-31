project(kesha C ASM)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --opt-code-size --peep-asm --peep-return --no-xinit-opt --reserve-regs-iy")
set(PC_ADDR 23800)
set(CMAKE_EXE_LINKER_FLAGS "--code-loc ${PC_ADDR} --data-loc 0xf600 --no-xinit-opt")

set(GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_ROOT})
set(PALETTE 0 6 2 7)

include_directories(${GENERATED_ROOT} sources)

macro(generate_texture)
	cmake_parse_arguments(_GT "" "NAME;SOURCE" "PALETTE;OPTIONS" ${ARGN})
	if ("X${_GT_PALETTE}" STREQUAL "X")
		set(_GT_PALETTE "${PALETTE}")
	endif()

	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/texture_${_GT_NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/texture_${_GT_NAME}.c ${GENERATED_ROOT}/texture_${_GT_NAME}.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${_GT_SOURCE} ${_GT_NAME} ${_GT_OPTIONS} -p ${_GT_PALETTE}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${_GT_SOURCE}
	)
endmacro()

macro(generate_font)
	cmake_parse_arguments(_GF "" "NAME;SOURCE" "" ${ARGN})
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/font_${_GF_NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/font_${_GF_NAME}.c ${GENERATED_ROOT}/font_${_GF_NAME}.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-font.py ${_GF_SOURCE} ${_GF_NAME} ${GENERATED_ROOT}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-font.py ${_GF_SOURCE}
	)
endmacro()

macro(generate_text)
	cmake_parse_arguments(_GT "" "" "SOURCES" ${ARGN})
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/text.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/text.c ${GENERATED_ROOT}/text.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-text.py ${GENERATED_ROOT} ${_GT_SOURCES}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-text.py ${_GT_SOURCES}
	)
endmacro()

macro(generate_dialogs)
	cmake_parse_arguments(_GT "" "" "" ${ARGN})
	add_custom_command(OUTPUT ${GENERATED_ROOT}/_dialogs.c ${GENERATED_ROOT}/_dialogs.h ${GENERATED_ROOT}/dialogs.json
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-dialogs.py ${GENERATED_ROOT}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-dialogs.py
	)
endmacro()

macro(generate_audio)
	cmake_parse_arguments(_GT "" "NAME;SOURCE" "OPTIONS" ${ARGN})
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/audio_${_GT_NAME}.c)
	add_custom_command(OUTPUT ${GENERATED_ROOT}/audio_${_GT_NAME}.c ${GENERATED_ROOT}/audio_${_GT_NAME}.h
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-audio.py ${_GT_SOURCE} ${_GT_NAME} ${GENERATED_ROOT} ${_GT_OPTIONS}
		WORKING_DIRECTORY ${GENERATED_ROOT}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-audio.py ${_GT_SOURCE}
	)
endmacro()

macro(generate_dtmf)
	#cmake_parse_arguments(_GT "" "" "" ${ARGN})
	list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/audio_dtmf.c)
	add_custom_command(
		OUTPUT ${GENERATED_ROOT}/audio_dtmf.c
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-dtmf.py > ${GENERATED_ROOT}/audio_dtmf.c
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-dtmf.py
	)
endmacro()

list(APPEND GENERATED_SOURCES ${GENERATED_ROOT}/screen_splash.s)
add_custom_command(
	OUTPUT ${GENERATED_ROOT}/screen_splash.s
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py ${CMAKE_CURRENT_SOURCE_DIR}/assets/splash.png splash -f screen -p 7 0 2 0 -t 8
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate-texture.py
	WORKING_DIRECTORY ${GENERATED_ROOT}
)

generate_texture(NAME drinking			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/drinking.png OPTIONS -c PALETTE 0 1 5 1)
generate_texture(NAME earth_1			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth.png	OPTIONS -c PALETTE 0 6 5 7)
generate_texture(NAME earth_2			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_2.png	OPTIONS -c PALETTE 0 6 5 7)
generate_texture(NAME earth_3			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_3.png	OPTIONS -c PALETTE 0 6 5 7)
generate_texture(NAME earth_4			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/earth_4.png	OPTIONS -c PALETTE 0 6 5 7)
generate_texture(NAME fday_device		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fday_devise.png OPTIONS -c)
generate_texture(NAME fish				SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish.png OPTIONS -c)
generate_texture(NAME fish_army			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/fish_army.png OPTIONS -c)
generate_texture(NAME frame				SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/frame.png OPTIONS -c PALETTE 5 2 3 0)
generate_texture(NAME galina			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina.png OPTIONS -c)
generate_texture(NAME galina_pests		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/galina_pests.png OPTIONS -c)
generate_texture(NAME memory_erizer		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/memory_erizer.png OPTIONS -c)
generate_texture(NAME ninja				SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja.png OPTIONS -c)
generate_texture(NAME ninja_fish		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_fish.png OPTIONS -c)
generate_texture(NAME ninja_kesha_1		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha.png OPTIONS -c)
generate_texture(NAME ninja_kesha_2		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_2.png OPTIONS -c)
generate_texture(NAME ninja_kesha_3		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/ninja_kills_kesha_3.png OPTIONS -c)
generate_texture(NAME phone				SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/phone_notepad.png OPTIONS -c PALETTE 2 1 5 4 -i 6)
generate_texture(NAME prison			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/prison.png OPTIONS -c)
generate_texture(NAME professor			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/big_pics/professor.png OPTIONS -c)
generate_texture(NAME room				SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/room.png OPTIONS -c PALETTE 7 1 2 0)

generate_texture(NAME head_kesha		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/kesha_v2.png OPTIONS -m)
generate_texture(NAME head_kesha_o		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/kesha_v2_open.png OPTIONS -m)
generate_texture(NAME head_kesha_e		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/kesha_v2_excited.png OPTIONS -m)
generate_texture(NAME head_cow			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/squirrel_2.png OPTIONS -m)
generate_texture(NAME head_professor	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/professor2.png OPTIONS -m)
generate_texture(NAME head_ninja		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/ninja2.png OPTIONS -m)
generate_texture(NAME head_ninja_kesha	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/ninja_kesha.png OPTIONS -m)
generate_texture(NAME head_lab			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/lab.png OPTIONS -m)
generate_texture(NAME head_disabled		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/disabled.png OPTIONS -m)
generate_texture(NAME head_fish			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/fish.png OPTIONS -m)
generate_texture(NAME head_pets			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/pets2.png OPTIONS -m)
generate_texture(NAME head_glitch		SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/heads/glitch.png OPTIONS -m)
generate_texture(NAME letter			SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/tiles/letter.png OPTIONS -m)

generate_font(NAME font					SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/font/5.font)

generate_dialogs()
generate_text(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets/en.json ${GENERATED_ROOT}/dialogs.json)

generate_audio(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/track3.wav NAME theme OPTIONS -l10)

generate_dtmf()

set(SOURCES
	sources/main.c
	sources/credits.c
	sources/init.s
	sources/runtime.c
	sources/audio.c
	sources/lz4.c
	sources/fb.c
	sources/tiles.c
	sources/days.c
	sources/texture.c
	sources/font.c
	sources/dialogs.c
	sources/phone.c
	sources/vars.c
	sources/call_galina.c
	sources/call_ninja.c
	sources/call_invalid.c
	sources/call_lab.c
	sources/call_pets.c
	sources/call_glitch.c
)
add_executable(kesha ${SOURCES} ${GENERATED_SOURCES})

add_custom_command(TARGET kesha POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ihx-to-z80.py $<TARGET_FILE:kesha> ${CMAKE_CURRENT_BINARY_DIR}/kesha.z80 ${PC_ADDR})
