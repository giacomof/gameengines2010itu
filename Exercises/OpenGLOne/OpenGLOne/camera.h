class Camera {
public:
	void camera() { builLoopUpTables(); };
	
private:
	void builLoopUpTables();
	void position(	float positionX, float positionY, float positionZ,
					float viewX,	 float viewY,	  float viewZ,
					float upX,		 float upY,		  float upZ );
	void move(float speed);
	void rotate(int rotX, int rotY, int rotZ);
	void doViewTransform();

};

