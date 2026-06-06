#!/usr/bin/env bash
# collect_class.sh <Class> — verify every method of a class-worker, screen asm-cheats, and
# integrate genuine exact matches into src/recovered/<Class>/ + matches.tsv.
Class="$1"
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
dir="work/classes/$Class"
out="src/recovered/$Class"; mkdir -p "$out"
# bring the shared header so method TUs compile standalone (#include "quaternion.h")
for h in "$dir"/src/*.h; do [ -f "$h" ] && cp "$h" "$out"/; done
exact=0; cheat=0; other=0
echo "== class $Class =="
while IFS=$'\t' read -r key method vaddr n; do
  [ -z "$key" ] && continue
  src="$dir/src/$key.cpp"
  if [ ! -f "$src" ]; then printf "  %-42s NO-SRC\n" "$key"; other=$((other+1)); continue; fi
  if grep -qE '\b(asm|__asm__|naked)\b' "$src"; then printf "  %-42s CHEAT(asm)\n" "$key"; cheat=$((cheat+1)); continue; fi
  pct=$(bash tools/try.sh "$src" "$method" "$vaddr" "$n" </dev/null 2>/dev/null | grep -oE 'MATCH [0-9.]+' | grep -oE '[0-9.]+' | head -1); pct=${pct:-0}
  if awk "BEGIN{exit !($pct>=100)}"; then
    printf "  %-42s %6s%% exact\n" "$key" "$pct"
    cp "$src" "$out/$key.cpp"
    grep -q "^$key	" matches.tsv || printf '%s\t%s\t%s\tbuild/obj/recovered/%s/%s.o\t%s\texact\n' "$key" "$vaddr" "$n" "$Class" "$key" "$method" >> matches.tsv
    exact=$((exact+1))
  else printf "  %-42s %6s%% (not exact)\n" "$key" "$pct"; other=$((other+1)); fi
done < "$dir/methods.tsv"
echo "  => $Class: exact=$exact cheat=$cheat other=$other"
