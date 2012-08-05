
include_directories(${apotheca_SOURCE_DIR})
link_directories (${apotheca_BINARY_DIR}/eikon)

add_executable (demo main.c)


target_link_libraries (demo eikon ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES})


add_executable (fontinfo fontinfo.c)
target_link_libraries (fontinfo eikon ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES})
