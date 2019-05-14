#pragma once

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include "gl.h"

class Log
{
public:
  static const bool ERROR = true;
  static const bool WARN = true;
  static const bool LOG = true;

  static void send(std::string message, std::string prefix, bool should_send)
  {
    if (should_send)
    {
      // Append new line if it doesn't exist so we flush
      std::string last_msg = message.back() == '\n' ? "" : "\n";
      std::cout << prefix << ": " << message << last_msg;
    }
  }

  static void log(std::string message)
  {
    send(message, "LOG", LOG);
  }
  static void warn(std::string message)
  {
    send(message, "WARNL", WARN);
  }
  static void error(std::string message)
  {
    send(message, "ERROR", ERROR);
  }

  static void check_error(const char *name)
  {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
      fprintf(stderr, "GL error 0x%X detected in %s\n", error, name);
    }
  }

  static void check_error(std::basic_string<char> str)
  {
    check_error(str.c_str());
  }
};

#endif