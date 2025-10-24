#include <iostream>
#include "Server.h"

int main() {
    Server server("127.0.0.1", 9001);
    server.Run();
    return 0;
}
