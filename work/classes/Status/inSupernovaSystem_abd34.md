# Status::inSupernovaSystem
elf 0xabd34 body 44
Sig: undefined __stdcall inSupernovaSystem(void)

## decompile
```c

/* Status::inSupernovaSystem() */

undefined4 Status::inSupernovaSystem(void)

{
  int in_r0;
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return 0;
  }
  iVar1 = SolarSystem::getIndex();
  uVar2 = 0;
  if ((iVar1 == 0x1b) && (*(int *)(in_r0 + 0x1e8) < 0x9e)) {
    uVar2 = 1;
  }
  return uVar2;
}

```
## target disasm
```
  000bbd34: push {r4,r6,r7,lr}
  000bbd36: add r7,sp,#0x8
  000bbd38: mov r4,r0
  000bbd3a: blx 0x000723d0
  000bbd3e: cbz r0,0x000bbd44
  000bbd40: movs r0,#0x0
  000bbd42: pop {r4,r6,r7,pc}
  000bbd44: ldr.w r0,[r4,#0x1a4]
  000bbd48: blx 0x00071a7c
  000bbd4c: mov r1,r0
  000bbd4e: movs r0,#0x0
  000bbd50: cmp r1,#0x1b
  000bbd52: bne 0x000bbd5e
  000bbd54: ldr.w r1,[r4,#0x1e8]
  000bbd58: cmp r1,#0x9e
  000bbd5a: it lt
  000bbd5c: mov.lt r0,#0x1
  000bbd5e: pop {r4,r6,r7,pc}
```
