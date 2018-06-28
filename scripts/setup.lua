-- setup --
-- 一些常量定义 --
IDOK=1
IDCANCEL=2
IDABORT=3
IDRETRY=4
IDIGNORE=5
IDYES=6
IDNO=7
MB_YESNO = 4
-- 定义NULL --
NULL = 0
-- 创建子进程时显示/隐藏子进程窗口
SW_SHOW = 5
SW_HIDE = 0
-- 几个特殊目录的ID --
CSIDL_COMMON_STARTMENU=0x0016			-- All Users\Start Menu
CSIDL_COMMON_PROGRAMS=0X0017			-- All Users\Start Menu\Programs
CSIDL_COMMON_STARTUP=0x0018				-- All Users\Startup
CSIDL_COMMON_DESKTOPDIRECTORY=0x0019	-- All Users\Desktop
-- 文件夹隐藏属性 --
FILE_ATTRIBUTE_HIDDEN = 0x02
-- 显示在桌面上的文本颜色和字体 --
RGB红色 = 0xFF0000
字体 = "Microsoft YaHei"
字体大小 = 20

-- 导出方法别名设置 --
存在路径 = OsExt.PathFileExist
设置LOGO = PEExt.SetLogo
设置墙纸 = PEExt.SetDesktopWallPaper
取屏幕大小 = PEExt.GetDesktopSize
执行子进程并等待它完成 = OsExt.ExecWait

--
-- ExecGetStdout 执行子进程并取标准输出
-- 参数1 子进程的命令行
-- 参数2 指定是否显示子进程的窗口。SW_HIDE/SW_SHOW
-- 返回两个值。第一个是进程退出码，如果进程执行失败了那么返回255。第二个返回值是子进程的标准输出
--
执行子进程并取标准输出 = OsExt.ExecGetStdout

--
-- OsExt.CreateDir创建一个目录，父目录必须存在,否则会失败
-- 参数1 需要创建目录的全路径
-- 返回一个boolean值代表成功或者失败
--
创建目录 = OsExt.CreateDir

-- 
-- PEExt.INIT 初始化PE，同PECMD的INIT I。去掉了PECMD INIT命令的各种参数
-- 无参数
-- 返回一个boolean值代表成功或者失败
-- 
初始化PE = PEExt.INIT

--
-- PEExt.SetEnvironment 设置环境变量
-- 参数1 环境变量名
-- 参数2 环境变量对应的值
-- 参数3 是否为PE系统全局环境变量，如果为TRUE则整个系统生效(ENVI $XXX=OOO),为FALSE则只对当前程序生效(ENVI #xxx=ooo)。
--
设置环境变量 = PEExt.SetEnvironment

--
-- PEExt.InitEnvironmentVariable
-- 设置以下环境变量：Favorites,Desktop,StartMenu,Startup,Programs,SendTo,Personal,QuickLaunch
-- 无参数，返回boolean类型
--
初始化系统环境变量 = PEExt.InitEnvironmentVariable

--
-- 杀死参数所指定的所有进程
-- 参数1 进程名(xxx.exe)
--
杀死进程=OsExt.KillProcessByName

--
-- 等待一段时间
-- 参数1 等待的毫秒数
--
等待=OsExt.Sleep

-- 停止一个服务
-- 参数1 服务名
关闭服务=OsExt.StopService

-- 参数1 进程路径+进程参数
-- 参数2 显示子进程窗口：SW_SHOW；隐藏子进程窗口：SW_HIDE
执行子进程 = OsExt.Exec

-- 参数列表： 文本，颜色，字体，字体大小，X坐标，Y坐标
-- 如果X,Y坐标都设置为-1，则表示置顶居中显示
写桌面文本 = PEExt.SetDesktopText

注册表写整数 = RegIni.WriteRegDWORD
注册表写字符串 = RegIni.WriteRegStr
注册表读文本 = RegIni.ReadRegStr
注册表读整数 = RegIni.ReadRegDWORD
注册表删除子项 = RegIni.DeleteRegKey
注册表删除键值 = RegIni.DeleteRegValue

--
-- RegIni.ReadRegMutiStr返回一个table,其中每一项都是一个字符串。
--
注册表读MULTI_SZ = RegIni.ReadRegMutiStr

--
-- 删除一个文件，如果被占用则重命名并标记为重启后删除。
--
删除文件 = OsExt.DeepDeleteFile

--
-- 递归删除一个目录下的所有文件和子目录
--
删除目录 = OsExt.RemoveDirectoryRecursively

--
-- 加载显示U盘分区，未实现
--
加载显示U盘分区 = PEExt.ShowUSBDisk

-- 
-- 展开一个带有环境变量的字符串
--
字符串_展开环境变量 = OsExt.ExpandStr

--
-- 创建一个文件的快捷方式
-- 参数1 快捷方式全路径
-- 参数2 源文件全路径
--
创建快捷方式 = OsExt.CreateShortCut

设置文件或文件夹属性 = OsExt.SetFileAttributes

ProgramFiles目录 = "X:\\Program Files"

--
-- 取环境变量 如os.getenv("windir") 等于 %windir%
-- 当为空时返回空字符串，不返回nil以防止连接字符串时发生错误
--
function 获取环境变量(pstrEnv)
	local value = os.getenv(pstrEnv)
	if nil == value then
		return ""
	else
		return value
	end
end

function 获取桌面目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end


function 获取开始菜单目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end


function 获取开始菜单程序目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_PROGRAMS)
end

--
-- 在字符串中反向查找一个标记，返回该标记之前的内容
-- 如: reverse_find("abc/def","/") == "abc"
-- 
function reverse_find(str, token)
	rs = string.reverse(str)
	pBeg, pEnd = string.find(rs,token)
	if nil == pBeg then
		-- 如果没有找到标记，那么返回整个原字符串
		return str
	end
	m = string.len(rs) - pEnd + 1
	return string.sub(str,1,m) 
end


function 获取文件名(文件全路径, 是否保留扩展名)
	-- 取得扩展名的文件名，如x:\\abc.7z ==> abc.7z
	local name_with_ext = OsExt.GetFileName(文件全路径)
	if name_with_ext == nil or name_with_ext == "" then
		return nil
	end
	
	if 是否保留扩展名 then
		return name_with_ext
	else
		-- 取得不带拓展名的文件名, 如abc.7z ==> abc
		local 文件名 = reverse_find(name_with_ext,".")
		return 文件名
	end
end


--
-- 取得devcon.exe获取的设备数量
--
function hwids()
	local 执行是否成功, 输出内容 = 执行子进程并取标准输出("devcon.exe findall *", SW_HIDE)
	-- 执行后内容最后一行是:"165 matching device(s) found",取得这个个数
	_,_,cnt = string.find(输出内容, "(%d+) matching device(s) found.")
	if cnt == nil then
		Log.info("devcon.exe findall没有找到任何内容")
		return 0
	else
		Log.info("devcon.exe findall找到的数量为" .. tostring(cnt))
		return cnt
	end
end


function 安装驱动(驱动包路径, 安装位置)
	--[[
	_SUB Insdriver
		NAME &&fname=%~1	--返回指定文件名的基本名(无目录、无扩展名) ；-ext则保留扩展名。
		FIND [ $custom=%&fname% | $custom_64=%&fname% ], CALL hwids &&ID1
		FIND# $Z=Z%2, EXEC =!%programfiles%\7-zip\7z.exe x %1 -o%temp%\pe-driver\%&fname% # EXEC =!%programfiles%\7-zip\7z.exe x %1 -o%2 
		FIND# $Z=Z%2, EXEC =!dpinst.exe /S /Path %temp%\pe-driver\%&fname% # EXEC =!dpinst.exe /S /Path %2
		WAIT 500
		FIND [ $custom=%&fname% | $custom_64=%&fname% ], CALL hwids &&ID2
		FIND |%&ID1%<%&ID2%, EXEC -hide -wait dpinst.exe /S /Path %temp%\pe-driver\%&fname%
		FIND# $DRIVERS_USB=%&fname%, EXEC =!dpinst.exe /S /Path %temp%\pe-driver\%&fname%
		FIND# $Z=Z%2, FILE %temp%\pe-driver
	_END
	]]
	if not 存在路径(驱动包路径) then
		return false
	end

	--
	-- install
	--
	local fname = 获取文件名(驱动包路径, false)
	if nil == fname or "" == fname then
		return false
	end
	
	-- 如果是custom*.7z或者是custom_64*.7z安装驱动前先保存一下现有的设备数
	local ID1, ID2
	if nil ~= string.find(fname,"custom") or nil ~= string.find(fname,"custom_64") then
		ID1 = hwids()
	end
	
	-- 如果没有指定要将驱动安装到哪里，那么将驱动安装到临时目录
	local inst_to_path = nil
	if nil == 安装位置 or "" == 安装位置 then
		inst_to_path = 获取环境变量("temp") .. "\\pe-driver\\" .. fname
	else
		inst_to_path = 安装位置
	end
	
	local path_7z = ProgramFiles目录 .. "\\7-zip\\7z.exe"
	local exec_cmd = path_7z .. " x " .. 驱动包路径 .. " -o" .. inst_to_path
	
	执行子进程并等待它完成(exec_cmd, SW_HIDE)
	exec_cmd = "cmd.exe /c dpinst.exe /S /Path " .. inst_to_path
	执行子进程并等待它完成(exec_cmd, SW_HIDE)
	OsExt.Sleep(500)
	
	-- 如果是custom*.7z或者是custom_64*.7z安装驱动后在临时目录尝试重新安装一次
	if nil ~= string.find(fname,"custom") or nil ~= string.find(fname,"custom_64") then
		ID2 = hwids()
		if ID1 ~= nil and ID2 ~=nil and ID1 < ID2 then
			执行子进程并等待它完成("cmd.exe /c dpinst.exe /S /Path " .. 获取环境变量("temp") .. "\\pe-driver\\" .. fname)
		end
	end
	
	-- 如果是DRIVERS_USB*.7z，安装驱动后在临时目录尝试重新安装一次。
	if nil ~= string.find(fname,"DRIVERS_USB") then
		执行子进程并等待它完成("cmd.exe /c dpinst.exe /S /Path " .. 获取环境变量("temp") .. "\\pe-driver\\" .. fname)
	end
	
	-- 如果是在临时目录中安装则在安装后删除%temp%\\pe-driver
	if nil == 安装位置 or "" == 安装位置 then
		删除目录(获取环境变量("temp") .. "\\pe-driver")
	end
	return true
end


function 修改WinNTSetup文件(tag)
	if not 存在路径("X:\\Program Files\\WinNTSetup") then
		创建目录("X:\\Program Files\\WinNTSetup")
	end
	--WRIT X:\Program Files\WinNTSetup\WinNTSetup.ini,1,[WinNT6]
    --WRIT X:\Program Files\WinNTSetup\WinNTSetup.ini,$2,Drivers=X:\raid\%1
	local filew=io.output("X:\\Program Files\\WinNTSetup\\WinNTSetup.ini")
	str = "[WinNT6]\nDrivers=X:\\raid\\" .. tag .. "\n"
	io.write(str)
	io.flush()
	io.close()
end


function 安装SATA驱动()
	local the_7z = ProgramFiles目录 .. "\\DRIVERS_RAID.7z"
	local target = 获取环境变量("SystemDrive") .. "\\raid"
	if not 安装驱动(the_7z, target) then
		return false
	end
	执行结果, 查找CC_0104设备结果 = 执行子进程并取标准输出("cmd.exe /c devcon status *CC_0104", SW_HIDE)
	if nil ~= string.find(查找CC_0104设备结果, "VEN_1022&DEV_7805", 1) then
		修改WinNTSetup文件("amd")
	end
	if nil ~= string.find(查找CC_0104设备结果, "VEN_8086&DEV_A", 1) then
		修改WinNTSetup文件("iaStorAC")
	end
	if nil ~= string.find(查找CC_0104设备结果, "VEN_8086&DEV_9") then
		修改WinNTSetup文件("iaStorAC")
	end
	return true
end


function 安装SATA驱动_VEN_8086()
	--[[
	exec* &&smbus=!devcon status PCI\VEN_8086*DEV_A123 PCI\VEN_8086*DEV_9D23 PCI\VEN_8086*DEV_A2A3
    SED &&U=?,VEN_8086,,%&smbus%
    ifex $0=%&U%, exit _sub
    CALL Insdriver "%ProgramFiles%\DRIVERS_RAID.7z" %SystemDrive%\raid
    call wini iaStorAC
	]]--
	执行结果, smbus = 执行子进程并取标准输出("cmd.exe /c devcon status PCI\\VEN_8086*DEV_A123 PCI\\VEN_8086*DEV_9D23 PCI\\VEN_8086*DEV_A2A3", SW_HIDE)
	if nil == string.find(smbus, "VEN_8086", 1) then
		return
	end
	local the_7z = ProgramFiles目录 .. "\\DRIVERS_RAID.7z"
	local target = 获取环境变量("SystemDrive") .. "\\raid"
	安装驱动(the_7z, target)
	修改WinNTSetup文件("iaStorAC")
end


function sapple()
	local the_7z = ProgramFiles目录 .. "\\DRIVERS_RAID.7z"
	local target = 获取环境变量("SystemDrive") .. "\\raid"
	if not 安装驱动(the_7z,target) then
		return false
	end
	执行结果, 子进程标准输出 = 执行子进程并取标准输出("cmd.exe /c devcon status PCI\\VEN_106B*CC_0180")
	if nil ~= string.find(子进程标准输出, "Name", 1) then
		修改WinNTSetup文件("AppleSSD64")
	end
end


function 安装自定义驱动(path)
	写桌面文本("加载自定义驱动.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
	OsExt.Sleep(500)
	local Drv = string.sub(path, 1, 1)
	-- 如果存在默认的安装包就安装默认的，否则根据找到的第一个安装。
	local default = drv .. ":\\WifiDriver\\custom_64.7z"
	if 存在路径(default) then
		安装驱动(default)
	else
		安装驱动(path)
	end
end


function 加载有线网卡驱动()
	写桌面文本("加载有线网卡驱动.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
	OsExt.Sleep(500)
	安装驱动(ProgramFiles目录 .. "\\DRIVERS_NET.7z")
end


function 启动无线服务()
	--[[
	TEAM TEXT 启动无线服务..... #LTRB + $20:Microsoft YaHei UI|WAIT 500
	EXEC =!net start Wlansvc
	WAIT 100
	forx !\WifiDriver\*.xml,&&con,1,set &&exi=%&con%
	find $Z=Z%&exi%, exit _sub
	FDRV &&fDrive=%&exi%
	TEAM TEXT 加载无线连接配置.....#LTRB + $20:Microsoft YaHei UI|WAIT 500
	forx %&fdrive%\WifiDriver\*.xml,&&con,0,exec !cmd.exe /c netsh wlan add profile filename=%&con%
	]]
	写桌面文本("启动无线服务.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
	OsExt.Sleep(500)
	执行子进程并等待它完成("cmd.exe /c net start Wlansvc")
	OsExt.Sleep(100)
	local WiFi配置文件存在 = false
	local Wifi驱动目录 = nil
	local 盘符_Z = string.byte("Z",1)
	for i = 0, 23 do
		local 盘符 = 盘符_Z - i
		Wifi驱动目录 = string.char(盘符) .. ":\\WifiDriver"
		if 存在路径(Wifi驱动目录) then
			-- 已集成lfs --
			for file in lfs.dir(Wifi驱动目录) do
				if file ~= "." and file ~= ".." then
					local full_path = Wifi驱动目录 .. "\\" .. file
					local attr = lfs.attributes(full_path)
					if attr.mode == "file" then
						-- 寻找*.xml
						if string_ends_with(file, ".xml") then
							WiFi配置文件存在 = true
							break
						end
					end
				end
			end
		end
	end
	-- 加载?:\WifiDriver目录下的所有xml配置
	if WiFi配置文件存在 then
		写桌面文本("加载无线连接配置.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
		for file in lfs.dir(Wifi驱动目录) do
			if file ~= "." and file ~= ".." then
				local full_path = Wifi驱动目录 .. "\\" .. file
				local attr = lfs.attributes(full_path)
				if attr.mode == "file" then
					if string_ends_with(file, ".xml") then
						执行子进程并等待它完成("cmd.exe /c netsh wlan add profile filename=\"" .. full_path .. "\"" )
					end
				end
			end
		end
	end
end


function 安装和启动无线网卡驱动()
	--[[
	 find $3=%&bx64%, envi &&wei=_64! envi &&wei=
     IFEX X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%.7z,! TEAM LINK %desktop%\搜索安装无线驱动,%&myname%,wifi.wcs,netshell.dll,157,,%programfiles% |exit _sub
     TEAM TEXT 加载无线网卡驱动..... #LTRB + $20:Microsoft YaHei UI|WAIT 500
     CALL Insdriver X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%.7z
	 CALL Insdriver X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%_2.7z
     call sload
	]]
	local 无线驱动包路径1 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS.7z"
	local 无线驱动包路径2 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_2.7z"
	if OsExt.IsWow64() then
		无线驱动包路径1 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_64.7z"
		无线驱动包路径2 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_64_2.7z"
	end
	local 桌面目录 = 获取桌面目录()
	if 存在路径(无线驱动包路径1) then
		-- 创建快捷方式 LINK %desktop%\搜索安装无线驱动,%&myname%,wifi.wcs,netshell.dll,157,,%programfiles%
		OsExt.CreateShortCutEx(桌面目录 .. "\\搜索安装无线驱动.lnk", "X:\\Windows\\System32\\pecmd.exe", "wifi.wcs", "netshell.dll", 157, ProgramFiles目录)
		写桌面文本("加载无线网卡驱动.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
		安装驱动(无线驱动包路径1)
		return
	end
	if 存在路径(无线驱动包路径2) then
		安装驱动(无线驱动包路径2)
	end
	启动无线服务()
end


function 重新安装rtux64w10()
	OsExt.DeepCopyFile(getenv("windir") .. "\\inf\\rtux64w10.inf", getenv("temp") .. "\\rtux64w10.inf")
	OsExt.DeepCopyFile(getenv("windir") .. "\\inf\\rtux64w10.sys", getenv("temp") .. "\\rtux64w10.sys")
	执行子进程并等待它完成("cmd.exe /c drvload " .. getenv("temp") .. "\\rtux64w10.inf")
	删除文件(getenv("temp") .. "\\rtux64w10.inf")
	删除文件(getenv("temp") .. "\\rtux64w10.sys")
end


function string_ends_with(str,substr)
	return string.find(str,substr,1) == string.len(str) - string.len(substr) + 1
end


--
-- 安装X:\\Windows\\inf\\OEM*.INF
--
function 安装OEM_INF()
	local count = 0
	local OEM_INF目录 = "X:\\Windows\\inf"
	if 存在路径(OEM_INF目录) then
		-- 已集成lfs --
		for file in lfs.dir(OEM_INF目录) do
			if file ~= "." and file ~= ".." then
				local full_path = OEM_INF目录 .. "\\" .. file
				local attr = lfs.attributes(full_path)
				if attr.mode == "file" then
					-- 寻找oem*.inf 
					if 1 == string.find(file, "oem") and string_ends_with(file, ".inf") then
						Log.info("找到了OEM_INF:" .. file)
						orz = 注册表读MULTI_SZ("HKLM","DRIVERS\\DriverDatabase\\DriverInfFiles\\",file)
						if table.getn(orz) == 0 then
							Log.info("安装OEM驱动:" .. file)
							执行子进程并等待它完成("cmd.exe /c drvload " .. full_path)
						end
						count = count + 1
					end
				end
			end
		end
	end
	return count
end


function 设置系统DPI(p_dpi)
	Log.info("设置系统DPI=" .. tostring(p_dpi))
	注册表删除子项("HKCU","Software\\Microsoft\\Windows NT\\CurrentVersion\\TaskManager\\Preferences",false)
	注册表删除键值("HKCU","Software\\Microsoft\\Windows NT\\CurrentVersion\\TaskManager","Preferences")
	if p_dpi > 0x60 and p_dpi < 0x84 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "600")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","32")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","新宋体,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","新宋体,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","新宋体,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Buttonbar","Buttonheight","33")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Buttonbar","SmallIcons","0")
	elseif p_dpi >  0x84 and p_dpi < 0xa8 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "750")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","32")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","新宋体,-24,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","新宋体,-24,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","新宋体,-24,134")
		RegIni.WriteINIStr("X:\\Windows\\tlb\\toolbars\\start\\setup.ini","settings","menuItemMarginY","12")
	elseif p_dpi > 0xa8 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "920")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","48")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","新宋体,-32,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","新宋体,-32,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","新宋体,-32,134")
	end
end


function 执行远程启动()
	Log.info("执行远程启动")
	ServerAddr = 注册表读文本("HKLM","SYSTEM\\CurrentControlSet\\Control","PEBootServerAddr")
	注册表写字符串("HKCU","Software\\TightVNC\\Control\\ReverseConnectionHistory","0",ServerAddr)
	执行子进程并等待它完成("cmd.exe /c tftp -i %&ServerAddr% get pxeautorun.txt %windir%\\system32\\pxeautorun.cmd",SW_HIDE)
	windir = 获取环境变量("windir")
	if 存在路径(windir .. "\\system32\\pxeautorun.cmd") then
		执行子进程并等待它完成("cmd.exe /c pxeautorun.cmd " .. ServerAddr)
		return true
	end
	return false
end


function setup()

	--
    -- 仅用于32位的程序
	-- 如果是64位系统则禁用WOW64文件重定向和禁用注册表重定向 --
	--if OsExt.IsWow64() then
	--	OsExt.DisableWow64FsRedir()
	--	RegIni.SetRegView(64)
	--end
	--
	
	-- 设置日志路径 --
	Log.SetLogFile("X:\\Windows\\PESetup.log")
	Log.info("开始执行setup脚本")
	-- 设置桌面背景 --
	盘符_Z = string.byte("Z",1)
	for i = 0, 23 do
		盘符 = 盘符_Z - i
		背景图片路径 = string.char(盘符) .. ":\\boot\\wall\\winpe.jpg"
		if 存在路径(背景图片路径) then
			local backGroundColor = 0
			设置LOGO(背景图片路径,背景颜色)
			break
		end
	end
	sum = 0
	屏幕宽,屏幕高 = 取屏幕大小()
	if 屏幕高 > 屏幕宽 then
		-- "X:\Program Files\Rotate\rotate.exe" C
		执行子进程并等待它完成("\"" .. ProgramFiles目录 .. "\\Rotate\\rotate.exe\" C" , SW_HIDE)
	end
	-- 
	RGB红色 = 0x00FF0000
	字体 = "Microsoft YaHei"
	字体大小 = 20
	写桌面文本("正在初始化....",RGB红色,字体,字体大小,-1,-1, -1, -1)
	
	-- 初始化SHELL
	初始化PE()
	-- 调用命令执行其他初始化
	执行子进程并等待它完成("cmd.exe /c X:\\Windows\\system32\\startnet.cmd", SW_HIDE)
	设置环境变量("APPDATA","X:\\Users\\Default\\AppData\\Roaming",true)
	初始化系统环境变量()
	-- 启动系统外壳(类似Explorer.exe)
	PEExt.SHELL("X:\\Windows\\WinXShell.exe")
	OsExt.Sleep(1000)
	while not OsExt.GetProcessId("WinXShell.exe") do
		OsExt.Sleep(500)
	end
	--执行子进程("WinXShell.exe",SW_SHOW)
	-- 设置SHELL风格
	执行子进程并等待它完成("cmd.exe /c regedit /s \"" .. ProgramFiles目录 .. "\\Classic Shell\\cs.reg\"", SW_HIDE)
	-- 隐藏回收站,网络
	注册表写字符串("HKLM","Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel","{645FF040-5081-101B-9F08-00AA002F954E}","1")
	注册表写字符串("HKLM","Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel","{F02C1A0D-BE21-4350-88B0-7367FC96EF3B}","1")
	-- 开始菜单收藏夹隐藏
	注册表写字符串("HKCU","Software\\IvoSoft\\ClassicStartMenu\\Settings", "Favorites", "Hide")
	-- 
	执行子进程并等待它完成( "cmd.exe /c regsvr32 /s " .. "\"" .. ProgramFiles目录 .. "\\Classic Shell\\StartMenuHelper64.dll\"", SW_HIDE)
	执行子进程("\"" .. ProgramFiles目录 .. "\\Classic Shell\\ClassicStartMenu.exe\"", SW_HIDE)
	local iRet = 执行子进程并等待它完成("X:\\Windows\\System32\\winpeshl.exe", SW_SHOW) 
	if iRet ~= 0 then
		Log.error("=====================================================")
		Log.error("         执行winpeshl.exe失败,错误id=" .. tostring(iRet))
		Log.error("=====================================================")
	end
	OsExt.ApplyEnvironmentVarsChange()
	Public目录 = 获取环境变量("PUBLIC")
	if Public目录 ~= "" then
		删除文件(Public目录 .. "\\desktop\\desktop.ini")
	end
	-- 检查、安装USB驱动
	执行结果,子进程标准输出 = 执行子进程并取标准输出("cmd.exe /c devcon status PCI\\VEN_1B73*DEV_1* PCI\\VEN_1106*DEV_9202 PCI\\VEN_1106*DEV_9201 PCI\\VEN_1106*DEV_3432 PCI\\VEN_1106*DEV_3483 PCI\\VEN_1106*DEV_3515",SW_HIDE)
	if nil ~= string.find(子进程标准输出,"Name",1) then
		安装驱动(ProgramFiles目录 .. "\\DRIVERS_USB.7z")
	end
	-- 使用ShowDrivs_Gui_x64寻找和加载UDM分区
	-- 命令ShowDrivs_Gui_x64.exe MOUN-udm -findboot -w \\.\PhysicalDrive* C-
	执行子进程并等待它完成("\"" .. ProgramFiles目录 .. "\\ShowDrivs_Gui_x64.exe\"" .." MOUN-udm -findboot -w \\\\.\\PhysicalDrive* C-", SW_HIDE)
	-- 
	-- 如果是系统启动选项带UDMBOOT，则尝试加载和显示所有未加载的U盘或USB移动硬盘分区
	-- 
	SystemStartOpt = 注册表读文本("HKLM","SYSTEM\\CurrentControlSet\\Control","SystemStartOptions")
	if nil == string.find(SystemStartOpt,"UDMBOOT",1) then
		-- 对应PECMD的（SHOW U:-1）命令
		加载显示U盘分区()
	end
	
	----------------------------------------------------------------- 
	-- 安装SATA驱动
	-- PCI\*CC_0104 : SATA RAID 驱动
	-- PCI\VEN_1022&DEV_7803 : AMD AHCI Compatible RAID Controller
	-- PCI\VEN_1B21*DEV_06* : ASM1061 SATA IDE Controller(祥硕科技)
	--
	执行结果,SATA设备查询结果 = 执行子进程并取标准输出("cmd.exe /c devcon status *CC_0104 PCI\\VEN_1022*DEV_7803 PCI\\VEN_1B21*DEV_06*", SW_HIDE)
	if nil ~= string.find(SATA设备查询结果, "problem", 1) then
		安装SATA驱动()
	end
	if nil ~= string.find(SATA设备查询结果, "running", 1) then
		安装SATA驱动_VEN_8086()
	end
	
	--
	-- VEN_144D : 
	--      -A820	NVMe SSD Controller 171X
	--      -A821	NVMe SSD Controller 172X
	--      -A822	NVMe SSD Controller 172Xa/172Xb
	--      -A808	NVMe SSD Controller SM981/PM981
	--      -1600	Apple PCIe SSD
	--      -A800	XP941 PCIe SSD
	--      -A802	NVMe SSD Controller SM951/PM951
	--      -A804	NVMe SSD Controller SM961/PM961
	--
	-- VEN_106B | Apple Inc. 各种设备
	--      -CC_0180 NOT FOUND!
	执行结果,子进程标准输出 = 执行子进程并取标准输出("cmd.exe /c devcon status PCI\\VEN_144D* PCI\\VEN_106B*CC_0180",SW_HIDE)
	if nil ~= string.find(子进程标准输出, "Name", 1) then
		sapple()
	end
	
	删除文件("X:\\Windows\\desktop\\desktop.ini")
	
	------------------------------------------------------------
	-- 安装自定义驱动，在?:\\WifiDriver目录下，名为custom*.7z
	------------------------------------------------------------
	盘符_Z = string.byte("Z",1)
	-- 寻找?:\\WifiDriver\\custom(_64)*.7z, 如果找到就安装它
	local 自定义驱动包路径 = nil
	for i = 0, 23 do
		盘符 = 盘符_Z - i
		Wifi驱动目录 = string.char(盘符) .. ":\\WifiDriver"
		if 存在路径(Wifi驱动目录) then
			-- 已集成lfs --
			for file in lfs.dir(Wifi驱动目录) do
				if file ~= "." and file ~= ".." then
					local full_path = Wifi驱动目录 .. "\\" .. file
					local attr = lfs.attributes(full_path)
					if attr.mode == "file" then
						-- 寻找custom*.7z
						if 1 == string.find(file, "custom", 1) and nil ~= string.find(file, ".7z", 1) then
							自定义驱动包路径 = full_path
							break
						end
					end
				end
			end
		end
	end
	if 自定义驱动包路径 ~= nil then
		安装自定义驱动(自定义驱动包路径)
	end
	
	-- 
	-- 修复Inter(R) PRO/1000 MT Network Connection
	--
	Log.info("=====================================================")
	Log.info("         处理有线网卡")
	Log.info("=====================================================")
	执行结果,子进程标准输出 = 执行子进程并取标准输出("cmd.exe /c devcon status *CC_0200", SW_HIDE)
	if nil ~= string.find(子进程标准输出, "problem", 1) then
		Log.info("需要安装/修复有线网卡驱动")
		加载有线网卡驱动()
	end
	
	Log.info("=====================================================")
	Log.info("         处理无线网卡")
	Log.info("=====================================================")
	执行结果,CC_0280状态 = 执行子进程并取标准输出("cmd.exe /c devcon status *CC_0280", SW_HIDE)
	CC_0280_Problem = false
	if nil ~= string.find(CC_0280状态, "problem", 1) then
		CC_0280_Problem = true
	end
	
	-- 处理无线网卡
	-- 1,在setupapi.dev.log中查找，调用devcon删除msdri.inf
	执行结果,子进程标准输出 = 执行子进程并取标准输出("cmd.exe /c find \"msdri.inf\'\" " .. 获取环境变量("windir") .. "\\inf\\setupapi.dev.log", SW_HIDE)
	-- 这里有问题，执行的结果永远为空。
	msdri_inf_0x00 = string.find(子进程标准输出, "0x00", 1)
	if nil ~= msdri_inf_0x00 then
		posbeg = string.find(子进程标准输出, "Published", 1) 
		posend = string.find(子进程标准输出, ".", posbeg) -- 找到包含Published的一行
		if posbeg ~= nil and posend ~= nil then
			to_split = string.sub(子进程标准输出,posbeg,posend)
			-- 以'字符分割字符串,如Published 'hdaudio.inf_amd64_a9d1a669e70897b9\hdaudio.inf' to 'hdaudio.inf'.
			-- 取得上例中的hdaudio.inf
			子串表 = 分割字符串(to_split,"\'")
			if table.maxn(子串表) >= 4 then
				需要删除的文件 = 子串表[4]
				执行子进程并等待它完成("cmd.exe /c devcon dp_delete " .. 需要删除的文件)
			end
		end
	end
	-- 2 如果无线网卡设备正在运行，则尝试启动无线服务
	if nil ~= string.find(CC_0280状态, "running", 1) then
		启动无线服务()
	end
	-- 3 安装和启动无线网卡驱动
	if CC_0280_Problem or msdri_inf_0x00 ~= nil then
		安装和启动无线网卡驱动()
	end
	
	--
	-- 处理Realtek网卡驱动
	-- Realtek USB FE/GbE NIC NDIS6.40 64-bit Driver
	-- rtux64w10.sys
	--
	if 存在路径("X:\\Windows\\system32\\drivers\\rtux64w10.sys") then
		重新安装rtux64w10()
	end
	
	--
	-- 安装声卡驱动
	--
	Log.info("=========================================")
	Log.info("          正在安装声卡驱动……")
	Log.info("=========================================")
	local env_windir = 获取环境变量("windir")
	local b,s = 执行子进程并取标准输出("cmd.exe /c drvload " .. env_windir .. "\\inf\\hdaudss.inf " .. env_windir .. "\\inf\\hdaudio.inf")
	Log.info("b = " .. tostring(b) .. ",s=" .. s)
	
	--
	-- 安装OEM—INF (X:\WINDOWS\INF\oem*.inf)
	--
	安装OEM_INF()

	--
	-- 设置DPI
	--
	local strdpi = 注册表读整数("HKCU","Control Panel\\Desktop\\WindowMetrics","AppliedDPI")
	local dpi = tonumber(strdpi)
	if dpi ~= nil and dpi ~= 0x60 then
		设置系统DPI(dpi)
	end
	
	--
	-- 创建快捷方式
	--
	local 桌面目录 = 获取桌面目录()
	if 存在路径("X:\\Program Files\\Network\\[Network]WanDrv6.exe") then
		创建快捷方式(桌面目录 .. "\\网卡万能驱动.lnk","X:\\Program Files\\Network\\[Network]WanDrv6.exe")
	end
	if 存在路径("X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe") then
		创建快捷方式(桌面目录 .. "\\浏览器.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	end
	-- TODO：创建以下两个快捷方式
	-- LINK %desktop%\加载无线网卡,%windir%\system32\pecmd.exe,"%ProgramFiles%\wifi.wcs",%windir%\system32\netshell.dll#157
	OsExt.CreateShortCutEx(桌面目录 .. "\\加载无线网卡.lnk", "X:\\Windows\\system32\\pecmd.exe", "\"" .. ProgramFiles目录 .. "\\wifi.wcs\"", "X:\\Windows\\system32\\netshell.dll",157)
	OsExt.CreateShortCut( 桌面目录 .. "\\宽带连接.lnk", "RASPHONE.PBK")
	-- LINK %Desktop%\宽带连接,RASPHONE.PBK
	创建快捷方式(桌面目录 .. "\\浏览器.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	local 开始菜单程序目录 = 获取开始菜单程序目录()
	创建目录(开始菜单程序目录 .. "\\实用工具")
	创建目录(开始菜单程序目录 .. "\\硬件驱动")
	创建目录(开始菜单程序目录 .. "\\系统维护")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\网络管理.lnk","X:\\Program Files\\PENetwork\\PENetwork.exe")
	if 存在路径("X:\\Program Files\\Network\\[Network]WanDrv6.exe") then
		创建快捷方式(开始菜单程序目录 .. "\\硬件驱动\\网卡万能驱动.lnk","X:\\Program Files\\Network\\[Network]WanDrv6.exe")
	end
	-- LINK %programs%\硬件驱动\加载无线网卡,%windir%\system32\pecmd.exe,"%ProgramFiles%\wifi.wcs",%windir%\system32\netshell.dll#157
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\任务管理器.lnk","X:\\Windows\\System32\\taskmgr.exe")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\7z文件.lnk","X:\\Program Files\\7-zip\\7zFM.exe")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\浏览器.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\记事本.lnk","X:\\Windows\\notepad.exe")
	创建快捷方式(开始菜单程序目录 .. "\\实用工具\\屏幕键盘.lnk","X:\\Windows\\FreeVK.exe")
	--LINK %programs%\实用工具\截图工具,%&myname%,%windir%\wall.ini sn,shell32.dll,141
	OsExt.CreateShortCutEx(开始菜单程序目录 .. "\\实用工具\\截图工具.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini sn", "netshell.dll", 141)
	--HOTK Win + x,LOAD %windir%\wall.ini sn
	创建快捷方式(开始菜单程序目录 .. "\\系统维护\\注册表编辑工具.lnk","X:\\Windows\\regedit.exe")
	local appdata_dir = "X:\\Users\\Default\\AppData\\Roaming"
	local sendto_dir = appdata_dir .. "\\Microsoft\\Windows\\SendTo"
	OsExt.CreateShortCutEx(sendto_dir .. "\\发送到桌面快捷方式.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini sd")
	--LINK %sendto%\发送到桌面快捷方式,%&myname%,%windir%\wall.ini sd
	sd = 注册表读文本("HKLM","SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96b-e325-11ce-bfc1-08002be10318}\\0000","DriverDate")
	if sd == "" then
		local QuickLaunchDir = appdata_dir .. "\\Microsoft\\Internet Explorer\\Quick Launch"
		OsExt.CreateShortCutEx(QuickLaunchDir .. "\\虚拟键盘.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini vk", "freevk.exe#0")
		--ifex $%&sd%=,LINK %QuickLaunch%\虚拟键盘, %&myname%,%windir%\wall.ini vk,freevk.exe#0
	end
	
	--
	-- 隐藏开始菜单/程序/管理工具, 开始菜单/程序/启动
	--
	startup路径 = 获取环境变量("startup")
	ProgramData路径 = 获取环境变量("ProgramData")
	设置文件或文件夹属性(ProgramData路径 .. "\\Microsoft\\Windows\\Start Menu\\Programs\\Administrative Tools",FILE_ATTRIBUTE_HIDDEN)
	设置文件或文件夹属性(ProgramData路径 .. "\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp",FILE_ATTRIBUTE_HIDDEN)
	设置文件或文件夹属性(startup路径,FILE_ATTRIBUTE_HIDDEN)
	--
	-- 设置我的电脑的右键属性
	--
	注册表删除子项("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer",false)
	注册表删除键值("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer","NoPropertiesMyComputer")
	--
	-- 设置PsSuspend.exe工具为已同意“最终用户许可协议”
	--
	注册表写整数("HKLM","Software\\Sysinternals\\PsSuspend","EulaAccepted",0x01)
	
	--
	-- 运行即插即用支持脚本
	--
	Log.info("======================================")
	Log.info("           处理即插即用")
	Log.info("======================================")
	执行子进程并等待它完成("cmd.exe /c \"" .. ProgramFiles目录 .. "\\hotplug.bat\" pci",SW_HIDE)
	
	执行子进程("X:\\Windows\\System32\\taskmgr.exe",SW_HIDE)
	写桌面文本("初始化完成",RGB红色,字体,字体大小,-1,-1, -1, -1)
	等待(500)
	
	if "X:\\Users\\Default\\AppData\\Roaming" ~= os.getenv("APPDATA") then
		if os.getenv("APPDATA") == nil then
			Log.error("设置APPDATA环境变量失败，当前值为nil")
		else
			Log.error("设置APPDATA环境变量失败，当前值为：" .. os.getenv("APPDATA"))
		end
	end
	--
	-- 支持USB无线网卡/连接手机上网
	-- netrndis.inf
	--     RNDIS也叫远端网络驱动接口协议,设备通过USB方式同主机连接,模拟网络连接以便用于下载和调试工作
	-- WceIsVista.INF
	--     Remote NDIS based Internet Sharing Device
	--
	
	-- 1 安装外置无限网卡时先关闭IKEEXT
	--     IKEEXT 服务托管 Internet 密钥交换(IKE)和身份验证 Internet 协议(AuthIP)键控模块。
	关闭服务("IKEEXT")
	-- 2 安装驱动
	Log.info("=========================================================")
	Log.info("安装RNDIS(远端网络驱动接口协议)驱动，用于支持手机、USB网卡上网。")
	Log.info("=========================================================")
	local x_windows = 获取环境变量("windir")
	执行子进程并等待它完成("cmd.exe /c drvload " .. x_windows .. "\\inf\\netrndis.inf " .. x_windows .. "\\inf\\wceisvista.inf")
	-- 3 将IKEEXT服务设置为按需启动
	注册表写整数("HKLM","SYSTEM\\ControlSet001\\Services\\IKEEXT\\start",3)
	杀死进程("taskmgr.exe")
	杀死进程("taskmgr.exe")
	
	--
	-- 如果PE是远程安装的，那么需要去服务器上取一个pxeautorun.cmd
	-- 我们的Win10 PE中PEBootType总是Ramdisk:OpticalDrive
	--
	PE启动类型 = 注册表读文本("HKLM","SYSTEM\\CurrentControlSet\\Control","PEBootType")
	if string.find(PE启动类型,"Remote") ~= nil then
		if true == 执行远程启动() then
			-- 如果执行了远程服务器上下载的autorun就不必继续执行下面的脚本了。
			return
		end
	end
	--
	-- 设置桌面背景 --
	--
	盘符_Z = string.byte("Z",1)
	for i = 0, 23 do
		盘符 = 盘符_Z - i
		背景图片路径 = string.char(盘符) .. ":\\boot\\wall\\winpe.jpg"
		if 存在路径(背景图片路径) then
			设置墙纸(背景图片路径)
			break
		end
	end
	
	--
	-- 将原来放到Y盘的WIM解压到PEProgram -- 
	--
	写桌面文本("正在加载相关工具.....",RGB红色,字体,字体大小,-1,-1, -1, -1)
	dofile("X:\\Windows\\main.lua")
end

setup()



