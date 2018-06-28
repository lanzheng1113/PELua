-- setup --
-- һЩ�������� --
IDOK=1
IDCANCEL=2
IDABORT=3
IDRETRY=4
IDIGNORE=5
IDYES=6
IDNO=7
MB_YESNO = 4
-- ����NULL --
NULL = 0
-- �����ӽ���ʱ��ʾ/�����ӽ��̴���
SW_SHOW = 5
SW_HIDE = 0
-- ��������Ŀ¼��ID --
CSIDL_COMMON_STARTMENU=0x0016			-- All Users\Start Menu
CSIDL_COMMON_PROGRAMS=0X0017			-- All Users\Start Menu\Programs
CSIDL_COMMON_STARTUP=0x0018				-- All Users\Startup
CSIDL_COMMON_DESKTOPDIRECTORY=0x0019	-- All Users\Desktop
-- �ļ����������� --
FILE_ATTRIBUTE_HIDDEN = 0x02
-- ��ʾ�������ϵ��ı���ɫ������ --
RGB��ɫ = 0xFF0000
���� = "Microsoft YaHei"
�����С = 20

-- ���������������� --
����·�� = OsExt.PathFileExist
����LOGO = PEExt.SetLogo
����ǽֽ = PEExt.SetDesktopWallPaper
ȡ��Ļ��С = PEExt.GetDesktopSize
ִ���ӽ��̲��ȴ������ = OsExt.ExecWait

--
-- ExecGetStdout ִ���ӽ��̲�ȡ��׼���
-- ����1 �ӽ��̵�������
-- ����2 ָ���Ƿ���ʾ�ӽ��̵Ĵ��ڡ�SW_HIDE/SW_SHOW
-- ��������ֵ����һ���ǽ����˳��룬�������ִ��ʧ������ô����255���ڶ�������ֵ���ӽ��̵ı�׼���
--
ִ���ӽ��̲�ȡ��׼��� = OsExt.ExecGetStdout

--
-- OsExt.CreateDir����һ��Ŀ¼����Ŀ¼�������,�����ʧ��
-- ����1 ��Ҫ����Ŀ¼��ȫ·��
-- ����һ��booleanֵ����ɹ�����ʧ��
--
����Ŀ¼ = OsExt.CreateDir

-- 
-- PEExt.INIT ��ʼ��PE��ͬPECMD��INIT I��ȥ����PECMD INIT����ĸ��ֲ���
-- �޲���
-- ����һ��booleanֵ����ɹ�����ʧ��
-- 
��ʼ��PE = PEExt.INIT

--
-- PEExt.SetEnvironment ���û�������
-- ����1 ����������
-- ����2 ����������Ӧ��ֵ
-- ����3 �Ƿ�ΪPEϵͳȫ�ֻ������������ΪTRUE������ϵͳ��Ч(ENVI $XXX=OOO),ΪFALSE��ֻ�Ե�ǰ������Ч(ENVI #xxx=ooo)��
--
���û������� = PEExt.SetEnvironment

--
-- PEExt.InitEnvironmentVariable
-- �������»���������Favorites,Desktop,StartMenu,Startup,Programs,SendTo,Personal,QuickLaunch
-- �޲���������boolean����
--
��ʼ��ϵͳ�������� = PEExt.InitEnvironmentVariable

--
-- ɱ��������ָ�������н���
-- ����1 ������(xxx.exe)
--
ɱ������=OsExt.KillProcessByName

--
-- �ȴ�һ��ʱ��
-- ����1 �ȴ��ĺ�����
--
�ȴ�=OsExt.Sleep

-- ֹͣһ������
-- ����1 ������
�رշ���=OsExt.StopService

-- ����1 ����·��+���̲���
-- ����2 ��ʾ�ӽ��̴��ڣ�SW_SHOW�������ӽ��̴��ڣ�SW_HIDE
ִ���ӽ��� = OsExt.Exec

-- �����б� �ı�����ɫ�����壬�����С��X���꣬Y����
-- ���X,Y���궼����Ϊ-1�����ʾ�ö�������ʾ
д�����ı� = PEExt.SetDesktopText

ע���д���� = RegIni.WriteRegDWORD
ע���д�ַ��� = RegIni.WriteRegStr
ע�����ı� = RegIni.ReadRegStr
ע�������� = RegIni.ReadRegDWORD
ע���ɾ������ = RegIni.DeleteRegKey
ע���ɾ����ֵ = RegIni.DeleteRegValue

--
-- RegIni.ReadRegMutiStr����һ��table,����ÿһ���һ���ַ�����
--
ע����MULTI_SZ = RegIni.ReadRegMutiStr

--
-- ɾ��һ���ļ��������ռ���������������Ϊ������ɾ����
--
ɾ���ļ� = OsExt.DeepDeleteFile

--
-- �ݹ�ɾ��һ��Ŀ¼�µ������ļ�����Ŀ¼
--
ɾ��Ŀ¼ = OsExt.RemoveDirectoryRecursively

--
-- ������ʾU�̷�����δʵ��
--
������ʾU�̷��� = PEExt.ShowUSBDisk

-- 
-- չ��һ�����л����������ַ���
--
�ַ���_չ���������� = OsExt.ExpandStr

--
-- ����һ���ļ��Ŀ�ݷ�ʽ
-- ����1 ��ݷ�ʽȫ·��
-- ����2 Դ�ļ�ȫ·��
--
������ݷ�ʽ = OsExt.CreateShortCut

�����ļ����ļ������� = OsExt.SetFileAttributes

ProgramFilesĿ¼ = "X:\\Program Files"

--
-- ȡ�������� ��os.getenv("windir") ���� %windir%
-- ��Ϊ��ʱ���ؿ��ַ�����������nil�Է�ֹ�����ַ���ʱ��������
--
function ��ȡ��������(pstrEnv)
	local value = os.getenv(pstrEnv)
	if nil == value then
		return ""
	else
		return value
	end
end

function ��ȡ����Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end


function ��ȡ��ʼ�˵�Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end


function ��ȡ��ʼ�˵�����Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_PROGRAMS)
end

--
-- ���ַ����з������һ����ǣ����ظñ��֮ǰ������
-- ��: reverse_find("abc/def","/") == "abc"
-- 
function reverse_find(str, token)
	rs = string.reverse(str)
	pBeg, pEnd = string.find(rs,token)
	if nil == pBeg then
		-- ���û���ҵ���ǣ���ô��������ԭ�ַ���
		return str
	end
	m = string.len(rs) - pEnd + 1
	return string.sub(str,1,m) 
end


function ��ȡ�ļ���(�ļ�ȫ·��, �Ƿ�����չ��)
	-- ȡ����չ�����ļ�������x:\\abc.7z ==> abc.7z
	local name_with_ext = OsExt.GetFileName(�ļ�ȫ·��)
	if name_with_ext == nil or name_with_ext == "" then
		return nil
	end
	
	if �Ƿ�����չ�� then
		return name_with_ext
	else
		-- ȡ�ò�����չ�����ļ���, ��abc.7z ==> abc
		local �ļ��� = reverse_find(name_with_ext,".")
		return �ļ���
	end
end


--
-- ȡ��devcon.exe��ȡ���豸����
--
function hwids()
	local ִ���Ƿ�ɹ�, ������� = ִ���ӽ��̲�ȡ��׼���("devcon.exe findall *", SW_HIDE)
	-- ִ�к��������һ����:"165 matching device(s) found",ȡ���������
	_,_,cnt = string.find(�������, "(%d+) matching device(s) found.")
	if cnt == nil then
		Log.info("devcon.exe findallû���ҵ��κ�����")
		return 0
	else
		Log.info("devcon.exe findall�ҵ�������Ϊ" .. tostring(cnt))
		return cnt
	end
end


function ��װ����(������·��, ��װλ��)
	--[[
	_SUB Insdriver
		NAME &&fname=%~1	--����ָ���ļ����Ļ�����(��Ŀ¼������չ��) ��-ext������չ����
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
	if not ����·��(������·��) then
		return false
	end

	--
	-- install
	--
	local fname = ��ȡ�ļ���(������·��, false)
	if nil == fname or "" == fname then
		return false
	end
	
	-- �����custom*.7z������custom_64*.7z��װ����ǰ�ȱ���һ�����е��豸��
	local ID1, ID2
	if nil ~= string.find(fname,"custom") or nil ~= string.find(fname,"custom_64") then
		ID1 = hwids()
	end
	
	-- ���û��ָ��Ҫ��������װ�������ô��������װ����ʱĿ¼
	local inst_to_path = nil
	if nil == ��װλ�� or "" == ��װλ�� then
		inst_to_path = ��ȡ��������("temp") .. "\\pe-driver\\" .. fname
	else
		inst_to_path = ��װλ��
	end
	
	local path_7z = ProgramFilesĿ¼ .. "\\7-zip\\7z.exe"
	local exec_cmd = path_7z .. " x " .. ������·�� .. " -o" .. inst_to_path
	
	ִ���ӽ��̲��ȴ������(exec_cmd, SW_HIDE)
	exec_cmd = "cmd.exe /c dpinst.exe /S /Path " .. inst_to_path
	ִ���ӽ��̲��ȴ������(exec_cmd, SW_HIDE)
	OsExt.Sleep(500)
	
	-- �����custom*.7z������custom_64*.7z��װ����������ʱĿ¼�������°�װһ��
	if nil ~= string.find(fname,"custom") or nil ~= string.find(fname,"custom_64") then
		ID2 = hwids()
		if ID1 ~= nil and ID2 ~=nil and ID1 < ID2 then
			ִ���ӽ��̲��ȴ������("cmd.exe /c dpinst.exe /S /Path " .. ��ȡ��������("temp") .. "\\pe-driver\\" .. fname)
		end
	end
	
	-- �����DRIVERS_USB*.7z����װ����������ʱĿ¼�������°�װһ�Ρ�
	if nil ~= string.find(fname,"DRIVERS_USB") then
		ִ���ӽ��̲��ȴ������("cmd.exe /c dpinst.exe /S /Path " .. ��ȡ��������("temp") .. "\\pe-driver\\" .. fname)
	end
	
	-- ���������ʱĿ¼�а�װ���ڰ�װ��ɾ��%temp%\\pe-driver
	if nil == ��װλ�� or "" == ��װλ�� then
		ɾ��Ŀ¼(��ȡ��������("temp") .. "\\pe-driver")
	end
	return true
end


function �޸�WinNTSetup�ļ�(tag)
	if not ����·��("X:\\Program Files\\WinNTSetup") then
		����Ŀ¼("X:\\Program Files\\WinNTSetup")
	end
	--WRIT X:\Program Files\WinNTSetup\WinNTSetup.ini,1,[WinNT6]
    --WRIT X:\Program Files\WinNTSetup\WinNTSetup.ini,$2,Drivers=X:\raid\%1
	local filew=io.output("X:\\Program Files\\WinNTSetup\\WinNTSetup.ini")
	str = "[WinNT6]\nDrivers=X:\\raid\\" .. tag .. "\n"
	io.write(str)
	io.flush()
	io.close()
end


function ��װSATA����()
	local the_7z = ProgramFilesĿ¼ .. "\\DRIVERS_RAID.7z"
	local target = ��ȡ��������("SystemDrive") .. "\\raid"
	if not ��װ����(the_7z, target) then
		return false
	end
	ִ�н��, ����CC_0104�豸��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status *CC_0104", SW_HIDE)
	if nil ~= string.find(����CC_0104�豸���, "VEN_1022&DEV_7805", 1) then
		�޸�WinNTSetup�ļ�("amd")
	end
	if nil ~= string.find(����CC_0104�豸���, "VEN_8086&DEV_A", 1) then
		�޸�WinNTSetup�ļ�("iaStorAC")
	end
	if nil ~= string.find(����CC_0104�豸���, "VEN_8086&DEV_9") then
		�޸�WinNTSetup�ļ�("iaStorAC")
	end
	return true
end


function ��װSATA����_VEN_8086()
	--[[
	exec* &&smbus=!devcon status PCI\VEN_8086*DEV_A123 PCI\VEN_8086*DEV_9D23 PCI\VEN_8086*DEV_A2A3
    SED &&U=?,VEN_8086,,%&smbus%
    ifex $0=%&U%, exit _sub
    CALL Insdriver "%ProgramFiles%\DRIVERS_RAID.7z" %SystemDrive%\raid
    call wini iaStorAC
	]]--
	ִ�н��, smbus = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status PCI\\VEN_8086*DEV_A123 PCI\\VEN_8086*DEV_9D23 PCI\\VEN_8086*DEV_A2A3", SW_HIDE)
	if nil == string.find(smbus, "VEN_8086", 1) then
		return
	end
	local the_7z = ProgramFilesĿ¼ .. "\\DRIVERS_RAID.7z"
	local target = ��ȡ��������("SystemDrive") .. "\\raid"
	��װ����(the_7z, target)
	�޸�WinNTSetup�ļ�("iaStorAC")
end


function sapple()
	local the_7z = ProgramFilesĿ¼ .. "\\DRIVERS_RAID.7z"
	local target = ��ȡ��������("SystemDrive") .. "\\raid"
	if not ��װ����(the_7z,target) then
		return false
	end
	ִ�н��, �ӽ��̱�׼��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status PCI\\VEN_106B*CC_0180")
	if nil ~= string.find(�ӽ��̱�׼���, "Name", 1) then
		�޸�WinNTSetup�ļ�("AppleSSD64")
	end
end


function ��װ�Զ�������(path)
	д�����ı�("�����Զ�������.....", RGB��ɫ, ����, �����С, -1, -1, -1, -1)
	OsExt.Sleep(500)
	local Drv = string.sub(path, 1, 1)
	-- �������Ĭ�ϵİ�װ���Ͱ�װĬ�ϵģ���������ҵ��ĵ�һ����װ��
	local default = drv .. ":\\WifiDriver\\custom_64.7z"
	if ����·��(default) then
		��װ����(default)
	else
		��װ����(path)
	end
end


function ����������������()
	д�����ı�("����������������.....", RGB��ɫ, ����, �����С, -1, -1, -1, -1)
	OsExt.Sleep(500)
	��װ����(ProgramFilesĿ¼ .. "\\DRIVERS_NET.7z")
end


function �������߷���()
	--[[
	TEAM TEXT �������߷���..... #LTRB + $20:Microsoft YaHei UI|WAIT 500
	EXEC =!net start Wlansvc
	WAIT 100
	forx !\WifiDriver\*.xml,&&con,1,set &&exi=%&con%
	find $Z=Z%&exi%, exit _sub
	FDRV &&fDrive=%&exi%
	TEAM TEXT ����������������.....#LTRB + $20:Microsoft YaHei UI|WAIT 500
	forx %&fdrive%\WifiDriver\*.xml,&&con,0,exec !cmd.exe /c netsh wlan add profile filename=%&con%
	]]
	д�����ı�("�������߷���.....", RGB��ɫ, ����, �����С, -1, -1, -1, -1)
	OsExt.Sleep(500)
	ִ���ӽ��̲��ȴ������("cmd.exe /c net start Wlansvc")
	OsExt.Sleep(100)
	local WiFi�����ļ����� = false
	local Wifi����Ŀ¼ = nil
	local �̷�_Z = string.byte("Z",1)
	for i = 0, 23 do
		local �̷� = �̷�_Z - i
		Wifi����Ŀ¼ = string.char(�̷�) .. ":\\WifiDriver"
		if ����·��(Wifi����Ŀ¼) then
			-- �Ѽ���lfs --
			for file in lfs.dir(Wifi����Ŀ¼) do
				if file ~= "." and file ~= ".." then
					local full_path = Wifi����Ŀ¼ .. "\\" .. file
					local attr = lfs.attributes(full_path)
					if attr.mode == "file" then
						-- Ѱ��*.xml
						if string_ends_with(file, ".xml") then
							WiFi�����ļ����� = true
							break
						end
					end
				end
			end
		end
	end
	-- ����?:\WifiDriverĿ¼�µ�����xml����
	if WiFi�����ļ����� then
		д�����ı�("����������������.....", RGB��ɫ, ����, �����С, -1, -1, -1, -1)
		for file in lfs.dir(Wifi����Ŀ¼) do
			if file ~= "." and file ~= ".." then
				local full_path = Wifi����Ŀ¼ .. "\\" .. file
				local attr = lfs.attributes(full_path)
				if attr.mode == "file" then
					if string_ends_with(file, ".xml") then
						ִ���ӽ��̲��ȴ������("cmd.exe /c netsh wlan add profile filename=\"" .. full_path .. "\"" )
					end
				end
			end
		end
	end
end


function ��װ������������������()
	--[[
	 find $3=%&bx64%, envi &&wei=_64! envi &&wei=
     IFEX X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%.7z,! TEAM LINK %desktop%\������װ��������,%&myname%,wifi.wcs,netshell.dll,157,,%programfiles% |exit _sub
     TEAM TEXT ����������������..... #LTRB + $20:Microsoft YaHei UI|WAIT 500
     CALL Insdriver X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%.7z
	 CALL Insdriver X:\WifiDriver\DRIVERS_NET_WIRELESS%&wei%_2.7z
     call sload
	]]
	local ����������·��1 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS.7z"
	local ����������·��2 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_2.7z"
	if OsExt.IsWow64() then
		����������·��1 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_64.7z"
		����������·��2 = "X:\\WifiDriver\\DRIVERS_NET_WIRELESS_64_2.7z"
	end
	local ����Ŀ¼ = ��ȡ����Ŀ¼()
	if ����·��(����������·��1) then
		-- ������ݷ�ʽ LINK %desktop%\������װ��������,%&myname%,wifi.wcs,netshell.dll,157,,%programfiles%
		OsExt.CreateShortCutEx(����Ŀ¼ .. "\\������װ��������.lnk", "X:\\Windows\\System32\\pecmd.exe", "wifi.wcs", "netshell.dll", 157, ProgramFilesĿ¼)
		д�����ı�("����������������.....", RGB��ɫ, ����, �����С, -1, -1, -1, -1)
		��װ����(����������·��1)
		return
	end
	if ����·��(����������·��2) then
		��װ����(����������·��2)
	end
	�������߷���()
end


function ���°�װrtux64w10()
	OsExt.DeepCopyFile(getenv("windir") .. "\\inf\\rtux64w10.inf", getenv("temp") .. "\\rtux64w10.inf")
	OsExt.DeepCopyFile(getenv("windir") .. "\\inf\\rtux64w10.sys", getenv("temp") .. "\\rtux64w10.sys")
	ִ���ӽ��̲��ȴ������("cmd.exe /c drvload " .. getenv("temp") .. "\\rtux64w10.inf")
	ɾ���ļ�(getenv("temp") .. "\\rtux64w10.inf")
	ɾ���ļ�(getenv("temp") .. "\\rtux64w10.sys")
end


function string_ends_with(str,substr)
	return string.find(str,substr,1) == string.len(str) - string.len(substr) + 1
end


--
-- ��װX:\\Windows\\inf\\OEM*.INF
--
function ��װOEM_INF()
	local count = 0
	local OEM_INFĿ¼ = "X:\\Windows\\inf"
	if ����·��(OEM_INFĿ¼) then
		-- �Ѽ���lfs --
		for file in lfs.dir(OEM_INFĿ¼) do
			if file ~= "." and file ~= ".." then
				local full_path = OEM_INFĿ¼ .. "\\" .. file
				local attr = lfs.attributes(full_path)
				if attr.mode == "file" then
					-- Ѱ��oem*.inf 
					if 1 == string.find(file, "oem") and string_ends_with(file, ".inf") then
						Log.info("�ҵ���OEM_INF:" .. file)
						orz = ע����MULTI_SZ("HKLM","DRIVERS\\DriverDatabase\\DriverInfFiles\\",file)
						if table.getn(orz) == 0 then
							Log.info("��װOEM����:" .. file)
							ִ���ӽ��̲��ȴ������("cmd.exe /c drvload " .. full_path)
						end
						count = count + 1
					end
				end
			end
		end
	end
	return count
end


function ����ϵͳDPI(p_dpi)
	Log.info("����ϵͳDPI=" .. tostring(p_dpi))
	ע���ɾ������("HKCU","Software\\Microsoft\\Windows NT\\CurrentVersion\\TaskManager\\Preferences",false)
	ע���ɾ����ֵ("HKCU","Software\\Microsoft\\Windows NT\\CurrentVersion\\TaskManager","Preferences")
	if p_dpi > 0x60 and p_dpi < 0x84 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "600")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","32")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","������,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","������,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","������,-20,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Buttonbar","Buttonheight","33")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Buttonbar","SmallIcons","0")
	elseif p_dpi >  0x84 and p_dpi < 0xa8 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "750")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","32")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","������,-24,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","������,-24,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","������,-24,134")
		RegIni.WriteINIStr("X:\\Windows\\tlb\\toolbars\\start\\setup.ini","settings","menuItemMarginY","12")
	elseif p_dpi > 0xa8 then
		RegIni.WriteINIStr("X:\\Program Files (x86)\\DesktopInfo\\desktopinfo.ini","options","width", "920")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","AllResolutions","Iconsize32","48")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font1","������,-32,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font2","������,-32,134")
		RegIni.WriteINIStr("X:\\Program Files\\TotalCommander\\WINCMD.INI","Lister","Font3","������,-32,134")
	end
end


function ִ��Զ������()
	Log.info("ִ��Զ������")
	ServerAddr = ע�����ı�("HKLM","SYSTEM\\CurrentControlSet\\Control","PEBootServerAddr")
	ע���д�ַ���("HKCU","Software\\TightVNC\\Control\\ReverseConnectionHistory","0",ServerAddr)
	ִ���ӽ��̲��ȴ������("cmd.exe /c tftp -i %&ServerAddr% get pxeautorun.txt %windir%\\system32\\pxeautorun.cmd",SW_HIDE)
	windir = ��ȡ��������("windir")
	if ����·��(windir .. "\\system32\\pxeautorun.cmd") then
		ִ���ӽ��̲��ȴ������("cmd.exe /c pxeautorun.cmd " .. ServerAddr)
		return true
	end
	return false
end


function setup()

	--
    -- ������32λ�ĳ���
	-- �����64λϵͳ�����WOW64�ļ��ض���ͽ���ע����ض��� --
	--if OsExt.IsWow64() then
	--	OsExt.DisableWow64FsRedir()
	--	RegIni.SetRegView(64)
	--end
	--
	
	-- ������־·�� --
	Log.SetLogFile("X:\\Windows\\PESetup.log")
	Log.info("��ʼִ��setup�ű�")
	-- �������汳�� --
	�̷�_Z = string.byte("Z",1)
	for i = 0, 23 do
		�̷� = �̷�_Z - i
		����ͼƬ·�� = string.char(�̷�) .. ":\\boot\\wall\\winpe.jpg"
		if ����·��(����ͼƬ·��) then
			local backGroundColor = 0
			����LOGO(����ͼƬ·��,������ɫ)
			break
		end
	end
	sum = 0
	��Ļ��,��Ļ�� = ȡ��Ļ��С()
	if ��Ļ�� > ��Ļ�� then
		-- "X:\Program Files\Rotate\rotate.exe" C
		ִ���ӽ��̲��ȴ������("\"" .. ProgramFilesĿ¼ .. "\\Rotate\\rotate.exe\" C" , SW_HIDE)
	end
	-- 
	RGB��ɫ = 0x00FF0000
	���� = "Microsoft YaHei"
	�����С = 20
	д�����ı�("���ڳ�ʼ��....",RGB��ɫ,����,�����С,-1,-1, -1, -1)
	
	-- ��ʼ��SHELL
	��ʼ��PE()
	-- ��������ִ��������ʼ��
	ִ���ӽ��̲��ȴ������("cmd.exe /c X:\\Windows\\system32\\startnet.cmd", SW_HIDE)
	���û�������("APPDATA","X:\\Users\\Default\\AppData\\Roaming",true)
	��ʼ��ϵͳ��������()
	-- ����ϵͳ���(����Explorer.exe)
	PEExt.SHELL("X:\\Windows\\WinXShell.exe")
	OsExt.Sleep(1000)
	while not OsExt.GetProcessId("WinXShell.exe") do
		OsExt.Sleep(500)
	end
	--ִ���ӽ���("WinXShell.exe",SW_SHOW)
	-- ����SHELL���
	ִ���ӽ��̲��ȴ������("cmd.exe /c regedit /s \"" .. ProgramFilesĿ¼ .. "\\Classic Shell\\cs.reg\"", SW_HIDE)
	-- ���ػ���վ,����
	ע���д�ַ���("HKLM","Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel","{645FF040-5081-101B-9F08-00AA002F954E}","1")
	ע���д�ַ���("HKLM","Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel","{F02C1A0D-BE21-4350-88B0-7367FC96EF3B}","1")
	-- ��ʼ�˵��ղؼ�����
	ע���д�ַ���("HKCU","Software\\IvoSoft\\ClassicStartMenu\\Settings", "Favorites", "Hide")
	-- 
	ִ���ӽ��̲��ȴ������( "cmd.exe /c regsvr32 /s " .. "\"" .. ProgramFilesĿ¼ .. "\\Classic Shell\\StartMenuHelper64.dll\"", SW_HIDE)
	ִ���ӽ���("\"" .. ProgramFilesĿ¼ .. "\\Classic Shell\\ClassicStartMenu.exe\"", SW_HIDE)
	local iRet = ִ���ӽ��̲��ȴ������("X:\\Windows\\System32\\winpeshl.exe", SW_SHOW) 
	if iRet ~= 0 then
		Log.error("=====================================================")
		Log.error("         ִ��winpeshl.exeʧ��,����id=" .. tostring(iRet))
		Log.error("=====================================================")
	end
	OsExt.ApplyEnvironmentVarsChange()
	PublicĿ¼ = ��ȡ��������("PUBLIC")
	if PublicĿ¼ ~= "" then
		ɾ���ļ�(PublicĿ¼ .. "\\desktop\\desktop.ini")
	end
	-- ��顢��װUSB����
	ִ�н��,�ӽ��̱�׼��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status PCI\\VEN_1B73*DEV_1* PCI\\VEN_1106*DEV_9202 PCI\\VEN_1106*DEV_9201 PCI\\VEN_1106*DEV_3432 PCI\\VEN_1106*DEV_3483 PCI\\VEN_1106*DEV_3515",SW_HIDE)
	if nil ~= string.find(�ӽ��̱�׼���,"Name",1) then
		��װ����(ProgramFilesĿ¼ .. "\\DRIVERS_USB.7z")
	end
	-- ʹ��ShowDrivs_Gui_x64Ѱ�Һͼ���UDM����
	-- ����ShowDrivs_Gui_x64.exe MOUN-udm -findboot -w \\.\PhysicalDrive* C-
	ִ���ӽ��̲��ȴ������("\"" .. ProgramFilesĿ¼ .. "\\ShowDrivs_Gui_x64.exe\"" .." MOUN-udm -findboot -w \\\\.\\PhysicalDrive* C-", SW_HIDE)
	-- 
	-- �����ϵͳ����ѡ���UDMBOOT�����Լ��غ���ʾ����δ���ص�U�̻�USB�ƶ�Ӳ�̷���
	-- 
	SystemStartOpt = ע�����ı�("HKLM","SYSTEM\\CurrentControlSet\\Control","SystemStartOptions")
	if nil == string.find(SystemStartOpt,"UDMBOOT",1) then
		-- ��ӦPECMD�ģ�SHOW U:-1������
		������ʾU�̷���()
	end
	
	----------------------------------------------------------------- 
	-- ��װSATA����
	-- PCI\*CC_0104 : SATA RAID ����
	-- PCI\VEN_1022&DEV_7803 : AMD AHCI Compatible RAID Controller
	-- PCI\VEN_1B21*DEV_06* : ASM1061 SATA IDE Controller(��˶�Ƽ�)
	--
	ִ�н��,SATA�豸��ѯ��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status *CC_0104 PCI\\VEN_1022*DEV_7803 PCI\\VEN_1B21*DEV_06*", SW_HIDE)
	if nil ~= string.find(SATA�豸��ѯ���, "problem", 1) then
		��װSATA����()
	end
	if nil ~= string.find(SATA�豸��ѯ���, "running", 1) then
		��װSATA����_VEN_8086()
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
	-- VEN_106B | Apple Inc. �����豸
	--      -CC_0180 NOT FOUND!
	ִ�н��,�ӽ��̱�׼��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status PCI\\VEN_144D* PCI\\VEN_106B*CC_0180",SW_HIDE)
	if nil ~= string.find(�ӽ��̱�׼���, "Name", 1) then
		sapple()
	end
	
	ɾ���ļ�("X:\\Windows\\desktop\\desktop.ini")
	
	------------------------------------------------------------
	-- ��װ�Զ�����������?:\\WifiDriverĿ¼�£���Ϊcustom*.7z
	------------------------------------------------------------
	�̷�_Z = string.byte("Z",1)
	-- Ѱ��?:\\WifiDriver\\custom(_64)*.7z, ����ҵ��Ͱ�װ��
	local �Զ���������·�� = nil
	for i = 0, 23 do
		�̷� = �̷�_Z - i
		Wifi����Ŀ¼ = string.char(�̷�) .. ":\\WifiDriver"
		if ����·��(Wifi����Ŀ¼) then
			-- �Ѽ���lfs --
			for file in lfs.dir(Wifi����Ŀ¼) do
				if file ~= "." and file ~= ".." then
					local full_path = Wifi����Ŀ¼ .. "\\" .. file
					local attr = lfs.attributes(full_path)
					if attr.mode == "file" then
						-- Ѱ��custom*.7z
						if 1 == string.find(file, "custom", 1) and nil ~= string.find(file, ".7z", 1) then
							�Զ���������·�� = full_path
							break
						end
					end
				end
			end
		end
	end
	if �Զ���������·�� ~= nil then
		��װ�Զ�������(�Զ���������·��)
	end
	
	-- 
	-- �޸�Inter(R) PRO/1000 MT Network Connection
	--
	Log.info("=====================================================")
	Log.info("         ������������")
	Log.info("=====================================================")
	ִ�н��,�ӽ��̱�׼��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status *CC_0200", SW_HIDE)
	if nil ~= string.find(�ӽ��̱�׼���, "problem", 1) then
		Log.info("��Ҫ��װ/�޸�������������")
		����������������()
	end
	
	Log.info("=====================================================")
	Log.info("         ������������")
	Log.info("=====================================================")
	ִ�н��,CC_0280״̬ = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c devcon status *CC_0280", SW_HIDE)
	CC_0280_Problem = false
	if nil ~= string.find(CC_0280״̬, "problem", 1) then
		CC_0280_Problem = true
	end
	
	-- ������������
	-- 1,��setupapi.dev.log�в��ң�����devconɾ��msdri.inf
	ִ�н��,�ӽ��̱�׼��� = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c find \"msdri.inf\'\" " .. ��ȡ��������("windir") .. "\\inf\\setupapi.dev.log", SW_HIDE)
	-- ���������⣬ִ�еĽ����ԶΪ�ա�
	msdri_inf_0x00 = string.find(�ӽ��̱�׼���, "0x00", 1)
	if nil ~= msdri_inf_0x00 then
		posbeg = string.find(�ӽ��̱�׼���, "Published", 1) 
		posend = string.find(�ӽ��̱�׼���, ".", posbeg) -- �ҵ�����Published��һ��
		if posbeg ~= nil and posend ~= nil then
			to_split = string.sub(�ӽ��̱�׼���,posbeg,posend)
			-- ��'�ַ��ָ��ַ���,��Published 'hdaudio.inf_amd64_a9d1a669e70897b9\hdaudio.inf' to 'hdaudio.inf'.
			-- ȡ�������е�hdaudio.inf
			�Ӵ��� = �ָ��ַ���(to_split,"\'")
			if table.maxn(�Ӵ���) >= 4 then
				��Ҫɾ�����ļ� = �Ӵ���[4]
				ִ���ӽ��̲��ȴ������("cmd.exe /c devcon dp_delete " .. ��Ҫɾ�����ļ�)
			end
		end
	end
	-- 2 ������������豸�������У������������߷���
	if nil ~= string.find(CC_0280״̬, "running", 1) then
		�������߷���()
	end
	-- 3 ��װ������������������
	if CC_0280_Problem or msdri_inf_0x00 ~= nil then
		��װ������������������()
	end
	
	--
	-- ����Realtek��������
	-- Realtek USB FE/GbE NIC NDIS6.40 64-bit Driver
	-- rtux64w10.sys
	--
	if ����·��("X:\\Windows\\system32\\drivers\\rtux64w10.sys") then
		���°�װrtux64w10()
	end
	
	--
	-- ��װ��������
	--
	Log.info("=========================================")
	Log.info("          ���ڰ�װ������������")
	Log.info("=========================================")
	local env_windir = ��ȡ��������("windir")
	local b,s = ִ���ӽ��̲�ȡ��׼���("cmd.exe /c drvload " .. env_windir .. "\\inf\\hdaudss.inf " .. env_windir .. "\\inf\\hdaudio.inf")
	Log.info("b = " .. tostring(b) .. ",s=" .. s)
	
	--
	-- ��װOEM��INF (X:\WINDOWS\INF\oem*.inf)
	--
	��װOEM_INF()

	--
	-- ����DPI
	--
	local strdpi = ע��������("HKCU","Control Panel\\Desktop\\WindowMetrics","AppliedDPI")
	local dpi = tonumber(strdpi)
	if dpi ~= nil and dpi ~= 0x60 then
		����ϵͳDPI(dpi)
	end
	
	--
	-- ������ݷ�ʽ
	--
	local ����Ŀ¼ = ��ȡ����Ŀ¼()
	if ����·��("X:\\Program Files\\Network\\[Network]WanDrv6.exe") then
		������ݷ�ʽ(����Ŀ¼ .. "\\������������.lnk","X:\\Program Files\\Network\\[Network]WanDrv6.exe")
	end
	if ����·��("X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe") then
		������ݷ�ʽ(����Ŀ¼ .. "\\�����.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	end
	-- TODO����������������ݷ�ʽ
	-- LINK %desktop%\������������,%windir%\system32\pecmd.exe,"%ProgramFiles%\wifi.wcs",%windir%\system32\netshell.dll#157
	OsExt.CreateShortCutEx(����Ŀ¼ .. "\\������������.lnk", "X:\\Windows\\system32\\pecmd.exe", "\"" .. ProgramFilesĿ¼ .. "\\wifi.wcs\"", "X:\\Windows\\system32\\netshell.dll",157)
	OsExt.CreateShortCut( ����Ŀ¼ .. "\\�������.lnk", "RASPHONE.PBK")
	-- LINK %Desktop%\�������,RASPHONE.PBK
	������ݷ�ʽ(����Ŀ¼ .. "\\�����.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	local ��ʼ�˵�����Ŀ¼ = ��ȡ��ʼ�˵�����Ŀ¼()
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���")
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\Ӳ������")
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\ϵͳά��")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\�������.lnk","X:\\Program Files\\PENetwork\\PENetwork.exe")
	if ����·��("X:\\Program Files\\Network\\[Network]WanDrv6.exe") then
		������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\Ӳ������\\������������.lnk","X:\\Program Files\\Network\\[Network]WanDrv6.exe")
	end
	-- LINK %programs%\Ӳ������\������������,%windir%\system32\pecmd.exe,"%ProgramFiles%\wifi.wcs",%windir%\system32\netshell.dll#157
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\���������.lnk","X:\\Windows\\System32\\taskmgr.exe")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\7z�ļ�.lnk","X:\\Program Files\\7-zip\\7zFM.exe")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\�����.lnk","X:\\Program Files (x86)\\TheWorld\\Application\\theworld.exe")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\���±�.lnk","X:\\Windows\\notepad.exe")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\��Ļ����.lnk","X:\\Windows\\FreeVK.exe")
	--LINK %programs%\ʵ�ù���\��ͼ����,%&myname%,%windir%\wall.ini sn,shell32.dll,141
	OsExt.CreateShortCutEx(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\��ͼ����.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini sn", "netshell.dll", 141)
	--HOTK Win + x,LOAD %windir%\wall.ini sn
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ϵͳά��\\ע���༭����.lnk","X:\\Windows\\regedit.exe")
	local appdata_dir = "X:\\Users\\Default\\AppData\\Roaming"
	local sendto_dir = appdata_dir .. "\\Microsoft\\Windows\\SendTo"
	OsExt.CreateShortCutEx(sendto_dir .. "\\���͵������ݷ�ʽ.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini sd")
	--LINK %sendto%\���͵������ݷ�ʽ,%&myname%,%windir%\wall.ini sd
	sd = ע�����ı�("HKLM","SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96b-e325-11ce-bfc1-08002be10318}\\0000","DriverDate")
	if sd == "" then
		local QuickLaunchDir = appdata_dir .. "\\Microsoft\\Internet Explorer\\Quick Launch"
		OsExt.CreateShortCutEx(QuickLaunchDir .. "\\�������.lnk", "X:\\Windows\\System32\\pecmd.exe", "X:\\Windows\\wall.ini vk", "freevk.exe#0")
		--ifex $%&sd%=,LINK %QuickLaunch%\�������, %&myname%,%windir%\wall.ini vk,freevk.exe#0
	end
	
	--
	-- ���ؿ�ʼ�˵�/����/������, ��ʼ�˵�/����/����
	--
	startup·�� = ��ȡ��������("startup")
	ProgramData·�� = ��ȡ��������("ProgramData")
	�����ļ����ļ�������(ProgramData·�� .. "\\Microsoft\\Windows\\Start Menu\\Programs\\Administrative Tools",FILE_ATTRIBUTE_HIDDEN)
	�����ļ����ļ�������(ProgramData·�� .. "\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp",FILE_ATTRIBUTE_HIDDEN)
	�����ļ����ļ�������(startup·��,FILE_ATTRIBUTE_HIDDEN)
	--
	-- �����ҵĵ��Ե��Ҽ�����
	--
	ע���ɾ������("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\NoPropertiesMyComputer",false)
	ע���ɾ����ֵ("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer","NoPropertiesMyComputer")
	--
	-- ����PsSuspend.exe����Ϊ��ͬ�⡰�����û����Э�顱
	--
	ע���д����("HKLM","Software\\Sysinternals\\PsSuspend","EulaAccepted",0x01)
	
	--
	-- ���м��弴��֧�ֽű�
	--
	Log.info("======================================")
	Log.info("           �����弴��")
	Log.info("======================================")
	ִ���ӽ��̲��ȴ������("cmd.exe /c \"" .. ProgramFilesĿ¼ .. "\\hotplug.bat\" pci",SW_HIDE)
	
	ִ���ӽ���("X:\\Windows\\System32\\taskmgr.exe",SW_HIDE)
	д�����ı�("��ʼ�����",RGB��ɫ,����,�����С,-1,-1, -1, -1)
	�ȴ�(500)
	
	if "X:\\Users\\Default\\AppData\\Roaming" ~= os.getenv("APPDATA") then
		if os.getenv("APPDATA") == nil then
			Log.error("����APPDATA��������ʧ�ܣ���ǰֵΪnil")
		else
			Log.error("����APPDATA��������ʧ�ܣ���ǰֵΪ��" .. os.getenv("APPDATA"))
		end
	end
	--
	-- ֧��USB��������/�����ֻ�����
	-- netrndis.inf
	--     RNDISҲ��Զ�����������ӿ�Э��,�豸ͨ��USB��ʽͬ��������,ģ�����������Ա��������غ͵��Թ���
	-- WceIsVista.INF
	--     Remote NDIS based Internet Sharing Device
	--
	
	-- 1 ��װ������������ʱ�ȹر�IKEEXT
	--     IKEEXT �����й� Internet ��Կ����(IKE)�������֤ Internet Э��(AuthIP)����ģ�顣
	�رշ���("IKEEXT")
	-- 2 ��װ����
	Log.info("=========================================================")
	Log.info("��װRNDIS(Զ�����������ӿ�Э��)����������֧���ֻ���USB����������")
	Log.info("=========================================================")
	local x_windows = ��ȡ��������("windir")
	ִ���ӽ��̲��ȴ������("cmd.exe /c drvload " .. x_windows .. "\\inf\\netrndis.inf " .. x_windows .. "\\inf\\wceisvista.inf")
	-- 3 ��IKEEXT��������Ϊ��������
	ע���д����("HKLM","SYSTEM\\ControlSet001\\Services\\IKEEXT\\start",3)
	ɱ������("taskmgr.exe")
	ɱ������("taskmgr.exe")
	
	--
	-- ���PE��Զ�̰�װ�ģ���ô��Ҫȥ��������ȡһ��pxeautorun.cmd
	-- ���ǵ�Win10 PE��PEBootType����Ramdisk:OpticalDrive
	--
	PE�������� = ע�����ı�("HKLM","SYSTEM\\CurrentControlSet\\Control","PEBootType")
	if string.find(PE��������,"Remote") ~= nil then
		if true == ִ��Զ������() then
			-- ���ִ����Զ�̷����������ص�autorun�Ͳ��ؼ���ִ������Ľű��ˡ�
			return
		end
	end
	--
	-- �������汳�� --
	--
	�̷�_Z = string.byte("Z",1)
	for i = 0, 23 do
		�̷� = �̷�_Z - i
		����ͼƬ·�� = string.char(�̷�) .. ":\\boot\\wall\\winpe.jpg"
		if ����·��(����ͼƬ·��) then
			����ǽֽ(����ͼƬ·��)
			break
		end
	end
	
	--
	-- ��ԭ���ŵ�Y�̵�WIM��ѹ��PEProgram -- 
	--
	д�����ı�("���ڼ�����ع���.....",RGB��ɫ,����,�����С,-1,-1, -1, -1)
	dofile("X:\\Windows\\main.lua")
end

setup()



