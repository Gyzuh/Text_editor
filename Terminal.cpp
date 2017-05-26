#include "Terminal.h"

#include <iostream>

#include <linux/input.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

terminal::terminal(void)
{
  SetupKeyboard();
}

terminal::~terminal(void)
{
  CloseKeyboard();
}

int terminal::GetWidth(void)
{
  struct winsize Size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &Size);
  return Size.ws_col;
}

int terminal::GetHeight(void)
{
  struct winsize Size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &Size);
  return Size.ws_row;
}

void terminal::Clear(void)
{
  std::cout << "\033[2J" << std::flush;
}

void terminal::SetCursor(int iColumn, int iRow)
{
  std::cout << "\033[" << iRow << ';' << iColumn << 'H' << std::flush;
}

bool terminal::GetKey(int & oKey)
{
  struct input_event Event;
  ssize_t Size = read(Keyboard, &Event, sizeof(Event));
  oKey = Event.code;
  return Size == sizeof(Event) && Event.type == EV_KEY && Event.value == 1;
}

void terminal::SetupKeyboard(void)
{
  Keyboard = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
  if (Keyboard == -1)
  {
    fputs("The keyboard device could not be opened.\n", stderr);
    exit(EXIT_FAILURE);
  }
}

void terminal::CloseKeyboard(void)
{
  close(Keyboard);
}

