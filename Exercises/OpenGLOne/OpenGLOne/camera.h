class Camera {
public:
	void camera() { builLoopUpTables(); };
	void builLoopUpTables();
	void position(	float positionX, float positionY, float positionZ,
					float viewX,	 float viewY,	  float viewZ,
					float upX,		 float upY,		  float upZ );
	void move(float speed);
	void rotate(float angle);
	void doViewTransform();

};

