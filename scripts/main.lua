dofile("const.lua")

-- ǰ�ö��嵼�����������ı��� --
������ʾ = OsExt.MessageBox

-- ����1 ����·��+���̲���
-- ����2 ��ʾ�ӽ��̴��ڣ�SW_SHOW�������ӽ��̴��ڣ�SW_HIDE
ִ���ӽ��� = OsExt.Exec

-- ����1 ����·��+���̲���
-- ����2 ��ʾ�ӽ��̴��ڣ�SW_SHOW�������ӽ��̴��ڣ�SW_HIDE
ִ���ӽ��̲��ȴ������ = OsExt.ExecWait

-- ����1 �ļ����ļ���·��
����·�� = OsExt.PathFileExist

Program·�� = "X:\\Program Files"

function �ͷ�SOFT2_WIM()
	�̷�_Z = string.byte("Z",1)
	for i = 0, 23 do
		�̷� = �̷�_Z - i
		WIM·�� = string.char(�̷�) .. ":\\BOOT\\EXTAPP\\X86\\SOFT2.WIM"
		--������ʾ(NULL,WIM·��,"·��",0)
		if ����·��(WIM·��) then
			��ѹ����·�� = "\"" .. Program·�� .. "\\7-zip\\7z.exe" .. "\""
			���̲��� = ��ѹ����·�� .. " x " .. WIM·�� .. " -oX:\\PEPrograms\\X86Soft2  -aoa"
			ִ���ӽ��̲��ȴ������(���̲���,SW_HIDE)
			--������ʾ(NULL,"SOFT2 Extract complete!","����",MB_OK)
			--��ѹ��ִ��Soft2Reg.lua��������ݷ�ʽ�ȡ�
			--�����Ϊִ��X:\PEPrograms\\X86Soft2\\Soft2Reg.lua
			if ����·��("X:\\PEPrograms\\X86Soft2\\Soft2Reg.lua") then
				dofile("X:\\PEPrograms\\X86Soft2\\Soft2Reg.lua")
			end
			break
		end
	end
end

function �ͷ�OEM_WIM()
  �̷�_Z = string.byte("Z",1)
	for i = 0, 23 do
		�̷� = �̷�_Z - i
		WIM·�� = string.char(�̷�) .. ":\\BOOT\\EXTAPP\\X86\\OEM.WIM"
		--������ʾ(NULL,WIM·��,"·��",0)
		if ����·��(WIM·��) then
			��ѹ����·�� = "\"" .. Program·�� .. "\\7-zip\\7z.exe" .. "\""
			���̲��� = ��ѹ����·�� .. " x " .. WIM·�� .. " -oX:\\PEPrograms\\oem  -aoa"
			ִ���ӽ��̲��ȴ������(���̲���,SW_HIDE)
			if ����·��("X:\\PEPrograms\\oem\\oem.lua") then
				dofile("X:\\PEPrograms\\oem\\oem.lua")
			end
			--������ʾ(NULL,"SOFT2 Extract complete!","����",MB_OK)
			break
		end
	end  
end

-- ������ -- 
function Main()
	�ͷ�SOFT2_WIM()
	�ͷ�OEM_WIM()
	--������ʾ(NULL,"Extract complete!","����",MB_OK)
end

Main()

