#include <iostream>
#include <string>
using namespace std;
class BaseCache {
public:
    virtual bool has(const std::string &key) = 0;
    virtual std::string read(const std::string &key) = 0;
    virtual void write(const std::string &key, const std::string &value) = 0;
    virtual void Delete(const std::string &key) = 0;
};
int main() {
    setlocale(LC_ALL, "");
    return 0;
}