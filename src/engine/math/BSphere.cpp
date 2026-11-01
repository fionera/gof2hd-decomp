#include "engine/math/BSphere.h"
#include <cmath>

#include "Matrix.h"
#include "Vector.h"
#include "Transform.h"

namespace AbyssEngine {
    namespace AEMath {
        void BSphere::Merge(const BSphere &other) {
            if (other.radius2 > this->radius2)
                this->radius2 = other.radius2;

            if (other.radius == 0.0f)
                return;

            if (this->radius == 0.0f) {
                // Empty sphere: take the other one over (the binary copies the fields and then
                // assigns the whole sphere once more).
                this->radius = other.radius;
                this->center.x = other.center.x;
                this->center.y = other.center.y;
                this->center.z = other.center.z;
                this->radius2 = other.radius2;
                *this = other;
                return;
            }

            float dx = other.center.x - this->center.x;
            float dy = other.center.y - this->center.y;
            float dz = other.center.z - this->center.z;
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

            float r1 = this->radius;
            float r2 = other.radius;

            if (dist == 0.0f) {
                if (r1 > r2)
                    r2 = r1;
                this->radius = r2;
                return;
            }

            // Already enclosed?
            if (!(r1 <= dist + r2))
                return;

            if (dist - r2 < -r1) {
                this->center.x = other.center.x;
                this->center.y = other.center.y;
                this->center.z = other.center.z;
                this->radius = r2;
                return;
            }

            float t = (((dist + r2) - r1) * 0.5f) / dist;
            this->center.x = this->center.x + dx * t;
            this->center.y = this->center.y + dy * t;
            this->center.z = this->center.z + dz * t;
            this->radius = (dist + r1 + r2) * 0.5f;
        }

        BSphere &BSphere::operator=(const BSphere &other) {
            this->center.x = other.center.x;
            this->center.y = other.center.y;
            this->center.z = other.center.z;
            this->radius = other.radius;
            this->radius2 = other.radius2;
            return *this;
        }


    }
}
