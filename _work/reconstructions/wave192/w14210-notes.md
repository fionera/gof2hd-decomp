# w14210 Radar::draw hunk 40
- baseline clean tree: raw 7201 / aligned 1233 / size 9908, .text delta 78565 (40 hunks)
- hunk 40 = +0x22fe..+0x26b0 (947 B, 870 differ): sound-guard ladder + planet blip tail
- evidence: orig tests [sp,#104] (cbnz, alwaysEnemySeen) then [sp,#92]; ours [sp,#96] via lsls (bool canonical) -> orig locals appear int-typed
- L1298 expect(alwaysEnemySeen) removal -> raw 7219 / aligned 1235 / size 9904 -> worse, reverted
- resume#14 re-measured: B1 planet blip direct-field both arms -> 7196/1240/9908; B2 station int frame+direct both arms -> 7207/1231/9912 (+4 size, gate-fatal); B2b arm-local blip -> 7172/1267/9920; B2d station frame-init-then-recompute -> 7207/1360/9892
- B1+B2 combined -> 7202/1238/9912; NEW outer `Sprite *blip;` assigned per-arm + shared blip->setFrame -> 7172/1239/9920 (reverted)
- conclusion: original genuinely reloads blip per arm into r5 and shares setFrame; every forced source form clang hoists or changes size. No measured aligned<1233 win. Tree clean, baseline 7201/1233/9908. No commit.
