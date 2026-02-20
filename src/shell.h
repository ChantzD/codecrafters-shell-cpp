#include <map>
#include <string>
#include <vector>
#include <filesystem>

namespace shell{
enum class InternalCommand {EXIT, ECHO, TYPE, UNKNOWN};

  class Shell{
    public:
      Shell();
      Shell(const char* path);
      void run();
    protected:
      void read();
      void eval(InternalCommand cmd);
      void EchoCommand();
      void TypeCommand();
      bool IsExecutable(const std::filesystem::path& path);
    private:
      std::string input_command;
      const char* path;
      std::vector<std::string> args;
      std::map<std::string, InternalCommand> int_command_map = {
    {"exit", InternalCommand::EXIT},
    {"echo", InternalCommand::ECHO},
    {"type", InternalCommand::TYPE}
  };
  };
}
