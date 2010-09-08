class Camera {
public:
	void camera() { builLoopUpTables(); };
	double const PI = 3.14;

private:
	void builLoopUpTables();
	void position(	float positionX, float positionY, float positionZ,
					float viewX,	 float viewY,	  float viewZ,
					float upX,		 float upY,		  float upZ );
	void move(float speed);
	void rotate(int rotX, int rotY, int rotZ);
	void doViewTransform();

	float sin_table[360], cos_table[360];
	Vector vPosition, vView, vUp;											// camera position, point of view, up vector

};

