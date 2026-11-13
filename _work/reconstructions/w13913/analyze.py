#!/usr/bin/env python3
"""Summarise sweep_results.json: per-TU flag table vs baseline."""
import json, os, sys, collections

BASE = {
 '_ZN11LevelScript7processEi': (53208, 18243, 57232),
 '_ZN5Radar4drawEP6PlayerP3Hudi': (6909, 1303, 9908),
 '_ZN6LayoutC1Ev': (5614, 1993, 7072),
 '_ZN6LayoutC2Ev': (5614, 1993, 7072),
 '_ZN5Level10createShipEiiiP8Waypointbb': (1190, 366, 2764),
 '_ZN5Level13createMissionEv': (4265, 386, 17172),
 '_ZN5MGame10OnTouchEndEiiPv': (3544, 697, 8080),
 '_ZN5MGame8OnUpdateEv': (16, 6, 11440),
 '_ZN12HangarWindow6renderEv': (1395, 396, 9700),
 '_ZN6Status16missionCompletedEbbx': (45, 28, 1432),
 '_ZN3Hud4drawExxP9PlayerEgobjj': (19, 6, 9332),
}
rows = json.load(open(sys.argv[1] if len(sys.argv) > 1 else 'sweep_results.json'))
by = collections.defaultdict(list)
for r in rows:
    by[r['tu']].append(r)
for tu in sorted(by):
    print('=' * 100)
    print(tu)
    for r in by[tu]:
        g = r.get('gate') or {}
        sc = r.get('scores')
        if not isinstance(sc, list):
            print('  %-32s ERROR %s' % (r['flags'] or '(base)', str(r.get('error') or sc)[:120]))
            continue
        parts = []
        for s in sc:
            if 'raw' not in s:
                parts.append('%s:MISSING' % s['name'].split('_ZN')[-1][:22]); continue
            b = BASE.get(s['name'])
            d = '' if not b else ('=' if (s['raw'], s['aligned']) == b[:2] else
                                  '  raw%+d al%+d sz%d/%d' % (s['raw'] - b[0], s['aligned'] - b[1], s['usize'], s['osize']))
            parts.append('%s:%d/%d/%d%s' % (s['name'].split('_ZN')[-1][:22], s['raw'], s['aligned'], s['usize'], d))
        print('  %-32s byte=%-4s miss=%s extra=%s | %s' %
              (r['flags'] or '(base)', g.get('byte_exact'), g.get('missing'), g.get('extra'), ' ; '.join(parts)))
