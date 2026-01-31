// #define RAPIDJSON_NOMEMBERITERATORCLASS 1

#include "service.hh"
#include "rule-engine.hh"
#include "string-utils.hh"
#include <exception>
// #include <filesystem>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <stdexcept>

using namespace Pistache;
using Response = Http::ResponseWriter;
namespace fs = std::filesystem;

bool match(const char *pat, const char *str)
{
    switch (*pat) {
        case '\0':
            return !*str;
        case '*':
            return match(pat + 1, str) || (*str && match(pat, str + 1));
        case '?':
            return *str && match(pat + 1, str + 1);
        default:
            return *pat == *str && match(pat + 1, str + 1);
    }
}

void Service::read_file(const std::string &rule_name,
                        const std::string &file_name)
{
    std::ifstream ifs(file_name.c_str(),
                      std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type file_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(file_size);
    ifs.read(&bytes[0], file_size);
    m_rules[rule_name] = string(&bytes[0], file_size);
}

void Service::read_rules()
{
    const char *pattern = "*.lua";
    std::string path = "rules/";
    for (const auto &entry : fs::directory_iterator(path)) {

        const char *f = entry.path().c_str(); // bez " "
        if (match(pattern, f)) {
            std::string rule_file = get_token_behind_last_delimiter(f, "/");
            std::string rule = get_token_before_delimiter(rule_file, ".");
            std::cout << "matched: " << f << " (" << rule << ")" << std::endl;
            read_file(rule, f);
        }
    }
}

void Service::run()
{
    std::cout << "Svc running on port: " << m_port << std::endl;

    try {
        read_rules();

        Address svc_address(m_host, m_port);
        m_endpoint = std::make_shared<Http::Endpoint>(svc_address);
        auto opts = Http::Endpoint::options().threads(m_nthreads);
        opts.flags(Tcp::Options::ReuseAddr);
        opts.sslHandshakeTimeout(std::chrono::milliseconds(1000));
        m_endpoint->init(opts);
        configure_routes();
        m_endpoint->setHandler(m_router.handler());
        m_endpoint->serve();
    } catch (const std::runtime_error &e) {
        std::cerr << "Run srv: " << e.what() << std::endl;
    }
}

void Service::configure_routes()
{
    Rest::Routes::Post(m_router, "/api/v1/calculate-rule",
                       Rest::Routes::bind(&Service::calculate, this));
}

void Service::calculate(const Request &request, Response response)
{
    try {
        const std::string json = request.body();
        rapidjson::Document doc;
        doc.Parse(json.c_str());

        if (doc.HasParseError()) {
            std::cerr << "Parse Error at offset " << doc.GetErrorOffset()
                      << ". "
                      << rapidjson::GetParseError_En(doc.GetParseError())
                      << std::endl;
            throw std::runtime_error("ERROR PARSING JSON");
        }

        if (!doc.HasMember("rule")) {
            throw std::runtime_error("ERROR MISSING OBJECT MEMBER: 'rule'");
        }

        if (!doc["rule"].IsString()) {
            throw std::runtime_error(
                "ERROR OBJECT MEMBER 'rule' IS NOT STRING");
        }

        if (!doc.HasMember("variables")) {
            throw std::runtime_error(
                "ERROR MISSING OBJECT MEMBER: 'variables'");
        }

        if (!doc["variables"].IsObject()) {
            throw std::runtime_error(
                "ERROR OBJECT MEMBER 'variables' IS NOT OBJECT");
        }

        // OK but not required for one shot only
        // auto getString = [&doc](const char *key) { return
        // doc[key].GetString(); }; std::string rule = getString("rule");
        // version = doc["version"].GetInt();

        std::string rule = doc["rule"].GetString();

        bool exists = (m_rules.find(rule) != m_rules.end());
        std::string res;
        if (exists) {
            const std::string script = m_rules.find(rule)->second;

            res = calculate_rule(script, json);
        }

        response.send(Http::Code::Ok, res, MIME(Text, Json));

    } catch (std::exception const &e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
        response.send(Http::Code::Bad_Request, "{ \"error\":-1}",
                      MIME(Text, Json));
    } catch (...) {
        response.send(Http::Code::Internal_Server_Error, "Internal error",
                      MIME(Text, Plain));
    }
}
