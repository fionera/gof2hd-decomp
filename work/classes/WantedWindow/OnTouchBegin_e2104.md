# WantedWindow::OnTouchBegin
elf 0xe2104 body 152
Sig: undefined __thiscall OnTouchBegin(WantedWindow * this, int param_1, int param_2)

## decompile
```c

/* WantedWindow::OnTouchBegin(int, int) */

undefined4 __thiscall WantedWindow::OnTouchBegin(WantedWindow *this,int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  if (this[0x14] == (WantedWindow)0x0) {
    *(int *)(this + 0x9c) = param_2;
    *(int *)(this + 0x88) = param_2;
    *(undefined4 *)(this + 0x90) = 0;
    this[0xa0] = (WantedWindow)0x1;
    ScrollTouchWindow::OnTouchBegin(*(int *)(this + 0x2c),param_1);
    for (uVar3 = 0; uVar3 < **(uint **)(this + 0xc); uVar3 = uVar3 + 1) {
      TouchButton::OnTouchBegin(*(int *)((*(uint **)(this + 0xc))[1] + uVar3 * 4),param_1);
    }
    Layout::OnTouchBegin(**(int **)(DAT_000f219c + 0xf2164),param_1);
    if ((*(int *)(this + 0x18) != 0) &&
       (iVar1 = Wanted::isActive(*(Wanted **)
                                  (*(int *)(*(int *)(this + 0x38) + 4) + *(int *)(this + 0x30) * 4))
       , iVar1 != 0)) {
      TouchButton::OnTouchBegin(*(int *)(this + 0x18),param_1);
    }
    uVar2 = getWantedAtPosition(this,param_1,param_2);
    *(undefined4 *)(this + 0x34) = uVar2;
  }
  else {
    StarMap::OnTouchBegin(*(int *)(this + 4),param_1);
  }
  return 0;
}

```

## target disasm
```
  000f2104: push {r4,r5,r6,r7,lr}
  000f2106: add r7,sp,#0xc
  000f2108: push.w r8
  000f210c: mov r5,r0
  000f210e: ldrb r0,[r0,#0x14]
  000f2110: mov r4,r2
  000f2112: mov r8,r1
  000f2114: cbz r0,0x000f2122
  000f2116: ldr r0,[r5,#0x4]
  000f2118: mov r1,r8
  000f211a: mov r2,r4
  000f211c: blx 0x00074704
  000f2120: b 0x000f2194
  000f2122: movs r6,#0x0
  000f2124: str.w r4,[r5,#0x9c]
  000f2128: str.w r4,[r5,#0x88]
  000f212c: movs r1,#0x1
  000f212e: str.w r6,[r5,#0x90]
  000f2132: mov r2,r4
  000f2134: ldr r0,[r5,#0x2c]
  000f2136: strb.w r1,[r5,#0xa0]
  000f213a: mov r1,r8
  000f213c: blx 0x00075ba4
  000f2140: b 0x000f2152
  000f2142: ldr r0,[r0,#0x4]
  000f2144: mov r1,r8
  000f2146: mov r2,r4
  000f2148: ldr.w r0,[r0,r6,lsl #0x2]
  000f214c: blx 0x00074734
  000f2150: adds r6,#0x1
  000f2152: ldr r0,[r5,#0xc]
  000f2154: ldr r1,[r0,#0x0]
  000f2156: cmp r6,r1
  000f2158: bcc 0x000f2142
  000f215a: ldr r0,[0x000f219c]
  000f215c: mov r1,r8
  000f215e: mov r2,r4
  000f2160: add r0,pc
  000f2162: ldr r0,[r0,#0x0]
  000f2164: ldr r0,[r0,#0x0]
  000f2166: blx 0x00074728
  000f216a: ldr r0,[r5,#0x18]
  000f216c: cbz r0,0x000f2188
  000f216e: ldr r1,[r5,#0x38]
  000f2170: ldr r0,[r5,#0x30]
  000f2172: ldr r1,[r1,#0x4]
  000f2174: ldr.w r0,[r1,r0,lsl #0x2]
  000f2178: blx 0x00073774
  000f217c: cbz r0,0x000f2188
  000f217e: ldr r0,[r5,#0x18]
  000f2180: mov r1,r8
  000f2182: mov r2,r4
  000f2184: blx 0x00074734
  000f2188: mov r0,r5
  000f218a: mov r1,r8
  000f218c: mov r2,r4
  000f218e: blx 0x00075bb0
  000f2192: str r0,[r5,#0x34]
  000f2194: movs r0,#0x0
  000f2196: pop.w r8
  000f219a: pop {r4,r5,r6,r7,pc}
```
