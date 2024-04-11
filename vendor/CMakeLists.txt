add_subdirectory(glad)
add_subdirectory(glm)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE glad glm)