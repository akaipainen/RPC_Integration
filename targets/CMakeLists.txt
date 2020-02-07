add_executable(gen_tree generate_tree.cpp)

target_link_libraries(gen_tree 
                      PRIVATE Utility)

install(TARGETS gen_tree DESTINATION bin)
# install(FILES $<TARGET_FILE:Custom> DESTINATION bin)