cmake_minimum_required(VERSION 3.5)

add_executable(parrot)

target_sources(parrot
  PRIVATE
    src/main.c
    src/config.c
)

install(TARGETS parrot)
