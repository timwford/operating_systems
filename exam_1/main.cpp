#include <dash.cpp>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

string prompt = "\033[1;34mdash>\033[0m ";

string QUIT_STRING = "quit";
string EXIT_STRING = "exit";

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
      istringstream iss(input);
      vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

      dash.run(tokens);
    }
  } while (active);

  return(0);
}