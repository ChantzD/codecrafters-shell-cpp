#include <iostream>
#include <string>
#include "shell.h"

namespace shell{
Shell::Shell(){
  int_command_map = {
    {"exit", InternalCommand::EXIT}
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
    std::cin >> this->command;
}

std::string Shell::eval(){
  InternalCommand cmd = int_command_map.count(command) ? int_command_map[command] : InternalCommand::UNKNOWN;
  switch (cmd) {
    case shell::InternalCommand::EXIT:
      exit(0);
      break;
    default:
      return command + ": command not found\n";
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  Shell shell = Shell();
  shell.run();
 }
}
