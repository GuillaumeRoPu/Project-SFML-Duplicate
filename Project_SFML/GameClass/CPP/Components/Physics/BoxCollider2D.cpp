#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/PolygonCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Objects/GameObject.h"

void BoxCollider2D::Start()
{
	bounds.setSize(owner->transform->scale);
	body = owner->GetComponent<Rigidbody2D>();
}

void BoxCollider2D::ApplyRotation()
{
	bounds.setSize(owner->transform->scale);

	Rect boxBounds({ transform().position.x() + bounds.center().x(),
								transform().position.y() + bounds.center().y() },
		{ bounds.width(), bounds.height() });

	OBBRect = { Vector2::RotateAngle({boxBounds.left(), boxBounds.top()}, transform().position, transform().rotation),
	Vector2::RotateAngle({boxBounds.left(), boxBounds.bottom()},transform().position, transform().rotation) ,
	Vector2::RotateAngle({boxBounds.right(), boxBounds.bottom()},transform().position, transform().rotation) ,
	Vector2::RotateAngle({boxBounds.right(), boxBounds.top()},transform().position, transform().rotation) };
}

void BoxCollider2D::UpdateCollision(GameObject* collider)
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

		if (!isTrigger && !_c->isTrigger)
		{
			if (_collider.GetComponent<BoxCollider2D>())
				AABBWithAABB(*_collider.GetComponent<BoxCollider2D>());

			if (_collider.GetComponent<CircleCollider2D>())
				CircleWithAABB(*_collider.GetComponent<CircleCollider2D>());

			if (_collider.GetComponent<PolygonCollider2D>())
				PolygonWithAABB(*_collider.GetComponent<PolygonCollider2D>());
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

bool BoxCollider2D::isPointInside(Vector2 point)
{
	Vector2 newPoint = Vector2::RotateAngle(point, transform().position, -transform().rotation);

	float left = transform().position.x() + bounds.left();
	float top = transform().position.y() + bounds.top();
	float right = transform().position.x() + bounds.right();
	float bottom = transform().position.y() + bounds.bottom();

	return newPoint.x() > left && newPoint.x() < right && newPoint.y() > top && newPoint.y() < bottom;
}

Vector2 BoxCollider2D::GetProjectInterval(Vector2 normal)
{

	float min = INFINITY;
	float max = -INFINITY;
	for (int i = 0; i < OBBRect.size(); i++)
	{
		float dot = Vector2::Dot(OBBRect[i], normal);
		min = dot < min ? dot : min;
		max = dot > max ? dot : max;
	}

	return Vector2(min, max);
}

void BoxCollider2D::AABBWithAABB(BoxCollider2D& collider)
{

	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool boxColl = false;
	bool box2Coll = false;

	for (int i = 0; i < OBBRect.size(); i++)
	{
		int j = i + 1 == OBBRect.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(OBBRect[i], OBBRect[j]);
		Vector2 n = edge.normal();

		Vector2 boxInter = GetProjectInterval(n);

		Vector2 box2Inter = collider.GetProjectInterval(n);

		boxColl = box2Inter.y() >= boxInter.x() && boxInter.y() >= box2Inter.x();
		if (!boxColl)
			break;
		else
		{
			float sepa = Mathf::Min(box2Inter.y(), boxInter.y()) - Mathf::Max(box2Inter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (box2Inter.x() + box2Inter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	for (int i = 0; i < collider.OBBRect.size(); i++)
	{
		int j = i + 1 == collider.OBBRect.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(collider.OBBRect[i], collider.OBBRect[j]);
		Vector2 n = edge.normal();

		Vector2 boxInter = GetProjectInterval(n);

		Vector2 box2Inter = collider.GetProjectInterval(n);

		box2Coll = box2Inter.y() >= boxInter.x() && boxInter.y() >= box2Inter.x();
		if (!box2Coll)
			break;
		else
		{
			float sepa = Mathf::Min(box2Inter.y(), boxInter.y()) - Mathf::Max(box2Inter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (box2Inter.x() + box2Inter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	if (boxColl && box2Coll)
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

void BoxCollider2D::CircleWithAABB(CircleCollider2D& collider)
{

	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool boxColl = false;
	bool circleColl = false;

	for (int i = 0; i < OBBRect.size(); i++)
	{
		int j = i + 1 == OBBRect.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(OBBRect[i], OBBRect[j]);
		Vector2 n = edge.normal();

		Vector2 boxInter = GetProjectInterval(n);

		Vector2 circleInter = collider.GetProjectInterval(n);

		boxColl = circleInter.y() >= boxInter.x() && boxInter.y() >= circleInter.x();
		if (!boxColl)
			break;
		else
		{
			float sepa = Mathf::Min(circleInter.y(), boxInter.y()) - Mathf::Max(circleInter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (circleInter.x() + circleInter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	Vector2 minCloset;
	float minClos = INFINITY;
	for (int i = 0; i < OBBRect.size(); i++)
	{
		int j = i + 1 == OBBRect.size() ? 0 : i + 1;

		Vector2 closet = Vector2::ClosetPointOnSegment(collider.transform().position,
			OBBRect[i], OBBRect[j]);

		if (Vector2::Edge(collider.transform().position, closet).magnitude() < minClos)
		{
			minClos = Vector2::Edge(collider.transform().position, closet).magnitude();
			minCloset = Vector2::Edge(collider.transform().position, closet);
		}
	}

	{

		Vector2 n = minCloset.normal();

		Vector2 boxInter = GetProjectInterval(n);
		Vector2 circleInter = collider.GetProjectInterval(n);

		float sepa = Mathf::Min(boxInter.y(), circleInter.y()) - Mathf::Max(boxInter.x(), circleInter.x());

		circleColl = sepa > 0;
		if (circleColl)
		{

			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (boxInter.x() + boxInter.y()) / 2 - (circleInter.x() + circleInter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	if (boxColl && circleColl)
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

void BoxCollider2D::PolygonWithAABB(PolygonCollider2D& collider)
{
	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool polyColl = false;
	bool boxColl = false;
	for (int i = 0; i < collider.OBBvertices.size(); i++)
	{
		int j = i + 1 == collider.OBBvertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(collider.OBBvertices[i], collider.OBBvertices[j]);
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

	for (int i = 0; i < OBBRect.size(); i++)
	{
		int j = i + 1 == OBBRect.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(OBBRect[i], OBBRect[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = collider.GetProjectInterval(n);

		Vector2 boxInter = GetProjectInterval(n);

		boxColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
		if (!boxColl)
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

	if (boxColl && polyColl)
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
