#!/usr/bin/python
#coding:utf-8
#----------------------------------
#只支持两种数据类型 uint32_t, uint8_t*
import re

def pack_method(name, fields):
    header = 'void %s_pack(struct %s * s, uint8_t * data, uint32_t len)' % (name, name)
    content = '{\n'
    for f in fields:
        if re.search('uint32_t', f):
            n = re.sub('uint32_t', '', f).strip()
            content += '\tput32bit(&data, s -> %s);\n' % n
        elif re.search('uint8_t *\*', f):
            n = re.sub('uint8_t *\*', '', f).strip()
            content += '\tputstr(&data, s -> %slength, s -> %s);\n' % (n, n)
    content += "}\n"
    return (header, content)

def unpack_method(name, fields):
    header = 'void %s_unpack(struct %s * s, const uint8_t * data, uint32_t len)' % (name, name)
    content = '{\n'
    for f in fields:
        if re.search('uint32_t', f):
            n = re.sub('uint32_t', '', f).strip()
            content += '\ts -> %s = get32bit(&data);\n' % n
        elif re.search('uint8_t *\*', f):
            n = re.sub('uint8_t *\*', '', f).strip()
            content += '\tgetstr(&data, s -> %slength, &(s -> %s));\n' % (n, n)
    content += "}\n"
    return (header, content)

protocol_h_templete_file = 'common/protocol.input.h'
protocol_h_file = 'common/protocol.h'
protocol_c_file = 'common/protocol.c'
protocol_c_test_file = 'test/protocol_test.t.c'

protocol_c_file_header = """
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "protocol.h"

"""

protocol_c_test_file_header = """
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "test.h"

"""

def file_clean(fname):
    f = file(fname, 'w') 
    f.write('') 
    f.close() 

def file_append(fname, statement):
    f = file(fname, 'a') 
    f.write(statement) 
    f.close() 

def deal_struct(name, fields):
    print 'struct', name
    fields = re.sub('/\*.*?\*/', '', fields) #del comments

    fields = fields.split(';')
    fields = [f.strip() for f in fields]

    for f in fields:
        print '\t', f 
    (head, body) = pack_method(name, fields)
    file_append(protocol_c_file, head+body)
    file_append(protocol_h_file, head+';\n')

    (head, body) = unpack_method(name, fields)
    file_append(protocol_c_file, head+body)
    file_append(protocol_h_file, head+';\n')

def init(input_file):
    file_clean(protocol_h_file)
    file_clean(protocol_c_test_file)
    file_clean(protocol_c_file)

    file_append(protocol_h_file, input_file)
    file_append(protocol_c_test_file, protocol_c_test_file_header)
    file_append(protocol_c_file, protocol_c_file_header)

def main():
    input_file = open(protocol_h_templete_file, 'rb').read()
    init(input_file)
    structs = re.findall(r'typedef struct (.*){(.*?)}(\1)', input_file, re.DOTALL)## 
    for struct in structs:
        name = struct[0]
        fields = struct[1]
        fields = fields.split('/*--')[0]
        #print fields
        deal_struct(name, fields)

if __name__ == '__main__':
    main()






