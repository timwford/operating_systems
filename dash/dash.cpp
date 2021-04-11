/*
Contains main filesystem interaction
*/

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

using namespace std;

class Dash {
public:
  int pid1;
  int pid2;
  int status;
  int waitpid;
  int fd[2];
  int who = RUSAGE_SELF;
  struct rusage usage;
  int ret;

  string COMMAND_NAME = "cmdnm";
  string PROCESS_ID = "pid";
  string SYSTEM_STATS = "systat";
  string CHANGE_DIRECTORY = "cd";
  string MEMORY_MANAGER = "memman";
  
  
  int PAGE_SIZE = 4096;

  /* 
  Runs most commands, 4 specialty, and then others just using exec
  */
  void run(vector<string> tokens) {
    if (tokens.size() < 1)
      return;

    string command = tokens[0];

    if (command == COMMAND_NAME) {
      vector<string> command;
      command.push_back("ps");
      command.push_back("-p");
      command.push_back(tokens.back());
      command.push_back("-o");
      command.push_back("command"); 

      pid1 = fork();
      if (pid1 == 0) {
        std::vector<char*> args;  
        for (auto const& token : command)
          args.push_back(const_cast<char*>(token.c_str()));

        args.push_back(nullptr);
        int error = execvp(command[0].c_str(), args.data());
        if (error == -1) {
          cout << "Command not recognized" << endl;
          return;
        }
      }
      waitpid = wait(&status);

    } else if (command == PROCESS_ID) {
      vector<string> command;
      command.push_back("pgrep");
      command.push_back(tokens.back());

      pid1 = fork();
      if (pid1 == 0) {
        std::vector<char*> args;  
        for (auto const& token : command)
          args.push_back(const_cast<char*>(token.c_str()));

        args.push_back(nullptr);
        execvp(command[0].c_str(), args.data());
      }
      waitpid = wait(&status);

    } else if (command == CHANGE_DIRECTORY) {
      int success;

      if (tokens.size() == 1) {
        success = chdir("/");
      }

      if (success != 0) {
        success = chdir(tokens[1].c_str());
      }

      if (success != 0) {
        string relativePath = workingDirectory() + tokens[1].c_str();
        success = chdir(relativePath.c_str());
      }

      if (success != 0) {
        cout << "Error changing directory" << endl;
      }
    } else if (command == SYSTEM_STATS) {

      cout << endl << "Version: " << endl;
      getInfo("/proc/version");
      cout << endl << "Uptime: " << endl;
      getInfo("/proc/uptime");
      cout << endl << "Memory info: " << endl;
      getInfo("/proc/meminfo");
      cout << endl <<"CPU info: " << endl;
      getInfo("/proc/cpuinfo");
      cout << endl;

    } else if (command == MEMORY_MANAGER) {
      memman(tokens);
    } else {
      handleCommand(tokens);
    }
  }

  private:
    /*
    Utility function to print system stuff
    */
    void getInfo(string value) {
      vector<string> command;
      command.push_back("cat");
      command.push_back(value);

      pid1 = fork();
        if (pid1 == 0) {
          std::vector<char*> args;  
          for (auto const& token : command)
            args.push_back(const_cast<char*>(token.c_str()));

          args.push_back(nullptr);
          execvp(command[0].c_str(), args.data());
        }

        waitpid = wait(&status);
    }
  
    string workingDirectory()
    {
      char temp[255];
      return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
    }

    
    void memman(vector<string> params){
      if(params.size() != 2)
        return;

      string s = params[1];
      for(int i = 0; i < s.length(); i++)
        if(!isdigit(s[i]))
            return;

      int address = stoi(params[1]);
      int page = address / PAGE_SIZE;
      int offset = address % PAGE_SIZE;

      cout << "The address " << address << " contains: " << endl << endl;
      cout << "page number = " << page << endl;
      cout << "offset = " << offset << endl << endl; 
  }

  void handleCommand(vector<string> tokens) {
    string command;
    vector<string> cmd1;
    vector<string> cmd2;

    // find if there is a pipe or redirect
    bool isPipe = false;
    bool isRedirect = false;

    if (std::find(tokens.begin(), tokens.end(), "|") != tokens.end()) isPipe = true;
    if (std::find(tokens.begin(), tokens.end(), ">") != tokens.end()) isRedirect = true;

    if (isPipe || isRedirect) {
      cout << "pipe or redirect" << endl;

      bool isCommand1 = true;
      for (auto &token: tokens) {
        if (token == "|" || token == ">") {
          isCommand1 = false;
        } else {
          if (isCommand1) {
            cmd1.push_back(token);
          } else {
            cmd2.push_back(token);
          }
        }
      }
    
      // create pipe
      if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }

      pid1 = fork();
      if (pid1 == 0) {

        // Grandchild executes command for output
        pid2 = fork();
        if (pid2 == 0) {
          close(1);
          dup(fd[1]);
          close(fd[0]);
          close(fd[0]);

          command = cmd1[0];
          char **args = new char *[cmd1.size()];
          for (size_t i = 0; i < cmd1.size(); i++) {
            args[i] = new char[cmd1[i].size() + 1];
            strcpy(args[i], cmd1[i].c_str());
          }

          int error = execvp(command.c_str(), args);
          if (error == -1) {
            cout << endl << "Command not recognized" << endl << endl;
            std::exit(-1);
          }

          exit(1);
        } else if (pid2 < 0) {
          cout << "Fork unsuccessful" << endl;
          exit(-1);
        } else {
          cout << "\033[1;32mNew child process ID: " << pid2 << "\033[0m" << endl;
        }

        // now the input side
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);

        command = cmd2[0];
        char **args = new char *[cmd2.size()];
        for (size_t i = 0; i < cmd2.size(); i++) {
          args[i] = new char[cmd2[i].size() + 1];
          strcpy(args[i], cmd2[i].c_str());
        }

        int error = execvp(command.c_str(), args);
        if (error == -1) {
          cout << endl << "Command not recognized" << endl << endl;
          std::exit(-1);
        }

        exit();

      } else if (pid1 > 0) {
        // Parent Process
        cout << "\033[1;32mNew child process ID: " << pid1 << "\033[0m" << endl;

      } else {
        cout << "error: Fork was not successful" << endl;
      }

    } else {
      // single command
      char **args = new char *[tokens.size()];
      for (size_t i = 0; i < tokens.size(); i++) {
        args[i] = new char[tokens[i].size() + 1];
        strcpy(args[i], tokens[i].c_str());
      }
      command = tokens[0];


      pid1 = fork();
      if (pid1 == 0) {   
        int error = execvp(command.c_str(), args);
        if (error == -1) {
          cout << endl << "Command not recognized" << endl << endl;
          std::exit(-1);
        }   
      } else if (pid1 > 0) {
        // Parent Process
        cout << "\033[1;32mNew child process ID: " << pid1 << "\033[0m" << endl;
      } else {
        cout << "error: Fork was not successful" << endl;
      }
    }


    waitpid = wait(&status);
    ret = getrusage(who, &usage);

    cout << "\033[1;31mUser Time: \033[0m" << usage.ru_utime.tv_sec << "."
         << usage.ru_utime.tv_usec << endl;
    cout << "\033[1;31mSystem Time: \033[0m" << usage.ru_stime.tv_sec << "."
         << usage.ru_stime.tv_usec << endl;
    cout << "\033[1;31mPage faults: " << usage.ru_minflt << "\033[0m" << endl;
    cout << "\033[1;31mSwaps: " << usage.ru_nswap << "\033[0m" << endl;
  }
};