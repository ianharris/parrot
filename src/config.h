#ifndef __PARROT_CONFIG_H__
#define __PARROT_CONFIG_H__

struct configuration {
  char *stdout_log_filename;
  char *stderr_log_filename;
  int process_argument_index;
};

void get_configuration(int argc, char **argv, struct configuration *config);

#endif // !__PARROT_CONFIG_H__
