#include "Object/GameObject.h"
#include "Components/Physics/Collider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/PolygonCollider2D.h"

void Collider2D::Start()
{

}

void Collider2D::UpdateCollision(std::unique_ptr<GameObject>& collider)
{

}

bool Collider2D::isPointInside(Vector2 point)
{
	return false;
}

bool Collider2D::findIntersection(Collider2D* firstCollider, Collider2D* secondCollider)
{
	auto& c1 = *firstCollider;
	auto& c2 = *secondCollider;
	bool isCollide = false;

	if (c1.owner->GetComponent<BoxCollider2D>())
	{
		auto box = c1.owner->GetComponent<BoxCollider2D>();

		if (c2.owner->GetComponent<BoxCollider2D>())
		{
			Rect boxBounds({ box->transform().position.x() + box->bounds.center().x(),
								box->transform().position.y() + box->bounds.center().y() },
				{ box->bounds.width(), box->bounds.height() });

			isCollide = c2.isPointInside({ boxBounds.left(), boxBounds.top() }) ||
				c2.isPointInside({ boxBounds.right(), boxBounds.top() }) ||
				c2.isPointInside({ boxBounds.left(), boxBounds.bottom() }) ||
				c2.isPointInside({ boxBounds.right(), boxBounds.bottom() });
		}
		else if (c2.owner->GetComponent<CircleCollider2D>())
		{
			auto circle = c2.owner->GetComponent<CircleCollider2D>();

			float left = c1.transform().position.x() + box->bounds.left();
			float top = c1.transform().position.y() + box->bounds.top();

			Vector2 closetPoint = { Mathf::clamp(circle->transform().position.x(), left, left + box->bounds.width()),
								Mathf::clamp(circle->transform().position.y(), top, top + box->bounds.height()) };

			Vector2 dir = Vector2::Edge(circle->transform().position, closetPoint);
			float distance = dir.sqrMagnitude();

			isCollide = distance < circle->radius * circle->radius;
		}
		else if (c2.owner->GetComponent<PolygonCollider2D>())
		{
			auto polygon = *c2.owner->GetComponent<PolygonCollider2D>();

			bool polyColl = false;
			bool boxColl = false;
			for (int i = 0; i < polygon.vertices.size(); i++)
			{
				int j = i + 1 == polygon.vertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon.vertices[i], polygon.vertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon.GetProjectInterval(n);

				Vector2 boxInter = box->GetProjectInterval(n);

				polyColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!polyColl)
					break;
			}

			const Vector2 boxAxes[] = { Vector2(0,1), Vector2(1,0) };
			for (Vector2 axe : boxAxes)
			{
				Vector2 polyInter = polygon.GetProjectInterval(axe);

				Vector2 boxInter = box->GetProjectInterval(axe);

				boxColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!boxColl)
					break;
			}

			isCollide = boxColl && polyColl;
		}


	}

	else if (c1.owner->GetComponent<CircleCollider2D>())
	{
		auto circle = c1.owner->GetComponent<CircleCollider2D>();
		float distance = 0.f;

		if (c2.owner->GetComponent<BoxCollider2D>())
		{
			auto box = c2.owner->GetComponent<BoxCollider2D>();

			float left = c2.transform().position.x() + box->bounds.left();
			float top = c2.transform().position.y() + box->bounds.top();

			Vector2 closetPoint = { Mathf::clamp(circle->transform().position.x(), left, left + box->bounds.width()),
								Mathf::clamp(circle->transform().position.y(), top, top + box->bounds.height())};

			Vector2 dir = Vector2::Edge(circle->transform().position, closetPoint);
			distance = dir.sqrMagnitude();

			isCollide = distance < circle->radius*circle->radius;
		}

		else if (c2.owner->GetComponent<CircleCollider2D>())
		{
			auto circle2 = c2.owner->GetComponent<CircleCollider2D>();
			Vector2 dir = Vector2::Edge(circle->transform().position, circle2->transform().position);
			distance = dir.magnitude();


			isCollide = distance < circle->radius + circle2->radius;
		}
		else if (c2.owner->GetComponent<PolygonCollider2D>())
		{
			auto polygon = c2.owner->GetComponent<PolygonCollider2D>();

			for (int i = 0; i < polygon->vertices.size(); i++)
			{
				int j = i + 1 == polygon->vertices.size() ? 0 : i + 1;

				Vector2 closet = Vector2::ClosetPointOnSegment(circle->transform().position,
					polygon->vertices[i] + polygon->transform().position,
					polygon->vertices[j] + polygon->transform().position);

				isCollide = circle->isPointInside(closet);
				if (isCollide)
					break;
			}
		}

	}

	else if (c1.owner->GetComponent<PolygonCollider2D>())
	{
		auto polygon = c1.owner->GetComponent<PolygonCollider2D>();

		if (c2.owner->GetComponent<BoxCollider2D>())
		{
			auto box = *c2.owner->GetComponent<BoxCollider2D>();

			bool polyColl = false;
			bool boxColl = false;
			for (int i = 0; i < polygon->vertices.size(); i++)
			{
				int j = i + 1 == polygon->vertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon->vertices[i], polygon->vertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 boxInter = box.GetProjectInterval(n);

				polyColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!polyColl)
					break;
			}

			const Vector2 boxAxes[] = { Vector2(0,1), Vector2(1,0) };
			for (Vector2 axe : boxAxes)
			{
				Vector2 polyInter = polygon->GetProjectInterval(axe);

				Vector2 boxInter = box.GetProjectInterval(axe);

				boxColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!boxColl)
					break;
			}
			
			isCollide = boxColl && polyColl;
		}

		else if (c2.owner->GetComponent<CircleCollider2D>())
		{
			auto circle = c2.owner->GetComponent<CircleCollider2D>();

			for (int i = 0; i < polygon->vertices.size(); i++)
			{
				int j = i + 1 == polygon->vertices.size() ? 0 : i + 1;

				Vector2 closet = Vector2::ClosetPointOnSegment(circle->transform().position, 
															polygon->vertices[i] + polygon->transform().position, 
															polygon->vertices[j] + polygon->transform().position);

				isCollide = circle->isPointInside(closet);
				if (isCollide)
					break;
			}
		}

		else if (c2.owner->GetComponent<PolygonCollider2D>())
		{
			auto polygon2 = c2.owner->GetComponent<PolygonCollider2D>();

			bool polyColl1 = false;
			bool polyColl2 = false;
			for (int i = 0; i < polygon->vertices.size(); i++)
			{
				int j = i + 1 == polygon->vertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon->vertices[i], polygon->vertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 poly2Inter = polygon2->GetProjectInterval(n);

				polyColl1 = polyInter.y() >= poly2Inter.x() && poly2Inter.y() >= polyInter.x();
				if (!polyColl1)
					break;
			}

			for (int i = 0; i < polygon2->vertices.size(); i++)
			{
				int j = i + 1 == polygon2->vertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon2->vertices[i], polygon2->vertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 poly2Inter = polygon2->GetProjectInterval(n);

				polyColl2 = polyInter.y() >= poly2Inter.x() && poly2Inter.y() >= polyInter.x();
				if (!polyColl2)
					break;
			}

			isCollide = polyColl1 && polyColl2;
		}
		
	}

	if (!isCollide)
	{
		if (c2.owner->GetComponent<BoxCollider2D>())
		{
			auto box = c2.owner->GetComponent<BoxCollider2D>();

			Rect boxBounds({ box->transform().position.x() + box->bounds.center().x(),
								box->transform().position.y() + box->bounds.center().y() },
				{ box->bounds.width(), box->bounds.height()});

			isCollide = c1.isPointInside({ boxBounds.left(), boxBounds.top()}) ||
				c1.isPointInside({ boxBounds.right(), boxBounds.top() }) ||
				c1.isPointInside({ boxBounds.left(), boxBounds.bottom()}) ||
				c1.isPointInside({ boxBounds.right(), boxBounds.bottom() });

		}
		if (c2.owner->GetComponent<PolygonCollider2D>())
		{
			auto polygon = c2.owner->GetComponent<PolygonCollider2D>();
			for (Vector2 v : polygon->vertices)
			{
				isCollide = c1.isPointInside(v + polygon->transform().position);
				if (isCollide)
					break;
			}
		}
	}

	return isCollide;
}