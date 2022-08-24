cmake_minimum_required(VERSION 3.18.4)
project(mod_napgist)

find_package(gist REQUIRED)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

set(INCLUDES src ${GIST_INCLUDE_DIR})

# Get our NAP modules dependencies from module.json
module_json_to_cmake()

add_library(${PROJECT_NAME} SHARED ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Modules)
# Remove lib prefix on Unix libraries
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

target_link_libraries(${PROJECT_NAME} napcore ${DEPENDENT_NAP_MODULES} debug ${GIST_LIBRARIES_DEBUG} optimized ${GIST_LIBRARIES_RELEASE} )

target_compile_definitions(${PROJECT_NAME} PRIVATE NAP_SHARED_LIBRARY PUBLIC USE_KISS_FFT)

# Deploy module.json as MODULENAME.json alongside module post-build
copy_module_json_to_bin()

# Package into platform release
if(APPLE)
    set(MACOS_EXTRA_RPATH_RELEASE ../../../../thirdparty/gist/macos/lib)
    set(MACOS_EXTRA_RPATH_DEBUG ${MACOS_EXTRA_RPATH_RELEASE})
elseif(UNIX)
    set(LINUX_EXTRA_RPATH ../../../../thirdparty/gist/linux/lib)
endif()
package_module()

# Package gist into platform release
install(FILES ${GIST_DIST_FILES} DESTINATION thirdparty/gist)
install(DIRECTORY ${GIST_INCLUDE_DIR}/ DESTINATION thirdparty/gist/include)
if(MSVC)
    file(GLOB GIST_SHARED_LIBS ${GIST_LIBRARY_DIR}/libGist*${CMAKE_SHARED_LIBRARY_SUFFIX})
    install(FILES ${GIST_SHARED_LIBS}
            DESTINATION thirdparty/gist/bin)
elseif(APPLE)
    file(GLOB GIST_DYLIBS ${GIST_LIBRARY_DIR}/libGist*${CMAKE_SHARED_LIBRARY_SUFFIX})
    install(FILES ${GIST_DYLIBS}
            DESTINATION thirdparty/gist/lib)
elseif(UNIX)
    file(GLOB GIST_DYLIBS ${GIST_LIBRARY_DIR}/libGist${CMAKE_SHARED_LIBRARY_SUFFIX}*)
    install(FILES ${GIST_DYLIBS}
            DESTINATION thirdparty/gist/lib)
endif()
