/** CPCS 351 Operating Systems - HW 2: Processes **/
// program runs terminal command provided by user
// elapsed_time is calculated using pipe() and fork()
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct timeval start_time, end_time, elapsed_time;
  const int STRUCT_TIMEEVAL_SIZE = sizeof(struct timeval);
  
  int time_pipe[2];
  if(pipe(time_pipe) == -1) {
    perror("Pipe failed");
    exit(EXIT_FAILURE);
  }
  
  pid_t pid = fork();
  if (pid < 0) {
    perror("Fork failed");
    exit(EXIT_FAILURE);
  } 
    
  else if (pid == 0) {  
    gettimeofday(&start_time, 0);
    
    // write start_time to read end of pipe (time_pipe[1])
    ssize_t write_result;
    write_result = write(time_pipe[1], &start_time, STRUCT_TIMEEVAL_SIZE);
  
    // run provided command
    execvp(argv[1], &argv[1]);
    } 
  else { 
    // parent process waits for child to complete 
    wait(nullptr);
    
    gettimeofday(&end_time, 0);
    
    // read start_time from write end of pipe (time_pipe[0])
    ssize_t read_result;
    read_result = read(time_pipe[0], &start_time, STRUCT_TIMEEVAL_SIZE );

    timersub(&end_time, &start_time, &elapsed_time);

    printf("\nElapsed time: %ld.%06ld seconds\n", elapsed_time.tv_sec,     
      elapsed_time.tv_usec);
    }

  return 0;
}