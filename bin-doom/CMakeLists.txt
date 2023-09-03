FILE(GLOB DOOM_LINUX_SOURCES *.c)
add_executable(bin-doom
  ${DOOM_LINUX_SOURCES}
)
target_compile_features(bin-doom PUBLIC c_std_99)
target_compile_options(bin-doom PUBLIC
  -g
  -Wall
  -DNORMALUNIX
  -DLINUX
)
find_package(SDL2 REQUIRED)
target_include_directories(bin-doom PUBLIC
  ${SDL2_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(bin-doom PUBLIC
  m
  ${SDL2_LIBRARIES}
)
FILE(COPY ${DOOM_LINUX_IWADS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
