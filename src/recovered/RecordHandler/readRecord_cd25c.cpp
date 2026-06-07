#include "class.h"

// Tail-called veneer at 0x1ac058 (recordStoreRead dispatch).
extern "C" void RecordHandler_readRecordTail(int param);

// RecordHandler::readRecord(int)
extern "C" void RecordHandler_readRecord(int param)
{
    return RecordHandler_readRecordTail(param);
}
