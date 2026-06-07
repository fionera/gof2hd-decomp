# Agent::setWingmanFriendNames
elf 0x177578 body 188
Sig: undefined __thiscall setWingmanFriendNames(Agent * this, Array * param_1)

## decompile
```c

/* Agent::setWingmanFriendNames(Array<AbyssEngine::String*>*) */

void __thiscall Agent::setWingmanFriendNames(Agent *this,Array *param_1)

{
  void *pvVar1;
  String *this_00;
  String *pSVar2;
  uint uVar3;
  
  if (*(int **)(this + 0xc) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xc) + 4))();
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(int **)(this + 0x10) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x10) + 4))();
  }
  *(undefined4 *)(this + 0x10) = 0;
  if (*(Array<AbyssEngine::String*> **)(this + 0x90) != (Array<AbyssEngine::String*> *)0x0) {
    pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                               (*(Array<AbyssEngine::String*> **)(this + 0x90));
    operator_delete(pvVar1);
    *(undefined4 *)(this + 0x90) = 0;
  }
  pvVar1 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  *(void **)(this + 0x90) = pvVar1;
  this_00 = operator_new(0xc);
  AbyssEngine::String::String(this_00,this,false);
  ArrayAdd<AbyssEngine::String*>(this_00,*(Array **)(this + 0x90));
  *(undefined4 *)(this + 0x14) = 0;
  if (param_1 == (Array *)0x0) {
    return;
  }
  uVar3 = *(uint *)param_1;
  if (uVar3 != 0) {
    pSVar2 = (String *)**(undefined4 **)(param_1 + 4);
    if (pSVar2 != (String *)0x0) {
      *(undefined4 *)(this + 0x14) = 1;
      *(String **)(this + 0xc) = pSVar2;
      ArrayAdd<AbyssEngine::String*>(pSVar2,*(Array **)(this + 0x90));
      uVar3 = *(uint *)param_1;
    }
    if ((1 < uVar3) && (pSVar2 = *(String **)(*(int *)(param_1 + 4) + 4), pSVar2 != (String *)0x0))
    {
      *(String **)(this + 0x10) = pSVar2;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
      ArrayAdd<AbyssEngine::String*>(pSVar2,*(Array **)(this + 0x90));
    }
  }
  Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)param_1);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00187578: push {r4,r5,r6,r7,lr}
  0018757a: add r7,sp,#0xc
  0018757c: push.w r8
  00187580: mov r5,r0
  00187582: ldr r0,[r0,#0xc]
  00187584: mov r4,r1
  00187586: cbz r0,0x0018758e
  00187588: ldr r1,[r0,#0x0]
  0018758a: ldr r1,[r1,#0x4]
  0018758c: blx r1
  0018758e: ldr r0,[r5,#0x10]
  00187590: movs r6,#0x0
  00187592: str r6,[r5,#0xc]
  00187594: cbz r0,0x0018759c
  00187596: ldr r1,[r0,#0x0]
  00187598: ldr r1,[r1,#0x4]
  0018759a: blx r1
  0018759c: ldr.w r0,[r5,#0x90]
  001875a0: str r6,[r5,#0x10]
  001875a2: cbz r0,0x001875b2
  001875a4: blx 0x0006f64c
  001875a8: blx 0x0006eb48
  001875ac: movs r0,#0x0
  001875ae: str.w r0,[r5,#0x90]
  001875b2: movs r0,#0xc
  001875b4: blx 0x0006eb24
  001875b8: mov r6,r0
  001875ba: blx 0x0006f628
  001875be: movs r0,#0xc
  001875c0: str.w r6,[r5,#0x90]
  001875c4: blx 0x0006eb24
  001875c8: mov r6,r0
  001875ca: mov r1,r5
  001875cc: movs r2,#0x0
  001875ce: mov.w r8,#0x0
  001875d2: blx 0x0006f028
  001875d6: ldr.w r1,[r5,#0x90]
  001875da: mov r0,r6
  001875dc: blx 0x0006f634
  001875e0: cmp r4,#0x0
  001875e2: str.w r8,[r5,#0x14]
  001875e6: beq 0x0018762e
  001875e8: ldr r1,[r4,#0x0]
  001875ea: cbz r1,0x0018761c
  001875ec: ldr r0,[r4,#0x4]
  001875ee: ldr r0,[r0,#0x0]
  001875f0: cbz r0,0x00187602
  001875f2: movs r1,#0x1
  001875f4: str r1,[r5,#0x14]
  001875f6: ldr.w r1,[r5,#0x90]
  001875fa: str r0,[r5,#0xc]
  001875fc: blx 0x0006f634
  00187600: ldr r1,[r4,#0x0]
  00187602: cmp r1,#0x2
  00187604: bcc 0x0018761c
  00187606: ldr r0,[r4,#0x4]
  00187608: ldr r0,[r0,#0x4]
  0018760a: cbz r0,0x0018761c
  0018760c: ldr r2,[r5,#0x14]
  0018760e: ldr.w r1,[r5,#0x90]
  00187612: adds r2,#0x1
  00187614: strd r0,r2,[r5,#0x10]
  00187618: blx 0x0006f634
  0018761c: mov r0,r4
  0018761e: blx 0x0006f64c
  00187622: pop.w r8
  00187626: pop.w {r4,r5,r6,r7,lr}
  0018762a: b.w 0x001ab098
  0018762e: pop.w r8
  00187632: pop {r4,r5,r6,r7,pc}
```
