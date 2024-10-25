cmake_minimum_required(VERSION 3.10)
project(MySFMLProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set SFML directory path (make sure it contains SFMLConfig.cmake)
set(SFML_DIR "C:\\FML-2.6.1/lib/cmake/SFML")

# Add CMake prefix path
set(CMAKE_PREFIX_PATH "C:\\SFML-2.6.1")

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp "prev.txt"   "src/SoftBody.cpp")

# Find SFML package
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
