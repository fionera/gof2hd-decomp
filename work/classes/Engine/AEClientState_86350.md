# Engine::AEClientState
elf 0x86350 body 246
Sig: undefined __thiscall AEClientState(Engine * this, uint param_1, bool param_2)

## decompile
```c

/* AbyssEngine::Engine::AEClientState(unsigned int, bool) */

void __thiscall AbyssEngine::Engine::AEClientState(Engine *this,uint param_1,bool param_2)

{
  uint uVar1;
  
  if (param_2) {
    switch(param_1) {
    case 0x8074:
      if ((int)((uint)(byte)this[0x4a4] << 0x1e) < 0) {
        return;
      }
      glEnableClientState(0x8074);
      uVar1 = *(uint *)(this + 0x4a4) | 2;
      break;
    case 0x8075:
      if (((byte)this[0x4a4] & 1) != 0) {
        return;
      }
      glEnableClientState(0x8075);
      uVar1 = *(uint *)(this + 0x4a4) | 1;
      break;
    case 0x8076:
      if ((int)((uint)(byte)this[0x4a4] << 0x1c) < 0) {
        return;
      }
      glEnableClientState(0x8076);
      uVar1 = *(uint *)(this + 0x4a4) | 8;
      break;
    default:
      goto switchD_00096364_caseD_8077;
    case 0x8078:
      if ((int)((uint)(byte)this[0x4a4] << 0x1d) < 0) {
        return;
      }
      glEnableClientState(0x8078);
      uVar1 = *(uint *)(this + 0x4a4) | 4;
    }
  }
  else {
    switch(param_1) {
    case 0x8074:
      if (-1 < (int)((uint)(byte)this[0x4a4] << 0x1e)) {
        return;
      }
      glDisableClientState(0x8074);
      uVar1 = *(uint *)(this + 0x4a4) & 0xfffffffd;
      break;
    case 0x8075:
      if (((byte)this[0x4a4] & 1) == 0) {
        return;
      }
      glDisableClientState(0x8075);
      uVar1 = *(uint *)(this + 0x4a4) & 0xfffffffe;
      break;
    case 0x8076:
      if (-1 < (int)((uint)(byte)this[0x4a4] << 0x1c)) {
        return;
      }
      glDisableClientState(0x8076);
      uVar1 = *(uint *)(this + 0x4a4) & 0xfffffff7;
      break;
    default:
      goto switchD_00096364_caseD_8077;
    case 0x8078:
      if (-1 < (int)((uint)(byte)this[0x4a4] << 0x1d)) {
        return;
      }
      glDisableClientState(0x8078);
      uVar1 = *(uint *)(this + 0x4a4) & 0xfffffffb;
    }
  }
  *(uint *)(this + 0x4a4) = uVar1;
switchD_00096364_caseD_8077:
  return;
}

```

## target disasm
```
  00096350: push {r4,r6,r7,lr}
  00096352: add r7,sp,#0x8
  00096354: mov r4,r0
  00096356: movw r0,#0x8074
  0009635a: subs r0,r1,r0
  0009635c: cbz r2,0x00096388
  0009635e: cmp r0,#0x4
  00096360: bhi.w 0x00096450
  00096364: tbb [pc,r0]
  0009636e: ldrb.w r0,[r4,#0x4a4]
  00096372: lsls r0,r0,#0x1e
  00096374: bmi 0x00096450
  00096376: movw r0,#0x8074
  0009637a: blx 0x000707e0
  0009637e: ldr.w r0,[r4,#0x4a4]
  00096382: orr r0,r0,#0x2
  00096386: b 0x0009644c
  00096388: cmp r0,#0x4
  0009638a: bhi 0x00096450
  0009638c: tbb [pc,r0]
  00096396: ldrb.w r0,[r4,#0x4a4]
  0009639a: lsls r0,r0,#0x1e
  0009639c: bpl 0x00096450
  0009639e: movw r0,#0x8074
  000963a2: blx 0x000707ec
  000963a6: ldr.w r0,[r4,#0x4a4]
  000963aa: bic r0,r0,#0x2
  000963ae: b 0x0009644c
  000963b0: ldrb.w r0,[r4,#0x4a4]
  000963b4: lsls r0,r0,#0x1f
  000963b6: bne 0x00096450
  000963b8: movw r0,#0x8075
  000963bc: blx 0x000707e0
  000963c0: ldr.w r0,[r4,#0x4a4]
  000963c4: orr r0,r0,#0x1
  000963c8: b 0x0009644c
  000963ca: ldrb.w r0,[r4,#0x4a4]
  000963ce: lsls r0,r0,#0x1c
  000963d0: bmi 0x00096450
  000963d2: movw r0,#0x8076
  000963d6: blx 0x000707e0
  000963da: ldr.w r0,[r4,#0x4a4]
  000963de: orr r0,r0,#0x8
  000963e2: b 0x0009644c
  000963e4: ldrb.w r0,[r4,#0x4a4]
  000963e8: lsls r0,r0,#0x1d
  000963ea: bmi 0x00096450
  000963ec: movw r0,#0x8078
  000963f0: blx 0x000707e0
  000963f4: ldr.w r0,[r4,#0x4a4]
  000963f8: orr r0,r0,#0x4
  000963fc: b 0x0009644c
  000963fe: ldrb.w r0,[r4,#0x4a4]
  00096402: lsls r0,r0,#0x1f
  00096404: beq 0x00096450
  00096406: movw r0,#0x8075
  0009640a: blx 0x000707ec
  0009640e: ldr.w r0,[r4,#0x4a4]
  00096412: bic r0,r0,#0x1
  00096416: b 0x0009644c
  00096418: ldrb.w r0,[r4,#0x4a4]
  0009641c: lsls r0,r0,#0x1c
  0009641e: it pl
  00096420: pop.pl {r4,r6,r7,pc}
  00096422: movw r0,#0x8076
  00096426: blx 0x000707ec
  0009642a: ldr.w r0,[r4,#0x4a4]
  0009642e: bic r0,r0,#0x8
  00096432: b 0x0009644c
  00096434: ldrb.w r0,[r4,#0x4a4]
  00096438: lsls r0,r0,#0x1d
  0009643a: bpl 0x00096450
  0009643c: movw r0,#0x8078
  00096440: blx 0x000707ec
  00096444: ldr.w r0,[r4,#0x4a4]
  00096448: bic r0,r0,#0x4
  0009644c: str.w r0,[r4,#0x4a4]
  00096450: pop {r4,r6,r7,pc}
```
