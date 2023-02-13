/*struct Vector3{
	
	float x, y, z;
	
};

struct Vector4{
	
	float x, y, z, t;
	
};

struct Matrix2{
	
	float component[4];
	
};

struct Matrix3{
	
	float component[9];
	
};

struct Matrix4{
	
	float component[16];
	
};

//---SUMA DE VECTORES---//
Vector2 SumaVector2(Vector2 vec1, Vector2 vec2){
	
	return {vec1.x+vec2.x, vec1.y+vec2.y};

}

Vector3 SumaVector3(Vector3 vec1, Vector3 vec2){
	
	return {vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z};
	
}

Vector4 SumaVector4(Vector4 vec1, Vector4 vec2){
	
	return {vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z, vec1.t+vec2.t};
	
}

//---RESTA DE VECTORES---//
Vector2 RestaVector2(Vector2 vec1, Vector2 vec2){
	
	return {vec2.x-vec1.x, vec2.y-vec1.y};

}

Vector3 RestaVector3(Vector3 vec1, Vector3 vec2){
	
	return {vec2.x-vec1.x, vec2.y-vec1.y, vec2.z-vec1.z};

}

Vector4 RestaVector4(Vector4 vec1, Vector4 vec2){
	
	return {vec2.x-vec1.x, vec2.y-vec1.y, vec2.z-vec1.z, vec2.t-vec1.t};

}

//---MULTIPLICACIÓN POR ESCALAR---//
Vector2 ProductVector2(Vector2 vec1, float escalar){
	
	return {vec1.x*escalar, vec1.y*escalar};
	
}

Vector3 ProductVector3(Vector3 vec1, float escalar){
	
	return {vec1.x*escalar, vec1.y*escalar, vec1.z*escalar};
	
}

Vector4 ProductVector4(Vector4 vec1, float escalar){
	
	return {vec1.x*escalar, vec1.y*escalar, vec1.z*escalar, vec1.t*escalar};
	
}

//---PRODUCTO ESCALAR---//
float DotProductVec2(Vector2 vec1, Vector2 vec2){
	
	return {(vec1.x*vec2.x) + (vec1.y*vec2.y)};
	
}

//---MÓDULO---//
float ModuleVector2(Vector2 vec1){
	
	return sqrt(vec1.x*vec1.x + vec1.y*vec1.y);
	
}

float ModuleVector3(Vector3 vec1){
	
	return sqrt(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z);
	
}

float ModuleVector4(Vector4 vec1){
	
	return sqrt(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z + vec1.t*vec1.t);
	
}

//---PRINT VECTOR---//
void ShowVector2(Vector2 vec1){
	
	printf("(%f, %f)", vec1.x, vec1.y);
	
}

void ShowVector3(Vector3 vec1){
	
	printf("(%f, %f, %f)", vec1.x, vec1.y, vec1.z);
	
}

void ShowVector4(Vector4 vec1){
	
	printf("(%f, %f, %f, %f)", vec1.x, vec1.y, vec1.z, vec1.t);
	
}

//---NORMALIZE VECTOR---//
Vector2 NormalizeVector2(Vector2 vec1){
	
	return {vec1.x/ModuleVector2(vec1), vec1.y/ModuleVector2(vec1)};
	
}

Vector3 NormalizeVector3(Vector3 vec1){
	
	return {vec1.x/ModuleVector3(vec1), vec1.y/ModuleVector3(vec1), vec1.z/ModuleVector3(vec1)};
	
}

Vector4 NormalizeVector4(Vector4 vec1){
	
	return {vec1.x/ModuleVector4(vec1), vec1.y/ModuleVector4(vec1), vec1.z/ModuleVector4(vec1), vec1.t/ModuleVector4(vec1)};
	
}

//---CHECK IF NORMALIZED---//
bool IsNormalizedVector2(Vector2 vec1){
	
	return (ModuleVector2(vec1) > 0.9999999 && 1.0000001 < ModuleVector2(vec1));
	
}

bool IsNormalizedVector3(Vector3 vec1){
	
	return (ModuleVector3(vec1) > 0.9999999 && 1.0000001 < ModuleVector3(vec1));
	
}

bool IsNormalizedVector4(Vector4 vec1){
	
	return (ModuleVector4(vec1) > 0.9999999 && 1.0000001 < ModuleVector4(vec1));
	
}

//---PERPENDICULARES---//
Vector2 NormalVector2v1(Vector2 vec1){
	
	return {-vec1.y, vec1.x};
	
}

Vector2 NormalVector2v2(Vector2 vec1){
	
	return {vec1.y, -vec1.x};
	
}

//---INPUT VECTOR---//
void InputVector2(Vector2 vec1){
	
	printf("[X]: ");
	scanf("%f", &vec1.x);
	printf("[Y]: ");
	scanf("%f", &vec1.y);

}

void InputVector3(Vector3 vec1){
	
	printf("[X]: ");
	scanf("%f", &vec1.x);
	printf("[Y]: ");
	scanf("%f", &vec1.y);
	printf("[Z]: ");
	scanf("%f", &vec1.z);
	
}

void InputVector4(Vector4 vec1){
	
	printf("[X]: ");
	scanf("%f", &vec1.x);
	printf("[Y]: ");
	scanf("%f", &vec1.y);
	printf("[Z]: ");
	scanf("%f", &vec1.z);
	printf("[T]: ");
	scanf("%f", &vec1.t);
	
}

//---SUMA DE MATRICES---//
Matrix2 Suma2Matrices2(Matrix2 m1, Matrix2 m2){
	
	Matrix2 matrix_result;
	
	matrix_result = {m1.component[0] + m2.component[0], m1.component[1] + m2.component[1],
					 m1.component[2] + m2.component[2], m1.component[3] + m2.component[3]};
					 
	return matrix_result;
	
}

Matrix3 Suma2Matrices3(Matrix3 m1, Matrix3 m2){
	
	Matrix3 matrix_result;
	
	matrix_result = {m1.component[0] + m2.component[0], m1.component[1] + m2.component[1], m1.component[2] + m2.component[2],
					 m1.component[3] + m2.component[3], m1.component[4] + m2.component[4], m1.component[4] + m2.component[5],
					 m1.component[6] + m2.component[6], m1.component[7] + m2.component[7], m1.component[8] + m2.component[8]};
					 
	return matrix_result;
	
}

Matrix4 Suma2Matrices4(Matrix4 m1, Matrix4 m2){
	
	Matrix4 matrix_result;
	
	matrix_result = {m1.component[0] + m2.component[0], m1.component[1] + m2.component[1], m1.component[2] + m2.component[2], m1.component[3] + m2.component[3],
					 m1.component[4] + m2.component[4], m1.component[5] + m2.component[5], m1.component[6] + m2.component[6], m1.component[7] + m2.component[7],
					 m1.component[8] + m2.component[8], m1.component[9] + m2.component[9], m1.component[10] + m2.component[10], m1.component[11] + m2.component[11],
					 m1.component[12] + m2.component[12], m1.component[13] + m2.component[13], m1.component[14] + m2.component[14], m1.component[15] + m2.component[15]};
					 
	return matrix_result;
	
}

//---MULTIPLICACIÓN MATRIZ POR ESCALAR---//
Matrix2 EscalarPorMatrix2(Matrix2 m1, float escalar){
	
	Matrix2 matrix_result;
	
	for(int i = 0; i < 4; ++i){
		
		matrix_result.component[i] = m1.component[i]*escalar;
		
	}
	
	return matrix_result;
	
}

Matrix3 EscalarPorMatrix3(Matrix3 m1, float escalar){
	
	Matrix3 matrix_result;
	
	for(int i = 0; i < 9; ++i){
		
		matrix_result.component[i] = m1.component[i]*escalar;
		
	}
	
	return matrix_result;
	
}

Matrix4 EscalarPorMatrix4(Matrix4 m1, float escalar){
	
	Matrix4 matrix_result;
	
	for(int i = 0; i < 16; ++i){
		
		matrix_result.component[i] = m1.component[i]*escalar;
		
	}
	
	return matrix_result;
	
}


//---SISTEMAS DE ECUACIONES---//
Vector3 Gauss2(Vector3 ec1, Vector3 ec2){
	
	float aux=1;
	Vector3 sol;
	
	aux = (-1)*(ec2.x/ec1.x);

	ec2.x  = ec1.x*aux +ec2.x;
			
	ec2.y  = ec1.y* aux+ec2.y;
	
	ec2.z = ec1.z*aux+ec2.z;
	
	
	sol.y = ec2.z/ec2.y;

	sol.x = (ec1.z-sol.y*ec1.y)/ec1.x;	
	
	sol.z = 1;
	
	return sol;
}

//---COLISIONES---//
bool CirclesCollide(Vector2 c1, Vector2 c2, float r1, float r2){	//c1: center 1, c2: center 2, r1: radius 1, r2: radius 2

	float distance;
	
	distance = ModuleVector2(RestaVector2(c2,c1));
	
	return distance < (r1+r2);
	
}

Vector3 LinesCollide(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2){
	
	Vector3 solution;
	Vector3 eq_1, eq_2; //Ecuación 1: Puntos X. Ecuación 2: Puntos Y.
	Vector2 side_vector = RestaVector2(p1, p2);
	Vector2 vector_director = RestaVector2(q1, q2);			
	
	eq_1.x = vector_director.x;
	eq_1.y = -side_vector.x;
	eq_1.z = p1.x - q2.x;// - q2.x;
	
	eq_2.x = vector_director.y;
	eq_2.y = -side_vector.y;
	eq_2.z = p1.y - q2.y;// - q2.y;
	
	solution = JBA::Gauss2(eq_1, eq_2);
	
	return solution;
			
}

Vector3 BarrierCollide(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2){
	
	Vector3 solution;
	Vector3 eq_1, eq_2; //Ecuación 1: Puntos X. Ecuación 2: Puntos Y.
	Vector2 side_vector = RestaVector2(p1, p2);
	Vector2 vector_director = RestaVector2(q1, q2);			
	
	eq_1.x = vector_director.x;
	eq_1.y = -side_vector.x;
	eq_1.z = p1.x - q1.x;// - q2.x;
	
	eq_2.x = vector_director.y;
	eq_2.y = -side_vector.y;
	eq_2.z = p1.y - q1.y;// - q2.y;
	
	solution = JBA::Gauss2(eq_1, eq_2);
	
	return solution;
			
}*/