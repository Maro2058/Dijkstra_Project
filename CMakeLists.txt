cmake_minimum_required(VERSION 3.27)
project(Dijkstra_Project)

set(CMAKE_CXX_STANDARD 17)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch SFML
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

# Add the executable target
add_executable(Dijkstra_Project Dijkstra.cpp Dijkstra.h DLL.cpp DLL.h)

# Link SFML libraries to the executable target
target_link_libraries(Dijkstra_Project PRIVATE sfml-graphics)

# Set C++ features for the target
target_compile_features(Dijkstra_Project PRIVATE cxx_std_17)

# If it's Windows, copy OpenAL DLL
if(WIN32)
    add_custom_command(
            TARGET Dijkstra_Project
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Dijkstra_Project>
            VERBATIM
    )
endif()

# Install the target
install(TARGETS Dijkstra_Project)