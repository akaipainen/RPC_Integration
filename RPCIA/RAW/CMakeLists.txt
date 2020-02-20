# Module
set(MODULE RAW)
add_definitions(-D_MODULE_="${MODULE}")

# Module include folder
include_directories(${RpcIA_SOURCE_DIR}/${MODULE})

# Additional include folders in alphabetical order except ROOT
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
include_directories(${RpcIA_SOURCE_DIR}/BASE)

# Set sources in alphabetical order
set(SRCS
    packet.cxx
    raw_decoder.cxx
    raw_reader.cxx
    raw_stream.cxx
    )

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Additional headers
# list(APPEND HDRS "header1.h" "header2.h")

# Generate the dictionary
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Generate the ROOT map
set(LIBDEPS BASE)
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Create object to be used in case of static libraries
# Otherwise sources will be compiled twice
add_library(${MODULE}-object OBJECT ${SRCS} G__${MODULE}.cxx)
# Add a library to the project using the object
add_library(${MODULE} SHARED $<TARGET_OBJECTS:${MODULE}-object>)

# Setting the correct headers for the object as gathered from the dependencies
target_include_directories(${MODULE}-object PUBLIC $<TARGET_PROPERTY:${MODULE},INCLUDE_DIRECTORIES>)
set_target_properties(${MODULE}-object PROPERTIES COMPILE_DEFINITIONS $<TARGET_PROPERTY:${MODULE},COMPILE_DEFINITIONS>)

# Public include folders that will be propogated to the dependencies
target_include_directories(${MODULE} PUBLIC ${incdirs})

# Additional compilation flags
set_target_properties(${MODULE} PROPERTIES COMPILE_FLAGS "")

# Linking the libraries
target_link_libraries(${MODULE} ${LIBDEPS})

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

# Static version
# if(RPCIA_STATIC)
#     add_library(${MODULE}-static STATIC $<TARGET_OBJECTS:${MODULE}-object>)
#     set_target_properties(${MODULE}-static PROPERTIES OUTPUT_NAME ${MODULE})
    
#     # List of shared dependencies / the name of the variable containing the list of static ones
#     generate_static_dependencies("${RPCIA_DEPENDENCIES}" "STATIC_RPCIA_DEPENDENCIES")
#     target_link_libraries(${MODULE}-static ${STATIC_RPCIA_DEPENDENCIES} Root RootExtra)

#     # Public include folders that will be propagated to the dependecies
#     target_include_directories(${MODULE}-static PUBLIC ${incdirs})

#     set_target_properties(${MODULE}-static PROPERTIES LINK_FLAGS "-Wl,--whole-archive")

#     # Installation
#     install(TARGETS ${MODULE}-static
#             ARCHIVE DESTINATION lib
#             LIBRARY DESTINATION lib)
# endif(RPCIA_STATIC)