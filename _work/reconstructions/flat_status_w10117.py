#!/usr/bin/env python3
import sys, os
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w10117/_work/reconstructions')
os.environ['W9620_SCRATCH']='/dev/shm/w10117/run'
import pgo_combine_w9620 as h
m=h.get_meta()
G1=[167,174,8,0,11,13,14]
G2=[150,151,152,153,154,155,158,162,166,170,184,189]
G3=[156,157,160,163,164,165,168,169,171,172]
def vals_for(groups):
    vals=[1]*m['ncounters']
    for g,val in zip(groups,(4,3,2)):
        for c in g:
            vals[m['case_ctr'][c]]=val
    return vals
extra=[]
if '--extra' in sys.argv:
    i=sys.argv.index('--extra'); extra=sys.argv[i+1:]; sys.argv=sys.argv[:i]
groups=[G1,G2,G3]
if len(sys.argv)>1:
    # custom: three ints
    groups=[G1,G2,G3]
    gv=[int(x) for x in sys.argv[1:4]]
    vals=[1]*m['ncounters']
    for g,val in zip(groups,gv):
        for c in g: vals[m['case_ctr'][c]]=val
else:
    vals=vals_for(groups)
tag='flat'
r=h.build(m,vals,tag,extra)
print('flat raw',r['raw'],'aligned',r['aligned'],'size',r['size'],'eager',r['eager'],'other',r['other_same'],'/',r['other_total'])
print('so',r['so'])
