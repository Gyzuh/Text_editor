#include "Shell_window.h"

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cerrno>

#include <panel.h>
#include <unistd.h>
#include <sys/select.h>

#define PIPE_READ  (0)
#define PIPE_WRITE (1)

void shell_window::Show(void)
{
  // Bash seems to be taking over the foreground all the time.
  // Solutions: - Run te as child. Drawback: PID changes all the time.
  //            - Run bash non-interactive.  Not working at the moment for some reason.
  //            - Run bash interactive in foreground.  Do not filter input, but use named pipes to communicate with te.

  WINDOW * Window_handle = newwin(Size.Y, Size.X, 0, 0);
  PANEL * Panel = new_panel(Window_handle);

  int Input_pipe[2];
  int Output_pipe[2];
  assert(pipe(Input_pipe) >= 0);
  assert(pipe(Output_pipe) >= 0);

//  endwin();

  pid_t PID = fork();
  assert(PID >= 0);
  if (PID == 0)
//  if (PID > 0)
  {
    assert(dup2(Input_pipe[PIPE_READ], STDIN_FILENO) != -1);
    assert(dup2(Output_pipe[PIPE_WRITE], STDOUT_FILENO) != -1);
    assert(dup2(Output_pipe[PIPE_WRITE], STDERR_FILENO) != -1);

    close(Input_pipe[PIPE_READ]);
    close(Input_pipe[PIPE_WRITE]);
    close(Output_pipe[PIPE_READ]);
    close(Output_pipe[PIPE_WRITE]);

    execl("/bin/bash", "/bin/bash", (char *) NULL);
  }
  else
  {
    // How do I put the parent in the foreground???
//    wprintw(Window_handle, "%i, %i\n", PID, getpgrp());
//  wprintw(Window_handle, "Parent: %i\n", getpgrp());
//    if (setpgid(PID, getpgrp()) == -1)
//      wprintw(Window_handle, "ERROR: %i", errno);
//    Try tcsetpgrp!!!
//    tcsetpgrp(getpgrp(), STDIN_FILENO);

    close(Input_pipe[PIPE_READ]);
    close(Output_pipe[PIPE_WRITE]);

//    signal(SIGTTIN, SIG_IGN);

    while (true)
    {
      fd_set Read_handles;
      FD_ZERO(&Read_handles);
      FD_SET(STDIN_FILENO, &Read_handles);
      FD_SET(Output_pipe[PIPE_READ], &Read_handles);

      struct timeval Timeout;
      Timeout.tv_sec = 5;
      Timeout.tv_usec = 0;

      int Result = select(std::max(STDIN_FILENO, Output_pipe[PIPE_READ]) + 1,
                          &Read_handles, NULL, NULL, &Timeout);
      assert(Result >= 0);
      if (Result == 0)
        continue;

      if (FD_ISSET(STDIN_FILENO, &Read_handles))
      {
        char Buffer[101];
        ssize_t Size = read(STDIN_FILENO, Buffer, 100);
        assert(Size >= 0);
        Buffer[Size] = 0;
        Size = write(Input_pipe[PIPE_WRITE], Buffer, Size);
        assert(Size >= 0);
      }

      if (FD_ISSET(Output_pipe[PIPE_READ], &Read_handles))
      {
        char Buffer[101];
        ssize_t Size = read(Output_pipe[PIPE_READ], Buffer, 100);
        assert(Size >= 0);
        Buffer[Size] = 0;
        waddstr(Window_handle, Buffer);
        update_panels();
        doupdate();
//        printf("OUTPUT: %s\n", Buffer);
      }
    }

    close(Input_pipe[PIPE_WRITE]);
    close(Output_pipe[PIPE_READ]);
  }

  del_panel(Panel);
  delwin(Window_handle);
}
