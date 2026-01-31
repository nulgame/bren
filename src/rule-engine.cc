#include "rule-engine.hh"

#define SOL_ALL_SAFETIES_ON 1
#include <rapidjson/document.h>
#include <set>
#include <sol/sol.hpp>
#include <tuple>

// static const char* json_type_names[] = { "null", "false", "true", // 0, 1, 2
//     "object", "array", "string", "number" };  // 3, 4, 5, 6

std::string calculate_rule(const std::string &script, const std::string &json)
{
    std::string res; // result JSON

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math,
                       sol::lib::string, sol::lib::table, sol::lib::os,
                       sol::lib::package);

    // const std::string package_path = lua["package"]["path"];
    // std::cout << "SOL package path: " << package_path << std::endl;

    // Read rule.script

    // lua.script(script); // no protection call

    // Load then Parse File
    try {
        lua.safe_script(script); // read from memory
    } catch (const sol::error &e) {
        std::cout << std::string(e.what()) << std::endl;
        res = "{\"error\":-3}";
        return res;
    }

    rapidjson::Document doc;
    doc.Parse(json.c_str());

    const rapidjson::Value &variables = doc["variables"];

    // https://dev.to/shahintv/json-files-in-c-rapidjson-1841  // schema json
    // validation
    for (auto &a : variables.GetObject()) {
        // printf("Type of member %s is %s - ", a.name.GetString(),
        // json_type_names[a.value.GetType()]);

        if (a.value.GetType() == 0) { // null
            // std::cout << "value: null , LUA nil" << std::endl;
            lua.set(a.name.GetString(), sol::lua_nil); //"nil");
        } else if (a.value.GetType() == 1) {           // false
            // std::cout << "value: " <<
            // variables[a.name.GetString()].Get<bool>() << std::endl;
            // lua.set(a.name.GetString(),
            // variables[a.name.GetString()].Get<bool>());
            lua.set(a.name.GetString(), sol::lua_nil); //"nil");
            // ************************* moze 0 lepiej
        } else if (a.value.GetType() == 2) { // true
            // std::cout << "value: " <<
            // variables[a.name.GetString()].Get<bool>() << std::endl;
            lua.set(a.name.GetString(), "1");
        } else if (a.value.GetType() == 5) { // string
            // std::cout << "value: " <<
            // variables[a.name.GetString()].GetString() << std::endl;
            lua.set(a.name.GetString(),
                    variables[a.name.GetString()].GetString());
        } else if (a.value.GetType() == 6) { // number
            // std::cout << "value: " <<
            // variables[a.name.GetString()].Get<float>() << std::endl;
            lua.set(a.name.GetString(),
                    variables[a.name.GetString()].Get<float>());
        } else
            std::cerr << "skipping unexpected object or array" << std::endl;
    }

    // prepare Lua's output object
    std::set<std::tuple<std::string, std::string, std::string>> output;
    lua["output"] = &output;

    // run rule.script

    // run rule computation procedure in LUA
    // lua["make_decision"](); // no protect call

    //  Safely call function
    sol::protected_function function_object = lua["make_decision"];
    if (function_object) {
        auto x = function_object();
        if (x.valid()) {
            // std::cout << "'compute' returned x = " << x.get<float>() << "\n";
        } else {
            std::cout << "'make_decision' function FAILED!" << std::endl;
            sol::error e = x;
            std::cout << e.what() << std::endl;
        }
    }

    // DEBUG: How many object elements?
    // std::cout << "[make_decission] output.size() = " << output.size() <<
    // "\n";

    // convert set into string json
    std::string k, v, t;
    std::string builder;
    for (const auto &p : output) {
        std::tie(k, v, t) = p;
        // std::cout << "[output]  = " << k << " | " << v << " | " << t <<
        // std::endl;
        if (!t.compare("string")) {
            builder += "\"" + k + "\":\"" + v + "\",";
        } else if (!t.compare("number")) {
            builder += "\"" + k + "\":" + v + ",";
        } else if (!t.compare("bool")) {
            std::string b;
            if (!v.compare("0"))
                b = "false";
            else
                b = "true";
            builder += "\"" + k + "\":" + b + ",";
        } else if (!t.compare("null")) {
            builder += "\"" + k + "\":null,";
        }
    }

    if (!builder.empty()) {
        builder.pop_back(); // remove last ',' character
        res = "{" + builder + "}";
    } else {
        res = "{\"error\":-2}";
    }

    return res;
}
