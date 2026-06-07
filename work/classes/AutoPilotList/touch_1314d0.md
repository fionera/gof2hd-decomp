# AutoPilotList::touch
elf 0x1314d0 body 88
Sig: undefined __thiscall touch(AutoPilotList * this, int param_1, int param_2)

## decompile
```c

/* AutoPilotList::touch(int, int) */

undefined4 __thiscall AutoPilotList::touch(AutoPilotList *this,int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if ((((param_1 < *(int *)(this + 4)) || (*(int *)(this + 4) + *(int *)(this + 0xc) <= param_1)) ||
      (iVar1 = *(int *)(this + 8), iVar2 = __aeabi_idiv((param_2 - iVar1) + -0xe,0xf),
      param_2 - iVar1 < -0xf)) || (**(uint **)(this + 0x10) <= iVar2 + 1U)) {
    uVar3 = 0xffffffff;
  }
  else {
    *(undefined4 *)this = 0;
    for (iVar1 = 0; iVar1 <= iVar2; iVar1 = iVar1 + 1) {
      down(this);
    }
    uVar3 = *(undefined4 *)this;
  }
  return uVar3;
}

```

## target disasm
```
  001414d0: push {r4,r5,r6,r7,lr}
  001414d2: add r7,sp,#0xc
  001414d4: push.w r11
  001414d8: mov r4,r0
  001414da: ldr r0,[r0,#0x4]
  001414dc: cmp r0,r1
  001414de: bgt 0x0014151e
  001414e0: ldr r3,[r4,#0xc]
  001414e2: add r0,r3
  001414e4: cmp r0,r1
  001414e6: ble 0x0014151e
  001414e8: ldr r0,[r4,#0x8]
  001414ea: movs r1,#0xf
  001414ec: subs r6,r2,r0
  001414ee: sub.w r0,r6,#0xe
  001414f2: blx 0x0007198c
  001414f6: cmn r6,#0xf
  001414fa: blt 0x0014151e
  001414fc: mov r5,r0
  001414fe: ldr r0,[r4,#0x10]
  00141500: adds r1,r5,#0x1
  00141502: ldr r0,[r0,#0x0]
  00141504: cmp r1,r0
  00141506: bcs 0x0014151e
  00141508: movs r6,#0x0
  0014150a: str r6,[r4,#0x0]
  0014150c: b 0x00141516
  0014150e: mov r0,r4
  00141510: blx 0x000768c4
  00141514: adds r6,#0x1
  00141516: cmp r6,r5
  00141518: ble 0x0014150e
  0014151a: ldr r0,[r4,#0x0]
  0014151c: b 0x00141522
  0014151e: mov.w r0,#0xffffffff
  00141522: pop.w r11
  00141526: pop {r4,r5,r6,r7,pc}
```
