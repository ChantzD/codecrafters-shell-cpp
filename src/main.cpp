#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include "shell.h"

namespace shell {
using std::stringstream;

Shell::Shell(){
  int_command_map = {
    {"exit", InternalCommand::EXIT},
    {"echo", InternalCommand::ECHO},
    {"type", InternalCommand::TYPE}
  };
}

void Shell::run(){
  while(true){
    std::cout << "$ ";
    read();
    eval(int_command_map.count(input_command) ? int_command_map[input_command] : InternalCommand::UNKNOWN);
  }
}

void Shell::read(){
  std::string temp;
  if (std::getline(std::cin, temp)){
    args.clear();
    stringstream ss(temp);
    ss >> input_command;

    std::string temp_arg;
    while (ss >> temp_arg){
      args.emplace_back(temp_arg);
    }
  }
}

void Shell::eval(InternalCommand cmd){
  switch (cmd) {
    case shell::InternalCommand::EXIT:
      exit(0);
      break;
    case shell::InternalCommand::ECHO:
      return EchoCommand();
      break;
    case shell::InternalCommand::UNKNOWN:
      std::cout << input_command + ": command not found\n";
      break;
    case shell::InternalCommand::TYPE:
      TypeCommand();
  }
}

void Shell::EchoCommand(){
  std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;
}

void Shell::TypeCommand(){
  if (int_command_map.contains(input_command)){
    std::cout << args[0] + " is a shell builtin\n";
  } else {
      std::cout << args[0] + ": not found\n";
  }
}

} // namespace shell

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  shell::Shell shell{};
  shell.run();
 }

