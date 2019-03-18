macro(header_directories return_list)
    file(GLOB_RECURSE new_list "*.h" "*.hpp")
    set(dir_list "")
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    set(${return_list} ${dir_list})
endmacro()

# vcpkg arguments.
# -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows

cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
project(Saxion_Y2Q2_Rendering)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

file(GLOB_RECURSE SOURCES "src/*.cpp")
header_directories(INCLUDES)

include_directories(${INCLUDES})

if(WIN32)
	add_executable(Saxion_Y2Q2_Rendering WIN32 ${SOURCES})
else()
	add_executable(Saxion_Y2Q2_Rendering ${SOURCES})
endif()

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
if(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
endif()

#if(WIN32)
#    set(SFML_ROOT C:/dev/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit/SFML-2.5.1/lib/cmake/SFML)
#endif()

find_package(lua 5.3 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype 2 REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
find_package(assimp REQUIRED)

if(WIN32)
    find_package(SFML 2.5 COMPONENTS audio graphics window system main REQUIRED)
else()
    find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
endif()

link_directories(${SFML_LIBRARY_DIRS})

if(WIN32)
	set(LUALIB ${LUA_LIBRARIES})
    set(SFMLLIB sfml-graphics sfml-audio sfml-window sfml-system sfml-main)
	set(FREETYPELIB Freetype::Freetype)
	set(ASSIMPLIB assimp${ASSIMP_LIBRARY_SUFFIX})
else()
	set(LUALIB lua)
    set(SFMLLIB sfml-graphics sfml-audio sfml-window sfml-system)
	set(FREETYPELIB freetype)
	set(ASSIMPLIB assimp)
endif()

target_link_libraries(Saxion_Y2Q2_Rendering ${LUALIB} glm ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SFMLLIB} ${FREETYPELIB} ${ASSIMPLIB})