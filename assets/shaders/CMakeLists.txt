FILE(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *)

cmrc_add_resource_library(shaders NAMESPACE shaders ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE shaders)