#include "utils/json.h"


void read_config_json(std::string filename, json &config_data)
{
    std::ifstream input_file(filename);
    config_data = json::parse(input_file);
}
/**
 * 带换行，写入json文件
 */
void writeJsonFile(std::string filename, json json_data)
{
    std::ofstream config_file(filename);
    std::string json_str = json_data.dump(4);  // 将JSON对象转换为字符串，并使用indent为4个空格的缩进
    config_file << json_str;
}
