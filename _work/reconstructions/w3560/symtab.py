import struct,sys,os
def parse(p):
    d=open(p,'rb').read()
    if d[:4]!=b'\x7fELF': return []
    shoff=struct.unpack_from('<I',d,0x20)[0]; shentsize=struct.unpack_from('<H',d,0x2e)[0]; shnum=struct.unpack_from('<H',d,0x30)[0]; shstrndx=struct.unpack_from('<H',d,0x32)[0]
    S=[]
    for i in range(shnum):
        off=shoff+i*shentsize
        v=struct.unpack_from('<IIIIIIIIII',d,off)
        S.append(dict(name=v[0],type=v[1],flags=v[2],addr=v[3],offset=v[4],size=v[5],link=v[6],info=v[7],align=v[8],entsize=v[9]))
    st=S[shstrndx]
    for s in S: s['sname']=d[st['offset']+s['name']:d.index(b'\0',st['offset']+s['name'])].decode()
    out=[]
    for s in S:
        if s['type'] not in (2,11): continue
        strtab=S[s['link']]
        for i in range(s['size']//16):
            off=s['offset']+i*16
            name,value,size,info,other,shndx=struct.unpack_from('<IIIBBH',d,off)
            nm=d[strtab['offset']+name:d.index(b'\0',strtab['offset']+name)].decode()
            bind=info>>4; typ=info&0xf
            out.append(dict(name=nm,value=value,size=size,bind=bind,typ=typ,shndx=shndx,tab=s['sname']))
    return out
if __name__=='__main__':
    for s in parse(sys.argv[1]):
        if s['bind'] in (1,2) and s['name']:
            print(s['tab'],s['bind'],s['typ'],s['shndx'],s['name'])
