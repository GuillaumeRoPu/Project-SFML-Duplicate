#include "Components/Physics/PolygonCollider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Objects/GameObject.h"


void PolygonCollider2D::Start()
{
	if (vertices.size() == 0)
	{
		vertices = { Vector2{-.5f, .5f} * transform().scale,
				Vector2{.5f, .5f} * transform().scale,
				Vector2{ 0.f, -.5f} * transform().scale };

	}

	body = owner->GetComponent<Rigidbody2D>();
}

void PolygonCollider2D::ApplyRotation()
{
	OBBvertices.clear();
	for (int i = 0; i < vertices.size(); i++)
	{
		OBBvertices.push_back(Vector2::RotateAngle(vertices[i], transform().rotation));
	}
}

void PolygonCollider2D::UpdateCollision(GameObject* collider)
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
				AABBWithPolygon(*_collider.GetComponent<BoxCollider2D>());

			if (_collider.GetComponent<CircleCollider2D>())
				CircleWithPolygon(*_collider.GetComponent<CircleCollider2D>());

			if (_collider.GetComponent<PolygonCollider2D>())
				PolygonWithPolygon(*_collider.GetComponent<PolygonCollider2D>());
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

bool PolygonCollider2D::isPointInside(Vector2 point)
{
	bool isInside = true;

	for (int i = 0; i < OBBvertices.size(); i++ )
	{
		int j = i + 1 == OBBvertices.size() ? 0 : i + 1;

		float area = (OBBvertices[j].x() - OBBvertices[i].x()) * (OBBvertices[j].y() + OBBvertices[i].y());

		// Calcul la distance entre la vertice currente et la suivante
		Vector2 v = Vector2::Edge(OBBvertices[i], OBBvertices[j]);

		// Calcul de la normal perpendiculaire de cette distance
		Vector2 n = v.normal();
		if (area < 0.f)
			n *= -1;

		// Calcul de la distance entre le point et la vertice currente
		Vector2 p = Vector2::Edge(OBBvertices[i] + transform().position, point);

		isInside = Vector2::Dot(p, n) <= 0;

		if (!isInside)
			break;
	}

	return isInside;
}

Vector2 PolygonCollider2D::GetProjectInterval(Vector2 normal)
{
	float min = INFINITY;
	float max = -INFINITY;
	for (int i = 0; i < OBBvertices.size(); i++)
	{
		float dot = Vector2::Dot(normal, OBBvertices[i] + transform().position);

		min = dot < min ? dot : min;
		max = max < dot ? dot : max;
	}

	return Vector2(min, max);
}

Vector2 PolygonCollider2D::center()
{
	Vector2 som;
	for (int i = 0; i < OBBvertices.size(); i++)
	{
		som += OBBvertices[i];
	}
	return som / (float)OBBvertices.size();
}

void PolygonCollider2D::AABBWithPolygon(BoxCollider2D& collider)
{
	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool polyColl = false;
	bool boxColl = false;
	for (int i = 0; i < OBBvertices.size(); i++)
	{
		int j = i + 1 == OBBvertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(OBBvertices[i], OBBvertices[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = GetProjectInterval(n);

		Vector2 boxInter = collider.GetProjectInterval(n);

		polyColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
		if (!polyColl)
			break;
		else
		{
			float sepa = Mathf::Min(polyInter.y(), boxInter.y()) - Mathf::Max(polyInter.x(), boxInter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal =  n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (boxInter.x() + boxInter.y()) / 2;
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

		Vector2 polyInter = GetProjectInterval(n);

		Vector2 boxInter = collider.GetProjectInterval(n);

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
			transform().position += correction * invMassA;
			collider.transform().position -= correction * invMassB;
		}
		else
		{
			transform().position += normal * (minSepa * correctionFactor);
		}
	}

}

void PolygonCollider2D::CircleWithPolygon(CircleCollider2D& collider)
{
	bool isCollide = false;
	Vector2 closet;
	for (int i = 0; i < OBBvertices.size(); i++)
	{
		int j = i + 1 == OBBvertices.size() ? 0 : i + 1;

		closet = Vector2::ClosetPointOnSegment(collider.transform().position,
			OBBvertices[i] + transform().position,
			OBBvertices[j] + transform().position);

		isCollide = collider.isPointInside(closet);
		if (isCollide)
			break;
	}

	if (isCollide)
	{
		Vector2 dir = Vector2::Edge(collider.transform().position, closet);

		float penetration = collider.radius - dir.magnitude();
		dir.normalize();

		if (collider.owner->GetComponent<Rigidbody2D>())
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
}

void PolygonCollider2D::PolygonWithPolygon(PolygonCollider2D& collider)
{
	float minSepa = INFINITY;
	Vector2 minSepaNormal = Vector2::zero();
	bool polyColl1 = false;
	bool polyColl2 = false;

	for (int i = 0; i < OBBvertices.size(); i++)
	{
		int j = i + 1 == OBBvertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(OBBvertices[i], OBBvertices[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = GetProjectInterval(n);

		Vector2 poly2Inter = collider.GetProjectInterval(n);

		polyColl1 = polyInter.y() >= poly2Inter.x() && poly2Inter.y() >= polyInter.x();
		if (!polyColl1)
			break;
		else
		{
			float sepa = Mathf::Min(polyInter.y(), poly2Inter.y()) - Mathf::Max(polyInter.x(), poly2Inter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (poly2Inter.x() + poly2Inter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	for (int i = 0; i < collider.OBBvertices.size(); i++)
	{
		int j = i + 1 == collider.OBBvertices.size() ? 0 : i + 1;

		Vector2 edge = Vector2::Edge(collider.OBBvertices[i], collider.OBBvertices[j]);
		Vector2 n = edge.normal();

		Vector2 polyInter = GetProjectInterval(n);

		Vector2 poly2Inter = collider.GetProjectInterval(n);

		polyColl2 = polyInter.y() >= poly2Inter.x() && poly2Inter.y() >= polyInter.x();
		if (!polyColl2)
			break;
		else
		{
			float sepa = Mathf::Min(polyInter.y(), poly2Inter.y()) - Mathf::Max(polyInter.x(), poly2Inter.x());
			if (sepa < minSepa)
			{
				minSepa = sepa;
				minSepaNormal = n;

				float d = (polyInter.x() + polyInter.y()) / 2 - (poly2Inter.x() + poly2Inter.y()) / 2;
				if (d < 0)
					minSepaNormal *= -1;

			}
		}
	}

	if (polyColl1 && polyColl2)
	{
		Vector2 normal = minSepaNormal.normalized();

		if (collider.owner->GetComponent<Rigidbody2D>())
		{
			Rigidbody2D& _otherRb = *collider.owner->GetComponent<Rigidbody2D>();

			float invMassA = 1.f / body->mass;
			float invMassB = 1.f / _otherRb.mass;
			float invMassSum = invMassA + invMassB;

			Vector2 correction = normal * ((minSepa / minSepaNormal.magnitude()) / invMassSum);
			transform().position += correction * invMassA;
			collider.transform().position -= correction * invMassB;
		}
		else
			transform().position += normal * (minSepa / minSepaNormal.magnitude());

	}
}
