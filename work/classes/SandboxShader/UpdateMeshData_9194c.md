# SandboxShader::UpdateMeshData
elf 0x9194c body 372
Sig: undefined __thiscall UpdateMeshData(SandboxShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::SandboxShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::SandboxShader::UpdateMeshData(SandboxShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x34),1,0,param_2 + 0x104);
  if (this[9] != (SandboxShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x40),1,param_2 + 0xd0);
    glUniform3f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    glUniform3f(*(undefined4 *)(this + 0x3c),*(undefined4 *)(param_2 + 0x34c),
                *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    this[9] = (SandboxShader)0x0;
  }
  pcVar1 = *(code **)(DAT_000a1ac0 + 0xa199c);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  (*pcVar1)(*(undefined4 *)(this + 0x2c));
  (*pcVar1)(*(undefined4 *)(this + 0x30));
  if (param_1[0x5c] != (Mesh)0x0) {
    pcVar1 = *(code **)(DAT_000a1ac4 + 0xa19c0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_000a1ac8 + 0xa19d6);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
    return;
  }
  pcVar1 = *(code **)(DAT_000a1acc + 0xa1a5e);
  (*pcVar1)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  (*pcVar1)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
  (*pcVar1)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
  (*pcVar1)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
  (*pcVar1)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
  return;
}

```

## target disasm
```
  000a194c: push {r4,r5,r6,r7,lr}
  000a194e: add r7,sp,#0xc
  000a1950: push {r6,r7,r8,r9,r11}
  000a1954: mov r4,r0
  000a1956: ldr r0,[r0,#0x34]
  000a1958: add.w r3,r2,#0x104
  000a195c: mov r6,r2
  000a195e: mov r8,r1
  000a1960: movs r1,#0x1
  000a1962: movs r2,#0x0
  000a1964: movs r5,#0x0
  000a1966: blx 0x00070990
  000a196a: ldrb r0,[r4,#0x9]
  000a196c: cbz r0,0x000a1994
  000a196e: ldr r0,[r4,#0x40]
  000a1970: add.w r2,r6,#0xd0
  000a1974: movs r1,#0x1
  000a1976: blx 0x00070984
  000a197a: add.w r3,r6,#0x330
  000a197e: ldmia r3,{r1,r2,r3}
  000a1980: ldr r0,[r4,#0x38]
  000a1982: blx 0x000709b4
  000a1986: add.w r3,r6,#0x34c
  000a198a: ldmia r3,{r1,r2,r3}
  000a198c: ldr r0,[r4,#0x3c]
  000a198e: blx 0x000709b4
  000a1992: strb r5,[r4,#0x9]
  000a1994: ldr r1,[0x000a1ac0]
  000a1996: ldr r0,[r4,#0x20]
  000a1998: add r1,pc
  000a199a: ldr r5,[r1,#0x0]
  000a199c: blx r5
  000a199e: ldr r0,[r4,#0x24]
  000a19a0: blx r5
  000a19a2: ldr r0,[r4,#0x28]
  000a19a4: blx r5
  000a19a6: ldr r0,[r4,#0x2c]
  000a19a8: blx r5
  000a19aa: ldr r0,[r4,#0x30]
  000a19ac: blx r5
  000a19ae: ldrb.w r0,[r8,#0x5c]
  000a19b2: cmp r0,#0x0
  000a19b4: beq 0x000a1a4e
  000a19b6: ldr r0,[0x000a1ac4]
  000a19b8: ldr.w r1,[r8,#0x60]
  000a19bc: add r0,pc
  000a19be: ldr r5,[r0,#0x0]
  000a19c0: movw r0,#0x8892
  000a19c4: blx r5
  000a19c6: ldr r1,[0x000a1ac8]
  000a19c8: mov.w r9,#0x0
  000a19cc: ldr r0,[r4,#0x20]
  000a19ce: movw r2,#0x1406
  000a19d2: add r1,pc
  000a19d4: movs r3,#0x0
  000a19d6: strd r9,r9,[sp,#0x0]
  000a19da: ldr r6,[r1,#0x0]
  000a19dc: movs r1,#0x3
  000a19de: blx r6
  000a19e0: ldr.w r1,[r8,#0x68]
  000a19e4: movw r0,#0x8892
  000a19e8: blx r5
  000a19ea: ldr r0,[r4,#0x24]
  000a19ec: movs r1,#0x2
  000a19ee: movw r2,#0x1406
  000a19f2: movs r3,#0x0
  000a19f4: strd r9,r9,[sp,#0x0]
  000a19f8: blx r6
  000a19fa: ldr.w r1,[r8,#0x6c]
  000a19fe: movw r0,#0x8892
  000a1a02: blx r5
  000a1a04: ldr r0,[r4,#0x28]
  000a1a06: movs r1,#0x3
  000a1a08: movw r2,#0x1406
  000a1a0c: movs r3,#0x0
  000a1a0e: strd r9,r9,[sp,#0x0]
  000a1a12: blx r6
  000a1a14: ldr.w r1,[r8,#0x70]
  000a1a18: movw r0,#0x8892
  000a1a1c: blx r5
  000a1a1e: ldr r0,[r4,#0x2c]
  000a1a20: movs r1,#0x3
  000a1a22: movw r2,#0x1406
  000a1a26: movs r3,#0x0
  000a1a28: strd r9,r9,[sp,#0x0]
  000a1a2c: blx r6
  000a1a2e: ldr.w r1,[r8,#0x74]
  000a1a32: movw r0,#0x8892
  000a1a36: blx r5
  000a1a38: ldr r0,[r4,#0x30]
  000a1a3a: movs r1,#0x3
  000a1a3c: movw r2,#0x1406
  000a1a40: movs r3,#0x0
  000a1a42: strd r9,r9,[sp,#0x0]
  000a1a46: blx r6
  000a1a48: pop.w {r2,r3,r8,r9,r11}
  000a1a4c: pop {r4,r5,r6,r7,pc}
  000a1a4e: ldr r0,[r4,#0x20]
  000a1a50: movs r6,#0x0
  000a1a52: ldr.w r1,[r8,#0x4]
  000a1a56: movs r3,#0x0
  000a1a58: ldr r2,[0x000a1acc]
  000a1a5a: add r2,pc
  000a1a5c: strd r6,r1,[sp,#0x0]
  000a1a60: movs r1,#0x3
  000a1a62: ldr r5,[r2,#0x0]
  000a1a64: movw r2,#0x1406
  000a1a68: blx r5
  000a1a6a: ldr.w r1,[r8,#0x8]
  000a1a6e: movw r2,#0x1406
  000a1a72: ldr r0,[r4,#0x24]
  000a1a74: movs r3,#0x0
  000a1a76: strd r6,r1,[sp,#0x0]
  000a1a7a: movs r1,#0x2
  000a1a7c: blx r5
  000a1a7e: ldr.w r1,[r8,#0x10]
  000a1a82: movw r2,#0x1406
  000a1a86: ldr r0,[r4,#0x28]
  000a1a88: movs r3,#0x0
  000a1a8a: strd r6,r1,[sp,#0x0]
  000a1a8e: movs r1,#0x3
  000a1a90: blx r5
  000a1a92: ldr.w r1,[r8,#0x14]
  000a1a96: movw r2,#0x1406
  000a1a9a: ldr r0,[r4,#0x2c]
  000a1a9c: movs r3,#0x0
  000a1a9e: strd r6,r1,[sp,#0x0]
  000a1aa2: movs r1,#0x3
  000a1aa4: blx r5
  000a1aa6: ldr.w r1,[r8,#0x18]
  000a1aaa: movw r2,#0x1406
  000a1aae: ldr r0,[r4,#0x30]
  000a1ab0: movs r3,#0x0
  000a1ab2: strd r6,r1,[sp,#0x0]
  000a1ab6: movs r1,#0x3
  000a1ab8: blx r5
  000a1aba: pop.w {r2,r3,r8,r9,r11}
  000a1abe: pop {r4,r5,r6,r7,pc}
```
