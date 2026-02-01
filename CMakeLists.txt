cmake_minimum_required(VERSION 3.8)

Project(SurviveTheHorde LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Download and add tmxlite (tiled editor loader) as a dependency
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG 8f25eb531d26f751e05c60976843f97a6aac9e20
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)

set(PROJECT_SOURCES
	src/Gameplay/Player.cpp
	src/Gameplay/Enemy.cpp
        src/Gameplay/MainMenu.cpp
	src/Gameplay/GameOverScreen.cpp
	src/Core/EnemyPool.cpp
        src/Core/main.cpp
	src/Core/ConfigLoader.cpp
        src/UI/Button.cpp
	src/UI/HealthBar.cpp
	src/Systems/ScoreManager.cpp
	src/Systems/MapLayer.cpp)

set(PROJECT_HEADERS
	include/Gameplay/Player.h
	include/Gameplay/Enemy.h
        include/Gameplay/MainMenu.h
	include/Gameplay/GameOverScreen.h
	include/Core/EnemyPool.h
	include/Core/ConfigLoader.h
	include/Systems/ScoreManager.h
	include/Systems/MapLayer.hpp
	include/UI/HealthBar.h
        include/UI/Button.h)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()
