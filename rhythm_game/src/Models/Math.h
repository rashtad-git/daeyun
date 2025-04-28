#pragma once

struct Point
{
	int x = 0;
	int y = 0;

	// 기본 생성자
	Point() :x(0), y(0) {}

	// 사용자 정의 생성자
	Point(int x, int y) : x(x), y(y) {}

	// 복사 생성자 / 복사 대입 연산자
	Point(const Point &) = default;
	Point &operator=(const Point &) = default;

	// 이동 생성자 / 이동 대입 연산자
	Point(Point &&) noexcept = default;
	Point &operator=(Point &&) noexcept = default;

	// 비교 연산자 (동등성 비교)
	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point &other) const
	{
		return !(*this == other);
	}

	Point operator+(const Point &other) const
	{
		return Point(x + other.x, y + other.y);
	}

	Point operator-(const Point& other) const
	{
		return Point(x - other.x, y - other.y);
	}
};

struct Size
{
	int width = 0;
	int height = 0;

	// 기본 생성자
	Size() : width(0), height(0) {}

	// 사용자 정의 생성자
	Size(int width, int height) : width(width), height(height) {}

	// 복사 생성자 / 복사 대입 연산자
	Size(const Size &) = default;
	Size &operator=(const Size &) = default;

	// 이동 생성자 / 이동 대입 연산자
	Size(Size &&) noexcept = default;
	Size &operator=(Size &&) noexcept = default;

	// 비교 연산자 (동등성 비교)
	bool operator==(const Size &other) const
	{
		return width == other.width && height == other.height;
	}

	bool operator!=(const Size &other) const
	{
		return !(*this == other);
	}

	Size operator+(const Size &other) const
	{
		return Size(width + other.width, height + other.height);
	}

	Size operator-(const Size& other) const
	{
		return Size(width - other.width, height - other.height);
	}
};

struct Rectangle
{
	Point position;
	Size size;

	Rectangle() = default;

	Rectangle(const Point &pos, const Size &sz)
		: position(pos), size(sz) {}

	Rectangle(int x, int y, int width, int height)
		: position(x, y), size(width, height) {}

	int Left() const { return position.x; }
	int Right() const { return position.x + size.width; }
	int Top() const { return position.y; }
	int Bottom() const { return position.y + size.height; }

	// 포함 여부 검사
	bool Contains(const Point &p) const
	{
		return Contains(p.x, p.y);
	}

	bool Contains(int x, int y) const
	{
		return (x >= position.x && x < position.x + size.width) &&
			   (y >= position.y && y < position.y + size.height);
	}

	// Rectangle 간 교차 여부 검사
	bool Intersects(const Rectangle &other) const
	{
		return !(position.x + size.width <= other.position.x ||
				 other.position.x + other.size.width <= position.x ||
				 position.y + size.height <= other.position.y ||
				 other.position.y + other.size.height <= position.y);
	}
};
