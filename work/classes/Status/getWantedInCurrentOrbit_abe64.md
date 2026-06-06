# Status::getWantedInCurrentOrbit
elf 0xabe64 body 130
Sig: undefined __thiscall getWantedInCurrentOrbit(Status * this)

## decompile
```c

/* Status::getWantedInCurrentOrbit() */

Wanted * __thiscall Status::getWantedInCurrentOrbit(Status *this)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  Wanted *this_00;
  
  puVar1 = *(uint **)this;
  this_00 = (Wanted *)0x0;
  if (puVar1 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      iVar2 = Wanted::isActive(*(Wanted **)(puVar1[1] + uVar4 * 4));
      if ((iVar2 != 0) &&
         (iVar2 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar4 * 4)),
         iVar2 == 0)) {
        iVar2 = Wanted::getCurrentLocation(*(Wanted **)(*(int *)(*(int *)this + 4) + uVar4 * 4));
        iVar3 = Station::getIndex(*(Station **)(this + 0x19c));
        if (iVar2 == iVar3) {
          if (this_00 != (Wanted *)0x0) {
            iVar2 = Wanted::getRequiredBounties(this_00);
            iVar3 = Wanted::getRequiredBounties
                              (*(Wanted **)(*(int *)(*(int *)this + 4) + uVar4 * 4));
            if (iVar3 <= iVar2) goto LAB_000bbed4;
          }
          this_00 = *(Wanted **)(*(int *)(*(int *)this + 4) + uVar4 * 4);
        }
      }
LAB_000bbed4:
      puVar1 = *(uint **)this;
    }
  }
  return this_00;
}

```
## target disasm
```
  000bbe64: push {r4,r5,r6,r7,lr}
  000bbe66: add r7,sp,#0xc
  000bbe68: push.w r8
  000bbe6c: mov r5,r0
  000bbe6e: ldr r0,[r0,#0x0]
  000bbe70: mov.w r8,#0x0
  000bbe74: cbz r0,0x000bbede
  000bbe76: movs r4,#0x0
  000bbe78: b 0x000bbed8
  000bbe7a: ldr r0,[r0,#0x4]
  000bbe7c: ldr.w r0,[r0,r4,lsl #0x2]
  000bbe80: blx 0x00073774
  000bbe84: cbz r0,0x000bbed4
  000bbe86: ldr r0,[r5,#0x0]
  000bbe88: ldr r0,[r0,#0x4]
  000bbe8a: ldr.w r0,[r0,r4,lsl #0x2]
  000bbe8e: blx 0x00071a04
  000bbe92: cbnz r0,0x000bbed4
  000bbe94: ldr r0,[r5,#0x0]
  000bbe96: ldr r0,[r0,#0x4]
  000bbe98: ldr.w r0,[r0,r4,lsl #0x2]
  000bbe9c: blx 0x00073780
  000bbea0: mov r6,r0
  000bbea2: ldr.w r0,[r5,#0x19c]
  000bbea6: blx 0x00071824
  000bbeaa: cmp r6,r0
  000bbeac: bne 0x000bbed4
  000bbeae: cmp.w r8,#0x0
  000bbeb2: beq 0x000bbecc
  000bbeb4: mov r0,r8
  000bbeb6: blx 0x00073918
  000bbeba: mov r6,r0
  000bbebc: ldr r0,[r5,#0x0]
  000bbebe: ldr r0,[r0,#0x4]
  000bbec0: ldr.w r0,[r0,r4,lsl #0x2]
  000bbec4: blx 0x00073918
  000bbec8: cmp r6,r0
  000bbeca: bge 0x000bbed4
  000bbecc: ldr r0,[r5,#0x0]
  000bbece: ldr r0,[r0,#0x4]
  000bbed0: ldr.w r8,[r0,r4,lsl #0x2]
  000bbed4: ldr r0,[r5,#0x0]
  000bbed6: adds r4,#0x1
  000bbed8: ldr r1,[r0,#0x0]
  000bbeda: cmp r4,r1
  000bbedc: bcc 0x000bbe7a
  000bbede: mov r0,r8
  000bbee0: pop.w r8
  000bbee4: pop {r4,r5,r6,r7,pc}
```
