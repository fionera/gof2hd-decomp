# DialogueWindow::DialogueWindow
elf 0x166bfc body 66
Sig: undefined __thiscall DialogueWindow(DialogueWindow * this, Mission * param_1, Level * param_2, int param_3)

## decompile
```c

/* DialogueWindow::DialogueWindow(Mission*, Level*, int) */

DialogueWindow * __thiscall
DialogueWindow::DialogueWindow(DialogueWindow *this,Mission *param_1,Level *param_2,int param_3)

{
  EVP_PKEY_CTX *ctx;
  
  AbyssEngine::String::String((String *)&this->m_pText);
  AbyssEngine::String::String((String *)&(this->m_pAgentName).size);
  init(this,ctx);
  this->field_58 = (int *)param_2;
  set((Mission *)this,(int)param_1,param_3);
  return this;
}

```

## target disasm
```
  00176bfc: push {r4,r5,r6,r7,lr}
  00176bfe: add r7,sp,#0xc
  00176c00: push {r8,r9,r10}
  00176c04: add.w r8,r0,#0x28
  00176c08: mov r5,r0
  00176c0a: mov r10,r3
  00176c0c: mov r4,r2
  00176c0e: mov r0,r8
  00176c10: mov r6,r1
  00176c12: blx 0x0006efbc
  00176c16: add.w r9,r5,#0x34
  00176c1a: mov r0,r9
  00176c1c: blx 0x0006efbc
  00176c20: mov r0,r5
  00176c22: blx 0x00077be4
  00176c26: str r4,[r5,#0x58]
  00176c28: mov r0,r5
  00176c2a: mov r1,r6
  00176c2c: mov r2,r10
  00176c2e: mov.w r3,#0xffffffff
  00176c32: blx 0x00075550
  00176c36: mov r0,r5
  00176c38: pop.w {r8,r9,r10}
  00176c3c: pop {r4,r5,r6,r7,pc}
```
