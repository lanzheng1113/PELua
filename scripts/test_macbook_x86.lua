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


--
-- ExecGetStdout 执行子进程并取标准输出
-- 参数1 子进程的命令行
-- 参数2 指定是否显示子进程的窗口。SW_HIDE/SW_SHOW
-- 返回两个值。第一个是进程退出码，如果进程执行失败了那么返回255。第二个返回值是子进程的标准输出
--
执行子进程并取标准输出 = OsExt.ExecGetStdout



function IsMacBook()
	Log.info("判断是不是MACBOOK")
	local cmd = 获取环境变量("windir") .. "\\system32\\wbem\\WMIC.exe ComputerSystem get Model"
	执行结果,ComputerSystem_Model = 执行子进程并取标准输出(cmd,SW_HIDE)
	ComputerSystem_Model = string.upper(ComputerSystem_Model)
	if string.find(ComputerSystem_Model,"MACBOOK") then
		return true
	else
		return false
	end
end

存在路径 = OsExt.PathFileExist

执行子进程并等待它完成 = OsExt.ExecWait

写桌面文本 = PEExt.SetDesktopText

function test()
	--
	-- MacBook 适配
	--
	if IsMacBook() then
		Log.info("=====================================================")
		Log.info("         适配MacBook驱动")
		Log.info("=====================================================")
		
		local 触控板inf路径 = "X:\\AppleDrv\\MultiTouchTrackPad64\\AppleMTP64.inf"
		if 存在路径(触控板inf路径) then
			执行子进程并等待它完成("cmd.exe /c drvload " .. 触控板inf路径)
		end
		-- 启用trackPad右键
		if 0 ~= PEExt.EnableAppleTrackPadRightClick(true) then
			Log.error("启用Macbook track pad右键失败。")
		end
		-- 安装macbook的wifi驱动
		local Wifi_INF = "X:\\AppleDrv\\Wireless64"
		if 存在路径(Wifi_INF) then
			执行子进程并等待它完成("cmd.exe /c drvload " .. Wifi_INF)
		end
		-- 启动无线服务
		写桌面文本("正在启动无线服务.....", RGB红色, 字体, 字体大小, -1, -1, -1, -1)
		OsExt.Sleep(500)
		执行子进程并等待它完成("cmd.exe /c net start Wlansvc")
	end
end

test()


