p='src/game/world/LevelScript.cpp'
s=open(p).read()
a_start=s.index('    else if (Globals::status->getCurrentCampaignMission() == 26 && Globals::status->inAlienOrbit()) {')
b_start=s.index('    } else if (Globals::status->getCurrentCampaignMission() == 5) {')
c_start=s.index('    } else if (Globals::status->getCurrentCampaignMission() == 50 && messages != nullptr) {')
a=s[a_start:b_start]   # includes leading 'else if' and trailing '} ' before next
b=s[b_start:c_start]
# a ends with '    } ' (closing brace of 26 block), b ends with '    } '
# Rebuild: b as 'else if', a as 'else if'
b2=b.replace('    } else if (Globals::status->getCurrentCampaignMission() == 5) {','    else if (Globals::status->getCurrentCampaignMission() == 5) {',1)
a2=a.replace('    else if (Globals::status->getCurrentCampaignMission() == 26 && Globals::status->inAlienOrbit()) {','    } else if (Globals::status->getCurrentCampaignMission() == 26 && Globals::status->inAlienOrbit()) {',1)
s=s[:a_start]+b2+a2+s[c_start:]
open(p,'w').write(s)
print('swapped')
