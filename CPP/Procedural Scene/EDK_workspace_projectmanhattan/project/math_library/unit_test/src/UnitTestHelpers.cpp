#include "UnitTestHelpers.h"
#include "Windows.h"
#include <stdio.h>
#include <cmath>

namespace UnitTest {

	int Test::numErrors = 0;
	int Test::numTests = 0;

	void Test::Log(const char* message)
	{
		printf(message);
		OutputDebugStringA(message);
	}

	void Test::Start(const char* name)
	{
		// reset
		numErrors = 0;
		numTests = 0;

		// log
		char text[512];

		sprintf_s(text, 512, "---------------------------------\n"
			"Starting: %s\n"
			"---------------------------------\n", name);

		Log(text);
	}

	void Test::End()
	{
		// log
		char text[512];

		sprintf_s(text, 512, "---------------------------------\n"
			"Results: %d/%d Errors\n"
			"---------------------------------\n\n", numErrors, numTests);

		Log(text);
	}

	void Assert::AreEqual(float expected, float actual, const char* message, const char* file, int line)
	{
		++Test::numTests;

		if (expected != actual)
		{
			++Test::numErrors;

			char text[512];
			sprintf_s(text, 512, "%s(%d): %s\n", file, line, message);
			Test::Log(text);
		}		
	}

	void Assert::AreEqual(float expected, float actual, float precission, const char* message, const char* file, int line)
	{
		++Test::numTests;

		if (abs(expected - actual) > precission)
		{
			++Test::numErrors;

			char text[512];
			sprintf_s(text, 512, "%s(%d): %s\n", file, line, message);
			Test::Log(text);
		}		
	}


}