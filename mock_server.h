#include <string>
#include "server.h"

class MockServer : public Server {
public:
    MOCK_METHOD1(runServer, bool(const char* filename));
    MOCK_METHOD1(getConfigValue, std::string(std::string configName));
};