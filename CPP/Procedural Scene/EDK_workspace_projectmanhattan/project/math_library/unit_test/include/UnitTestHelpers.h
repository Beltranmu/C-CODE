#ifndef __UNIT_TEST_HELPERS_H__
#define __UNIT_TEST_HELPERS_H__ 1

namespace UnitTest
{	
	// Test utils
	class Test
	{
	public:	
		static void Log(const char* message);
		
		static void Start(const char* name);
		static void End();

	public:
		static int numErrors;
		static int numTests;

	};

	// Assert utils
	class Assert
	{
	public:
		static void AreEqual(float expected, float actual, const char* message, const char* file, int line);		
		static void AreEqual(float expected, float actual, float precission, const char* message, const char* file, int line);		
	};
}

#endif


