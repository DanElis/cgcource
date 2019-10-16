#include "matrices.h"


Matrix4x1 convertVector3(const Vector3 &v)
{
    float data[4] = { v.x(), v.y(), v.z(), 1 };
    return Matrix4x1(data);
}
