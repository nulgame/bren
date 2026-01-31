//
// Mariusz Pietraś @ 2025 mp@nulgame.com
//
#include <algorithm>
#include <iostream>

#include "service.hh"

#define clear_screen() printf("\033[H\033[J")

int parse_port(int argc, char **begin, const std::string &option,
               int or_use = 0)
{
    char **end = begin + argc;
    char **it = std::find(begin, end, option);
    if (it != end && ++it != end) {
        try {
            return std::stoi(*it);
        } catch (const std::invalid_argument &e) {
            return or_use;
        }
    }
    return or_use;
}

void usage()
{
    std::cout << "C/Lua based Business Rule Engine 1.0, mp@nulgame.com"
              << std::endl
              << "Allowed switch:" << std::endl
              << "  --port 16432 : runs on specified port (default 2346)"
              << std::endl
              << std::endl;
}

int main(int argc, char **argv)
{
    clear_screen();
    usage();

    int port = parse_port(argc, argv, "--port", 2346);
    const char *host = "0.0.0.0";

    try {
        Service service(port, host);
        service.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
