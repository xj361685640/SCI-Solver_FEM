
#include <signal.h>
#include <error.h>

typedef void (*signal_handler)(int);
const int NUM_SIGS=11;
static int SIGNALS[NUM_SIGS] = {SIGINT,SIGQUIT,SIGILL,SIGABRT,SIGFPE,SIGKILL,SIGSEGV,SIGTERM,SIGPIPE,SIGUSR1,SIGUSR2};

/****************************************
 * converts a signal to a string
 ****************************************/
inline const char* getSigString(int sig) {
  switch(sig)
  {
    case SIGINT:
      return "SIGINT (interrupt)";
    case SIGQUIT:
      return "SIGQUIT (quit)";
    case SIGILL:
      return "SIGILL (illegal instruction)";
    case SIGABRT:
      return "SIGABRT (abort)";
    case SIGFPE:
      return "SIGFPE (floating point exception)";
    case SIGKILL:
      return "SIGKILL (killed)";
    case SIGSEGV:
      return "SIGSEGV (segmentation violation)";
    case SIGTERM:
      return "SIGTERM (terminated)";
    case SIGPIPE:
      return "SIGPIPE (broken pipe)";
    case SIGUSR1:
      return "SIGUSR1 (user 1)";
    case SIGUSR2:
      return "SIGUSR2 (user 2)";
    default:
      return "UNKNOWN";
  }
}

/*****************************************
 * handles the signals by printing the 
 * error message, the stack, and exiting
 * where appropriate
 ****************************************/
inline void handle_signals(int sig) {
  printf("Caught signal %d - %s\n",sig,getSigString(sig));
  switch(sig) {
    case SIGINT:
    case SIGKILL:
    case SIGQUIT:
    case SIGTERM:
      //don't print stack trace since the user interrupted this one
      exit(1);
      break;     
    case SIGUSR1:  case SIGUSR2: //user defined signal to print the backtrace but continue running
      printStackTrace();
      break;
    default:
      printStackTrace();
      exit(1);
  }
}

#include <amg_signal.h>
SignalHandler::SignalHandler() {
  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_flags=0;
  action.sa_handler = handle_signals;
  for(int i=0;i<NUM_SIGS;i++)
    sigaction(SIGNALS[i],&action,NULL);
}

