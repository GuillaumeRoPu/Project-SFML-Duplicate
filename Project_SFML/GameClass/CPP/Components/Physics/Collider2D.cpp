#include "Components/Physics/Collider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/PolygonCollider2D.h"
#include "Objects/GameObject.h"

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
			for (int i = 0; i < box->OBBRect.size(); i++)
			{
				isCollide = c2.isPointInside(box->OBBRect[i]);
				if (isCollide)
					break;
			}
		}
		else if (c2.owner->GetComponent<CircleCollider2D>())
		{
			auto circle = c2.owner->GetComponent<CircleCollider2D>();

			for (int i = 0; i < box->OBBRect.size(); i++)
			{
				int j = i + 1 == box->OBBRect.size() ? 0 : i + 1;
				Vector2 closetPoint = Vector2::ClosetPointOnSegment(circle->transform().position, box->OBBRect[i], box->OBBRect[j]);

				Vector2 dir = Vector2::Edge(circle->transform().position, closetPoint);
				float distance = dir.sqrMagnitude();

				isCollide = distance < circle->radius * circle->radius;
				if (isCollide)
					break;
			}

		}
		else if (c2.owner->GetComponent<PolygonCollider2D>())
		{
			auto polygon = *c2.owner->GetComponent<PolygonCollider2D>();

			bool polyColl = false;
			bool boxColl = false;
			for (int i = 0; i < polygon.OBBvertices.size(); i++)
			{
				int j = i + 1 == polygon.vertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon.OBBvertices[i], polygon.OBBvertices[j]);
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

			Vector2 newCirclePos = Vector2::RotateAngle(circle->transform().position, c2.transform().position, -c2.transform().rotation);

			Vector2 closetPoint = { Mathf::clamp(newCirclePos.x(), left, left + box->bounds.width()),
								Mathf::clamp(newCirclePos.y(), top, top + box->bounds.height())};

			Vector2 dir = Vector2::Edge(newCirclePos, closetPoint);
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

			isCollide = polygon->isPointInside(circle->transform().position);
			if (!isCollide)
			{
				for (int i = 0; i < polygon->OBBvertices.size(); i++)
				{
					int j = i + 1 == polygon->OBBvertices.size() ? 0 : i + 1;

					Vector2 closet = Vector2::ClosetPointOnSegment(circle->transform().position,
						polygon->OBBvertices[i] + polygon->transform().position,
						polygon->OBBvertices[j] + polygon->transform().position);

					isCollide = circle->isPointInside(closet);
					if (isCollide)
						break;
				}
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
			for (int i = 0; i < polygon->OBBvertices.size(); i++)
			{
				int j = i + 1 == polygon->OBBvertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon->OBBvertices[i], polygon->OBBvertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 boxInter = box.GetProjectInterval(n);

				polyColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!polyColl)
					break;
			}

			for (int i = 0; i < box.OBBRect.size(); i++)
			{
				int j = i + 1 == box.OBBRect.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(box.OBBRect[i], box.OBBRect[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 boxInter = box.GetProjectInterval(n);

				boxColl = polyInter.y() >= boxInter.x() && boxInter.y() >= polyInter.x();
				if (!boxColl)
					break;
			}
			
			isCollide = boxColl && polyColl;
		}

		else if (c2.owner->GetComponent<CircleCollider2D>())
		{
			auto circle = c2.owner->GetComponent<CircleCollider2D>();

			for (int i = 0; i < polygon->OBBvertices.size(); i++)
			{
				int j = i + 1 == polygon->OBBvertices.size() ? 0 : i + 1;

				Vector2 closet = Vector2::ClosetPointOnSegment(circle->transform().position, 
															polygon->OBBvertices[i] + polygon->transform().position,
															polygon->OBBvertices[j] + polygon->transform().position);

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
			for (int i = 0; i < polygon->OBBvertices.size(); i++)
			{
				int j = i + 1 == polygon->OBBvertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon->OBBvertices[i], polygon->OBBvertices[j]);
				Vector2 n = edge.normal();

				Vector2 polyInter = polygon->GetProjectInterval(n);

				Vector2 poly2Inter = polygon2->GetProjectInterval(n);

				polyColl1 = polyInter.y() >= poly2Inter.x() && poly2Inter.y() >= polyInter.x();
				if (!polyColl1)
					break;
			}

			for (int i = 0; i < polygon2->OBBvertices.size(); i++)
			{
				int j = i + 1 == polygon2->OBBvertices.size() ? 0 : i + 1;

				Vector2 edge = Vector2::Edge(polygon2->OBBvertices[i], polygon2->OBBvertices[j]);
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
		if (c2.owner->GetComponent<BoxCollider2D>() && c1.owner->GetComponent<BoxCollider2D>())
		{
			auto box = c2.owner->GetComponent<BoxCollider2D>();

			for (int i = 0; i < box->OBBRect.size(); i++)
			{
				isCollide = c1.isPointInside(box->OBBRect[i]);
				if (isCollide)
					break;
			}

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