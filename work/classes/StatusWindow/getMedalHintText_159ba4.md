# StatusWindow::getMedalHintText
elf 0x159ba4 body 952
Sig: undefined __stdcall getMedalHintText(int param_1)

## decompile
```c

/* StatusWindow::getMedalHintText(int) */

void StatusWindow::getMedalHintText(int param_1)

{
  int iVar1;
  int *piVar2;
  char *pcVar3;
  uint *puVar4;
  int in_r2;
  int *piVar5;
  uint uVar6;
  int *piVar7;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar5 = *(int **)(DAT_00169f6c + 0x169bba);
  local_28 = *piVar5;
  iVar1 = Achievements::getMedals((Achievements *)**(undefined4 **)(DAT_00169f68 + 0x169bbc));
  iVar1 = *(int *)(iVar1 + in_r2 * 4);
  AbyssEngine::String::String((String *)param_1);
  if (iVar1 != 0) {
    if (in_r2 == 2 && iVar1 == 2) {
      AbyssEngine::String::String(aSStack_40,(char *)(DAT_00169fd0 + 0x169cc2),false);
      piVar7 = *(int **)(DAT_00169fd4 + 0x169cce);
      GameText::getText(*piVar7);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::~String(aSStack_40);
      piVar2 = *(int **)(DAT_00169fd8 + 0x169d08);
      pcVar3 = (char *)(DAT_00169fdc + 0x169d0e);
      for (uVar6 = 0; puVar4 = *(uint **)(*piVar2 + 0x94), uVar6 < *puVar4; uVar6 = uVar6 + 1) {
        if (*(char *)(puVar4[1] + uVar6) == '\0') {
          AbyssEngine::String::String(aSStack_40,pcVar3,false);
          GameText::getText(*piVar7);
          AbyssEngine::operator+(aSStack_34,aSStack_40);
          AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
          AbyssEngine::String::~String(aSStack_34);
          AbyssEngine::String::~String(aSStack_40);
        }
      }
    }
    else if (in_r2 == 3 && iVar1 == 2) {
      AbyssEngine::String::String(aSStack_40,(char *)(DAT_00169fe0 + 0x169d60),false);
      piVar7 = *(int **)(DAT_00169fe4 + 0x169d6c);
      GameText::getText(*piVar7);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::~String(aSStack_40);
      piVar2 = *(int **)(DAT_00169fe8 + 0x169da6);
      pcVar3 = (char *)(DAT_00169fec + 0x169dac);
      for (uVar6 = 0; puVar4 = *(uint **)(*piVar2 + 0x98), uVar6 < *puVar4; uVar6 = uVar6 + 1) {
        if (*(char *)(puVar4[1] + uVar6) == '\0') {
          AbyssEngine::String::String(aSStack_40,pcVar3,false);
          GameText::getText(*piVar7);
          AbyssEngine::operator+(aSStack_34,aSStack_40);
          AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
          AbyssEngine::String::~String(aSStack_34);
          AbyssEngine::String::~String(aSStack_40);
        }
      }
    }
    else if (in_r2 == 9 && iVar1 == 2) {
      AbyssEngine::String::String(aSStack_40,(char *)(DAT_00169ff0 + 0x169dfe),false);
      piVar7 = *(int **)(DAT_00169ff4 + 0x169e0a);
      GameText::getText(*piVar7);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::~String(aSStack_40);
      piVar2 = *(int **)(DAT_00169ff8 + 0x169e44);
      pcVar3 = (char *)(DAT_00169ffc + 0x169e4a);
      for (uVar6 = 0; puVar4 = *(uint **)(*piVar2 + 0xac), uVar6 < *puVar4; uVar6 = uVar6 + 1) {
        if (*(char *)(puVar4[1] + uVar6) == '\0') {
          AbyssEngine::String::String(aSStack_40,pcVar3,false);
          GameText::getText(*piVar7);
          AbyssEngine::operator+(aSStack_34,aSStack_40);
          AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
          AbyssEngine::String::~String(aSStack_34);
          AbyssEngine::String::~String(aSStack_40);
        }
      }
    }
    else if (in_r2 == 0xd && iVar1 == 2) {
      AbyssEngine::String::String(aSStack_40,(char *)(DAT_0016a000 + 0x169e9c),false);
      piVar2 = *(int **)(DAT_0016a004 + 0x169ea8);
      GameText::getText(*piVar2);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::~String(aSStack_40);
      piVar7 = *(int **)(DAT_0016a008 + 0x169ee0);
      pcVar3 = (char *)(DAT_0016a00c + 0x169ee8);
      for (uVar6 = 0; uVar6 < 0xd; uVar6 = uVar6 + 1) {
        if (*(char *)(*(int *)(*(int *)(*(int *)(*piVar7 + 0x18) + 4) + uVar6 * 4) + 8) == '\0') {
          AbyssEngine::String::String(aSStack_40,pcVar3,false);
          iVar1 = *piVar2;
          BluePrint::getIndex(*(BluePrint **)(*(int *)(*(int *)(*piVar7 + 0x18) + 4) + uVar6 * 4));
          GameText::getText(iVar1);
          AbyssEngine::operator+(aSStack_34,aSStack_40);
          AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
          AbyssEngine::String::~String(aSStack_34);
          AbyssEngine::String::~String(aSStack_40);
        }
      }
    }
    else if (in_r2 == 0xe && iVar1 == 2) {
      AbyssEngine::String::String(aSStack_40,(char *)(DAT_00169f74 + 0x169c10),false);
      piVar2 = *(int **)(DAT_00169f78 + 0x169c1c);
      GameText::getText(*piVar2);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::~String(aSStack_40);
      piVar7 = *(int **)(DAT_00169f7c + 0x169c54);
      pcVar3 = (char *)(DAT_00169f80 + 0x169c5c);
      for (uVar6 = 0; uVar6 < 0xd; uVar6 = uVar6 + 1) {
        if (*(int *)(*(int *)(*(int *)(*(int *)(*piVar7 + 0x18) + 4) + uVar6 * 4) + 0xc) == 0) {
          AbyssEngine::String::String(aSStack_40,pcVar3,false);
          iVar1 = *piVar2;
          BluePrint::getIndex(*(BluePrint **)(*(int *)(*(int *)(*piVar7 + 0x18) + 4) + uVar6 * 4));
          GameText::getText(iVar1);
          AbyssEngine::operator+(aSStack_34,aSStack_40);
          AbyssEngine::String::operator+=((String *)param_1,aSStack_34);
          AbyssEngine::String::~String(aSStack_34);
          AbyssEngine::String::~String(aSStack_40);
        }
      }
    }
  }
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00169ba4: push {r4,r5,r6,r7,lr}
  00169ba6: add r7,sp,#0xc
  00169ba8: push {r8,r9,r10,r11}
  00169bac: sub sp,#0x2c
  00169bae: mov r8,r0
  00169bb0: ldr r0,[0x00169f6c]
  00169bb2: ldr r1,[0x00169f68]
  00169bb4: mov r4,r2
  00169bb6: add r0,pc
  00169bb8: add r1,pc
  00169bba: ldr r2,[r0,#0x0]
  00169bbc: ldr r0,[r1,#0x0]
  00169bbe: str r2,[sp,#0xc]
  00169bc0: ldr r1,[r2,#0x0]
  00169bc2: ldr r0,[r0,#0x0]
  00169bc4: str r1,[sp,#0x28]
  00169bc6: blx 0x00074ac4
  00169bca: ldr.w r5,[r0,r4,lsl #0x2]
  00169bce: mov r0,r8
  00169bd0: blx 0x0006efbc
  00169bd4: cmp r5,#0x0
  00169bd6: beq.w 0x00169f46
  00169bda: cmp r4,#0x2
  00169bdc: it eq
  00169bde: cmp.eq r5,#0x2
  00169be0: beq 0x00169cbc
  00169be2: cmp r4,#0x3
  00169be4: it eq
  00169be6: cmp.eq r5,#0x2
  00169be8: beq.w 0x00169d5a
  00169bec: cmp r4,#0x9
  00169bee: it eq
  00169bf0: cmp.eq r5,#0x2
  00169bf2: beq.w 0x00169df8
  00169bf6: cmp r4,#0xd
  00169bf8: it eq
  00169bfa: cmp.eq r5,#0x2
  00169bfc: beq.w 0x00169e96
  00169c00: cmp r4,#0xe
  00169c02: it eq
  00169c04: cmp.eq r5,#0x2
  00169c06: bne.w 0x00169f46
  00169c0a: ldr r1,[0x00169f74]
  00169c0c: add r1,pc
  00169c0e: add r0,sp,#0x10
  00169c10: movs r2,#0x0
  00169c12: blx 0x0006ee18
  00169c16: ldr r0,[0x00169f78]
  00169c18: add r0,pc
  00169c1a: ldr r0,[r0,#0x0]
  00169c1c: str r0,[sp,#0x4]
  00169c1e: ldr r0,[r0,#0x0]
  00169c20: mov.w r1,#0x114
  00169c24: blx 0x00072f70
  00169c28: mov r2,r0
  00169c2a: add r0,sp,#0x1c
  00169c2c: add r1,sp,#0x10
  00169c2e: blx 0x0006ef98
  00169c32: add r1,sp,#0x1c
  00169c34: mov r0,r8
  00169c36: blx 0x0006ef5c
  00169c3a: add.w r11,sp,#0x1c
  00169c3e: mov r0,r11
  00169c40: blx 0x0006ee30
  00169c44: add r6,sp,#0x10
  00169c46: mov r0,r6
  00169c48: blx 0x0006ee30
  00169c4c: ldr r0,[0x00169f7c]
  00169c4e: movs r5,#0x0
  00169c50: add r0,pc
  00169c52: ldr.w r10,[r0,#0x0]
  00169c56: ldr r0,[0x00169f80]
  00169c58: add r0,pc
  00169c5a: str r0,[sp,#0x8]
  00169c5c: b 0x00169cb6
  00169c5e: ldr.w r0,[r10,#0x0]
  00169c62: ldr r0,[r0,#0x18]
  00169c64: ldr r0,[r0,#0x4]
  00169c66: ldr.w r0,[r0,r5,lsl #0x2]
  00169c6a: ldr r0,[r0,#0xc]
  00169c6c: cbnz r0,0x00169cb4
  00169c6e: ldr r1,[sp,#0x8]
  00169c70: mov r0,r6
  00169c72: movs r2,#0x0
  00169c74: blx 0x0006ee18
  00169c78: ldr.w r0,[r10,#0x0]
  00169c7c: ldr r1,[sp,#0x4]
  00169c7e: ldr r0,[r0,#0x18]
  00169c80: ldr r4,[r1,#0x0]
  00169c82: ldr r0,[r0,#0x4]
  00169c84: ldr.w r0,[r0,r5,lsl #0x2]
  00169c88: blx 0x000733fc
  00169c8c: addw r1,r0,#0x4fa
  00169c90: mov r0,r4
  00169c92: blx 0x00072f70
  00169c96: mov r2,r0
  00169c98: mov r0,r11
  00169c9a: mov r1,r6
  00169c9c: blx 0x0006ef98
  00169ca0: mov r0,r8
  00169ca2: mov r1,r11
  00169ca4: blx 0x0006ef5c
  00169ca8: mov r0,r11
  00169caa: blx 0x0006ee30
  00169cae: mov r0,r6
  00169cb0: blx 0x0006ee30
  00169cb4: adds r5,#0x1
  00169cb6: cmp r5,#0xd
  00169cb8: bcc 0x00169c5e
  00169cba: b 0x00169f46
  00169cbc: ldr r1,[0x00169fd0]
  00169cbe: add r1,pc
  00169cc0: add r0,sp,#0x10
  00169cc2: movs r2,#0x0
  00169cc4: blx 0x0006ee18
  00169cc8: ldr r0,[0x00169fd4]
  00169cca: add r0,pc
  00169ccc: ldr.w r10,[r0,#0x0]
  00169cd0: ldr.w r0,[r10,#0x0]
  00169cd4: mov.w r1,#0x114
  00169cd8: blx 0x00072f70
  00169cdc: mov r2,r0
  00169cde: add r0,sp,#0x1c
  00169ce0: add r1,sp,#0x10
  00169ce2: blx 0x0006ef98
  00169ce6: add r1,sp,#0x1c
  00169ce8: mov r0,r8
  00169cea: blx 0x0006ef5c
  00169cee: add.w r11,sp,#0x1c
  00169cf2: mov r0,r11
  00169cf4: blx 0x0006ee30
  00169cf8: add r6,sp,#0x10
  00169cfa: mov r0,r6
  00169cfc: blx 0x0006ee30
  00169d00: ldr r0,[0x00169fd8]
  00169d02: movs r4,#0x0
  00169d04: add r0,pc
  00169d06: ldr r5,[r0,#0x0]
  00169d08: ldr r0,[0x00169fdc]
  00169d0a: add r0,pc
  00169d0c: mov r9,r0
  00169d0e: b 0x00169d4c
  00169d10: ldr r0,[r0,#0x4]
  00169d12: ldrb r0,[r0,r4]
  00169d14: cbnz r0,0x00169d4a
  00169d16: mov r0,r6
  00169d18: mov r1,r9
  00169d1a: movs r2,#0x0
  00169d1c: blx 0x0006ee18
  00169d20: ldr.w r0,[r10,#0x0]
  00169d24: addw r1,r4,#0x594
  00169d28: blx 0x00072f70
  00169d2c: mov r2,r0
  00169d2e: mov r0,r11
  00169d30: mov r1,r6
  00169d32: blx 0x0006ef98
  00169d36: mov r0,r8
  00169d38: mov r1,r11
  00169d3a: blx 0x0006ef5c
  00169d3e: mov r0,r11
  00169d40: blx 0x0006ee30
  00169d44: mov r0,r6
  00169d46: blx 0x0006ee30
  00169d4a: adds r4,#0x1
  00169d4c: ldr r0,[r5,#0x0]
  00169d4e: ldr.w r0,[r0,#0x94]
  00169d52: ldr r1,[r0,#0x0]
  00169d54: cmp r4,r1
  00169d56: bcc 0x00169d10
  00169d58: b 0x00169f46
  00169d5a: ldr r1,[0x00169fe0]
  00169d5c: add r1,pc
  00169d5e: add r0,sp,#0x10
  00169d60: movs r2,#0x0
  00169d62: blx 0x0006ee18
  00169d66: ldr r0,[0x00169fe4]
  00169d68: add r0,pc
  00169d6a: ldr.w r10,[r0,#0x0]
  00169d6e: ldr.w r0,[r10,#0x0]
  00169d72: mov.w r1,#0x114
  00169d76: blx 0x00072f70
  00169d7a: mov r2,r0
  00169d7c: add r0,sp,#0x1c
  00169d7e: add r1,sp,#0x10
  00169d80: blx 0x0006ef98
  00169d84: add r1,sp,#0x1c
  00169d86: mov r0,r8
  00169d88: blx 0x0006ef5c
  00169d8c: add.w r11,sp,#0x1c
  00169d90: mov r0,r11
  00169d92: blx 0x0006ee30
  00169d96: add r6,sp,#0x10
  00169d98: mov r0,r6
  00169d9a: blx 0x0006ee30
  00169d9e: ldr r0,[0x00169fe8]
  00169da0: movs r4,#0x0
  00169da2: add r0,pc
  00169da4: ldr r5,[r0,#0x0]
  00169da6: ldr r0,[0x00169fec]
  00169da8: add r0,pc
  00169daa: mov r9,r0
  00169dac: b 0x00169dea
  00169dae: ldr r0,[r0,#0x4]
  00169db0: ldrb r0,[r0,r4]
  00169db2: cbnz r0,0x00169de8
  00169db4: mov r0,r6
  00169db6: mov r1,r9
  00169db8: movs r2,#0x0
  00169dba: blx 0x0006ee18
  00169dbe: ldr.w r0,[r10,#0x0]
  00169dc2: addw r1,r4,#0x59f
  00169dc6: blx 0x00072f70
  00169dca: mov r2,r0
  00169dcc: mov r0,r11
  00169dce: mov r1,r6
  00169dd0: blx 0x0006ef98
  00169dd4: mov r0,r8
  00169dd6: mov r1,r11
  00169dd8: blx 0x0006ef5c
  00169ddc: mov r0,r11
  00169dde: blx 0x0006ee30
  00169de2: mov r0,r6
  00169de4: blx 0x0006ee30
  00169de8: adds r4,#0x1
  00169dea: ldr r0,[r5,#0x0]
  00169dec: ldr.w r0,[r0,#0x98]
  00169df0: ldr r1,[r0,#0x0]
  00169df2: cmp r4,r1
  00169df4: bcc 0x00169dae
  00169df6: b 0x00169f46
  00169df8: ldr r1,[0x00169ff0]
  00169dfa: add r1,pc
  00169dfc: add r0,sp,#0x10
  00169dfe: movs r2,#0x0
  00169e00: blx 0x0006ee18
  00169e04: ldr r0,[0x00169ff4]
  00169e06: add r0,pc
  00169e08: ldr.w r10,[r0,#0x0]
  00169e0c: ldr.w r0,[r10,#0x0]
  00169e10: mov.w r1,#0x114
  00169e14: blx 0x00072f70
  00169e18: mov r2,r0
  00169e1a: add r0,sp,#0x1c
  00169e1c: add r1,sp,#0x10
  00169e1e: blx 0x0006ef98
  00169e22: add r1,sp,#0x1c
  00169e24: mov r0,r8
  00169e26: blx 0x0006ef5c
  00169e2a: add.w r11,sp,#0x1c
  00169e2e: mov r0,r11
  00169e30: blx 0x0006ee30
  00169e34: add r6,sp,#0x10
  00169e36: mov r0,r6
  00169e38: blx 0x0006ee30
  00169e3c: ldr r0,[0x00169ff8]
  00169e3e: movs r4,#0x0
  00169e40: add r0,pc
  00169e42: ldr r5,[r0,#0x0]
  00169e44: ldr r0,[0x00169ffc]
  00169e46: add r0,pc
  00169e48: mov r9,r0
  00169e4a: b 0x00169e88
  00169e4c: ldr r0,[r0,#0x4]
  00169e4e: ldrb r0,[r0,r4]
  00169e50: cbnz r0,0x00169e86
  00169e52: mov r0,r6
  00169e54: mov r1,r9
  00169e56: movs r2,#0x0
  00169e58: blx 0x0006ee18
  00169e5c: ldr.w r0,[r10,#0x0]
  00169e60: addw r1,r4,#0x57e
  00169e64: blx 0x00072f70
  00169e68: mov r2,r0
  00169e6a: mov r0,r11
  00169e6c: mov r1,r6
  00169e6e: blx 0x0006ef98
  00169e72: mov r0,r8
  00169e74: mov r1,r11
  00169e76: blx 0x0006ef5c
  00169e7a: mov r0,r11
  00169e7c: blx 0x0006ee30
  00169e80: mov r0,r6
  00169e82: blx 0x0006ee30
  00169e86: adds r4,#0x1
  00169e88: ldr r0,[r5,#0x0]
  00169e8a: ldr.w r0,[r0,#0xac]
  00169e8e: ldr r1,[r0,#0x0]
  00169e90: cmp r4,r1
  00169e92: bcc 0x00169e4c
  00169e94: b 0x00169f46
  00169e96: ldr r1,[0x0016a000]
  00169e98: add r1,pc
  00169e9a: add r0,sp,#0x10
  00169e9c: movs r2,#0x0
  00169e9e: blx 0x0006ee18
  00169ea2: ldr r0,[0x0016a004]
  00169ea4: add r0,pc
  00169ea6: ldr r0,[r0,#0x0]
  00169ea8: str r0,[sp,#0x4]
  00169eaa: ldr r0,[r0,#0x0]
  00169eac: mov.w r1,#0x114
  00169eb0: blx 0x00072f70
  00169eb4: mov r2,r0
  00169eb6: add r0,sp,#0x1c
  00169eb8: add r1,sp,#0x10
  00169eba: blx 0x0006ef98
  00169ebe: add r1,sp,#0x1c
  00169ec0: mov r0,r8
  00169ec2: blx 0x0006ef5c
  00169ec6: add.w r11,sp,#0x1c
  00169eca: mov r0,r11
  00169ecc: blx 0x0006ee30
  00169ed0: add r6,sp,#0x10
  00169ed2: mov r0,r6
  00169ed4: blx 0x0006ee30
  00169ed8: ldr r0,[0x0016a008]
  00169eda: movs r5,#0x0
  00169edc: add r0,pc
  00169ede: ldr.w r10,[r0,#0x0]
  00169ee2: ldr r0,[0x0016a00c]
  00169ee4: add r0,pc
  00169ee6: str r0,[sp,#0x8]
  00169ee8: b 0x00169f42
  00169eea: ldr.w r0,[r10,#0x0]
  00169eee: ldr r0,[r0,#0x18]
  00169ef0: ldr r0,[r0,#0x4]
  00169ef2: ldr.w r0,[r0,r5,lsl #0x2]
  00169ef6: ldrb r0,[r0,#0x8]
  00169ef8: cbnz r0,0x00169f40
  00169efa: ldr r1,[sp,#0x8]
  00169efc: mov r0,r6
  00169efe: movs r2,#0x0
  00169f00: blx 0x0006ee18
  00169f04: ldr.w r0,[r10,#0x0]
  00169f08: ldr r1,[sp,#0x4]
  00169f0a: ldr r0,[r0,#0x18]
  00169f0c: ldr r4,[r1,#0x0]
  00169f0e: ldr r0,[r0,#0x4]
  00169f10: ldr.w r0,[r0,r5,lsl #0x2]
  00169f14: blx 0x000733fc
  00169f18: addw r1,r0,#0x4fa
  00169f1c: mov r0,r4
  00169f1e: blx 0x00072f70
  00169f22: mov r2,r0
  00169f24: mov r0,r11
  00169f26: mov r1,r6
  00169f28: blx 0x0006ef98
  00169f2c: mov r0,r8
  00169f2e: mov r1,r11
  00169f30: blx 0x0006ef5c
  00169f34: mov r0,r11
  00169f36: blx 0x0006ee30
  00169f3a: mov r0,r6
  00169f3c: blx 0x0006ee30
  00169f40: adds r5,#0x1
  00169f42: cmp r5,#0xd
  00169f44: bcc 0x00169eea
  00169f46: ldr r0,[sp,#0x28]
  00169f48: ldr r1,[sp,#0xc]
  00169f4a: ldr r1,[r1,#0x0]
  00169f4c: subs r0,r1,r0
  00169f4e: ittt eq
  00169f50: add.eq sp,#0x2c
  00169f52: pop.eq.w {r8,r9,r10,r11}
  00169f56: pop.eq {r4,r5,r6,r7,pc}
  00169f58: blx 0x0006e824
```
