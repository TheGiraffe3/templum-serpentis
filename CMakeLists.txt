cmake_minimum_required(VERSION 3.23)
project(TemplumSerpentis LANGUAGES CXX)

# Download and build dependencies.
include(FetchContent)

FetchContent_Declare(SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 3.0.2
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(candle
	GIT_REPOSITORY https://github.com/MiguelMJ/Candle.git
	GIT_TAG master
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM)
set(BUILD_DEMO OFF CACHE BOOL "Disable Candle demo" FORCE)
set(RADIAL_LIGHT_FIX ON CACHE BOOL "Prevent mutex lock error" FORCE)
FetchContent_MakeAvailable(candle)

FetchContent_Declare(nfd-e
	GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
	GIT_TAG v1.4.0
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM)
set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_SDL2_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_SDL3_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_BUILD_GLFW3_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(nfd-e)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
	# Set certain files that we don't need in the built data folder.
	set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/data/TemplumSerpentis.icns)
	set(APP_ICON_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/data/TemplumSerpentis.ico)
	set(APP_ICON_WINDOWS_2 ${CMAKE_CURRENT_SOURCE_DIR}/data/window-icon.png)

	set_source_files_properties(${APP_ICON_MACOSX}
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
	)

	add_executable(TemplumSerpentis MACOSX_BUNDLE ${APP_ICON_MACOSX})

	# Plagiarized from OttoMatic and modified
	set_target_properties(TemplumSerpentis PROPERTIES
		MACOSX_BUNDLE_EXECUTABLE_NAME		"Templum Serpentis"
		MACOSX_BUNDLE_SHORT_VERSION_STRING	"0.1.0"
		MACOSX_BUNDLE_COPYRIGHT				"Created by TheGiraffe3"
		MACOSX_BUNDLE_BUNDLE_NAME			"Templum Serpentis"
		MACOSX_BUNDLE_GUI_IDENTIFIER		"com.loymdayddaud.templumserpentis"
		OUTPUT_NAME							"Templum Serpentis"
	)

	# Plagiarized from Endless Sky
	foreach(folder "data")
		file(GLOB_RECURSE RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*")
		target_sources(TemplumSerpentis PRIVATE "${RESOURCES}")

		foreach(FILE ${RESOURCES})
			# Don't copy if we don't need them.
			if(FILE STREQUAL APP_ICON_MACOSX OR FILE STREQUAL APP_ICON_WINDOWS OR FILE STREQUAL APP_ICON_WINDOWS_2)
				continue()
			endif()

			# Get the relative path from the root folder to the current file.
			file(RELATIVE_PATH NEW_FILE "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE}")

			# Get the parent directory for the new location.
			get_filename_component(FILE_PATH "${NEW_FILE}" DIRECTORY)

			# Resources belong under Resources/.
			set_source_files_properties("${FILE}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${FILE_PATH}")
		endforeach()
	endforeach()
elseif(WIN32)
	# WIN32 flag means that we are a GUI application and don't require terminal.
	add_executable(TemplumSerpentis WIN32 src/windows_resources.rc)
else()
	add_executable(TemplumSerpentis)
endif()

target_include_directories(TemplumSerpentis PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Candle automatically links sfml-graphics and sfml-window, so we don't need to explicitly link those here.
target_link_libraries(TemplumSerpentis PUBLIC
	sfml-audio
	Candle-s
	nfd
)

# Link items that NFD requires.
if(APPLE)
	find_library(APPKIT_LIBRARY AppKit REQUIRED)
	find_library(UTI_LIBRARY UniformTypeIdentifiers REQUIRED)
	target_link_libraries(TemplumSerpentis PRIVATE ${APPKIT_LIBRARY} ${UTI_LIBRARY} "-framework CoreFoundation")
elseif(WIN32)
	target_link_libraries(TemplumSerpentis PRIVATE ole32 uuid shell32)
elseif(UNIX AND NOT APPLE)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	target_include_directories(TemplumSerpentis PRIVATE ${GTK3_INCLUDE_DIRS})
	target_link_libraries(TemplumSerpentis PRIVATE ${GTK3_LIBRARIES})
endif()

target_sources(TemplumSerpentis PRIVATE
	src/Game.cpp
	src/LoadFile.cpp
	src/main.cpp
	src/MenuFunctions.cpp
)
