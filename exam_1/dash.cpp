#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Dash
{
public:
  string COMMAND_NAME = "cmdnm";
  string PROCESS_ID = "pid";
  string SYSTEM_STATS = "systat";

  void run(vector<string> tokens)
  {
    if (tokens.size() < 1) return;
    string command = tokens[0];
    if (command == COMMAND_NAME) {
      cout << "command name" << endl;
    } else if (command == PROCESS_ID) {
      cout << "process id" << endl;
    } else if (command == SYSTEM_STATS) {
      cout << "statz" << endl;
    } else {
      cout << "different command" << endl;
    }
  }

private:
  int privateNum;
};