#!/bin/bash
# encoding: UTF-8
# git脚本，函数

# 执行git add commit操作, 不包括git push
add(){
    msg=$1
    if [[ $msg == '' ]];then
        msg="modify"
    fi
    echo $msg
    git add .
    git commit -m "$msg"
}
add_push(){
    add
    git push
}
# 执行git init操作
init(){
    git init
    add "first commit"
}
# 执行git_config_user操作
config_user(){
    name=$1
    email=$2
    if [[ $name == '' ]];then
        name="JiuYu77"
    fi
    if [[ $email == '' ]];then
        email="1992817771@qq.com"
    fi
    git config --global user.name $name
    git config --global user.email $email
}
# 执行git_config_list操作
config_list(){
    git config --list
}
# 执行git rm -r --cached .操作; 更新仓库管理规则
reset_cache(){
    git rm -r --cached .
    git add .
    git commit -m 'update .gitignore'
}