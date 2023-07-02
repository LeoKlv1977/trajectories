add_executable(utests utests.cpp)

target_include_directories(utests PRIVATE ${ROOT_SOURCE_DIR}/lib)
target_link_libraries(utests lib)
