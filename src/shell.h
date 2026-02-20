#include <map>
#include <string>

namespace shell{
enum class InternalCommand {EXIT, UNKNOWN};

  class Shell{
    public:
      Shell();
      void run();
    protected:
      void read();
      std::string eval();
    private:
      std::string command;
      std::map<std::string, InternalCommand> int_command_map;
  };
}
