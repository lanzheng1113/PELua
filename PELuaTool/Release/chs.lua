-- 库函数的中文别名 --
-- chs包--
local CURRENT_MODULE_NAME = chs

chs = {}

IDOK=1
IDCANCEL=2
IDABORT=3
IDRETRY=4
IDIGNORE=5
IDYES=6
IDNO=7
MB_YESNO = 4

chs.弹窗提示 = OsExt.MessageBox
 
function chs.测试(标题,内容)
    弹窗提示(0,内容,标题,0)
end
OsExt.MessageBox(0,"123","456",0)
return chs

