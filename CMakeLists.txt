add_executable(parrot)

target_sources(parrot
  PRIVATE
    src/main.c
)

install(TARGETS parrot)
