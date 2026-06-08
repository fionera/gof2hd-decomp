#include "class.h"

// ModStation::leaveStation() — a 16-byte veneer the decompiler could only recover as "bad
// instruction data"; it tail-calls the real leave-station handler (which tears the station
// module down and transitions back to flight). Expressed faithfully as that forwarding call.
extern "C" void ModStation_leaveStation_impl(ModStation *self);

extern "C" void ModStation_leaveStation(ModStation *self)
{
    ModStation_leaveStation_impl(self);
}
