/*
// Name: Vincent Fazio and Tyler Percy
// Class: CSC 4210 
// Date: 2/18/21
// About: 

// Compile Instructions: gcc -Wall fazio_percy_character_flip.c
//                       ./a.out <text <101> >
*/

#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#define BUFFER_SIZE 255

int main(){
   char read_msg[BUFFER_SIZE];
   char write_msg[BUFFER_SIZE];
   char *file1 = "/tmp/pipe1";
   char *file2 = "/tmp/pipe2";
   mkfifo(file1, 0666);
   mkfifo(file2, 0666);

  
   pid_t pid = fork();

   //Checking for pipe failure
    if (pid < 0){
        perror("Error: Pipe Failed");
        exit(0);
    }
    //parent process, taking message from inout and writing it in parent pipe
    else if (pid > 0){
      int write_pipe, read_pipe;

      write_pipe = open(file1, O_WRONLY);
      read_pipe = open(file2, O_RDONLY);

      printf("Enter a message to be case reverse: ");
      fgets(write_msg,BUFFER_SIZE,stdin);

      write(write_pipe, write_msg, strlen(write_msg)+1);
      close(write_pipe);

      wait(0);

      read(read_pipe, read_msg, strlen(read_msg));
      printf("New MSG: %s\n",read_msg);
      close(read_pipe);

    }
    //child process, reading in parents message through its write (pipe)
    else {
      int write_pipe, read_pipe;

      read_pipe = open(file1, O_RDONLY);
      write_pipe = open(file2, O_WRONLY);

      read(read_pipe,read_msg,strlen(read_msg));
      close(read_pipe);
      //Swapping case's of message
      printf("%lu",strlen(read_msg)+1);
      for(int i = 0; i<strlen(read_msg); i++){
         if(isupper(read_msg[i])){
               read_msg[i] = tolower(read_msg[i]);
         }
         else {
               read_msg[i] = toupper(read_msg[i]);
         }
      }

      
      //write_pipe = open(file2, O_WRONLY);
      write(write_pipe, read_msg, strlen(read_msg));
      close(write_pipe);
    }
    return 0;
}