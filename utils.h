#pragma once
#include <string>

/*
 * Case Insensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
bool startsWith(std::string mainStr, std::string toMatch)
{
	// Convert mainStr to lower case
	std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
	// Convert toMatch to lower case
	std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);

	return (mainStr.find(toMatch) == 0);
}
