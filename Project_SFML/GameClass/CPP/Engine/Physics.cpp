#include "Engine/Physics2D.h"
#include <iostream>

RaycastHit Physics2D::RaycastAABB(Vector2 start, Vector2 end, Rect box)
{
    RaycastHit result;

    Vector2 dir = end - start;
    Vector2 invDir(1.0f / dir.x(), 1.0f / dir.y());

    float left = box.center().x() - box.width() / 2;
    float right = box.center().x() + box.width() / 2;
    float top = box.center().y() - box.height() / 2;
    float bottom = box.center().y() + box.height() / 2;

    Vector2 t1 = (Vector2(left, top) - start);
    t1 *= {invDir.x(), 1.f};
    t1 *= {1.f, invDir.y()};
    Vector2 t2 = (Vector2(right, bottom) - start);
    t2 *= {invDir.x(), 1.f};
    t2 *= {1.f, invDir.y()};

    Vector2 tminVec = { Mathf::Min(t1.x(), t2.x()), Mathf::Min(t1.y(), t2.y()) };
    Vector2 tmaxVec = { Mathf::Max(t1.x(), t2.x()), Mathf::Max(t1.y(), t2.y()) };

    float tmin = Mathf::Max(tminVec.x(), tminVec.y());
    float tmax = Mathf::Min(tmaxVec.x(), tmaxVec.y());

    if (tmax >= tmin && tmin >= 0.0f && tmin <= 1.0f)
    {
        result.hit = true;
        result.distance = tmin * Mathf::sqrt(dir.x() * dir.x() + dir.y() * dir.y());
        result.point = start + dir * tmin;

        // Calculer la normale de la surface touchée
        if (tmin == t1.x()) result.normal = { -1, 0 };
        else if (tmin == t2.x()) result.normal = { 1, 0 };
        else if (tmin == t1.y()) result.normal = { 0, -1 };
        else result.normal = { 0, 1 };
    }

    return result;
}