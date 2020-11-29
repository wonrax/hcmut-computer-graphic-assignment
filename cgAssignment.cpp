#include <windows.h>
#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>
#include <iostream>

using namespace std;

#define PI			3.1415926
#define DTR			0.0174533
#define	COLORNUM		14


float	ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0}, 
								{1.0, 1.0,  0.0}, { 1.0, 0.0, 1.0},{ 0.0, 1.0, 1.0}, 
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {0.69, 0.69, 0.69}};

class Point3 {
    public:
        float x, y, z;
        void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz;}
        void set(Point3& p) { x = p.x; y = p.y; z = p.z;}
        Point3() { x = y = z = 0;}
        Point3(float dx, float dy, float dz) { x = dx; y = dy; z = dz;}
};

class VertexID {
    public:
        int	vertIndex;
        int	colorIndex;
};

class Face {
    public:
        int	nVerts;
        VertexID* vert;
        
        Face() {
            nVerts = 0;
            vert = NULL;
        }
        ~Face() {
            if(vert !=NULL) {
                delete[] vert;
                vert = NULL;
            }
            nVerts = 0;
        }
};

class Mesh {
public:
	int	numVerts;
	Point3*	pt;
	
	int	numFaces;
	Face* face;
public:
	Mesh() {
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
	}
	~Mesh() {
		if (pt != NULL) delete[] pt;
		if(face != NULL) delete[] face;
		numVerts = 0;
		numFaces = 0;
	}
    
	void DrawWireframe() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int f = 0; f < numFaces; f++)
        {
            glBegin(GL_POLYGON);
            for (int v = 0; v < face[f].nVerts; v++)
            {
                int		iv = face[f].vert[v].vertIndex;

                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
    }
	void DrawColor(int colorNumIndex) {
        glPolygonMode(GL_FRONT, GL_FILL);
        for (int f = 0; f < numFaces; f++)
        {
            glBegin(GL_POLYGON);
            for (int v = 0; v < face[f].nVerts; v++)
            {
                int		iv = face[f].vert[v].vertIndex;
                int		ic = face[f].vert[v].colorIndex;
                
                ic = f % COLORNUM;
                // ic = colorNumIndex;

                glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]); 
                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
    }

	void CreateCylinder(int nSegment, float fHeight, float fRadius) {
        numVerts = (nSegment + 1) * 2;
        numFaces = nSegment * 3;

        pt = new Point3[numVerts];
        face = new Face[numFaces];

        pt[0].set(0, fHeight / 2, 0); //top centric point
        pt[1].set(0, -fHeight / 2, 0); //bottom centric point

        int firstTopPoint = 2;
        int firstBotPoint = 2 + nSegment;

        float alpha = 0;
        for (int i = 0; i < nSegment; i++)
        {
            float x = fRadius * sin(alpha * DTR);
            float y = fHeight / 2;
            float z = fRadius * cos(alpha * DTR);
            pt[i + 2].set(x, y, z);
            pt[i + 2 + nSegment].set(x, -y, z);

            //One top segment face
            face[i].nVerts = 3;
            face[i].vert = new VertexID[face[i].nVerts];
            face[i].vert[0].vertIndex = 0; //top centric point
            face[i].vert[1].vertIndex = i + firstTopPoint;
            if (i == nSegment - 1) face[i].vert[2].vertIndex = firstTopPoint;
            else face[i].vert[2].vertIndex = i + firstTopPoint + 1;
            //One bottom segment face
            face[i + nSegment].nVerts = 3;
            face[i + nSegment].vert = new VertexID[face[i + nSegment].nVerts];
            face[i + nSegment].vert[0].vertIndex = 1; //bottom centric point
            face[i + nSegment].vert[1].vertIndex = i + firstBotPoint;
            if (i == nSegment - 1) face[i + nSegment].vert[2].vertIndex = firstBotPoint;
            else face[i + nSegment].vert[2].vertIndex = i + firstBotPoint;

            //One segment side face
            face[i + 2 * nSegment].nVerts = 4;
            face[i + 2 * nSegment].vert = new VertexID[face[i + 2 * nSegment].nVerts];
            face[i + 2 * nSegment].vert[0].vertIndex = i + 2;
            face[i + 2 * nSegment].vert[1].vertIndex = i + 2 + nSegment;
            if (i == nSegment - 1) {
                face[i + 2 * nSegment].vert[2].vertIndex = 2 + nSegment;
                face[i + 2 * nSegment].vert[3].vertIndex = 2;
            }
            else {
                face[i + 2 * nSegment].vert[2].vertIndex = i + 2 + nSegment + 1;
                face[i + 2 * nSegment].vert[3].vertIndex = i + 2 + 1;
            }

            alpha += (float)360 / nSegment;
        }
    }

    void CreateCube(float fSize) {
	int i;

	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set( fSize, fSize, fSize);
	pt[2].set( fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set( fSize, -fSize, fSize);
	pt[6].set( fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);


	numFaces= 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	
	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for(i = 0; i<face[4].nVerts ; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for(i = 0; i<face[5].nVerts ; i++)
		face[5].vert[i].colorIndex = 5;

}
    void CreateShape1(float smallBase, float largeBase, int nSegment, float fHeight, float fDepth, float pivotFromLBase);
};

void Mesh::CreateShape1(float smallBase, float largeBase, int nSegment, float fHeight, float fDepth, float pivotFromLBase) {
    numVerts = (4 + nSegment) * 2;
    numFaces = (1 + nSegment) * 2 + 5 + nSegment;

    pt = new Point3[numVerts];
    face = new Face[numFaces];
    float positiveDepth = fDepth / 2;
    float positiveBoundY = largeBase / 2;
    float positiveBoundX = (fHeight + smallBase) / 2;

    float centricCircleX = -((fHeight + smallBase) / 2 - smallBase);
    float centricCircleY = - positiveBoundY;
    pt[0].set(centricCircleX, centricCircleY, positiveDepth);
    pt[3].set(centricCircleX, centricCircleY, - positiveDepth);

    pt[1].set(positiveBoundX, - positiveBoundY, positiveDepth);
    pt[2].set(positiveBoundX, positiveBoundY, positiveDepth);
    pt[4].set(positiveBoundX, - positiveBoundY, - positiveDepth);
    pt[5].set(positiveBoundX, positiveBoundY, - positiveDepth);

    int circleStartPoint = 6;
    pt[circleStartPoint].set(centricCircleX, largeBase / 2 - largeBase + smallBase, positiveDepth);
    pt[circleStartPoint + nSegment + 1].set(centricCircleX, largeBase / 2 - largeBase + smallBase, - positiveDepth);
    pt[circleStartPoint + nSegment].set(- positiveBoundX, - positiveBoundY, positiveDepth);
    pt[circleStartPoint + nSegment + 1 + nSegment].set(- positiveBoundX, - positiveBoundY, - positiveDepth);

    face[0].nVerts = 4;
    face[0].vert = new VertexID[face[0].nVerts];
    face[0].vert[0].vertIndex = 1;
    face[0].vert[1].vertIndex = 4;
    face[0].vert[2].vertIndex = 5;
    face[0].vert[3].vertIndex = 2;

    face[1].nVerts = 4;
    face[1].vert = new VertexID[face[1].nVerts];
    face[1].vert[0].vertIndex = 1;
    face[1].vert[1].vertIndex = 2;
    face[1].vert[2].vertIndex = 6;
    face[1].vert[3].vertIndex = 0;

    face[2].nVerts = 4;
    face[2].vert = new VertexID[face[2].nVerts];
    face[2].vert[0].vertIndex = 2;
    face[2].vert[1].vertIndex = 5;
    face[2].vert[2].vertIndex = circleStartPoint + nSegment + 1;
    face[2].vert[3].vertIndex = 6;

    face[3].nVerts = 4;
    face[3].vert = new VertexID[face[3].nVerts];
    face[3].vert[0].vertIndex = 5;
    face[3].vert[1].vertIndex = 4;
    face[3].vert[2].vertIndex = 3;
    face[3].vert[3].vertIndex = circleStartPoint + nSegment + 1;

    face[4].nVerts = 4;
    face[4].vert = new VertexID[face[4].nVerts];
    face[4].vert[0].vertIndex = 1;
    face[4].vert[1].vertIndex = 4;
    face[4].vert[2].vertIndex = 3;
    face[4].vert[3].vertIndex = 0;

    face[numFaces - 1].nVerts = 4;
    face[numFaces - 1].vert = new VertexID[face[numFaces - 1].nVerts];
    face[numFaces - 1].vert[0].vertIndex = 0;
    face[numFaces - 1].vert[1].vertIndex = 3;
    face[numFaces - 1].vert[2].vertIndex = numVerts - 1;
    face[numFaces - 1].vert[3].vertIndex = numVerts - nSegment - 2;

    float alpha = 0;
    for (int i = 0; i < nSegment; i++)
    {
        if (i < nSegment - 1) {
            alpha += ((float)90 / nSegment) * DTR;
            float circleX = - smallBase * sin(alpha) + centricCircleX;
            float circleY = smallBase * cos(alpha) - largeBase / 2;
            pt[i + circleStartPoint + 1].set(circleX, circleY, positiveDepth);
            pt[i + circleStartPoint + 1 + nSegment + 1].set(circleX, circleY, - positiveDepth);
        }
        face[i + 5].nVerts = 4;
        face[i + 5].vert = new VertexID[face[i + 5].nVerts];
        face[i + 5].vert[0].vertIndex = i + 6;
        face[i + 5].vert[1].vertIndex = i + 6 + nSegment + 1;
        face[i + 5].vert[2].vertIndex = i + 6 + nSegment + 2;
        face[i + 5].vert[3].vertIndex = i + 6 + 1;

        face[i + 5 + nSegment].nVerts = 3;
        face[i + 5 + nSegment].vert = new VertexID[face[i + 5 + nSegment].nVerts];
        face[i + 5 + nSegment].vert[0].vertIndex = 0;
        face[i + 5 + nSegment].vert[1].vertIndex = i + 6;
        face[i + 5 + nSegment].vert[2].vertIndex = i + 7;

        face[i + 5 + nSegment * 2].nVerts = 3;
        face[i + 5 + nSegment * 2].vert = new VertexID[face[i + 5 + nSegment * 2].nVerts];
        face[i + 5 + nSegment * 2].vert[0].vertIndex = 3;
        face[i + 5 + nSegment * 2].vert[1].vertIndex = i + 6 + nSegment + 2;
        face[i + 5 + nSegment * 2].vert[2].vertIndex = i + 6 + nSegment + 1;
    }
}


int		screenWidth = 1200;
int		screenHeight = 600;

Mesh shape1;

float camPosX = 1;
float camPosY = 3;
float camPosZ = 7;

bool isWireframeMode = false;
bool isOrtho = false;

void reshape(GLsizei width, GLsizei height) {
    screenWidth = width;
    screenHeight = height;
    float aspect = (float)width / height;
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();
    gluPerspective(60, aspect, 0.001, 300);
    // glutPostRedisplay();
}

void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
        glColor3f(1,0,0);
		glVertex3f(0, 0, 0);
		glVertex3f(4, 0, 0);

        glColor3f(0,1,0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 4, 0);

        glColor3f(0,0,1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 4);
	glEnd();
}

void myDisplay() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    if (!isOrtho) {
	    gluLookAt(camPosX, camPosY, camPosZ, 0, 0, 0, 0, 1, 0);
    }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	// glViewport(0, 0, screenWidth/2, screenHeight);
	
	// drawAxis();

	// glColor3f(0, 0, 0);
	// cylinder.DrawWireframe();

	glViewport(0, 0, screenWidth, screenHeight);

	
    if (!isWireframeMode) {
        shape1.DrawColor(1);
    }
    else {
        drawAxis();
        shape1.DrawWireframe();
    }

	glFlush();
    glutSwapBuffers();
}

void moveCamera(int direction) {
    //-1: left, 1: right, -2: down, 2: up...
    if (isOrtho) return;
    if (direction == 1 || direction == -1) {
        float moveAngle = - 10 * DTR;
        if (direction == 1) moveAngle = - moveAngle;

        float camRadius = sqrt(camPosX * camPosX + camPosZ * camPosZ);
        float camPosXid = camPosX / camRadius;
        float camPosZid = camPosZ / camRadius;

        float camAngle = acos(camPosXid);
        if (camPosZid < 0) {
            camAngle += 2 * (PI - camAngle);
        }
        float newCamAngle = fmod(camAngle + moveAngle, 2 * PI);

        camPosX = camRadius * cos(newCamAngle);
        camPosZ = camRadius * sin(newCamAngle);
    }
    else if (direction == 2 || direction == -2) {
        float moveVertical = 0.1;
        if (direction == 2) {
            camPosY += moveVertical;
        }
        else camPosY -= moveVertical;
    }
    else {
        float moveDistance = 0.1;
        if (direction == 3) moveDistance = - moveDistance;
        float dirAngle = atan(camPosZ/camPosX);
        camPosX += moveDistance * cos(dirAngle);
        camPosZ += moveDistance * sin(dirAngle);
    }

    // gluLookAt(camPosX, camPosY, camPosZ, 0, 0, 0, 0, 1, 0);
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            moveCamera(-1);
            break;
        case GLUT_KEY_RIGHT:
            moveCamera(1);
            break;
        case GLUT_KEY_UP:
            moveCamera(2);
            break;
        case GLUT_KEY_DOWN:
            moveCamera(-2);
            break;    
   }
}

void toggleWireframe() {
    isWireframeMode = !isWireframeMode;
    glutPostRedisplay();
}

void toggleOrtho() {
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    if (!isOrtho) {
        float aRatio = (float)screenWidth/screenHeight;
        glOrtho(- aRatio * 4, aRatio * 4, -4, 4, -300, 300);
        glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
        gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
    }
    else {
        gluPerspective(60, (float)screenWidth/screenHeight, 0.001, 300);
        glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
        gluLookAt(camPosX, camPosY, camPosZ, 0, 0, 0, 0, 1, 0);
    }
    isOrtho = !isOrtho;
    glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            moveCamera(3);
            break;
        case '-':
            moveCamera(-3);
            break;   
        case 'w':
            toggleWireframe();
            break;
        case 'W':
            toggleWireframe();
            break;
        case 'v':
            toggleOrtho();
            break;
        case 'V':
            toggleOrtho();
            break;
   }
}

void myInit()
{
	float	fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);
    gluPerspective(60, (float)screenWidth/screenHeight, 0.001, 300);
}

int main(int argc, char* argv[])
{

	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Ha Huy Long Hai - 1812064"); // open the screen window

	shape1.CreateShape1(0.6, 0.8, 12, 4, 0.5, 0);

    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyInput);
    glutReshapeFunc(reshape);
	myInit();
    glutDisplayFunc(myDisplay);
	  
	glutMainLoop();
	return 0;
}

