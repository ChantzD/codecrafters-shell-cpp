#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include "shell.h"
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

namespace shell {
  using std::stringstream;

  Shell::Shell(){}
  Shell::Shell(const char* path) : path(path){}

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
//ss >> input_command;

      std::string temp_arg;
      while (ss >> temp_arg){
        args.emplace_back(temp_arg);
      }
      input_command = args[0];
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
        if (!ExtCommand()){
          std::cout << input_command + ": command not found\n";
        }
        break;
      case shell::InternalCommand::TYPE:
        TypeCommand();
    }
  }

  void Shell::EchoCommand(){
    std::copy(args.begin() + 1, args.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
  }

  void Shell::TypeCommand(){
    std::string type_arg = args[1];
    if (int_command_map.contains(type_arg)){
      std::cout << type_arg + " is a shell builtin\n";
      return;
    } else {
      std::vector<std::string> path_list;
      stringstream ss(path);
      std::string temp;

      while(std::getline(ss, temp, ':')){
        path_list.emplace_back(temp);
      }

      for (const auto& path : path_list){
        std::filesystem::path full_path = std::filesystem::path(path) / type_arg;
        if (std::filesystem::exists(full_path) && std::filesystem::is_regular_file(full_path)){
          if (access(full_path.c_str(), X_OK) == 0){
            // The .string on the path is annoying and is only there to pass tests -- remove later
            std::cout << type_arg + " is " << full_path.string() << std::endl;
            return;
          }
        }
      }
    }
    std::cout << type_arg + ": not found\n";
  }

  bool Shell::ExtCommand(){
    // Basically the same as type command -- need to refactor to get rid of repeat code
    std::vector<std::string> path_list;
    stringstream ss(path);
    std::string temp;

    while(std::getline(ss, temp, ':')){
      path_list.emplace_back(temp);
    }

    bool proper_command{false};
    for (const auto& path : path_list){
      std::filesystem::path full_path = std::filesystem::path(path) / input_command;
      if (std::filesystem::exists(full_path) && std::filesystem::is_regular_file(full_path)){
        if (access(full_path.c_str(), X_OK) == 0){
          // The .string on the path is annoying and is only there to pass tests -- remove later
          proper_command = true;
        }
      }
    }
    if(!proper_command){
      return proper_command;
    }
    // convert args (vector<string>) to char* -- probably need to adjust everything to char*
    std::vector<char*> c_args;
    for (const auto& arg : args){
      c_args.emplace_back(const_cast<char*>(arg.c_str()));
    }
    c_args.emplace_back(nullptr);

    pid_t pid = fork();
    if (pid < 0){
      perror("Fork failed");
    }
    else if (pid == 0){
      if (execvp(input_command.c_str(), c_args.data()) == -1){
        perror("Execution failed");
      }
      exit(EXIT_FAILURE);
    } else {
      int status;
      waitpid(pid, &status, 0);
    }
    return proper_command;
  }
} // namespace shell

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  const char* path_env = std::getenv("PATH");

  if (path_env != nullptr) {
    auto shell = shell::Shell(path_env);
    shell.run();
  } else {
    std::cerr << "PATH not found!";
  }
}

