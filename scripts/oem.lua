-- ����oem�Ŀ�ݷ�ʽ --

-- ע���ɾ��������
-- ����1 ����
-- ����2 ��Ҫɾ����·��
-- ����3 һ��booleanֵ�����Ϊtrue�����������������Ӽ���ɾ�������Ϊfalse�������Ƿ����Ӽ�������ֱ��ɾ���������м�ֵ�ͱ��
-- ����һ��booleanֵ���ڱ�ʶɾ���Ƿ�ɹ���
ע���ɾ���� = RegIni.DeleteRegKey

-- ע���д����˵��
-- ��ע�����д��һ��DWORD��If the registry key doesn't exist it will be created.
-- �ĸ�RegIni.WriteRegDWORD�Ĳ����������£�
-- ����(string)����Ч�ĸ���"HKLM"->HKEY_LOCAL_MACHINE,"HKCU"->HKEY_CURRENT_USER ...
-- �ӽ�·��(string)
-- ����(string)
-- DWORDֵ(integer)
-- ����ֵ������Ϊboolean�ɹ�����trueʧ�ܷ���false.
-- WriteRegDWORD("HKLM","Software\\My Company\\My Software","DWORD Value",0xDEADBEEF)
ע���д���� = RegIni.WriteRegDWORD
ע���д�ַ��� = RegIni.WriteRegStr

function ��ȡ����Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end

function ��ȡ��ʼ�˵�Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end

function ��ȡ��ʼ�˵�����Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_PROGRAMS)
end

function ע��oem()
	-- �����ݷ�ʽ
	����Ŀ¼ = ��ȡ����Ŀ¼()
	--��ʼ�˵�
	��ʼ�˵�Ŀ¼ = ��ȡ��ʼ�˵�Ŀ¼()
	--��ʼ�˵�/����
	��ʼ�˵�����Ŀ¼ = ��ȡ��ʼ�˵�����Ŀ¼()
	
	������ݷ�ʽ = OsExt.CreateShortCut
	����Ŀ¼ = OsExt.CreateDir        -- ��Ŀ¼�������,�����ʧ��
	�𼶴���Ŀ¼ = OsExt.CreateDirRecursively -- ��Ŀ¼�����������һ������
	����·�� = OsExt.PathFileExist
	
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\���ݻ�ԭ")
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���")
	����Ŀ¼(��ʼ�˵�����Ŀ¼ .. "\\�����޸�")
	
	--���ݻ�ԭ����
	����Ŀ¼ = "X:\\PEPrograms\\oem"
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\���ݻ�ԭ\\װ����ʦ.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	������ݷ�ʽ(����Ŀ¼ .. "\\װ����ʦ.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\װ����ʦ.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\SystemInstallTool.EXE")
	
	-- �����޸�
	if ����·��(����Ŀ¼ .. "\\SystemInstallTool\\tools\\BootFix.exe") then
		������ݷ�ʽ(����Ŀ¼ .. "\\�����޸�.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\BootFix.exe")
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�����޸�.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\BootFix.exe")
		������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\�����޸�\\�����޸�.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\BootFix.exe")
	end
	
	-- �����
	������ݷ�ʽ(����Ŀ¼ .. "\\�����.lnk", ����Ŀ¼ .. "\\TheWorld\\Application\\theworld.exe")
	������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\ʵ�ù���\\�����.lnk", ����Ŀ¼ .. "\\TheWorld\\Application\\theworld.exe")
	
	-- GHOST
	if ����·��(����Ŀ¼ .. "\\SystemInstallTool\\tools\\GHOST.EXE") then
		������ݷ�ʽ(����Ŀ¼ .. "\\�ֶ�Ghost.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\GHOST.EXE")
		������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\���ݻ�ԭ\\�ֶ�Ghost.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\GHOST.EXE")
	end
	
	-- EveryThing
	if ����·��(����Ŀ¼ .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�ļ���������.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe")
		������ݷ�ʽ(��ʼ�˵�����Ŀ¼ .. "\\�ļ���������.lnk", ����Ŀ¼ .. "\\SystemInstallTool\\tools\\EveryThing\\Everything.exe")
	end
	
	-- ˢ������
	-- SEND #0x74
	-- SEND VK_F5
	֪ͨϵͳ�ļ����������Ѹı� = OsExt.ShellNotifyAssoChanged
	֪ͨϵͳ�ļ����������Ѹı�()
	
	-- ��������SystemInstallTool.exe
	-- ����1 ����·��+���̲���
	-- ����2 ��ʾ�ӽ��̴��ڣ�SW_SHOW�������ӽ��̴��ڣ�SW_HIDE
	ִ���ӽ��� = OsExt.Exec
	ִ���ӽ���(����Ŀ¼ .. "\\SystemInstallTool\\SystemInstallTool.EXE", SW_SHOW)
	OsExt.Sleep(1024)
	
	-- ɾ��PECMD��ʱĿ¼
	-- PECMD TEMP @DELETE
end

ע��oem()