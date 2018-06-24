// stdafx.cpp : 只包括标准包含文件的源文件
// LibLuaExt.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

/**
 * \mainpage 首页
 * 这一份文档描述了LibLuaExt模块的功能和实现方式。
 * LibLuaExt被设计作为LUA脚本的导出接口。
 * 提供了以下lua模块接口：
 * - Firewall:添加/删除防火墙例外程序。
 * - KaiJiaExt：与铠甲相关的一些接口
 * - lfs：文件系统的操作。
 * - OsExt：常用的一些系统API的封装。
 * - RegIni：注册表和INI配置文件的操作封装。
 * 此文档补充于20170810。
 *
 * \section intro_sec  概述
 * 模块的功能为普通LUA扩展功能。
 *   
 * \section main_files 主要文件
 * 模块功能的实现主要依赖以下几个文件：
 * - LibLuaExt.cpp 特别注意这个文件中的LuaOpenExtendLib函数。是导入LUA接口的总入口函数。
 * - Firewall.cpp 这个文件实现了添加删除防火墙例外项的接口。
 * - KaiJiaExt.cppp 这个文件实现了铠甲操作的LUA接口。
 * - lfs.cpp 开源组件，实现了文件和目录操作。
 * - luacurl.c 开源组件，实现了对curl的封装（依赖libcurl）。
 * - OsCommon.cpp 实现了系统常用的接口。
 * - RegIni.cpp 实现了注册表和ini的接口。
 *
 * \section notes_sec 注意点
 * - Firewall模块使用COM接口实现防火墙例外项的操作，它依赖工程目录下的netfw.tlb。
 * - lfs和luacurl是开源模块，更详细的内容请自行谷歌。
 * - 其他程序使用LuaOpenExtendLib函数即可导入libluaext所有lua模块。
 * 
 * \section dependcy 依赖关系
 * - 此模块依赖于以下工程 ： libcurl
 * - 依赖此模块的工程 ： 略
 *
 * \section CONNTEXT 联系方式
 * 如果对此文档有疑问可以发送邮件作者邮箱:lanzheng_1123@163.com 
 * Duo C. Fan
 * 2016/8/23 
 */