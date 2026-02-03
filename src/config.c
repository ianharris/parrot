#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

char DEFAULT_STDOUT_FILENAME[] = "stdout.log";
char DEFAULT_STDERR_FILENAME[] = "stderr.log";

void get_process_argument_index(
  int argc,
  char **argv,
  struct configuration *config
)
{
  config->process_argument_index = 1;
  for(int i=0; i<argc; ++i) {
    if (strcmp(argv[i], "--") == 0) {
      config->process_argument_index = i+1;
    }
  }
}

void get_configuration(
  int argc,
  char **argv,
  struct configuration* config
)
{
  int opt;
  char **parrot_argv;

  get_process_argument_index(argc, argv, config);

  parrot_argv = (char**)malloc(sizeof(char *) * config->process_argument_index);
  for(int i=0; i<config->process_argument_index-1; ++i) {
    parrot_argv[i] = argv[i];
  }
  parrot_argv[config->process_argument_index-1] = NULL;
  config->stdout_log_filename = DEFAULT_STDOUT_FILENAME;
  config->stderr_log_filename = DEFAULT_STDERR_FILENAME;

  while ((opt = getopt(config->process_argument_index-1, parrot_argv, "o:e:")) != -1) {
    switch(opt) {
      case 'o':
        config->stdout_log_filename = optarg;
        break;
      case 'e':
        config->stderr_log_filename = optarg;
        break;
    }
  }

  free(parrot_argv);
}
