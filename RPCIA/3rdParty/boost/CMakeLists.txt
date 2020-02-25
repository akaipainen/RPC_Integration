add_library(boost INTERFACE)

set(COMPONENTS ptr_container)

set(Boost_USE_STATIC_LIBS "${RPCIA_STATIC}")
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50 REQUIRED COMPONENTS "${COMPONENTS}")
message(STATUS "Boost version: ${Boost_VERSION}")

# This is needed if your Boost version is newer than your CMake version
# or if you have an old version of CMake (<3.5)
foreach(COMPONENT "${COMPONENTS}")
    if(NOT TARGET COMPONENT)
        add_library("Boost::${COMPONENT}" IMPORTED INTERFACE)
        set_property(TARGET "Boost::${COMPONENT}" PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
        set_property(TARGET "Boost::${COMPONENT}" PROPERTY
            INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
    endif()
    target_link_libraries(MyExeOrLibrary PUBLIC Boost::filesystem)
endforeach(COMPONENT "${COMPONENTS}")

