# SpaceLounge::getSpecificSoundForRace
elf 0x17177c body 108
Sig: undefined __thiscall getSpecificSoundForRace(SpaceLounge * this, int param_1, int param_2, bool param_3)

## decompile
```c

/* SpaceLounge::getSpecificSoundForRace(int, int, bool) */

int __thiscall
SpaceLounge::getSpecificSoundForRace(SpaceLounge *this,int param_1,int param_2,bool param_3)

{
  switch(param_2) {
  case 0:
  case 5:
    if (param_3) {
      if (param_1 - 0x2f7U < 0x30) {
        return param_1 + 0xf0;
      }
    }
    else if (param_1 - 0x2f7U < 0x30) {
      return param_1 + 0xc0;
    }
    break;
  case 1:
    if (param_1 - 0x2f7U < 0x30) {
      return param_1 + 0x120;
    }
    break;
  case 2:
  case 3:
    if (param_1 - 0x2f7U < 0x30) {
      return param_1 + 0x90;
    }
    break;
  case 4:
    if (param_1 - 0x2f7U < 0x30) {
      return param_1 + 0x60;
    }
    break;
  case 6:
    goto switchD_00181780_caseD_6;
  case 7:
    if (param_1 - 0x2f7U < 0x30) {
      return param_1 + 0x30;
    }
  }
  param_1 = -1;
switchD_00181780_caseD_6:
  return param_1;
}

```

## target disasm
```
  0018177c: cmp r2,#0x7
  0018177e: bhi 0x001817e8
  00181780: tbb [pc,r2]
  0018178c: subw r0,r1,#0x2f7
  00181790: cbz r3,0x001817de
  00181792: cmp r0,#0x30
  00181794: itt cc
  00181796: add.cc.w r0,r1,#0xf0
  0018179a: bx.cc lr
  0018179c: b 0x001817e8
  0018179e: subw r0,r1,#0x2f7
  001817a2: cmp r0,#0x30
  001817a4: itt cc
  001817a6: add.cc.w r0,r1,#0x90
  001817aa: bx.cc lr
  001817ac: b 0x001817e8
  001817ae: subw r0,r1,#0x2f7
  001817b2: cmp r0,#0x30
  001817b4: itt cc
  001817b6: add.cc.w r0,r1,#0x120
  001817ba: bx.cc lr
  001817bc: b 0x001817e8
  001817be: subw r0,r1,#0x2f7
  001817c2: cmp r0,#0x30
  001817c4: itt cc
  001817c6: add.cc.w r0,r1,#0x60
  001817ca: bx.cc lr
  001817cc: b 0x001817e8
  001817ce: subw r0,r1,#0x2f7
  001817d2: cmp r0,#0x30
  001817d4: itt cc
  001817d6: add.cc.w r0,r1,#0x30
  001817da: bx.cc lr
  001817dc: b 0x001817e8
  001817de: cmp r0,#0x30
  001817e0: itt cc
  001817e2: add.cc.w r0,r1,#0xc0
  001817e6: bx.cc lr
  001817e8: mov.w r1,#0xffffffff
  001817ec: mov r0,r1
  001817ee: bx lr
```
