include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/sfml/sfml
    GIT_TAG 2.5.1
)

FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif ()

list(APPEND SRC_FILES
    Board.cpp
    BoardBuilder.cpp
    BoardView.cpp
    Game.cpp
    GameState.cpp
    Move.cpp
    System.cpp
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SRC_FILES})
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wpedantic -Werror)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    sfml-system
    sfml-window
    sfml-graphics
    pthread
)