#include <map>
#include <string>
#include <vector>

namespace shell{
enum class InternalCommand {EXIT, ECHO, UNKNOWN};

  class Shell{
    public:
      Shell();
      void run();
    protected:
      void read();
      void eval();
      void EchoCommand();
    private:
      std::string input_command;
      std::vector<std::string> args;
      std::map<std::string, InternalCommand> int_command_map;
  };
}
