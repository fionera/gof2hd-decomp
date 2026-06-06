# Status::inSupernovaOrbit
elf 0xabd98 body 34
Sig: undefined __stdcall inSupernovaOrbit(void)

## decompile
```c

/* Status::inSupernovaOrbit() */

bool Status::inSupernovaOrbit(void)

{
  int in_r0;
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return false;
  }
  iVar1 = Station::getIndex(*(Station **)(in_r0 + 0x19c));
  return iVar1 == 0x6d;
}

```
## target disasm
```
  000bbd98: push {r4,r6,r7,lr}
  000bbd9a: add r7,sp,#0x8
  000bbd9c: mov r4,r0
  000bbd9e: blx 0x000723d0
  000bbda2: cbz r0,0x000bbda8
  000bbda4: movs r0,#0x0
  000bbda6: pop {r4,r6,r7,pc}
  000bbda8: ldr.w r0,[r4,#0x19c]
  000bbdac: blx 0x00071824
  000bbdb0: subs r0,#0x6d
  000bbdb2: clz r0,r0
  000bbdb6: lsrs r0,r0,#0x5
  000bbdb8: pop {r4,r6,r7,pc}
```
