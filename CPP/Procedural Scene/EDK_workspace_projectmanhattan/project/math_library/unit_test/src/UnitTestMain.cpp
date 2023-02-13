#include "VectorTest.h"
#include "MatrixTest.h"

#include <stdio.h>
#include <stdlib.h> 

int main() {

	// Vectors
	UnitTest::Vector2Test();
	UnitTest::Vector3Test();
	UnitTest::Vector4Test();

	// Matrices
	UnitTest::Matrix2x2Test();
	UnitTest::Matrix3x3Test();
	UnitTest::Matrix4x4Test();

	system("pause"); // windows only...

	return 0;
}


