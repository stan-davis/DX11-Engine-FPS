#include "Collider.h"

bool Collider::IsColliding(Collider collider)
{
    bool ret = RectCollider(collider);
    
    /*
    switch (collider.GetColliderType())
    {
        case ColliderShape::RECT:
            ret = RectCollider(collider);
            break;
        case ColliderShape::SPHERE:
            //do nothing for now
            break;
        case ColliderShape::NONE:
            break;
    }
    */

    return ret;
}

bool Collider::RectCollider(Collider& collider)
{
    //AABB
    Vector3 ap = position;
    Vector3 as = size;

    Vector3 tp = collider.GetPosition();
    Vector3 ts = collider.GetSize();

    bool ret = ap.x + as.x >= tp.x &&
        tp.x + ts.x >= ap.x &&
        ap.z + as.z >= tp.z &&
        tp.z + ts.z >= ap.z;

    return ret;
}
