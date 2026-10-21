import sys,itertools,json,struct
import os; sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import emu
F=['iPadHD','n9','iPad','iPadLarge','retinaDisplay']
sym=sys.argv[1]; so=sys.argv[2] if len(sys.argv)>2 else None
if so:
    emu.e=emu.V.Elf32(so); emu.got=emu.e.got_map()
res={}
for combo in itertools.product([0,1],repeat=5):
    fl=dict(zip(F,combo))
    st=emu.run(sym,fl)
    res[''.join(map(str,combo))]={str(k):v for k,v in st.items()}
json.dump(res,open(sys.argv[3] if len(sys.argv)>3 else '/dev/stdout','w'))
