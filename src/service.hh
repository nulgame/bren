#ifndef __SERVICE_HH
#define __SERVICE_HH

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <unordered_map>

using Request = Pistache::Rest::Request;
using Response = Pistache::Http::ResponseWriter;

class Service
{
        uint16_t m_port;
        std::string m_host;
        unsigned int m_nthreads;

        std::shared_ptr<Pistache::Http::Endpoint> m_endpoint;
        Pistache::Rest::Router m_router;
        std::unordered_map<std::string, std::string> m_rules;
        // "rule100"->json | ->lua

    public:
        Service(uint16_t port, const char *host = "0.0.0.0")
            : m_port(port),
              m_host(host),
              m_nthreads(1)
        {
        }
        ~Service() {}

        void run();

    private:
        void read_file(const std::string &rule_name,
                       const std::string &file_name);
        void read_rules();
        void configure_routes();
        void calculate(const Request &request, Response response);
};

#endif //__SERVICE_HH
