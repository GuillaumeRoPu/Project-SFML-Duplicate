#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components\Physics\PolygonCollider2D.h"
#include "Objects/GameObject.h"

void BoxCollider2D::Start()
{
	bounds.setSize(owner->transform->scale);
	body = owner->GetComponent<Rigidbody2D>();
}

void BoxCollider2D::ApplyRotation()
{
	Rect boxBounds({ transform().position.x() + bounds.center().x(),
								transform().position.y() + bounds.center().y() },
		{ bounds.width(), bounds.height() });

	OBBRect = { Vector2::RotateAngle({boxBounds.left(), boxBounds.top()}, transform().rotation),
	Vector2::RotateAngle({boxBounds.left(), boxBounds.bottom()}, transform().rotation) ,
	Vector2::RotateAngle({boxBounds.right(), boxBounds.bottom()}, transform().rotation) ,
	Vector2::RotateAngle({boxBounds.right(), boxBounds.top()}, transform().rotation) };
}

void BoxCollider2D::UpdateCollision(std::unique_ptr<GameObject>& collider)
{
	auto& _collider = *collider;
	auto _c = _collider.GetComponent<Collider2D>();
	auto& _target = *owner;
	auto& _body = *body;

	Rect boxBounds({ transform().position.x() + bounds.center().x(),
								transform().position.y() + bounds.center().y() },
		{ bounds.width(), bounds.height() });

	auto it = std::find(_collision.begin(), _collision.end(), _c);

	if (findIntersection(owner->GetComponent<Collider2D>(), _c))
	{ 

		if (_collider.GetComponent<BoxCollider2D>())
			AABBWithAABB(*_collider.GetComponent<BoxCollider2D>());

		if (_collider.GetComponent<CircleCollider2D>())
			CircleWithAABB(*_collider.GetComponent<CircleCollider2D>());

		if (_collider.GetComponent<PolygonCollider2D>())
			PolygonWithAABB(*_collider.GetComponent<PolygonCollider2D>());

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

bool BoxCollider2D::isPointInside(Vector2 point)
{
	const float e = 0.f;

	float left = transform().position.x() + bounds.left() - e;
	float top = transform().position.y() + bounds.top() - e;
	float right = transform().position.x() + bounds.right() + e;
	float bottom = transform().position.y() + bounds.bottom() + e;

	return point.x() > left && point.x() < right && point.y() > top && point.y() < bottom;
}

Vector2 BoxCollider2D::GetProjectInterval(Vector2 normal)
{

	Rect boxBounds({ transform().position.x() + bounds.center().x(),
								transform().position.y() + bounds.center().y() },
		{ bounds.width(), bounds.height() });

	float dot1 = Vector2::Dot({ boxBounds.left(), boxBounds.top() }, normal);
	float dot2 = Vector2::Dot({ boxBounds.left(), boxBounds.bottom() }, normal);
	float dot3 = Vector2::Dot({ boxBounds.right(), boxBounds.bottom() }, normal);
	float dot4 = Vector2::Dot({ boxBounds.right(), boxBounds.top() }, normal);

	float min = Mathf::Min(Mathf::Min(dot1, dot2), Mathf::Min(dot3, dot4));
	float max = Mathf::Max(Mathf::Max(dot1, dot2), Mathf::Max(dot3, dot4));

	return Vector2(min, max);
}

void BoxCollider2D::AABBWithAABB(BoxCollider2D& collider)
{

	Rect c_bounds = collider.bounds;

	// ----- Construction des AABB ----- \\

	Rect targetRect(
		Vector2(transform().position.x() + bounds.center().x(), transform().position.y() + bounds.center().y()),
		Vector2(bounds.width(), bounds.height())
	);

	Rect colliderRect(
		Vector2(collider.transform().position.x() + c_bounds.center().x(), collider.transform().position.y() + c_bounds.center().y()),
		Vector2(c_bounds.width(), c_bounds.height())
	);

	// ----- Création des collisions ----- \\

	float dx = targetRect.center().x() - colliderRect.center().x();
	float px = (bounds.width() / 2 + c_bounds.width() / 2) - std::abs(dx);

	float dy = targetRect.center().y() - colliderRect.center().y();;
	float py = (bounds.height() / 2 + c_bounds.height() / 2) - std::abs(dy);

	if (collider.owner->GetComponent<Rigidbody2D>() != nullptr)
	{
		Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

		float totalMass = body->mass + _otherRb.mass;
		float ratioA = _otherRb.mass / totalMass;
		float ratioB = body->mass / totalMass;
		if (px < py) {
			float move = px * .5f; // ratio
			transform().position += {(dx < 0 ? -move * ratioA : move * ratioA), 0.f};
			collider.transform().position += {(dx < 0 ? move * ratioB : -move * ratioB), 0.f};
		}
		else {
			float move = py * .5f;
			transform().position += {0.f, (dy < 0 ? -move * ratioA : move * ratioA)};
			collider.transform().position += {0.f, (dy < 0 ? move * ratioB : -move * ratioB)};
		}
	}
	else
	{
		if (px < py) {
			dx < 0 ? transform().position -= {px, 0.f} : transform().position += {px, 0.f};
			body->velocity = { 0.f, body->velocity.y()};
		}
		else {
			dy < 0 ? transform().position -= {0.f, py} : transform().position += {0.f, py};
			body->velocity = { body->velocity.x(), 0.f };
		}
	}
	
	return;

}

void BoxCollider2D::CircleWithAABB(CircleCollider2D& collider)
{
	// ----- Création des collisions ----- \\

	auto circle = collider.owner->GetComponent<CircleCollider2D>();

	float left = transform().position.x() + bounds.left();
	float top = transform().position.y() + bounds.top();

	Vector2 closetPoint = { Mathf::clamp(circle->transform().position.x(), left, left + bounds.width()),
						Mathf::clamp(circle->transform().position.y(), top, top + bounds.height()) };

	Vector2 dir = Vector2::Edge(closetPoint, circle->transform().position);


	float penetration = circle->radius - dir.magnitude();
	Vector2 normal = dir / dir.magnitude();
	if (collider.owner->GetComponent<Rigidbody2D>() != nullptr)
	{
		Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

		float invMassA = 1.f / body->mass;
		float invMassB = 1.f / _otherRb.mass;
		float invMassSum = invMassA + invMassB;

		Vector2 correction = normal * (penetration / invMassSum);

		circle->transform().position += correction * invMassB;
		transform().position -= correction * invMassA;
	}
	else
	{
		transform().position -= normal * penetration;
	}

	return;
}

void BoxCollider2D::PolygonWithAABB(PolygonCollider2D& collider)
{
	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool polyColl = false;
	bool boxColl = false;
	for (int i = 0; i < collider.vertices.size(); i++)
	{
		int j = i + 1 == collider.vertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(collider.vertices[i], collider.vertices[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = collider.GetProjectInterval(n);

		Vector2 boxInter = GetProjectInterval(n);

		polyColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
		if (!polyColl)
			break;
		else
		{
			float sepa = Mathf::Min(polyInter.y(), boxInter.y()) - Mathf::Max(polyInter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	const Vector2 boxAxes[] = { Vector2(0,1), Vector2(1,0) };
	for (Vector2 axe : boxAxes)
	{
		Vector2 polyInter = collider.GetProjectInterval(axe);

		Vector2 boxInter = GetProjectInterval(axe);

		boxColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
		if (!boxColl)
			break;
		else
		{
			float sepa = Mathf::Min(polyInter.y(), boxInter.y()) - Mathf::Max(polyInter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = axe;

				float d = (polyInter.x() + polyInter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	if (boxColl && polyColl)
	{
		Vector2 normal = minSepaNormal.normalized();
		bool isBoxAxis = (Mathf::abs(normal.x()) < 0.01f || Mathf::abs(normal.y()) < 0.01f);

		bool isCornerCollision = !isBoxAxis;

		float correctionFactor = isCornerCollision ? 0.5f : 1.0f;
		if (collider.owner->GetComponent<Rigidbody2D>())
		{
			Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

			float invMassA = 1.f / body->mass;
			float invMassB = 1.f / _otherRb.mass;
			float invMassSum = invMassA + invMassB;

			Vector2 correction = normal * ((minSepa * correctionFactor) / invMassSum);
			transform().position -= correction * invMassA;
			collider.transform().position += correction * invMassB;
		}
		else
		{
			transform().position -= normal * (minSepa * correctionFactor);
		}

	}
}
