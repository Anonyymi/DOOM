FILE(GLOB DOOM_LINUX_SOURCES *.c)
add_executable(bin-doom
  ${DOOM_LINUX_SOURCES}
)
set_target_properties(bin-doom PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON C_EXTENSIONS ON)
target_compile_options(bin-doom PUBLIC
  -g
  -Wall
  -DNORMALUNIX
  -DLINUX
)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
target_include_directories(bin-doom PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(bin-doom PUBLIC
  m
  SDL2::Main
  SDL2::Mixer
)
FILE(COPY ${DOOM_LINUX_IWADS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${DOOM_LINUX_PWADS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
