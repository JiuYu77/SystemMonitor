#ifndef JSON_H
#define JSON_H

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// 读取json配置文件
void read_config_json(std::string filename, json &config_data);
// 写入json文件, 带换行
void writeJsonFile(std::string filename, json json_data);


// std::ifstream input_file("../config/config.json");
// json config_data = json::parse(input_file);
// std::string cpp_string = config_data["windowIcon"];
// std::string str = config_data.dump();
// std::cout << "Name: " << cpp_string << std::endl;
// std::cout << "Name: " << str << std::endl;
// std::cout << "Name: " << config_data["toolIcon"] << std::endl;

#endif // JSON_H
