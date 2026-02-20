#include <iostream>
#include <string>
#include "shell.h"

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
  return command + ": command not found";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  Shell shell = Shell();
  shell.run();
 }

