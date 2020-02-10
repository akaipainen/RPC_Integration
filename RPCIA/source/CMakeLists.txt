find_package(ROOT)
include_directories(${ROOT_INCLUDE_DIRS})
include(${ROOT_USE_FILE})

INCLUDE_DIRECTORIES(../include)
ROOT_GENERATE_DICTIONARY(CustomClassesDictionary
                         event.hpp
                         hit.hpp
                         LINKDEF ../include/linkdef.h)

if (${ROOT_VERSION} VERSION_GREATER "6.0")
	install(
		FILES
		${CMAKE_CURRENT_BINARY_DIR}/libCustomClassesDict_rdict.pcm
		${CMAKE_CURRENT_BINARY_DIR}/libCustomClassesDict.rootmap
		DESTINATION lib/)
endif (${ROOT_VERSION} VERSION_GREATER "6.0")

file(GLOB sources *.cpp)
file(GLOB headers ../include/*.hpp)

add_library(Utility SHARED ${sources} ${headers} CustomClassesDictionary.cxx)

# find_package(ROOT REQUIRED)
target_include_directories(Utility PUBLIC ${ROOT_INCLUDE_DIRS} include)
target_link_libraries(Utility ${ROOT_LIBRARIES})

target_include_directories(Utility 
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include> 
                           $<INSTALL_INTERFACE:include>)