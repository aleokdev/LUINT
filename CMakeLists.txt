cmake_minimum_required(VERSION "3.12.18")

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_CXX_STANDARD 17)

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG)
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "build/")
endforeach()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
project("LUINT" VERSION 0.1
				LANGUAGES CXX C)


set(LUINT_SOURCES
	"src/main.cpp"
	"src/gui.cpp"
	"src/biosedit_window.cpp"
	"src/api/button.cpp"
	"src/api/keyboard.cpp"
	"src/api/LED.cpp"
	"src/api/machine.cpp"
	"src/api/network.cpp"
	"src/api/processing_unit.cpp"
	"src/api/ptm_monitor.cpp"
	"src/api/UID.cpp"
)
set(LUINT_INCLUDE_DIRECTORIES "include/" "include/misc" "lua/")
set(LUINT_LINK_LIBRARIES "")
set(LUINT_LINK_DIRECTORIES "")

add_subdirectory("lib/")

add_subdirectory(lib/glfw)

add_executable(${PROJECT_NAME} ${LUINT_SOURCES})
target_compile_options(${PROJECT_NAME} PUBLIC "-Wall")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_include_directories(${PROJECT_NAME} PUBLIC glfw ${LUINT_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} glfw ${LUINT_LINK_LIBRARIES})

add_dependencies(${PROJECT_NAME} glfw)