# -*_coding:gb2312-*-
import os
import codecs
import shutil
import subprocess
from optparse import OptionParser


ultra_iso_path = u"D:\\PE\\UltraISO\\UltraISO.exe"


def main():
    cmdResult = subprocess.call(u'reg.exe UNLOAD HKLM\\AANewBee')
    print u"卸载结果:{0}".format(cmdResult)
    os.remove(ur"D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE")
    shutil.copy(ur"D:\360Downloads\SOFTWARE_newBEE备份",
                ur"D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE")
    if 0 != subprocess.call(ur'reg.exe LOAD HKLM\AANewBee "D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE"'):
        print u"执行挂载操作失败了"
        return False
    print u"已加载SOFTWARE"
    print u"导入Classes"
    if 0 != subprocess.call(
            ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes\In_A_NOT_In_B.txt.reg'):
        print u"导入'Classes' 失败了"
        return False
    print u"导入Classes_CLSID"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_CLSID\In_A_NOT_In_B.txt.reg'):
        print u"导入Classes_CLSID失败了"
        return False
    print u"导入Interface"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_Interface\In_A_NOT_In_B.txt.reg'):
        print u"导入Classes_Interface失败了"
        return False
    print u"导入Classes_WOW6432Node_CLSID"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_WOW6432Node_CLSID\In_A_NOT_In_B.txt.reg'):
        print u"导入Classes_Interface失败了"
        return False
    print u"导入Classes_WOW6432Node_Interface"
    if 0 != subprocess.call(
            ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_WOW6432Node_Interface\In_A_NOT_In_B.txt.reg'):
        print u"导入Classes_Interface失败了"
        return False
    print u"导入Microsoft"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Microsoft\In_A_NOT_In_B.txt.reg'):
        print u"导入Microsoft失败了"
        return False
    print u"导入IE"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Internet Explorer\Internet Explorer.reg"'):
        print u"导入IE失败了"
        return False
    print u"导入Microsoft_Windows NT_CurrentVersion"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows NT_CurrentVersion\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows NT_CurrentVersion失败了！"
        return False
    print u"导入Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensions"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensions\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensions失败了！"
        return False
    print u"导入Microsoft_Windows_CurrentVersion"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion失败了！"
        return False
    print u"导入Microsoft_Windows_CurrentVersion_Explorer"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_Explorer\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion_Explorer失败了！"
        return False
    print u"导入Microsoft_Windows_CurrentVersion_SideBySide_Winners"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_SideBySide_Winners\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion_SideBySide_Winners失败了！"
        return False
    print u"导入Microsoft_Windows_CurrentVersion_WINEVT_Channels"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_WINEVT_Channels\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion_WINEVT_Channels失败了！"
        return False
    print u"导入Microsoft_Windows_CurrentVersion_WINEVT_Publishers"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_WINEVT_Publishers\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion_WINEVT_Publishers失败了！"
        return False
    print u"导入Microsoft_WindowsRuntime_ActivatableClassId"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_WindowsRuntime_ActivatableClassId\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_Windows_CurrentVersion_WINEVT_Channels失败了！"
        return False
    print u"导入Microsoft_WindowsRuntime_CLSID"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_WindowsRuntime_CLSID\In_A_NOT_In_B.txt.reg"'):
        print u"导入Microsoft_WindowsRuntime_CLSID失败了！"
        return False
    print u"导入Wow6432Node"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Wow6432Node\In_A_NOT_In_B.txt.reg"'):
        print u"导入Wow6432Node失败了！"
        return False
    print u"导入WOW6432Node_Microsoft"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\WOW6432Node_Microsoft\In_A_NOT_In_B.txt.reg"'):
        print u"导入WOW6432Node_Microsoft失败了！"
        return False
    print u"成功"


def get_file_mame(name):
    pos = name.rfind('.')
    if pos == -1:
        return None
    else:
        return name[:pos]


def wim_save_as(dir, wim_file, bootable=False):
    name = os.path.basename(wim_file)
    name = get_file_mame(name)
    if bootable:
        cmdline = u'dism /Capture-Image /ImageFile:"{0}" /CaptureDir:"{1}" /Name:{2} /Compress:Max /Bootable'.format(wim_file, dir, name)
    else:
        cmdline = u'dism /Capture-Image /ImageFile:"{0}" /CaptureDir:"{1}" /Name:{2} /Compress:Max'.format(wim_file, dir, name)
    print u'另存为命令：{0}'.format(cmdline)
    return subprocess.call(cmdline)


def iso_replace(iso_path, dir_in_iso, file_add_or_replace, isboot):
    u"""
    在ISO文件中增加或者替换一个文件
    :param iso_path: 需要替换文件或者增加文件的ISO全路径
    :param dir_in_iso: 文件添加到ISO中的路径
    :param file_add_or_replace: 将要替换或者增加到ISO中的文件
    :param isboot: ISO是否可引导，这个参数暂时保留未用。
    :return: 如果成功则返回0，否则返回其他值。
    """
    if isboot:
        print(u"保留未用")
    cmdline = '{0} -in {1} -chdir {2} -f {3}'.format(ultra_iso_path, iso_path, dir_in_iso, file_add_or_replace)
    print u"执行命令 ",cmdline
    return subprocess.call(cmdline)


def main2():
    cmdResult = subprocess.call(u'reg.exe UNLOAD HKLM\\AANewBee')
    print u"卸载结果:{0}".format(cmdResult)

    if 0 != wim_save_as(r'D:\360Downloads\cmp2\BootNew', r"D:\360Downloads\boot_newbee.wim", True):
        print u"保存WIM到boot_newbee.wim失败了"
        return False
    if os.path.exists(r"D:\360Downloads\boot.wim"):
        os.remove(r"D:\360Downloads\boot.wim")
    os.rename(r"D:\360Downloads\boot_newbee.wim", r"D:\360Downloads\boot.wim")
    if 0 != iso_replace(r"D:\360Downloads\10PEx64.iso",
                        r'/sources',
                        r"D:\360Downloads\boot.wim",
                        False):
        print u"打包WIM到10PEx64.iso失败了"
        return False

    return True


if __name__ == '__main__':
    parser = OptionParser(usage="%prog [options]")
    parser.add_option("-p", "--pack", action="store_true", dest="pack", default=False,
                      help=u"执行后段任务。")
    (options, args) = parser.parse_args()
    if not options.pack:
        main()
    else:
        main2()

