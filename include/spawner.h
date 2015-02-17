/*
 * Spawn.h
 *
 *  Created on: Feb 15, 2015
 *      Author: Davide A. Cucci
 *
 * derived from spawn.cpp
 * Example of communication with a subprocess via stdin/stdout
 * Author: Konstantin Tretyakov
 * License: MIT
 *
 */

#ifndef SPAWN_H_
#define SPAWN_H_

#include <ext/stdio_filebuf.h> // NB: Specific to libstdc++
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <exception>

// Wrapping pipe in a class makes sure they are closed when we leave scope
class cpipe {
private:
  int fd[2];
public:
  const inline int read_fd() const {
    return fd[0];
  }
  const inline int write_fd() const {
    return fd[1];
  }
  cpipe() {
    if (pipe(fd))
      throw std::runtime_error("Failed to create pipe");
  }
  void close() {
    ::close(fd[0]);
    ::close(fd[1]);
  }
  ~cpipe() {
    close();
  }
};

//
// Usage:
//   Spawn s(argv)
//   s.stdin << ...
//   s.stdout >> ...
//   s.send_eol()
//   s.wait()
//
class Spawner {
private:
  cpipe write_pipe;
  cpipe read_pipe;
public:
  int child_pid = -1;
  std::unique_ptr<__gnu_cxx ::stdio_filebuf<char> > write_buf = NULL;
  std::unique_ptr<__gnu_cxx ::stdio_filebuf<char> > read_buf = NULL;
  std::ostream stdin;
  std::istream stdout;

  Spawner(const char* const argv[], bool with_path = false,
      const char* const envp[] = 0);

  void send_eof();
  int wait();

};

#endif

