/**
 * \file nsisFirewall.h
 * \brief 防火墙例外列表的添加删除操作
 */
#pragma once

/**
 * \defgroup FIREWALL_OPERATION 操作防火墙例外列表
 * \{
 */

/**
 * \brief 添加程序到防火墙例外中。
 * \param ExceptionName 程序名
 * \param ProcessPath 程序进程全路径
 * \return 使用SUCCEEDED(hr)宏判断函数调用是否成功。
 */
HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName, LPCTSTR ProcessPath);

/**
 * \brief 添加本程序到防火墙例外中。
 * \param ExceptionName 程序名
 * \return 使用SUCCEEDED(hr)宏判断函数调用是否成功。
 */
HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName); // (overload) add current process

/**
 * \brief 从防火墙例外中移除程序。
 * \param 程序的全路径。
 * \return 使用SUCCEEDED(hr)宏判断函数调用是否成功。
 */
HRESULT RemoveAuthorizedApplication(LPCTSTR ProcessPath);

/**
 * \}
 */