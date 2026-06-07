# DialogueWindow::DialogueWindow
elf 0x166f5c body 44
Sig: undefined __thiscall DialogueWindow(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::DialogueWindow() */

DialogueWindow * __thiscall DialogueWindow::DialogueWindow(DialogueWindow *this)

{
  EVP_PKEY_CTX *ctx;
  
  AbyssEngine::String::String((String *)&this->m_pText);
  AbyssEngine::String::String((String *)&(this->m_pAgentName).size);
  init(this,ctx);
  return this;
}

```

## target disasm
```
  00176f5c: push {r4,r5,r6,r7,lr}
  00176f5e: add r7,sp,#0xc
  00176f60: push.w r11
  00176f64: add.w r4,r0,#0x28
  00176f68: mov r5,r0
  00176f6a: mov r0,r4
  00176f6c: blx 0x0006efbc
  00176f70: add.w r6,r5,#0x34
  00176f74: mov r0,r6
  00176f76: blx 0x0006efbc
  00176f7a: mov r0,r5
  00176f7c: blx 0x00077be4
  00176f80: mov r0,r5
  00176f82: pop.w r11
  00176f86: pop {r4,r5,r6,r7,pc}
```
