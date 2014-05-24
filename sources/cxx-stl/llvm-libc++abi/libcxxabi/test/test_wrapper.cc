#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern "C" {
int on_exit(void (*function)(int , void *), void *arg);
static void register_printers() __attribute__ ((constructor));
static void exit_status_printer(int i, void* a);
static void signal_printer(int sig);
}

static void exit_status_printer(int i, void* a) {
  if (i != 0) {
    printf("FAIL: exit status: %d\n", i);
  } else {
    printf("PASS\n");
  }
}

static void register_printers() {
  signal(SIGABRT, signal_printer);
  signal(SIGALRM, signal_printer);
  signal(SIGBUS, signal_printer);
//  signal(SIGCHLD, signal_printer);  // ignored by default
  signal(SIGCONT, signal_printer);
  signal(SIGFPE, signal_printer);
  signal(SIGHUP, signal_printer);
  signal(SIGILL, signal_printer);
  signal(SIGINT, signal_printer);
  signal(SIGKILL, signal_printer);
  signal(SIGPIPE, signal_printer);
  signal(SIGQUIT, signal_printer);
  signal(SIGSEGV, signal_printer);
  signal(SIGSTOP, signal_printer);
  signal(SIGTERM, signal_printer);
  signal(SIGTSTP, signal_printer);
  signal(SIGTTIN, signal_printer);
  signal(SIGTTOU, signal_printer);
  signal(SIGUSR1, signal_printer);
  signal(SIGUSR2, signal_printer);
  signal(SIGPROF, signal_printer);
  signal(SIGSYS, signal_printer);
  signal(SIGTRAP, signal_printer);
//  signal(SIGURG, signal_printer); // ignored by default
  signal(SIGXCPU, signal_printer);
  signal(SIGXFSZ, signal_printer);

  on_exit(exit_status_printer, 0);
}

static void signal_printer(int sig) {
  switch (sig) {
#define CASE(x) \
  case x: write(1,"FAIL: signal " #x "\n", \
         sizeof("FAIL: signal " #x "\n")); \
    break
  CASE(SIGABRT);
  CASE(SIGALRM);
  CASE(SIGBUS);
  CASE(SIGCHLD);
  CASE(SIGCONT);
  CASE(SIGFPE);
  CASE(SIGHUP);
  CASE(SIGILL);
  CASE(SIGINT);
  CASE(SIGKILL);
  CASE(SIGPIPE);
  CASE(SIGQUIT);
  CASE(SIGSEGV);
  CASE(SIGSTOP);
  CASE(SIGTERM);
  CASE(SIGTSTP);
  CASE(SIGTTIN);
  CASE(SIGTTOU);
  CASE(SIGUSR1);
  CASE(SIGUSR2);
  CASE(SIGPROF);
  CASE(SIGSYS);
  CASE(SIGTRAP);
  CASE(SIGURG);
  CASE(SIGXCPU);
  CASE(SIGXFSZ);
  default: break;
  }
}
