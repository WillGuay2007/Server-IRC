#include "entrypoint.h"
#include <string>

int main(int argc, char** argv)
{
    std::string address = "127.0.0.1:6667";

    if (argc > 1)
    {
        address = argv[1];
    }

    client_start(address);

    return 0;
}
