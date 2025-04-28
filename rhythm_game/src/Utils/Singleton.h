#pragma once

template<typename T>
class Singleton
{
public:
	// Delete copy constructor and assignment operator to prevent copying
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// Public static accessor for the singleton instance
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	// Constructor must be private to prevent direct instantiation
	Singleton() = default;
	// Destructor should be public to allow proper cleanup
	virtual ~Singleton() = default;
};
