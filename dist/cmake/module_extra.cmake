include(${NAP_ROOT}/cmake/dist_shared_crossplatform.cmake)

if(NOT TARGET gist)
    find_package(gist REQUIRED)
endif()
set(MODULE_NAME_EXTRA_LIBS gist)
if(WIN32)
     set(MODULE_NAME_EXTRA_LIBS "${MODULE_NAME_EXTRA_LIBS}" winmm)
endif()
add_include_to_interface_target(mod_napgist ${GIST_INCLUDE_DIR})
add_define_to_interface_target(mod_napgist USE_KISS_FFT)

# Install gist lib into packaged app
if(APPLE)
    install(FILES $<TARGET_FILE:gist> DESTINATION lib)
elseif(UNIX)
    file(GLOB GIST_DYLIBS ${THIRDPARTY_DIR}/gist/linux/lib/libGist${CMAKE_SHARED_LIBRARY_SUFFIX}*)
    install(FILES ${GIST_DYLIBS} DESTINATION lib)
endif()

# Install gist license into packaged project
install(FILES ${GIST_DIST_FILES} DESTINATION licenses/gist)