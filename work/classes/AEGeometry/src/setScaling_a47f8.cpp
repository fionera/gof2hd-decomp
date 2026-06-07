#include "class.h"

// AEGeometry::setScaling(float) -> tail-call setScaling(s, s, s)
struct AEGeometry {
    void setScaling(float, float, float);
    void setScaling(float s);
};
void AEGeometry::setScaling(float s)
{
    return setScaling(s, s, s);
}
