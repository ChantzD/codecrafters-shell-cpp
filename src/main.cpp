#include <iostream>
#include <string>
#include <sstream>
#include "shell.h"

namespace shell {
using std::stringstream;

Shell::Shell(){
  int_command_map = {
    {"exit", InternalCommand::EXIT},
    {"echo", InternalCommand::ECHO}
  };
}

void Shell::run(){
  while(true){
    std::cout << "$ ";
    read();
    std::cout << eval();
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

std::string Shell::eval(){
  InternalCommand cmd = int_command_map.count(input_command) ? int_command_map[input_command] : InternalCommand::UNKNOWN;
  switch (cmd) {
    case shell::InternalCommand::EXIT:
      exit(0);
      break;
    case shell::InternalCommand::ECHO:
      return EchoCommand();
      break;
    // TODO: change this to UNKNOWN
    default:
      return input_command + ": command not found\n";
      break;
  }
}

std::string Shell::EchoCommand(){
  std::string total_echo;
  for (std::string arg : args){
    total_echo += arg + " ";
  }
  return total_echo + "\n";
}

} // namespace shell

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  shell::Shell shell{};
  shell.run();
 }

