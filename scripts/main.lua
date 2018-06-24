dofile("const.lua")

-- 前置定义导出函数的中文别名 --
弹窗提示 = OsExt.MessageBox

-- 参数1 进程路径+进程参数
-- 参数2 显示子进程窗口：SW_SHOW；隐藏子进程窗口：SW_HIDE
执行子进程 = OsExt.Exec

-- 参数1 进程路径+进程参数
-- 参数2 显示子进程窗口：SW_SHOW；隐藏子进程窗口：SW_HIDE
执行子进程并等待它完成 = OsExt.ExecWait

-- 参数1 文件或文件夹路径
存在路径 = OsExt.PathFileExist

Program路径 = "X:\\Program Files"

function 释放SOFT2_WIM()
	盘符_Z = string.byte("Z",1)
	for i = 0, 23 do
		盘符 = 盘符_Z - i
		WIM路径 = string.char(盘符) .. ":\\BOOT\\EXTAPP\\X86\\SOFT2.WIM"
		--弹窗提示(NULL,WIM路径,"路径",0)
		if 存在路径(WIM路径) then
			解压程序路径 = "\"" .. Program路径 .. "\\7-zip\\7z.exe" .. "\""
			进程参数 = 解压程序路径 .. " x " .. WIM路径 .. " -oX:\\PEPrograms\\X86Soft2  -aoa"
			执行子进程并等待它完成(进程参数,SW_HIDE)
			--弹窗提示(NULL,"SOFT2 Extract complete!","测试",MB_OK)
			--解压后执行Soft2Reg.lua，创建快捷方式等。
			--这里改为执行X:\PEPrograms\\X86Soft2\\Soft2Reg.lua
			if 存在路径("X:\\PEPrograms\\X86Soft2\\Soft2Reg.lua") then
				dofile("X:\\PEPrograms\\X86Soft2\\Soft2Reg.lua")
			end
			break
		end
	end
end

function 释放OEM_WIM()
  盘符_Z = string.byte("Z",1)
	for i = 0, 23 do
		盘符 = 盘符_Z - i
		WIM路径 = string.char(盘符) .. ":\\BOOT\\EXTAPP\\X86\\OEM.WIM"
		--弹窗提示(NULL,WIM路径,"路径",0)
		if 存在路径(WIM路径) then
			解压程序路径 = "\"" .. Program路径 .. "\\7-zip\\7z.exe" .. "\""
			进程参数 = 解压程序路径 .. " x " .. WIM路径 .. " -oX:\\PEPrograms\\oem  -aoa"
			执行子进程并等待它完成(进程参数,SW_HIDE)
			if 存在路径("X:\\PEPrograms\\oem\\oem.lua") then
				dofile("X:\\PEPrograms\\oem\\oem.lua")
			end
			--弹窗提示(NULL,"SOFT2 Extract complete!","测试",MB_OK)
			break
		end
	end  
end

-- 主函数 -- 
function Main()
	释放SOFT2_WIM()
	释放OEM_WIM()
	--弹窗提示(NULL,"Extract complete!","测试",MB_OK)
end

Main()

