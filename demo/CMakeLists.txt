
include_directories(${APOTHECA_SOURCE_DIR}/eikon)

link_directories (${APOTHECA_BINARY_DIR}/eikon)

add_executable (demo main.c)

include_directories(${APOTHECA_SOURCE_DIR}/eikon)
target_link_libraries (demo eikon ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES})


