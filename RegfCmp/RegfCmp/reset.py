# -*_coding:gb2312-*-
import os
import codecs
import shutil
import subprocess
from optparse import OptionParser


ultra_iso_path = u"D:\\PE\\UltraISO\\UltraISO.exe"


def main():
    cmdResult = subprocess.call(u'reg.exe UNLOAD HKLM\\AANewBee')
    print u"ж�ؽ��:{0}".format(cmdResult)
    os.remove(ur"D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE")
    shutil.copy(ur"D:\360Downloads\SOFTWARE_newBEE����",
                ur"D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE")
    if 0 != subprocess.call(ur'reg.exe LOAD HKLM\AANewBee "D:\360Downloads\cmp2\BootNew\Windows\System32\config\SOFTWARE"'):
        print u"ִ�й��ز���ʧ����"
        return False
    print u"�Ѽ���SOFTWARE"
    print u"����Classes"
    if 0 != subprocess.call(
            ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes\In_A_NOT_In_B.txt.reg'):
        print u"����'Classes' ʧ����"
        return False
    print u"����Classes_CLSID"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_CLSID\In_A_NOT_In_B.txt.reg'):
        print u"����Classes_CLSIDʧ����"
        return False
    print u"����Interface"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_Interface\In_A_NOT_In_B.txt.reg'):
        print u"����Classes_Interfaceʧ����"
        return False
    print u"����Classes_WOW6432Node_CLSID"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_WOW6432Node_CLSID\In_A_NOT_In_B.txt.reg'):
        print u"����Classes_Interfaceʧ����"
        return False
    print u"����Classes_WOW6432Node_Interface"
    if 0 != subprocess.call(
            ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Classes_WOW6432Node_Interface\In_A_NOT_In_B.txt.reg'):
        print u"����Classes_Interfaceʧ����"
        return False
    print u"����Microsoft"
    if 0 != subprocess.call(ur'reg.exe import E:\source\github\PELua\RegfCmp\Debug\Microsoft\In_A_NOT_In_B.txt.reg'):
        print u"����Microsoftʧ����"
        return False
    print u"����IE"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Internet Explorer\Internet Explorer.reg"'):
        print u"����IEʧ����"
        return False
    print u"����Microsoft_Windows NT_CurrentVersion"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows NT_CurrentVersion\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows NT_CurrentVersionʧ���ˣ�"
        return False
    print u"����Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensions"
    if 0 != subprocess.call(ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensions\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows NT_CurrentVersion_Winlogon_GPExtensionsʧ���ˣ�"
        return False
    print u"����Microsoft_Windows_CurrentVersion"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersionʧ���ˣ�"
        return False
    print u"����Microsoft_Windows_CurrentVersion_Explorer"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_Explorer\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersion_Explorerʧ���ˣ�"
        return False
    print u"����Microsoft_Windows_CurrentVersion_SideBySide_Winners"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_SideBySide_Winners\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersion_SideBySide_Winnersʧ���ˣ�"
        return False
    print u"����Microsoft_Windows_CurrentVersion_WINEVT_Channels"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_WINEVT_Channels\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersion_WINEVT_Channelsʧ���ˣ�"
        return False
    print u"����Microsoft_Windows_CurrentVersion_WINEVT_Publishers"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_Windows_CurrentVersion_WINEVT_Publishers\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersion_WINEVT_Publishersʧ���ˣ�"
        return False
    print u"����Microsoft_WindowsRuntime_ActivatableClassId"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_WindowsRuntime_ActivatableClassId\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_Windows_CurrentVersion_WINEVT_Channelsʧ���ˣ�"
        return False
    print u"����Microsoft_WindowsRuntime_CLSID"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Microsoft_WindowsRuntime_CLSID\In_A_NOT_In_B.txt.reg"'):
        print u"����Microsoft_WindowsRuntime_CLSIDʧ���ˣ�"
        return False
    print u"����Wow6432Node"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\Wow6432Node\In_A_NOT_In_B.txt.reg"'):
        print u"����Wow6432Nodeʧ���ˣ�"
        return False
    print u"����WOW6432Node_Microsoft"
    if 0 != subprocess.call(
            ur'reg.exe import "E:\source\github\PELua\RegfCmp\Debug\WOW6432Node_Microsoft\In_A_NOT_In_B.txt.reg"'):
        print u"����WOW6432Node_Microsoftʧ���ˣ�"
        return False
    print u"�ɹ�"


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
    print u'���Ϊ���{0}'.format(cmdline)
    return subprocess.call(cmdline)


def iso_replace(iso_path, dir_in_iso, file_add_or_replace, isboot):
    u"""
    ��ISO�ļ������ӻ����滻һ���ļ�
    :param iso_path: ��Ҫ�滻�ļ����������ļ���ISOȫ·��
    :param dir_in_iso: �ļ���ӵ�ISO�е�·��
    :param file_add_or_replace: ��Ҫ�滻�������ӵ�ISO�е��ļ�
    :param isboot: ISO�Ƿ�����������������ʱ����δ�á�
    :return: ����ɹ��򷵻�0�����򷵻�����ֵ��
    """
    if isboot:
        print(u"����δ��")
    cmdline = '{0} -in {1} -chdir {2} -f {3}'.format(ultra_iso_path, iso_path, dir_in_iso, file_add_or_replace)
    print u"ִ������ ",cmdline
    return subprocess.call(cmdline)


def main2():
    cmdResult = subprocess.call(u'reg.exe UNLOAD HKLM\\AANewBee')
    print u"ж�ؽ��:{0}".format(cmdResult)

    if 0 != wim_save_as(r'D:\360Downloads\cmp2\BootNew', r"D:\360Downloads\boot_newbee.wim", True):
        print u"����WIM��boot_newbee.wimʧ����"
        return False
    if os.path.exists(r"D:\360Downloads\boot.wim"):
        os.remove(r"D:\360Downloads\boot.wim")
    os.rename(r"D:\360Downloads\boot_newbee.wim", r"D:\360Downloads\boot.wim")
    if 0 != iso_replace(r"D:\360Downloads\10PEx64.iso",
                        r'/sources',
                        r"D:\360Downloads\boot.wim",
                        False):
        print u"���WIM��10PEx64.isoʧ����"
        return False

    return True


if __name__ == '__main__':
    parser = OptionParser(usage="%prog [options]")
    parser.add_option("-p", "--pack", action="store_true", dest="pack", default=False,
                      help=u"ִ�к������")
    (options, args) = parser.parse_args()
    if not options.pack:
        main()
    else:
        main2()

