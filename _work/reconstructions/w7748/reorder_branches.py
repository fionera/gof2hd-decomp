import re,sys
p='src/game/world/LevelScript.cpp'
order=[int(x) for x in sys.argv[1].split(',')]
s=open(p).read()
start=s.index('        } else if (m_nState == 3) {')
end=s.index('        } else {\n            switch (m_nState) {',start)
seg=s[start:end]
# split into blocks each starting with '        } else if (m_nState == N'
parts=re.split(r'(?=        \} else if \(m_nState == \d)',seg)
head=parts[0]; blocks=[]
for part in parts[1:]:
    m=re.match(r'        \} else if \(m_nState == (\d)',part)
    blocks.append((int(m.group(1)),part))
print('blocks',[b[0] for b in blocks])
d=dict(blocks)
assert set(order)==set(d)
new=''.join(d[c] for c in order)
s=s[:start]+new+s[end:]
open(p,'w').write(s)
print('reordered',order)
