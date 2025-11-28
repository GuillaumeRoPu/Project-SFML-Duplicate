#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <map>

#include "Engine\Mathf.h"

namespace fry
{
	const float pixelPerPoint = 50;

    class Vector2
    {
    public:
        // Vector with double zero axis (x and y)
        Vector2();
        // Vector with double axis (x and y)
        Vector2(float x, float y);
        // Vector with double axis (x and y)
        Vector2(int x, int y);

        // ---STATIC---
        // Get vector (0,1)
        static Vector2 up() { return { 0,1 }; }
        // Get vector (0,-1)
        static Vector2 down() { return { 0,-1 }; }
        // Get vector (1,0)
        static Vector2 right() { return { 1,0 }; }
        // Get vector (-1,0)
        static Vector2 left() { return { -1,0 }; }
        // Get vector (1,1)
        static Vector2 once() { return { 1,1 }; }
        // Get vector (0,0)
        static Vector2 zero() { return { 0,0 }; }

        // Get the vector of an angle in degrees
        static Vector2 VectorAngle(float angle) { return {Mathf::cos(angle), Mathf::sin(angle) }; }

        // Get the float angle in degrees between two vectors
        static float Angle(Vector2 from, Vector2 to);

        // Rotate a vector to an angle in degrees
        static Vector2 RotateAngle(Vector2 vector, float angle);
        static Vector2 RotateAngle(Vector2 vector, Vector2 center, float angle);

        // Get the the linearly interpolate vector between two vectors by time
        static Vector2 Lerp(Vector2 initialVector, Vector2 resultVector, float time);

        // Get the closet point to p in the segment ab
        static Vector2 ClosetPointOnSegment(Vector2 p, Vector2 a, Vector2 b);

        // Get the distance vector between two vectors
        static Vector2 Edge(Vector2 from, Vector2 to);

        // Get the dot product of two vectors
        static float Dot(Vector2 a, Vector2 b);

        // Get a copy of the vector clamped magnitude with the max length
        static Vector2 ClampMagnitude(Vector2 vector, float maxLength);

        // Get a vector with max composant of two vectors
        static Vector2 Max(Vector2 a, Vector2 b);

        // Get a vector with min composant of two vectors
        static Vector2 Min(Vector2 a, Vector2 b);

        // ---METHODS---

        // Get the length of the vector
        float magnitude();

        // Get the square length of the vector
        float sqrMagnitude();

        // Get normalized vector, with a length of 1
        Vector2 normalized();

        // Set the length of the vector to 1
        Vector2 normalize();

        // Get the normal of the vector
        Vector2 normal();

        // Return the string of the vector
        std::string ToString();

        // Get the x coordinate
        float x() const { return _x; }

        // Get the y coordinate
        float y() const { return _y; }


        // ---OPERATORS--

        bool operator==(Vector2 v);
        bool operator!=(Vector2 v);

        Vector2 operator+(Vector2 v);
        Vector2 operator-(Vector2 v);
        Vector2 operator*(Vector2 v);
        Vector2 operator*(float f);
        Vector2 operator*(int i);
        Vector2 operator/(Vector2 v);
        Vector2 operator/(float f);
        Vector2 operator/(int i);

        void operator+=(Vector2 v);
        void operator-=(Vector2 v);
        void operator*=(Vector2 v);
        void operator*=(float f);
        void operator*=(int i);
        void operator/=(Vector2 v);
        void operator/=(float f);
        void operator/=(int i);

    private:
        float _x;
        float _y;
    };

	class Random
	{
    public:
        // Initialise le générateur aléatoire une seule fois
        static void Init();

        // Retourne un entier dans [min, max]
        static int Range(int min, int max);

        // Retourne un float dans [min, max]
        static float Range(float min, float max);

        // Retourne true ou false avec une chance de 50/50
        static bool Bool();

        // Retourne un float dans [0.0, 1.0)
        static float Value();

    private:
        inline static std::mt19937 generator; // Mersenne Twister (PRNG de haute qualité)
	};

    class Gradient
    {
    public:
        // Add a color to the gradient
        //@param Color: color to add
        //@param Time : color time [0-100]
        Gradient AddColor(sf::Color color, int time);

        // Get the color at give time
        //@param Time : time to the color [0-1]
        sf::Color GetColor(float time);

        int GetSize() { return (int)_colors.size(); }
        void SetGradient(std::map<int, sf::Color> g) { _colors = g; }
    private:
        std::map<int, sf::Color> _colors;

    };

    class Lerp
    {
    public:
        static sf::Color Color(sf::Color initalColor, sf::Color resultColor, float time);
    };

    class Rect
    {
    public:
        // Rectange with centered position and size
        Rect(Vector2 position, Vector2 size)
        {
            _position = position;
            _size = size;
        }
        // Rectange with centered position and size
        Rect()
        {
            _position = { 0,0 };
            _size = { 1,1 };
        }

        // Set a new size to the rect
        void setSize(Vector2 newSize) { _size = newSize; }

        // Set a new centered position to the rect
        void setPosition(Vector2 newPosition) { _position = newPosition; }

        // Return the size of the rect in pixel (1,1) = (50,50)
        //Vector2 PixelSize() { return _size * pixelPerPoint; }

        Vector2 center() const { return _position; }

        float left() { return _position.x() - _size.x() / 2; }
        float right() { return _position.x() + _size.x() / 2; }
        float top() { return _position.y() - _size.y() / 2; }
        float bottom() { return _position.y() + _size.y() / 2; }

        float height() { return _size.y(); }
        float width() { return _size.x(); }

    private:
        Vector2 _position;
        Vector2 _size;
    };

}