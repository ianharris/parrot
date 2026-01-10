#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int imax(int a, int b) 
{
  if (a < b) {
    return b;
  }
  return a;
}

int READ_BUFFER_SIZE = 16384;
char NO_ARGUMENTS[] = "No arguments provided to 'parrot' .. exiting\n";

int main(int argc, char **argv)
{
  pid_t cpid;
  int stdout_pipe_fd[2];
  int stderr_pipe_fd[2];
  char stdout_buf[READ_BUFFER_SIZE];
  char stderr_buf[READ_BUFFER_SIZE];

  if (argc == 1) {
    write(STDOUT_FILENO, NO_ARGUMENTS, strlen(NO_ARGUMENTS));
    return 0;
  }

  if (pipe(stdout_pipe_fd) == -1) {
    err(EXIT_FAILURE, "pipe");
  }
  fcntl(stdout_pipe_fd[0], F_SETFD, O_NONBLOCK);
  fcntl(stdout_pipe_fd[1], F_SETFD, O_NONBLOCK);

  if (pipe(stderr_pipe_fd) == -1) {
    err(EXIT_FAILURE, "pipe");
  }
  fcntl(stderr_pipe_fd[0], F_SETFD, O_NONBLOCK);
  fcntl(stderr_pipe_fd[1], F_SETFD, O_NONBLOCK);

  cpid = fork();
  if(cpid == -1) {
    err(EXIT_FAILURE, "fork");
  }

  if (cpid == 0) {
    if (close(stdout_pipe_fd[0]) == -1 || close(stderr_pipe_fd[0]) == -1) {
      err(EXIT_FAILURE, "child close");
    }
    if (close(STDOUT_FILENO) == -1 || close(STDERR_FILENO) == -1) {
      err(EXIT_FAILURE, "child could not close stdout");
    }
    fcntl(stdout_pipe_fd[1], F_DUPFD, STDOUT_FILENO);
    fcntl(stderr_pipe_fd[1], F_DUPFD, STDERR_FILENO);
    execvp(argv[1], argv+1);
  }
  else {

    fd_set rfds;
    struct timeval tv;
    int stdout_pipe_closed = 0;
    int stderr_pipe_closed = 0;

    if (close(stdout_pipe_fd[1]) == -1 || close(stderr_pipe_fd[1]) == -1) {
      err(EXIT_FAILURE, "parent close");
    }
    while (!stdout_pipe_closed || !stderr_pipe_closed) {

      FD_ZERO(&rfds);
      FD_SET(stdout_pipe_fd[0], &rfds);
      FD_SET(stderr_pipe_fd[0], &rfds);

      tv.tv_sec = 0;
      tv.tv_usec = 100000;

      int num_available = select(
        imax(stdout_pipe_fd[0], stderr_pipe_fd[0]) + 1,
        &rfds,
        NULL,
        NULL,
        &tv
      );

      if (num_available == -1) {
        err(EXIT_FAILURE, "Select");
      }

      if (num_available == 0) {
        continue;
      }

      if (FD_ISSET(stdout_pipe_fd[0], &rfds)) {
        int bytes_read = read(stdout_pipe_fd[0], stdout_buf, READ_BUFFER_SIZE);
        if (bytes_read == 0) {
          stdout_pipe_closed = 1;
        }
        else {
          write(STDOUT_FILENO, stdout_buf, strlen(stdout_buf));
        }
      }
      if (FD_ISSET(stderr_pipe_fd[0], &rfds)) {
        int bytes_read = read(stderr_pipe_fd[0], stderr_buf, READ_BUFFER_SIZE);
        if (bytes_read == 0) {
          stderr_pipe_closed = 1;
        }
        else {
          write(STDERR_FILENO, stderr_buf, strlen(stderr_buf));
        }
      }
    }
    int status;
    do {
      waitpid(cpid, &status, WNOHANG);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    return WEXITSTATUS(status);
  }
  return 0;
}
