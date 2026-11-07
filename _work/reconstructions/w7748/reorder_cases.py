import re,sys
p='src/game/world/LevelScript.cpp'
order=[int(x) for x in sys.argv[1].split(',')]
lines=open(p).read().split('\n')
si=None
for i,l in enumerate(lines):
    if l.strip()=='switch (m_nState) {' and 700<i<760: si=i; break
ei=None
for j in range(si+1,len(lines)):
    if lines[j]=='            }': ei=j; break
body=lines[si+1:ei]
blocks=[]; cur=[]; curcase=None
for l in body:
    m=re.match(r'^                case (\d+): \{',l)
    if m:
        if cur: blocks.append((curcase,cur))
        curcase=int(m.group(1)); cur=[l]
    else: cur.append(l)
if cur: blocks.append((curcase,cur))
d=dict(blocks)
assert set(order)==set(d), (set(order),set(d))
new=[]
for c in order: new.extend(d[c])
lines[si+1:ei]=new
open(p,'w').write('\n'.join(lines))
print('reordered',order)
