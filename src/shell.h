#include <string>

class Shell{
  public:
    void run();
  protected:
    void read();
    std::string eval();
  private:
    std::string command;

};
