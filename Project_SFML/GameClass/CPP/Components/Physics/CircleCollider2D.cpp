#include "Components\Physics\PolygonCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Objects/GameObject.h"

void CircleCollider2D::Start()
{
	radius = Mathf::Max(owner->transform->scale.x()/2, owner->transform->scale.y() / 2);
	body = owner->GetComponent<Rigidbody2D>();
}

void CircleCollider2D::ApplyRotation()
{
	if (transform().rotation != _lastRotation)
	{
		_lastRotation = transform().rotation;
	}
	else
		return;

	radius = Mathf::Max(owner->transform->scale.x() / 2, owner->transform->scale.y() / 2);
}

void CircleCollider2D::UpdateCollision(GameObject* collider)
{
	auto& _collider = *collider;
	auto _c = _collider.GetComponent<Collider2D>();
	auto& _target = *owner;
	auto& _body = *body;

	auto it = std::find(_collision.begin(), _collision.end(), _c);

	if (findIntersection(owner->GetComponent<Collider2D>(), _c))
	{
		if (!isTrigger && !_c->isTrigger)
		{
			if (_collider.GetComponent<BoxCollider2D>())
				AABBWithCircle(*_collider.GetComponent<BoxCollider2D>());

			else if (_collider.GetComponent<CircleCollider2D>())
				CircleWithCircle(*_collider.GetComponent<CircleCollider2D>());

			else if (_collider.GetComponent<PolygonCollider2D>())
				PolygonWithCircle(*_collider.GetComponent<PolygonCollider2D>());
		}

		if (it == _collision.end())
		{
			_collision.push_back(_collider.GetComponent<Collider2D>());

			if (isTrigger)
				owner->OnTriggerEnter2D(_collider.GetComponent<Collider2D>());
			else
				owner->OnCollisionEnter2D(_collider.GetComponent<Collider2D>());
		}
		else
		{
			if (isTrigger)
				owner->OnTriggerStay2D(_collider.GetComponent<Collider2D>());
			else
				owner->OnCollisionStay2D(_collider.GetComponent<Collider2D>());
		}
	}
	else if (it != _collision.end())
	{
		_collision.erase(it);
		if (isTrigger)
			owner->OnTriggerExit2D(_collider.GetComponent<Collider2D>());
		else
			owner->OnCollisionExit2D(_collider.GetComponent<Collider2D>());
	}

}

bool CircleCollider2D::isPointInside(Vector2 point)
{

	float length = (point.x() - transform().position.x()) * (point.x() - transform().position.x()) +
					(point.y() - transform().position.y()) * (point.y() - transform().position.y());

	return length <= radius * radius;
}

Vector2 CircleCollider2D::GetProjectInterval(Vector2 normal)
{
	Vector2 distance = normal.normalized();
	Vector2 radDistence = distance * radius;

	Vector2 p1 = transform().position + radDistence;
	Vector2 p2 = transform().position - radDistence;

	float min = Vector2::Dot(normal, p1);
	float max = Vector2::Dot(normal, p2);

	if (min > max)
	{
		float t = min;
		min = max;
		max = t;
	}

	return { min, max };
}

void CircleCollider2D::AABBWithCircle(BoxCollider2D& collider)
{
	// ----- Création des collisions ----- \\

	float left = collider.transform().position.x() + collider.bounds.left();
	float top = collider.transform().position.y() + collider.bounds.top();
	float right = collider.transform().position.y() + collider.bounds.right();
	float bottom = collider.transform().position.y() + collider.bounds.bottom();

	Vector2 newCirclePos = Vector2::RotateAngle(transform().position, collider.transform().position, -collider.transform().rotation);

	Vector2 closetPoint = { Mathf::clamp(newCirclePos.x(), left, left + collider.bounds.width()),
						Mathf::clamp(newCirclePos.y(), top, top + collider.bounds.height()) };

	Vector2 dir = Vector2::Edge(closetPoint, newCirclePos);

	float penetration = radius - dir.magnitude();
	Vector2 normal = dir / dir.magnitude();

	if (dir.magnitude() < 0.1f)
	{
		// Trouver la direction de sortie la plus courte
		float leftPen = newCirclePos.x() - left;
		float rightPen = right - newCirclePos.x();
		float topPen = newCirclePos.y() - top;
		float bottomPen = bottom - newCirclePos.y();

		// Trouver la plus petite
		float minPen = std::min({ leftPen, rightPen, topPen, bottomPen });

		if (minPen == leftPen)       normal = Vector2(-1, 0);
		else if (minPen == rightPen) normal = Vector2(1, 0);
		else if (minPen == topPen)   normal = Vector2(0, -1);
		else                         normal = Vector2(0, 1);

		penetration = radius + minPen; // ou selon ton système
	}
	

	if (collider.owner->GetComponent<Rigidbody2D>() != nullptr)
	{
		Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

		float invMassA = 1.f / body->mass;
		float invMassB = 1.f / _otherRb.mass;
		float invMassSum = invMassA + invMassB;

		
		Vector2 correction = normal * (penetration/ invMassSum);

		newCirclePos += correction * invMassA;
		collider.transform().position -= Vector2::RotateAngle(correction * invMassB, collider.transform().position, collider.transform().rotation);
	}
	else
	{
		newCirclePos += normal * penetration;
	}

	transform().position = Vector2::RotateAngle(newCirclePos, collider.transform().position, collider.transform().rotation);

	return;

}

void CircleCollider2D::CircleWithCircle(CircleCollider2D& collider)
{
	// ----- Création des collisions ----- \\

	Vector2 dir = Vector2::Edge(collider.transform().position, transform().position);

	float penetration = radius + collider.radius - dir.magnitude();
	dir.normalize();

	if (collider.owner->GetComponent<Rigidbody2D>() != nullptr)
	{
		Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

		float invMassA = 1.f / body->mass;
		float invMassB = 1.f / _otherRb.mass;
		float invMassSum = invMassA + invMassB;


		Vector2 correction = dir * (penetration / invMassSum);

		transform().position += correction * invMassA;
		collider.transform().position -= correction * invMassB;
	}
	else
		transform().position += dir * penetration;
}

void CircleCollider2D::PolygonWithCircle(PolygonCollider2D& collider)
{
	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool polyColl = false;
	bool circleColl = false;

	for (int i = 0; i < collider.OBBvertices.size(); i++)
	{
		int j = i + 1 == collider.OBBvertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(collider.OBBvertices[i], collider.OBBvertices[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = collider.GetProjectInterval(n);
		Vector2 circleInter = GetProjectInterval(n);

		float sepa = Mathf::Min(polyInter.y(), circleInter.y()) - Mathf::Max(polyInter.x(), circleInter.x());

		polyColl = sepa > 0;

		if (!polyColl)
			break;
		else
		{
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (circleInter.x() + circleInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	Vector2 minCloset;
	float minClos = INFINITY;
	for (int i = 0; i < collider.OBBvertices.size(); i++)
	{
		int j = i + 1 == collider.OBBvertices.size() ? 0 : i + 1;

		Vector2 closet = Vector2::ClosetPointOnSegment(transform().position - collider.transform().position, 
			collider.OBBvertices[i], collider.OBBvertices[j]);

		if (Vector2::Edge(transform().position - collider.transform().position, closet).magnitude() < minClos)
		{
			minClos = Vector2::Edge(transform().position - collider.transform().position, closet).magnitude();
			minCloset = Vector2::Edge(transform().position - collider.transform().position, closet);
		}
	}

	{

		Vector2 n = minCloset.normal();

		Vector2 polyInter = collider.GetProjectInterval(n);
		Vector2 circleInter = GetProjectInterval(n);

		float sepa = Mathf::Min(polyInter.y(), circleInter.y()) - Mathf::Max(polyInter.x(), circleInter.x());

		circleColl = sepa > 0;
		if (circleColl)
		{

			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (circleInter.x() + circleInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	if (polyColl && circleColl)
	{
		Vector2 normal = minSepaNormal.normalized();

		if (collider.owner->GetComponent<Rigidbody2D>())
		{
			Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

			float invMassA = 1.f / body->mass;
			float invMassB = 1.f / _otherRb.mass;
			float invMassSum = invMassA + invMassB;

			Vector2 correction = normal * ((minSepa / minSepaNormal.magnitude()) / invMassSum);
			transform().position -= correction * invMassA;
			collider.transform().position += correction * invMassB;
		}
		else
		{
			transform().position -= normal * (minSepa / minSepaNormal.magnitude());
		}
	}
}
