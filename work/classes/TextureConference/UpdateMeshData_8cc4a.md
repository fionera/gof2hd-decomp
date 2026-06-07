# TextureConference::UpdateMeshData
elf 0x8cc4a body 192
Sig: undefined __thiscall UpdateMeshData(TextureConference * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TextureConference::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TextureConference::UpdateMeshData
          (TextureConference *this,Mesh *param_1,Engine *param_2)

{
  longlong lVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  longlong lVar5;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x2c),1,0,param_2 + 0x104);
  if (this[9] != (TextureConference)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x30),1,param_2 + 0xd0);
    this[9] = (TextureConference)0x0;
  }
  uVar4 = ApplicationManager::GetElapsedTimeMillis();
  lVar5 = FUN_001aae54((int)uVar4,(int)((ulonglong)uVar4 >> 0x20),5,0);
  lVar5 = lVar5 + *(longlong *)(this + 0x38);
  uVar2 = (uint)lVar5;
  iVar3 = (int)((ulonglong)lVar5 >> 0x20);
  lVar1 = CONCAT44(iVar3 - (uint)(uVar2 < 0xe10),uVar2 - 0xe10);
  if ((int)(-(uint)(0xe10 < uVar2) - iVar3) < 0 ==
      (SBORROW4(0,iVar3) != SBORROW4(-iVar3,(uint)(0xe10 < uVar2)))) {
    lVar1 = lVar5;
  }
  *(longlong *)(this + 0x38) = lVar1;
  glUniform1i(*(undefined4 *)(this + 0x28));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  if ((int)((uint)(byte)*param_1 << 0x1e) < 0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
  }
  return;
}

```

## target disasm
```
  0009cc4a: push {r4,r5,r6,r7,lr}
  0009cc4c: add r7,sp,#0xc
  0009cc4e: push {r6,r7,r8,r9,r11}
  0009cc52: mov r5,r0
  0009cc54: ldr r0,[r0,#0x2c]
  0009cc56: add.w r3,r2,#0x104
  0009cc5a: mov r6,r2
  0009cc5c: mov r9,r1
  0009cc5e: movs r1,#0x1
  0009cc60: movs r2,#0x0
  0009cc62: mov.w r8,#0x0
  0009cc66: blx 0x00070990
  0009cc6a: ldrb r0,[r5,#0x9]
  0009cc6c: cbz r0,0x0009cc7e
  0009cc6e: ldr r0,[r5,#0x30]
  0009cc70: add.w r2,r6,#0xd0
  0009cc74: movs r1,#0x1
  0009cc76: blx 0x00070984
  0009cc7a: strb.w r8,[r5,#0x9]
  0009cc7e: ldr r0,[r6,#0x30]
  0009cc80: blx 0x00070e4c
  0009cc84: movs r2,#0x5
  0009cc86: movs r3,#0x0
  0009cc88: bl 0x001aae54
  0009cc8c: ldrd r3,r6,[r5,#0x38]
  0009cc90: ldr r2,[r5,#0x28]
  0009cc92: adds r0,r0,r3
  0009cc94: adc.w r3,r6,r1
  0009cc98: subs.w r1,r0,#0xe10
  0009cc9c: sbc r6,r3,#0x0
  0009cca0: rsbs.w r4,r0,#0xe10
  0009cca4: sbcs.w r4,r8,r3
  0009cca8: mov.w r4,#0x0
  0009ccac: it lt
  0009ccae: mov.lt r4,#0x1
  0009ccb0: cmp r4,#0x0
  0009ccb2: itt eq
  0009ccb4: mov.eq r6,r3
  0009ccb6: mov.eq r1,r0
  0009ccb8: mov r0,r2
  0009ccba: strd r1,r6,[r5,#0x38]
  0009ccbe: blx 0x0007096c
  0009ccc2: ldr r0,[r5,#0x20]
  0009ccc4: blx 0x00070a5c
  0009ccc8: ldr r0,[r5,#0x24]
  0009ccca: blx 0x00070a5c
  0009ccce: ldr.w r1,[r9,#0x4]
  0009ccd2: movw r2,#0x1406
  0009ccd6: ldr r0,[r5,#0x20]
  0009ccd8: movs r3,#0x0
  0009ccda: strd r8,r1,[sp,#0x0]
  0009ccde: movs r1,#0x3
  0009cce0: blx 0x000709c0
  0009cce4: ldrb.w r0,[r9,#0x0]
  0009cce8: lsls r0,r0,#0x1e
  0009ccea: bpl 0x0009cd04
  0009ccec: ldr.w r1,[r9,#0x8]
  0009ccf0: movs r2,#0x0
  0009ccf2: ldr r0,[r5,#0x24]
  0009ccf4: movs r3,#0x0
  0009ccf6: strd r2,r1,[sp,#0x0]
  0009ccfa: movs r1,#0x2
  0009ccfc: movw r2,#0x1406
  0009cd00: blx 0x000709c0
  0009cd04: pop.w {r2,r3,r8,r9,r11}
  0009cd08: pop {r4,r5,r6,r7,pc}
```
