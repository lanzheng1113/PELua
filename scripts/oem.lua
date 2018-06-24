-- 创建oem的快捷方式 --

-- 注册表删除键参数
-- 参数1 根键
-- 参数2 需要删除的路径
-- 参数3 一个boolean值，如果为true这个项下有子项或者子键则不删除，如果为false则无视是否有子键和子项直接删除项下所有键值和本项。
-- 返回一个boolean值用于标识删除是否成功。
注册表删除键 = RegIni.DeleteRegKey

-- 注册表写整数说明
-- 往注册表中写入一个DWORD。If the registry key doesn't exist it will be created.
-- 四个RegIni.WriteRegDWORD的参数依次如下：
-- 根键(string)，有效的根键"HKLM"->HKEY_LOCAL_MACHINE,"HKCU"->HKEY_CURRENT_USER ...
-- 子健路径(string)
-- 键名(string)
-- DWORD值(integer)
-- 返回值的类型为boolean成功返回true失败返回false.
-- WriteRegDWORD("HKLM","Software\\My Company\\My Software","DWORD Value",0xDEADBEEF)
注册表写整数 = RegIni.WriteRegDWORD
注册表写字符串 = RegIni.WriteRegStr

function 获取桌面目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end

function 获取开始菜单目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end

function 获取开始菜单程序目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_PROGRAMS)
end

function 注册oem()
	-- 桌面快捷方式
	桌面目录 = 获取桌面目录()
	--开始菜单
	开始菜单目录 = 获取开始菜单目录()
	--开始菜单/程序
	开始菜单程序目录 = 获取开始菜单程序目录()
	
	创建快捷方式 = OsExt.CreateShortCut
	创建目录 = OsExt.CreateDir        -- 父目录必须存在,否则会失败
	逐级创建目录 = OsExt.CreateDirRecursively -- 父目录如果不存在则一并创建
	存在路径 = OsExt.PathFileExist
	
	创建目录(开始菜单程序目录 .. "\\备份还原")
	创建目录(开始菜单程序目录 .. "\\实用工具")
	创建目录(开始菜单程序目录 .. "\\引导修复")
	
	--备份还原工具
	程序目录 = "X:\\PEPrograms\\oem"
	创建快捷方式(开始菜单程序目录 .. "\\备份还原\\装机大师.lnk", 程序目录 .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	创建快捷方式(桌面目录 .. "\\装机大师.lnk", 程序目录 .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	创建快捷方式(开始菜单目录 .. "\\装机大师.lnk", 程序目录 .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	
	-- 引导修复
	if 存在路径(程序目录 .. "\\SystemInstallTool\\tools\\BootFix.exe") then
		创建快捷方式(桌面目录 .. "\\引导修复.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\BootFix.exe")
		创建快捷方式(开始菜单目录 .. "\\引导修复.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\BootFix.exe")
		创建快捷方式(开始菜单程序目录 .. "\\引导修复\\引导修复.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\BootFix.exe")
	end
	
	-- 浏览器
	创建快捷方式(桌面目录 .. "\\浏览器.lnk", 程序目录 .. "\\TheWorld\\Application\\theworld.exe")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\浏览器.lnk", 程序目录 .. "\\TheWorld\\Application\\theworld.exe")
	
	-- GHOST
	if 存在路径(程序目录 .. "\\SystemInstallTool\\tools\\GHOST.EXE") then
		创建快捷方式(桌面目录 .. "\\手动Ghost.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\GHOST.EXE")
		创建快捷方式(开始菜单程序目录 .. "\\备份还原\\手动Ghost.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\GHOST.EXE")
	end
	
	-- EveryThing
	if 存在路径(程序目录 .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe") then
		创建快捷方式(开始菜单目录 .. "\\文件快速搜索.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe")
		创建快捷方式(开始菜单程序目录 .. "\\文件快速搜索.lnk", 程序目录 .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe")
	end
	
	-- 刷新桌面
	-- SEND #0x74
	-- SEND VK_F5
	通知系统文件关联设置已改变 = OsExt.ShellNotifyAssoChanged
	通知系统文件关联设置已改变()
	
	-- 创建进程SystemInstallTool.exe
	-- 参数1 进程路径+进程参数
	-- 参数2 显示子进程窗口：SW_SHOW；隐藏子进程窗口：SW_HIDE
	执行子进程 = OsExt.Exec
	执行子进程(程序目录 .. "\\SystemInstallTool\\SystemInstallTool.EXE", SW_SHOW)
	OsExt.Sleep(1024)
	
	-- 删除PECMD临时目录
	-- PECMD TEMP @DELETE
end

注册oem()