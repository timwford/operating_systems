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

using namespace std;

class Dash {
public:
  int childProcessID;
  int status;
  int waitpid;
  int who = RUSAGE_SELF;
  struct rusage usage;
  int ret;

  string COMMAND_NAME = "cmdnm";
  string PROCESS_ID = "pid";
  string SYSTEM_STATS = "systat";

  void getInfo(string value) {
    vector<string> command;
    command.push_back("cat");
    command.push_back(value);

    childProcessID = fork();
      if (childProcessID == 0) {
        std::vector<char*> args;  
        for (auto const& token : command)
          args.push_back(const_cast<char*>(token.c_str()));

        args.push_back(nullptr);
        execvp(command[0].c_str(), args.data());
      }

      waitpid = wait(&status);
  }

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

      childProcessID = fork();
      if (childProcessID == 0) {
        std::vector<char*> args;  
        for (auto const& token : command)
          args.push_back(const_cast<char*>(token.c_str()));

        args.push_back(nullptr);
        execvp(command[0].c_str(), args.data());
      }

      waitpid = wait(&status);
    } else if (command == PROCESS_ID) {
      vector<string> command;
      command.push_back("pgrep");
      command.push_back(tokens.back());

      childProcessID = fork();
      if (childProcessID == 0) {
        std::vector<char*> args;  
        for (auto const& token : command)
          args.push_back(const_cast<char*>(token.c_str()));

        args.push_back(nullptr);
        execvp(command[0].c_str(), args.data());
      }

      waitpid = wait(&status);
    } else if (command == SYSTEM_STATS) {

      cout << "Version" << endl;
      getInfo("/proc/version");
      cout << "Uptime" << endl;
      getInfo("/proc/uptime");
      cout << "Memory info" << endl;
      getInfo("/proc/meminfo");
      cout << "CPU info" << endl;
      getInfo("/proc/cpuinfo");


    } else {
      char **args = new char *[tokens.size()];
      for (size_t i = 0; i < tokens.size(); i++) {
        args[i] = new char[tokens[i].size() + 1];
        strcpy(args[i], tokens[i].c_str());
      }

      childProcessID = fork();
      if (childProcessID == 0) {
        // child gets 0
        execvp(command.c_str(), args);
      } else if (childProcessID != 0) {
        // parent gets the child's process ID
        cout << "\033[1;32mNew child process ID: " << childProcessID
             << "\033[0m" << endl;
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
  }
};