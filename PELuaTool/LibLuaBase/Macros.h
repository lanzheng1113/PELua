/**
 * \file Macros.h
 * \brief 此文件定义了一个自定义替代宏的方法 ReplacePathMacro 
 * 这个接口将一个自定义的路径例如${TEMP}转换为全路径"C:\Users\Duo\Temp"。
 *
 * 目前支持的路径包括
 * <ul>
 *    <li>INSTDIR 程序的运行目录，注意<b>不是安装目录</b></li>
 *    <li>SYSTEM32 X:\\windows\\system32</li>
 *    <li>SYSWOW64 X:\\windows\\SysWOW64</li>
 *    <li>UPDATE 程序运行目录\\UPDATE </li>
 *    <li>其它支持的宏见<a href="https://msdn.microsoft.com/en-us/library/windows/desktop/bb762494(v=vs.85).aspx">CSIDL</a>的定义</li>
 * </ul>
 * \author fanduoduo 
 * \date 2016/11/10
 */

#pragma once
#include <string>
namespace os
{
	/**
	 * \brief 将一个自定义的路径例如${TEMP}转换为全路径"C:\Users\Duo\Temp"。
	 * \param KPCmd 带宏的自定义路径。
	 * \return 返回全路径。
	 */
	std::string ReplacePathMacro( const std::string& KPCmd );
}