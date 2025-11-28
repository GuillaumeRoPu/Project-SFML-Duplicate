#include "Engine/FryEngine.h"
using namespace fry;

Vector2::Vector2()
{
	_x = 0.F;
	_y = 0.F;
}

Vector2::Vector2(float x, float y)
{
	_x = x;
	_y = y;
}

Vector2::Vector2(int x, int y)
{
	_x = (float)x;
	_y = (float)y;
}

#pragma region STATIC

float Vector2::Angle(Vector2 from, Vector2 to)
{
	float angle = Mathf::atan2(to.y(), to.x()) - Mathf::atan2(from.y(), from.x());
	return angle;
}

Vector2 fry::Vector2::RotateAngle(Vector2 vector, float angle)
{
	float currentAngle = Vector2::Angle(Vector2::right(), vector.normalized());
	Vector2 a = { Mathf::cos(currentAngle + angle), Mathf::sin(currentAngle + angle) };
	vector = a * vector.magnitude();
	return vector;
}
Vector2 Vector2::RotateAngle(Vector2 vector, Vector2 center, float angle)
{
	float currentAngle = Vector2::Angle(Vector2::right(), (vector - center).normalized());
	Vector2 a = { Mathf::cos(currentAngle + angle), Mathf::sin(currentAngle + angle) };
	vector = a * (vector - center).magnitude();
	return vector + center;
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	Vector2 v = Vector2(Mathf::Lerp(a.x(), b.x(), t), Mathf::Lerp(a.y(), b.y(), t));

	return v;
}

Vector2 Vector2::ClosetPointOnSegment(Vector2 p, Vector2 a, Vector2 b)
{

	Vector2 v = Vector2::Edge(a, b);

	Vector2 s = Vector2::Edge(a, p);

	float dotPs = Vector2::Dot(v, s);
	if (dotPs <= 0.f)
	{
		return a;
	}

	float dotSs = Dot(v, v);

	if (dotPs >= dotSs)
	{
		return b;
	}

	return Vector2(
		a.x() + (dotPs / dotSs) * v.x(),
		a.y() + (dotPs / dotSs) * v.y()
	);
}

Vector2 Vector2::Edge(Vector2 from, Vector2 to)
{
	return {to.x() - from.x(), to.y() - from.y()};
}

float Vector2::Dot(Vector2 a, Vector2 b)
{
	return a.x() * b.x() + a.y() * b.y();
}

Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength)
{
	if (vector.magnitude() > maxLength)
		vector = vector.normalized() * maxLength;

	return vector;
}

Vector2 Vector2::Max(Vector2 a, Vector2 b)
{
	return {Mathf::Max(a.x(), b.x()), Mathf::Max(a.y(), b.y()) };
}

Vector2 Vector2::Min(Vector2 a, Vector2 b)
{
	return { Mathf::Min(a.x(), b.x()), Mathf::Min(a.y(), b.y()) };
}

#pragma endregion

#pragma region METHODS

float Vector2::magnitude()
{
	return sqrtf(sqrMagnitude());
}

float Vector2::sqrMagnitude()
{
	return _x * _x + _y * _y;
}

Vector2 Vector2::normalized()
{
	if (magnitude() == 0) return Vector2::zero();

	return { _x / magnitude(), _y / magnitude() };
}

Vector2 Vector2::normalize()
{

	if (magnitude() == 0) return Vector2::zero();

	_x /= magnitude();
	_y /= magnitude();

	return { _x, _y };
}

Vector2 Vector2::normal()
{
	return { -_y, _x };
}

std::string fry::Vector2::ToString()
{
	return "{ " + std::to_string(_x) + ", " + std::to_string(_y) + " }";
}

#pragma endregion

#pragma region OPERATORS

bool Vector2::operator==(Vector2 v)
{
	return _x == v.x() && _y == v.y();
}

bool Vector2::operator!=(Vector2 v)
{
	return _x != v.x() || _y != v.y();
}


Vector2 Vector2::operator+(Vector2 v)
{
	return { _x + v.x(), _y + v.y() };
}

Vector2 Vector2::operator-(Vector2 v)
{
	return { _x - v.x(), _y - v.y() };
}

Vector2 Vector2::operator*(Vector2 v)
{
	return { _x * v.x(), _y * v.y() };
}

Vector2 Vector2::operator*(float f)
{
	return { _x * f, _y * f };
}

Vector2 Vector2::operator*(int i)
{
	return { _x * (float)i, _y * (float)i };
}

Vector2 Vector2::operator/(Vector2 v)
{
	return {_x/v.x(), _y/v.y()};
}

Vector2 Vector2::operator/(float f)
{
	return { _x / f, _y/f };
}

Vector2 fry::Vector2::operator/(int i)
{
	return {_x / (float)i, _y / (float)i};
}

void Vector2::operator+=(Vector2 v)
{
	_x += v.x();
	_y += v.y();
}

void Vector2::operator-=(Vector2 v)
{
	_x -= v.x();
	_y -= v.y();
}

void Vector2::operator*=(Vector2 v)
{
	_x *= v.x();
	_y *= v.y();
}

void Vector2::operator*=(float f)
{
	_x *= f;
	_y *= f;
}

void Vector2::operator*=(int i)
{
	_x *= (float)i;
	_y *= (float)i;
}

void fry::Vector2::operator/=(Vector2 v)
{
	_x /= v.x();
	_y /= v.y();
}

void fry::Vector2::operator/=(float f)
{
	_x /= f;
	_y /= f;
}

void fry::Vector2::operator/=(int i)
{
	_x /= (float)i;
	_y /= (float)i;
}

#pragma endregion