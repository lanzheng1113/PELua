# -*_coding:gb2312-*-
import os
import codecs
from optparse import OptionParser


def bu(source_file):
    if source_file is None:
        print u"你必须指定要处理的清单文件"
    else:
        fpath = os.path.dirname(source_file)
        fn = os.path.basename(source_file)
        list_file = open(source_file, "r")
        vec = []
        for line in list_file:
            a = line.strip()
            b = "[-" + a + "]"
            vec.append(b)
        list_file.close()
        fn = fn + ".reg"
        print "write to " + fn
        reg_file = open(os.path.join(fpath,fn), "w")
        reg_file.write("Windows Registry Editor Version 5.00\r\n\r\n")
        for i in vec:
            reg_file.write(i + "\r\n")
        reg_file.close()


def fen(caifen_file):
    if not os.path.exists(caifen_file):
        print(u"需要拆分的文件并不存在")
        return False
    ppath = os.path.dirname(caifen_file)
    print u"文件路径:{0}".format(ppath)

    pfn = os.path.basename(caifen_file)
    print u"文件名:{0}".format(pfn)

    fp = open(caifen_file)
    arc = []
    for line in fp:
        line = line.strip()
        line = line.strip('\r\n')
        if line == '' or line == "Windows Registry Editor Version 5.00":
            continue
        arc.append(line)
    fp.close()
    length_of_arc = len(arc)
    if length_of_arc == 0 or length_of_arc == 1:
        print(u"少于可分割的大小，无法分割")
        return 0

    first_part = arc[:length_of_arc/2]
    second_part = arc[length_of_arc/2:]
    pfn_no_ext = pfn[:pfn.rfind(".")]
    pfn_ext = pfn[pfn.rfind("."):]
    print u"第一部分！"
    pfn_first_part = pfn_no_ext + "_l" + pfn_ext
    fp_first = open(os.path.join(ppath,pfn_first_part),"w")
    fp_first.write("Windows Registry Editor Version 5.00\r\n")
    for i in first_part:
        fp_first.write(i + "\r\n")
    fp_first.close()

    print u"第二部分！"
    pfn_second_part = pfn_no_ext + "_r" + pfn_ext
    fp_second = open(os.path.join(ppath, pfn_second_part), "w")
    fp_second.write("Windows Registry Editor Version 5.00\r\n")
    for i in second_part:
        fp_second.write(i + "\r\n")
    fp_second.close()

if __name__ == '__main__':
    u"""
    这个脚本用于将补集清单文件改成.reg。
    """
    parser = OptionParser(usage="%prog [options]")
    parser.add_option("-s", "--source", action="store", type="string", dest="source_file")
    parser.add_option("-c", "--chaifen", action="store", type="string", dest="caifen_file")
    (options, args) = parser.parse_args()
    if options.source_file:
        bu(options.source_file)
    elif options.caifen_file:
        fen(options.caifen_file)


