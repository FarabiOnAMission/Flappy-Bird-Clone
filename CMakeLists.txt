cmake_minimum_required(VERSION 3.10)

# 1. Name your project (You can change "MyGame" to anything)
project(MyGame)

# 2. Find the SFML 3.0 library
# If you get an error here, it means VS can't find where you installed SFML.
find_package(SFML 3 COMPONENTS Graphics Audio Window System REQUIRED)

# 3. Add your source file
add_executable(MyGame main.cpp)

# 4. Link the libraries so the code knows what "sf::" means
target_link_libraries(MyGame PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)