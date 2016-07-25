
if (MSVC)
	# No WinMain defined -> go with main() and shut the fup
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

set(LIBS soci_core_static soci_odbc_static)

# Look foor sources, headers to compile & libs to link with
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp )
file(GLOB SOCI_LIB_DIR LIST_DIRECTORIES true ${SOCI_LIB_DIR}/*)

include_directories(${SOCI_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PLATFORM_LIBS} ${LIBS})
