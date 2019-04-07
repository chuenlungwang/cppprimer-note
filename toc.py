#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import os
import re

headPattern = re.compile(r"\s*((#+)\s*(.*))")
blockPattern = re.compile(r"\s*(````)")

def listfiles(dir):
    files = os.listdir(dir)
    files.sort()
    for i in xrange(len(files)):
        files[i] = dir + "/" + files[i]
    return files

def parsefile(f):
    ret = {"file":f,"heads":[]}
    codeblock = False
    for line in file(f):
        if codeblock:
            m = False
            if blockPattern.match(line):
                codeblock = False
        else:
            m = headPattern.match(line)
            if blockPattern.match(line):
                codeblock = True
        if m:
            # 移除 Windows 的换行符 \r
            hstr = m.group(3).replace('\r', '')
            ret['heads'].append((len(m.group(2)), hstr))
    return ret

def tomarkdown(fstruct):
    str_lst = []
    pattern = re.compile(ur"""[!\"#$%&'()*+,./:;<=>?@[\]^`{|}~]""")
    str_lst.append("\n[{}]({})".format(fstruct['file'], fstruct['file']))
    for h in fstruct['heads']:
        hstr = h[1]
        hstr = pattern.sub('', hstr)
        hstr = hstr.replace('（', '')
        hstr = hstr.replace('）', '')
        hstr = hstr.replace('：', '')
        hstr = hstr.replace(' ', '-')
        str_lst.append("{}- [{}]({}#{})".format(" "*(h[0]-1)*2, h[1], fstruct['file'], hstr))
    return '\n'.join(str_lst)

if __name__ == "__main__":
    for f in listfiles("docs"):
        fstruct = parsefile(f)
        print(tomarkdown(fstruct))