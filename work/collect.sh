#!/usr/bin/env bash
# collect.sh — re-verify every worker's output and summarize match %. Prints matches.tsv-ready
# lines for the ones that pass (exact=100% / near>=90%) so winners can be folded into src/.
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
CTX="${CTX:-work/tranche_ctx.tsv}"
echo "== worker results ($CTX) =="
while IFS=$'\t' read -r name vaddr n; do
  [ -z "$name" ] && continue
  src="work/src/$name.cpp"
  if [ ! -f "$src" ]; then printf "  %-26s NO-SRC\n" "$name"; continue; fi
  pct=$(bash tools/try.sh "$src" "$name" "$vaddr" "$n" </dev/null 2>/dev/null | grep -oE 'MATCH [0-9.]+' | grep -oE '[0-9.]+' | head -1)
  pct=${pct:-0}
  status=fail; awk "BEGIN{exit !($pct>=100)}" && status=exact || { awk "BEGIN{exit !($pct>=90)}" && status=near; }
  printf "  %-26s %6s%%  %s\n" "$name" "$pct" "$status"
  [ "$status" != fail ] && echo "$name	$vaddr	$n	$src	$name	$status" >> /tmp/new_matches.tsv
done < "$CTX"
echo "== matches.tsv-ready lines in /tmp/new_matches.tsv =="; cat /tmp/new_matches.tsv 2>/dev/null
