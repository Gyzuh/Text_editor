#include "Shell_window.h"

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cerrno>

#include <panel.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>

void shell_window::Show(void)
{
  WINDOW * Window_handle = newwin(Size.Y, Size.X, 0, 0);
  PANEL * Panel = new_panel(Window_handle);

  scrollok(Window_handle, true);

  int Terminal_master = posix_openpt(O_RDWR);
  assert(Terminal_master >= 0);

  assert(grantpt(Terminal_master) == 0);
  assert(unlockpt(Terminal_master) == 0);

  int Terminal_slave = open(ptsname(Terminal_master), O_RDWR);

  if (fork())
  {
    close(Terminal_slave);

    while(true)
    {
      fd_set Read_handles;
      FD_ZERO(&Read_handles);
      FD_SET(STDIN_FILENO, &Read_handles);
      FD_SET(Terminal_master, &Read_handles);

      int Result = select(Terminal_master + 1,
                          &Read_handles, NULL, NULL, NULL);
      assert(Result >= 0);

      if (FD_ISSET(STDIN_FILENO, &Read_handles))
      {
        char Buffer[101];
        ssize_t Size = read(STDIN_FILENO, Buffer, 100);
        assert(Size >= 0);
        Buffer[Size] = 0;
        waddstr(Window_handle, Buffer);
        update_panels();
        doupdate();
        Size = write(Terminal_master, Buffer, Size);
        assert(Size >= 0);
      }

      if (FD_ISSET(Terminal_master, &Read_handles))
      {
        char Buffer[101];
        ssize_t Size = read(Terminal_master, Buffer, 100);
        assert(Size >= 0);
        Buffer[Size] = 0;
        waddstr(Window_handle, Buffer);
        update_panels();
        doupdate();
      }
    }
  }
  else
  {
    close(Terminal_master);

    struct termios Old_settings;
    struct termios New_settings;
    assert(tcgetattr(Terminal_slave, &Old_settings) == 0);
    New_settings = Old_settings;
    cfmakeraw(&New_settings);
    assert(tcsetattr(Terminal_slave, TCSANOW, &New_settings) == 0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    assert(dup(Terminal_slave) == STDIN_FILENO);
    assert(dup(Terminal_slave) == STDOUT_FILENO);
    assert(dup(Terminal_slave) == STDERR_FILENO);

    close(Terminal_slave);

    setsid();

    ioctl(0, TIOCSCTTY, 1);

    execl("/bin/bash", "/bin/bash", (char *) NULL);
  }

  scrollok(Window_handle, false);

  del_panel(Panel);
  delwin(Window_handle);
}
