# Module include folder
include_directories(${RpcIA}/RPCIA)

# message("Root Libs: ${ROOT_LIBS}")

# Additional include folders in alphabetical order
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})

# Main executable
add_executable(gen_tree generate_tree.cxx)
target_link_libraries(gen_tree RPCIA Core Tree Hist RIO)

install(TARGETS gen_tree RUNTIME DESTINATION bin)
