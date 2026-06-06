# Status::inDeepScienceOrbit
elf 0xabdba body 50
Sig: undefined __stdcall inDeepScienceOrbit(void)

## decompile
```c

/* Status::inDeepScienceOrbit() */

bool Status::inDeepScienceOrbit(void)

{
  int in_r0;
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return false;
  }
  iVar1 = Station::getIndex(*(Station **)(in_r0 + 0x19c));
  if (iVar1 == 10) {
    return true;
  }
  iVar1 = Station::getIndex(*(Station **)(in_r0 + 0x19c));
  return iVar1 == 100;
}

```
## target disasm
```
  000bbdba: push {r4,r6,r7,lr}
  000bbdbc: add r7,sp,#0x8
  000bbdbe: mov r4,r0
  000bbdc0: blx 0x000723d0
  000bbdc4: cbz r0,0x000bbdca
  000bbdc6: movs r0,#0x0
  000bbdc8: pop {r4,r6,r7,pc}
  000bbdca: ldr.w r0,[r4,#0x19c]
  000bbdce: blx 0x00071824
  000bbdd2: cmp r0,#0xa
  000bbdd4: bne 0x000bbdda
  000bbdd6: movs r0,#0x1
  000bbdd8: pop {r4,r6,r7,pc}
  000bbdda: ldr.w r0,[r4,#0x19c]
  000bbdde: blx 0x00071824
  000bbde2: subs r0,#0x64
  000bbde4: clz r0,r0
  000bbde8: lsrs r0,r0,#0x5
  000bbdea: pop {r4,r6,r7,pc}
```
