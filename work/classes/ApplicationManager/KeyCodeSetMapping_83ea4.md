# ApplicationManager::KeyCodeSetMapping
elf 0x83ea4 body 62
Sig: undefined __thiscall KeyCodeSetMapping(ApplicationManager * this, Array * param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::KeyCodeSetMapping(Array<AbyssEngine::KeyCode*>*) */

void __thiscall
AbyssEngine::ApplicationManager::KeyCodeSetMapping(ApplicationManager *this,Array *param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  
  if (*(int *)param_1 == 0x40) {
    uVar1 = 0x40;
    iVar5 = 0;
    for (uVar4 = 0; uVar4 < uVar1; uVar4 = uVar4 + 1) {
      puVar3 = *(undefined4 **)(*(int *)(param_1 + 4) + uVar4 * 4);
      iVar2 = *(int *)(this + 0x10);
      *(undefined4 *)(iVar2 + iVar5) = *puVar3;
      String::operator=((String *)((undefined4 *)(iVar2 + iVar5) + 1),(String *)(puVar3 + 1));
      iVar5 = iVar5 + 0x10;
      uVar1 = *(uint *)param_1;
    }
  }
  return;
}

```

## target disasm
```
  00093ea4: push {r4,r5,r6,r7,lr}
  00093ea6: add r7,sp,#0xc
  00093ea8: push.w r8
  00093eac: mov r8,r0
  00093eae: ldr r0,[r1,#0x0]
  00093eb0: mov r4,r1
  00093eb2: cmp r0,#0x40
  00093eb4: bne 0x00093edc
  00093eb6: movs r0,#0x40
  00093eb8: movs r6,#0x0
  00093eba: movs r5,#0x0
  00093ebc: b 0x00093ed8
  00093ebe: ldr r0,[r4,#0x4]
  00093ec0: ldr.w r1,[r0,r5,lsl #0x2]
  00093ec4: ldr.w r0,[r8,#0x10]
  00093ec8: add r0,r6
  00093eca: ldmia r1!,{r2}
  00093ecc: stmia r0!,{r2}
  00093ece: blx 0x0006f2b0
  00093ed2: adds r6,#0x10
  00093ed4: ldr r0,[r4,#0x0]
  00093ed6: adds r5,#0x1
  00093ed8: cmp r5,r0
  00093eda: bcc 0x00093ebe
  00093edc: pop.w r8
  00093ee0: pop {r4,r5,r6,r7,pc}
```
