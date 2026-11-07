import re,sys,struct,difflib
def classify(val):
    if val==0: return 'Z'
    if 0x1000 <= val < 0x100000: return 'A'
    if 0x100000 <= val < 0x200000: return 'C'
    f=struct.unpack('<f',struct.pack('<I',val))[0]
    if abs(f)>1e-9 and abs(f)<1e12 and (val & 0x7f800000)!=0x7f800000:
        return 'F:%r'%f
    return 'I:%x'%val
def parse(fn):
    seq=[]
    for line in open(fn):
        m=re.match(r'([0-9a-f]{8}) (\S.*?) -> ([0-9a-f]{8}) ([0-9a-f]{8}) (.*)$',line)
        if not m: continue
        addr=int(m.group(1),16); val=int(m.group(4),16)
        seq.append((addr,classify(val),val))
    return seq
a=parse(sys.argv[1]); b=parse(sys.argv[2])
print(len(a),len(b))
sa=[x[1] for x in a]; sb=[x[1] for x in b]
sm=difflib.SequenceMatcher(None,sa,sb,autojunk=False)
print('ratio',sm.ratio())
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal':
        print(tag,'O[%d:%d]'%(i1,i2),'U[%d:%d]'%(j1,j2))
        print('  O:',' | '.join(sa[i1:i2])[:300])
        print('  U:',' | '.join(sb[j1:j2])[:300])
