#include <dash.cpp>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

string prompt = "\033[1;34mdash>\033[0m ";

string QUIT_STRING = "quit";
string EXIT_STRING = "exit";

vector<string> split(string text, char delim) {
    string line;
    vector<string> vec;
    stringstream ss(text);
    while(getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
}

int main() {
  Dash dash;
  string input;
  bool active = true;

  do {
    cout << prompt;
    getline(cin, input);

    if (input == QUIT_STRING || input == EXIT_STRING) {
      active = false;
    } else {
      vector<string> tokens = split(input, ' ');

      dash.run(tokens);
    }
  } while (active);

  return(0);
}