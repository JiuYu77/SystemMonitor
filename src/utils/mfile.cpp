#include "utils/mfile.h"
#include <pwd.h>
#include <unistd.h>


void removeFile(std::string file_path)
{
    if (remove(file_path.c_str()) != 0) { // 尝试删除文件
        std::cout << "Failed to delete file." << std::endl;
    } else {
        std::cout << "File deleted successfully." << std::endl;
    }
}
/**
 * @description: 复制文件
 * @param src {std::string} 源文件
 * @param dest {std::string} 目标文件
 * @author
 * @date
 */
void copyFile(std::string src, std::string dest)
{
    std::ifstream source_file(src);
    if(!source_file.is_open()){
        std::cout<< "open source_file faild..." << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << source_file.rdbuf();
    std::string str = buffer.str();
    std::ofstream dest_file(dest);
    if(!dest_file.is_open()){
        std::cout<< "open dest_file faild..." << std::endl;
        return;
    }
    // dest_file << str << std::endl;  // 换行符可以立即写入文件
    dest_file << str;  // 函数执行完毕才写入文件
}
/**
 * 当前有效用户的家目录
 */
std::string getHomePath()
{
    uid_t user_id = geteuid(); // 获取当前有效用户ID
    struct passwd *pw_entry = getpwuid(user_id); // 获取passwd条目
 
    if (pw_entry) {
        std::cout << "Home directory: " << pw_entry->pw_dir << std::endl;
    } else {
        std::cerr << "Error retrieving user information." << std::endl;
    }
    std::string home_path = pw_entry->pw_dir;
    // 当不再需要passwd结构体时，应释放它
    endpwent();
    return home_path;
}