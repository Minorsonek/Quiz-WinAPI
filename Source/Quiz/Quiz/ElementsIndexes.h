#include <string>
#include <sstream>
#pragma once

/* ID for recognising */
#define ID_BUTTONA 501
#define ID_BUTTONB 502
#define ID_BUTTONC 503
#define ID_BUTTOND 504
#define ID_BUTTONSTART 505
#define ID_BUTTONHELP 506

template <typename T>
std::string ToString(T val)
{
	std::stringstream stream;
	stream << val;
	return stream.str();
}