#!/bin/bash
# encoding: UTF-8

# . script/git.sh
source script/git.sh

arg1=$1
arg2=$2
arg3=$3


help(){
    if [[ $arg1 != '' && $arg1 != 'help' && $arg1 != 'h' && $arg1 != '-h' ]];then
        echo "  未知命令: " $arg1
    fi
    echo -e \
        "  Usage:\n"\
        "\tbash cmd.sh h, -h, help, --help          执行: help information\n"\
        "\tbash cmd.sh install                      执行: install\n"\
        "\tbash cmd.sh uninstall                    执行: uninstall\n"\
        "\tbash cmd.sh tar                          执行: generate tar.gz\n"\
        "\tbash cmd.sh lang                         执行: generate language file\n"\
        "\tbash cmd.sh ldd file_path                执行: List dynamic link libraries\n"\
        \
        "\tbash cmd.sh add [message]                执行: git add and commit, not include git push\n"\
        "\tbash cmd.sh push, add_push [message]     执行: git add, commit and push\n"\
        "\tbash cmd.sh init                         执行: git init and first commit\n"\
        "\tbash cmd.sh user [username] [email]      执行: git config user\n"\
        "\tbash cmd.sh conf_list                    执行: git config --list\n"\
        "\tbash cmd.sh reset_cache                  执行: git rm cache 更新仓库管理规则"
}

if [[ $arg1 = 'install' ]];then
    bash script/install.sh
elif [[ $arg1 = "uninstall" ]];then
    bash script/uninstall.sh
elif [[ $arg1 = 'tar' ]];then
    bash script/make_binary_tar.sh
elif [[ $arg1 = 'lang' ]];then
    bash script/language.sh
elif [[ $arg1 = 'ldd' ]];then
    bash script/ldd.sh $arg2
# git
elif [[ $arg1 == 'add' ]];then
    add "$arg2"
elif [[ $arg1 == 'add_push' || $arg1 == 'push' ]];then
    add "$arg2"
elif [[ $arg1 == 'init' ]];then
    init
elif [[ $arg1 == 'user' ]];then
    config_user $arg2 $arg3
elif [[ $arg1 == 'conf_list' ]];then
    config_list
elif [[ $arg1 == 'reset_cache' ]];then
    reset_cache
# help
elif [[ $arg1 == 'help' || $arg1 == '--help' || $arg1 == 'h' || $arg1 == '-h' ]];then
    help
else
    help
fi