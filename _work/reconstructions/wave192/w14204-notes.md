=== w14204 LevelScript::process hunks 4 & 11 ===
baseline raw 54870 / aligned 17438 / size 57232; .text 78565 .ARM.extab 555
hunk11: moved `case 10` block (17 lines) before `case 8` to test textual emit order -> raw 54,870 / aligned 17,438 / size 57,232 (unchanged); reverted.
hunk11 analysis: clang emits case-10 body after case 9 regardless of textual order; statement order/clamp already match orig.
hunk4 not attempted: register-role split (this/messages fp vs sl) and basic-block ordering remain; no measured win, tree clean, no commit.
