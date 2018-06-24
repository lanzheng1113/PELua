/**
 * \file LibLuaExt.h
 * \brief 此文件定义了导入LUA模块的接口LuaOpenExtendLib，通过调用它导入以下LUA模块:
 * - lfs 文件操作拓展接口
 * - OsExt 通用拓展接口
 * - RegIni 注册表和INI文件操作
 * - curl 网络
 * - Firewall 防火墙
 * - 操纵铠甲程序退出，启动的全局库
 */

#pragma once

/**
 * \defgroup LUA_RUNTIME LUA库支持
 * \{
 */

/**
 * \brief 导入lua 脚本库
 * \param lua_State 类型的指针
 * \note 你需要使用lua_open()以及相关的lua C API创建一个LUA环境。然后你的程序在调用LuaOpenExtendLib后
 * 就可以使用接口提供的LUA模块来执行LUA脚本了。这个功能目前用于执行KJSelfFix的自修复脚本。
 */
void LuaOpenExtendLib(PVOID pLuaState);


/**
 * \}
 */
