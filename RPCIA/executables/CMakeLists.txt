# Module include folder
# include_directories(${RpcIA}/executables)

# Additional include folders in alphabetical order
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
include_directories(${RpcIA_SOURCE_DIR}/BASE
                    ${RpcIA_SOURCE_DIR}/RAW
                    ${RpcIA_SOURCE_DIR}/RECON)

# Main executable
add_executable(gen_tree generate_tree.cxx)
target_link_libraries(gen_tree RECON RAW BASE MAPPING Core Tree Hist RIO)

install(TARGETS gen_tree RUNTIME DESTINATION bin)
