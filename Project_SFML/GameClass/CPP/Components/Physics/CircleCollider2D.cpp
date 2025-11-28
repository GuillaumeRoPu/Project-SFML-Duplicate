#include "Components\Physics\PolygonCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Objects/GameObject.h"

void CircleCollider2D::Start()
{
	radius = Mathf::Max(owner->transform->scale.x()/2, owner->transform->scale.y() / 2);
	body = owner->GetComponent<Rigidbody2D>();
}

void CircleCollider2D::UpdateCollision(std::unique_ptr<GameObject>& collider)
{
	auto& _collider = *collider;
	auto _c = _collider.GetComponent<Collider2D>();
	auto& _target = *owner;
	auto& _body = *body;

	auto it = std::find(_collision.begin(), _collision.end(), _c);

	if (findIntersection(owner->GetComponent<Collider2D>(), _c))
	{

		if (_collider.GetComponent<BoxCollider2D>())
			AABBWithCircle(*_collider.GetComponent<BoxCollider2D>());

		else if (_collider.GetComponent<CircleCollider2D>())
			CircleWithCircle(*_collider.GetComponent<CircleCollider2D>());

		else if (_collider.GetComponent<PolygonCollider2D>())
			PolygonWithCircle(*_collider.GetComponent<PolygonCollider2D>());

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

void CircleCollider2D::AABBWithCircle(BoxCollider2D& collider)
{
	// ----- Création des collisions ----- \\

	float left = collider.transform().position.x() + collider.bounds.left();
	float top = collider.transform().position.y() + collider.bounds.top();
	float right = collider.transform().position.x() + collider.bounds.right();
	float bottom = collider.transform().position.y() + collider.bounds.bottom();

	Vector2 closetPoint = { Mathf::clamp(transform().position.x(), left, left + collider.bounds.width()),
						Mathf::clamp(transform().position.y(), top, top + collider.bounds.height()) };

	Vector2 dir = Vector2::Edge(closetPoint, transform().position);

	float penetration = radius - dir.magnitude();
	Vector2 normal = dir / dir.magnitude();

	if (dir.magnitude() == 0.0f)
	{
		// Trouver la direction de sortie la plus courte
		float leftPen = transform().position.x() - left;
		float rightPen = right - transform().position.x();
		float topPen = transform().position.y() - top;
		float bottomPen = bottom - transform().position.y();

		// Trouver la plus petite
		float minPen = std::min({ leftPen, rightPen, topPen, bottomPen });

		if (minPen == leftPen)       normal = Vector2(-1, 0);
		else if (minPen == rightPen) normal = Vector2(1, 0);
		else if (minPen == topPen)   normal = Vector2(0, -1);
		else                         normal = Vector2(0, 1);

		penetration = radius + minPen; // ou selon ton système
	}
	else
	{
		// Cas normal : normal = direction du centre vers l’extérieur
		normal = dir / dir.magnitude();
		penetration = radius - dir.magnitude();
	}

	if (collider.owner->GetComponent<Rigidbody2D>() != nullptr)
	{
		Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

		float invMassA = 1.f / body->mass;
		float invMassB = 1.f / _otherRb.mass;
		float invMassSum = invMassA + invMassB;

		
		Vector2 correction = normal * (penetration/ invMassSum);

		transform().position += correction * invMassA;
		collider.transform().position -= correction * invMassB;
	}
	else
	{
		transform().position += normal * penetration;
	}

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
	bool isCollide = false;
	Vector2 closet;
	for (int i = 0; i < collider.vertices.size(); i++)
	{
		int j = i + 1 == collider.vertices.size() ? 0 : i + 1;

		closet = Vector2::ClosetPointOnSegment(transform().position,
			collider.vertices[i] + collider.transform().position,
			collider.vertices[j] + collider.transform().position);

		isCollide = isPointInside(closet);
		if (isCollide)
			break;
	}

	if (isCollide)
	{
		Vector2 dir = Vector2::Edge(transform().position, closet);

		float penetration = radius - dir.magnitude();
		dir.normalize();

		if (collider.owner->GetComponent<Rigidbody2D>())
		{
			Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

			float invMassA = 1.f / body->mass;
			float invMassB = 1.f / _otherRb.mass;
			float invMassSum = invMassA + invMassB;

			Vector2 correction = dir * (penetration / invMassSum);
			transform().position -= correction * invMassA;
			collider.transform().position += correction * invMassB;
		}
		else
			transform().position -= dir * penetration;
	}
}
