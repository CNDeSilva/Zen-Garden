#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>
#include <stdio.h>




const int GRID_SIZE = 50;  // Resolution of pond mesh
const float POND_WIDTH = 3.0f;
const float POND_HEIGHT = 2.0f;
const float MAX_WAVE_HEIGHT = 0.1f;
float timeCounter = 0.0f;  // Time variable for wave animation



float rotation = 0.0;
double M_PI = 3.14;
const int size = 100;

//To on/off grids and axes
int gridOn = 0;
int axesOn = 0;

GLfloat light_pos0[] = { -size / 2, size / 2, 0, 1 };

GLuint tex;

void loadTexture() {
	tex = SOIL_load_OGL_texture(
		"H:/CSC3081/practice/download(1).jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (!tex) {
		printf("Texture loading failed: %s\n", SOIL_last_result());
	}
}

//variables to move the camera
GLfloat camX = 0.0; GLfloat camY = 0.0; GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

GLfloat sceT = 0.0;
GLfloat rot = 0.0;

////variables to  move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(line, -0.4, 20);
		glVertex3f(line, -0.4, -20);

		glVertex3f(20, -0.4, line);
		glVertex3f(-20, -0.4, line);
	}
	glEnd();
}

//Skylight
float skyColor[] = { 139, 90, 0 };
float initColor[] = { 191, 239, 255 };
float finalColor[] = { 104,131,139 };
float timeOfDay = 0;  // To switch from Sunrise to Sunset


void setLightingAndShading() {
	// Enable lighting
	glEnable(GL_LIGHTING); // tell OpenGL that we are setting the lighting

	// Create a light source - light source 0
	//glEnable(GL_LIGHT0);

	// Define the illumination or intensity of the light source
	GLfloat l0amb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat l0diff[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat l0spec[] = { 1.0, 1.0, 1.0, 1.0 };

	// Define the position of the light source
	GLfloat l0pos[] = { -10.0, 1.0, -0.5, 1.0 };

	// Set the properties of light source 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);

	// Create a light source - light source 1
	//glEnable(GL_LIGHT1);

	// Define the illumination or intensity of the light source
	GLfloat l1amb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat l1diff[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat l1spec[] = { 1.0, 1.0, 1.0, 1.0 };

	// Define the position of the light source
	GLfloat l1pos[] = { 10.0, 1.0, -0.5, 1.0 };

	// Set the properties of light source 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l1spec);
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);

	// Set the material properties for the surface
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH); // Phong Shading

	// Set the specular intensity
	GLfloat specRef[] = { 0.7, 0.7, 0.7, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specRef);

	// Set the shininess
	int shininess = 128;
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Set the ambient and diffuse properties
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}



void drawAxes() {
	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-200, 0, 0);
	glVertex3f(200, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -200, 0);
	glVertex3f(0, 200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 0, 200);

	glEnd();
}



void init(void) {
	glClearColor(0.5, 0.6, 0.5, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);



}




void drawLand() {
	float size = 5.0f;       // Length and width of the square
	float thickness = 0.5f;  // Thickness of the land

	glEnable(GL_TEXTURE_2D); //enable glu quadratic surface texture
	glBindTexture(GL_TEXTURE_2D, tex);


	// Top surface 
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, thickness, -size); // Bottom-left
	glTexCoord2f(5.0f, 0.0f); glVertex3f(size, thickness, -size); // Bottom-right
	glTexCoord2f(5.0f, 5.0f); glVertex3f(size, thickness, size); // Top-right
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-size, thickness, size); // Top-left
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Draw the sides and bottom 
	glBegin(GL_QUADS);
	// Front side
	glVertex3f(-size, 0.0f, -size);
	glVertex3f(size, 0.0f, -size);
	glVertex3f(size, thickness, -size);
	glVertex3f(-size, thickness, -size);

	// Back side
	glVertex3f(-size, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, thickness, size);
	glVertex3f(-size, thickness, size);

	// Left side
	glVertex3f(-size, 0.0f, -size);
	glVertex3f(-size, 0.0f, size);
	glVertex3f(-size, thickness, size);
	glVertex3f(-size, thickness, -size);

	// Right side
	glVertex3f(size, 0.0f, -size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, thickness, size);
	glVertex3f(size, thickness, -size);

	// Bottom surface
	glVertex3f(-size, 0.0f, -size);
	glVertex3f(size, 0.0f, -size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(-size, 0.0f, size);
	glEnd();
}
void drawStone(float x, float y, float z, float scale, float r, float g, float b) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale * 0.8f, scale); // Flatten the sphere for a "stone" look
	glColor3f(r, g, b);
	glutSolidSphere(0.6, 16, 16);
	glPopMatrix();
}

void drawSteppingStones() {
	float positions[7][3] = {
		{ -1.5f, 0.3f, -1.5f },
		{ -0.5f, 0.3f, -0.5f },
		{  0.5f, 0.3f, -1.0f },
		{  1.5f, 0.3f, -0.2f },
		{  2.5f, 0.3f, -1.0f },
		{  3.5f, 0.3f, -1.0f }

	};

	for (int i = 0; i < 6; ++i) {
		drawStone(positions[i][0], positions[i][1], positions[i][2], 0.7f, 0.7f, 0.7f, 0.7f); // Gray rock
	}
}

void drawFence() {
	float size = 5.0f;       // Half-length of the land (square)
	float fenceHeight = 2.0f; // Height of the fence
	float postWidth = 0.1f;  // Thickness of vertical posts
	float barWidth = 0.2f;   // Thickness of horizontal bars
	float barSpacing = 0.5f; // Vertical spacing between horizontal bars
	int numBars = 3;         // Number of horizontal bars
	int numVerticalBars = 15; // Increased number of vertical bars (cover entire fence)

	glColor3f(1.0f, 0.0f, 0.0f); // Fence color (red)

	// Draw vertical posts (cover all four sides)
	glBegin(GL_QUADS);
	for (float x = -size; x <= size; x += size * 2) {
		for (float z = -size; z <= size; z += size * 2) {
			// Draw post (as a thin vertical cube)
			glVertex3f(x - postWidth / 2, 0.0f, z - postWidth / 2);
			glVertex3f(x + postWidth / 2, 0.0f, z - postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z - postWidth / 2);
			glVertex3f(x - postWidth / 2, fenceHeight, z - postWidth / 2);

			glVertex3f(x - postWidth / 2, 0.0f, z + postWidth / 2);
			glVertex3f(x + postWidth / 2, 0.0f, z + postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z + postWidth / 2);
			glVertex3f(x - postWidth / 2, fenceHeight, z + postWidth / 2);
		}
	}
	glEnd();

	// Add vertical bars on all four sides (left, right, front, back)
	for (float z = -size; z <= size; z += size * 2) { // Front and back sides
		for (int i = 1; i <= numVerticalBars; i++) {
			float x = -size + i * (2 * size / (numVerticalBars + 1));

			// Vertical bar (as thin vertical cubes)
			glBegin(GL_QUADS);
			glVertex3f(x - postWidth / 2, 0.0f, z - postWidth / 2);
			glVertex3f(x + postWidth / 2, 0.0f, z - postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z - postWidth / 2);
			glVertex3f(x - postWidth / 2, fenceHeight, z - postWidth / 2);

			glVertex3f(x - postWidth / 2, 0.0f, z + postWidth / 2);
			glVertex3f(x + postWidth / 2, 0.0f, z + postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z + postWidth / 2);
			glVertex3f(x - postWidth / 2, fenceHeight, z + postWidth / 2);
			glEnd();

			// Triangular top for each vertical post
			glBegin(GL_TRIANGLES);
			// Front side of the triangle
			glVertex3f(x - postWidth / 2, fenceHeight, z - postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z - postWidth / 2);
			glVertex3f(x, fenceHeight + 0.3f, z);

			// Back side of the triangle
			glVertex3f(x - postWidth / 2, fenceHeight, z + postWidth / 2);
			glVertex3f(x + postWidth / 2, fenceHeight, z + postWidth / 2);
			glVertex3f(x, fenceHeight + 0.3f, z);
			glEnd();
		}
	}

	// Add vertical bars on the right side
	for (int i = 1; i <= numVerticalBars; i++) {
		float x = size - (size * 2 / (numVerticalBars + 1)) * i;

		// Vertical bar (right side)
		glBegin(GL_QUADS);
		glVertex3f(x - postWidth / 2, 0.0f, -size - postWidth / 2);
		glVertex3f(x + postWidth / 2, 0.0f, -size - postWidth / 2);
		glVertex3f(x + postWidth / 2, fenceHeight, -size - postWidth / 2);
		glVertex3f(x - postWidth / 2, fenceHeight, -size - postWidth / 2);

		glVertex3f(x - postWidth / 2, 0.0f, size + postWidth / 2);
		glVertex3f(x + postWidth / 2, 0.0f, size + postWidth / 2);
		glVertex3f(x + postWidth / 2, fenceHeight, size + postWidth / 2);
		glVertex3f(x - postWidth / 2, fenceHeight, size + postWidth / 2);
		glEnd();

		// Triangular top for right side vertical post
		glBegin(GL_TRIANGLES);
		// Front side of the triangle
		glVertex3f(x - postWidth / 2, fenceHeight, -size - postWidth / 2);
		glVertex3f(x + postWidth / 2, fenceHeight, -size - postWidth / 2);
		glVertex3f(x, fenceHeight + 0.3f, -size);

		// Back side of the triangle
		glVertex3f(x - postWidth / 2, fenceHeight, size + postWidth / 2);
		glVertex3f(x + postWidth / 2, fenceHeight, size + postWidth / 2);
		glVertex3f(x, fenceHeight + 0.3f, size);
		glEnd();
	}

	// Draw horizontal bars (cover all sides)
	glBegin(GL_QUADS);
	for (int i = 1; i <= numBars; i++) {
		float barHeight = i * barSpacing;

		// Back side
		glVertex3f(-size, barHeight, size - barWidth / 2);
		glVertex3f(size, barHeight, size - barWidth / 2);
		glVertex3f(size, barHeight, size + barWidth / 2);
		glVertex3f(-size, barHeight, size + barWidth / 2);

		// Front side
		glVertex3f(-size, barHeight, -size - barWidth / 2);
		glVertex3f(size, barHeight, -size - barWidth / 2);
		glVertex3f(size, barHeight, -size + barWidth / 2);
		glVertex3f(-size, barHeight, -size + barWidth / 2);

		// Right side only
		glVertex3f(size - barWidth / 2, barHeight, -size);
		glVertex3f(size - barWidth / 2, barHeight, size);
		glVertex3f(size + barWidth / 2, barHeight, size);
		glVertex3f(size + barWidth / 2, barHeight, -size);
	}
	glEnd();
}

void drawTree() {

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.14);
	glBegin(GL_POLYGON);

	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.00, 0.0);    // Tree
	glVertex3f(-0.74, 0.00, 0.0);

	glVertex3f(-0.74, 0.00, 0.0);
	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.74, -0.05, 0.4);   //Tree-left
	glVertex3f(-0.74, 0.00, 0.4);
	glVertex3f(-0.74, 0.00, 0.0);

	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.4);    //Tree-front
	glVertex3f(-0.74, -0.05, 0.4);
	glVertex3f(-0.74, -0.05, 0.0);

	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.00, 0.0);
	glVertex3f(-0.7, -0.00, 0.4);   //Tree-right
	glVertex3f(-0.7, -0.05, 0.4);
	glVertex3f(-0.7, -0.05, 0.0);

	glVertex3f(-0.74, 0.00, 0.0);
	glVertex3f(-0.7, 0.00, 0.0);
	glVertex3f(-0.7, 0.00, 0.4);    //Tree-back
	glVertex3f(-0.74, 0.00, 0.4);
	glVertex3f(-0.74, 0.00, 0.0);


	glEnd();
	glPopMatrix();

	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);
	glTranslatef(-0.74, -0.025, 0.4);     //tree-leaf
	glutSolidSphere(0.13, 36, 2);

	glPopMatrix();
	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);
	glTranslatef(-0.7, -0.025, 0.4);
	glutSolidSphere(0.13, 36, 2);       //tree-leaf

	glPopMatrix();

	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);
	glTranslatef(-0.72, -0.05, 0.4);
	glutSolidSphere(0.13, 36, 2);        //tree-leaf

	glPopMatrix();
	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);
	glTranslatef(-0.72, -0.0, 0.4);
	glutSolidSphere(0.13, 36, 2);        //tree-leaf

	glPopMatrix();


}

void drawPinkTree() {

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.14);
	glBegin(GL_POLYGON);

	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.00, 0.0);    // Tree
	glVertex3f(-0.74, 0.00, 0.0);

	glVertex3f(-0.74, 0.00, 0.0);
	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.74, -0.05, 0.4);   //Tree-left
	glVertex3f(-0.74, 0.00, 0.4);
	glVertex3f(-0.74, 0.00, 0.0);

	glVertex3f(-0.74, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.05, 0.4);    //Tree-front
	glVertex3f(-0.74, -0.05, 0.4);
	glVertex3f(-0.74, -0.05, 0.0);

	glVertex3f(-0.7, -0.05, 0.0);
	glVertex3f(-0.7, -0.00, 0.0);
	glVertex3f(-0.7, -0.00, 0.4);   //Tree-right
	glVertex3f(-0.7, -0.05, 0.4);
	glVertex3f(-0.7, -0.05, 0.0);

	glVertex3f(-0.74, 0.00, 0.0);
	glVertex3f(-0.7, 0.00, 0.0);
	glVertex3f(-0.7, 0.00, 0.4);    //Tree-back
	glVertex3f(-0.74, 0.00, 0.4);
	glVertex3f(-0.74, 0.00, 0.0);


	glEnd();
	glPopMatrix();

	glPushMatrix();

	glColor3f(1.0, 0.75, 0.8);
	glTranslatef(-0.74, -0.025, 0.4);     //tree-leaf
	glutSolidSphere(0.13, 36, 2);

	glPopMatrix();
	glPushMatrix();

	glColor3f(1.0, 0.75, 0.8);
	glTranslatef(-0.7, -0.025, 0.4);
	glutSolidSphere(0.13, 36, 2);       //tree-leaf

	glPopMatrix();

	glPushMatrix();

	glColor3f(1.0, 0.7, 0.8);
	glTranslatef(-0.72, -0.05, 0.4);
	glutSolidSphere(0.13, 36, 2);        //tree-leaf

	glPopMatrix();
	glPushMatrix();

	glColor3f(1.0, 0.7, 0.5);
	glTranslatef(-0.72, -0.0, 0.4);
	glutSolidSphere(0.13, 36, 2);        //tree-leaf

	glPopMatrix();


}


void drawBushes() {
	glColor3f(0.0f, 1.0f, 0.0f); // Color for bushes (green)

	// Bush radius and position offset
	float bushRadius = 0.5f;



	// **Right Side Bushes** (Near the right edge of the land)
	// 1st Bush: Front-right side near the corner
	glPushMatrix();
	glTranslatef(4.0f, 0.6f, -4.0f);  // Position near the front-right corner
	glutSolidSphere(bushRadius, 10, 10);  // Draw the bush (a sphere here)
	glPopMatrix();

	// 2nd Bush: Near the right side, in the middle
	glPushMatrix();
	glTranslatef(4.0f, 0.6f, -2.0f);  // Position near the middle of right side
	glutSolidSphere(bushRadius, 10, 10);  // Draw the bush (a sphere here)
	glPopMatrix();

	// 3rd Bush: Near the right side, towards the back
	glPushMatrix();
	glTranslatef(4.0f, 0.6f, 0.0f);  // Position near the back-right side
	glutSolidSphere(bushRadius, 10, 10);  // Draw the bush (a sphere here)
	glPopMatrix();

	// 4th Bush: Further along the right side towards the back-right corner
	glPushMatrix();
	glTranslatef(4.0f, 0.6f, 2.0f);  // Position further towards the back-right corner
	glutSolidSphere(bushRadius, 10, 10);  // Draw the bush (a sphere here)
	glPopMatrix();

	// 5th Bush: Near the back-right corner
	glPushMatrix();
	glTranslatef(4.0f, 0.6f, 4.0f);  // Position near the back-right corner
	glutSolidSphere(bushRadius, 10, 10);  // Draw the bush (a sphere here)
	glPopMatrix();
}




// Function to draw a cylinder
void drawCylinder(float base, float top, float height, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, base, top, height, 16, 16);
	gluDeleteQuadric(quad);
}

// Function to draw a cube
void drawCube(float size, float r, float g, float b) {
	glColor3f(r, g, b);
	glutSolidCube(size);
}

// Function to draw a sphere
void drawSphere(float radius, float r, float g, float b) {
	glColor3f(r, g, b);
	glutSolidSphere(radius, 16, 16);
}

//Function to draw stone lantern
void drawStoneLantern() {


	// Base
	glPushMatrix();
	glTranslatef(14.0f, 0.8f, 14.0f);
	glRotatef(90.0, 1, 0, 0);
	drawCylinder(0.3f, 0.25f, 0.3f, 0.9f, 0.9f, 0.9f); // White stone base
	glPopMatrix();

	// Shaft
	glPushMatrix();
	glTranslatef(14.0f, 1.6f, 14.0f);
	glRotatef(90.0, 1, 0, 0);
	drawCylinder(0.15f, 0.15f, 1.3f, 0.9f, 0.9f, 0.9f); // White stone shaft
	glPopMatrix();

	// Light chamber
	glPushMatrix();
	glTranslatef(14.0f, 1.9f, 14.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	drawCube(0.6f, 0.9f, 0.9f, 0.9f); // White stone light chamber
	glPopMatrix();

	// Top cap
	glPushMatrix();
	glTranslatef(14.0f, 2.1f, 14.0f);
	glScalef(1.5f, 0.2f, 1.5f);
	drawCube(0.6f, 0.9f, 0.9f, 0.9f); // Flat stone top
	glPopMatrix();

	// Decorative sphere at the very top
	glPushMatrix();
	glTranslatef(14.0f, 2.32f, 14.0f);
	drawSphere(0.1f, 0.9f, 0.9f, 0.9f); // Decorative sphere
	glPopMatrix();


}

// funtion to draw Torii Gate
void drawBottomCylinder() {
	GLUquadric* quad = gluNewQuadric();



	glPushMatrix();
	glRotatef(-90, 1, 0, 0);

	glColor3f(0, 0, 0);
	gluCylinder(quad, .6, .6, 1.5, 100, 100);
	glPopMatrix();

	gluDeleteQuadric(quad);
}

void drawMiddleCylinder() {
	GLUquadric* quad = gluNewQuadric();



	glPushMatrix();
	glRotatef(-90, 1, 0, 0);

	glColor3f(0, 0, 0);
	gluCylinder(quad, .5, .4, 0.3, 100, 100);
	glPopMatrix();

	gluDeleteQuadric(quad);
}

void drawUpMiddleCylinder() {
	GLUquadric* quad = gluNewQuadric();



	glPushMatrix();
	glRotatef(-90, 1, 0, 0);

	glColor3f(0, 0, 0);
	gluCylinder(quad, .4, .3, 0.3, 100, 100);
	glPopMatrix();

	gluDeleteQuadric(quad);
}

void drawSideCylinder() {
	GLUquadric* quad = gluNewQuadric();



	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3f(1.0, 0, 0);
	gluCylinder(quad, .4, .3, 8, 100, 100);
	glPopMatrix();

	gluDeleteQuadric(quad);
}

void drawMainPole() {
	glPushMatrix();

	drawBottomCylinder();

	drawSideCylinder();

	glTranslatef(0, 1.5, 0);
	drawMiddleCylinder();

	glTranslatef(0, 6, 0);
	drawUpMiddleCylinder();

	glPopMatrix();

}



void drawSPoles() {

	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-3, -8, 3);
	glPushMatrix();
	drawBottomCylinder();
	drawSideCylinder();
	glTranslatef(0, 1.5, 0);
	drawMiddleCylinder();
	glTranslatef(0, 6, 0);
	drawUpMiddleCylinder();
	glPopMatrix();



}

void drawSmallBar() {
	glPushMatrix();
	glColor3f(1.0, 0, 0);
	glScalef(9.0f, 0.4f, 0.4f);
	
	glutSolidCube(1.0);         // Draw a cube scaled to form a cuboid
	glPopMatrix();
}

void drawBar() {



	glPushMatrix();
	glColor3f(1.0, 0, 0);
	glScalef(8.0f, 0.2f, 0.2f); 
	glutSolidCube(1.0);         // Draw a cube scaled to form a cuboid
	glPopMatrix();


}

void drawThinBar() {
	glPushMatrix();
	glScalef(8.4f, 0.05f, 0.1f);
	glColor3f(1.0, 0, 0);
	glutSolidCube(1.0);         // Draw a cube scaled to form a cuboid
	glPopMatrix();
}

void drawTopBar() {
	glPushMatrix();
	glScalef(8.4f, 0.2f, 0.3f);
	glColor3f(1.0, 0, 0);
	glutSolidCube(1.0);         // Draw a cube scaled to form a cuboid
	glPopMatrix();
}

void drawTopLongBar() {
	glPushMatrix();
	glScalef(8.9f, 0.4f, 0.3f);
	glColor3f(0.0, 0, 0);
	glutSolidCube(1.0);         // Draw a cube scaled to form a cuboid
	glPopMatrix();
}

//Final Torii Gate
void drawToriiGate() {
	glPushMatrix();
	glScalef(0.9, 1.4, 0.9);
	glTranslated(0, 0, -1.5);
	drawMainPole();
	glTranslated(0, 0, 6);
	drawMainPole();

	glRotated(90, 0, 1, 0);
	glTranslated(2.8, 7, 0);
	drawBar();

	glTranslated(0, 0.1, 0);
	drawThinBar();

	glTranslatef(0, 0.8, 0);
	drawTopBar();

	glTranslatef(0, 0.3, 0);
	drawTopLongBar();

	glPopMatrix();







}


// Function to draw a cylinder (pillar)
void drawCylinder(float baseRadius, float topRadius, float height, int slices, int stacks, float r, float g, float b) {
	glColor3f(r, g, b);
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(quad);
}

// Function to draw a disk (base)
void drawDisk(float innerRadius, float outerRadius, int slices, float r, float g, float b) {
	//glColor3f(r, g, b);
	GLUquadric* quad = gluNewQuadric();
	gluDisk(quad, innerRadius, outerRadius, slices, 1);
	gluDeleteQuadric(quad);
}

// Function to draw a pyramid (roof)
void drawPyramid(float baseSize, float height, float r, float g, float b) {
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	// Front face
	glVertex3f(0, height, 0);
	glVertex3f(-baseSize / 2, 0, baseSize / 2);
	glVertex3f(baseSize / 2, 0, baseSize / 2);

	// Right face
	glVertex3f(0, height, 0);
	glVertex3f(baseSize / 2, 0, baseSize / 2);
	glVertex3f(baseSize / 2, 0, -baseSize / 2);

	// Back face
	glVertex3f(0, height, 0);
	glVertex3f(baseSize / 2, 0, -baseSize / 2);
	glVertex3f(-baseSize / 2, 0, -baseSize / 2);

	// Left face
	glVertex3f(0, height, 0);
	glVertex3f(-baseSize / 2, 0, -baseSize / 2);
	glVertex3f(-baseSize / 2, 0, baseSize / 2);
	glEnd();
}
// Function to draw a vertical bar (fence post)
void drawFencePost(float radius, float height, float r, float g, float b) {
	glColor3f(r, g, b);
	glPushMatrix();
	glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotate to make the cylinder vertical
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, radius, radius, height, 20, 20); // Cylinder for the post
	glPopMatrix();
	gluDeleteQuadric(quad);
}

// Function to draw the round fence with an open side
void drawRoundFence(float radius, int numPosts, float openAngle, float postHeight) {
	float angleStep = 360.0f / numPosts; // Angle between each post
	float halfOpenAngle = openAngle / 2.0f; // Half of the open side angle

	for (int i = 0; i < numPosts; ++i) {
		float angle = i * angleStep; // Current angle for the post
		if (angle > 360.0f - halfOpenAngle || angle < halfOpenAngle) {
			// Skip posts in the open section
			continue;
		}

		// Convert polar coordinates to Cartesian for post placement
		float x = radius * cos(angle * M_PI / 180.0f);
		float z = radius * sin(angle * M_PI / 180.0f);

		// Draw the post
		glPushMatrix();
		glTranslatef(x, 0.0f, z); // Move to the position of the post
		glRotatef(angle, 0.0f, 1.0f, 0.0f); // Align the post to face the center
		drawFencePost(0.05f, postHeight, 0.4f, 0.2f, 0.1f); // Dark brown post
		glPopMatrix();
	}
}

// Function to draw staircase steps
void drawStaircase(float stepWidth, float stepDepth, float stepHeight, int numSteps, float r, float g, float b) {
	glColor3f(r, g, b);

	for (int i = 0; i < numSteps; i++) {
		glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, i * stepHeight, -i * stepDepth);
		glScalef(stepWidth, stepHeight, stepDepth);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}

// draw pagoda

void drawPagodaRoof() {
	glPushMatrix();
	glTranslatef(0, 0.0f, 0);
	glRotatef(5.0, 0, 1, 0);
	drawPyramid(12.0f, 4.0f, 0.4f, 0.2f, 0.1f); // Dark brown
	glPopMatrix();
}
void drawPagodaFence() {

	glRotatef(175, 0, 1, 0);
	glTranslatef(0, 0, 0);
	drawRoundFence(5.8f, 30, 45.0, 10);

}
void drawPagoda() {



	// Draw base disk
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	drawCylinder(6, 6, 3, 0.8, 0.2, 0);
	drawDisk(0, 6.0, 50, 0.8, 0.2, 0);
	glPopMatrix();

	// stairecase
	glTranslatef(-8, -2.0, 0.0);
	glPushMatrix();

	drawStaircase(2.0, 0.6, 0.6, 5, 0.0, 0.0, 0.0);
	glPopMatrix();


	glTranslatef(10, 10, -9);
	//drawPagodaFence();
	drawPagodaRoof();

}
void drawEntrancePath() {
	//tile 1
	glPushMatrix();
	glTranslated(-15, 1, 0);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 1, 0);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 1, 0);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 2

	glPushMatrix();
	glTranslated(-13, 1, 0);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-13, 1, 0);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-13, 1, 0);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();
	//tile 3

	glPushMatrix();
	glTranslated(-11, 1, 0);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-11, 1, 0);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-11, 1, 0);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 4

	glPushMatrix();
	glTranslated(-9, 1, -2);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9, 1, -2);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9, 1, -2);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 5

	glPushMatrix();
	glTranslated(-9, 1, 2);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9, 1, 2);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9, 1, 2);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 6

	glPushMatrix();
	glTranslated(-7, 1, -4);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7, 1, -4);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7, 1, -4);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 7

	glPushMatrix();
	glTranslated(-7, 1, 4);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7, 1, 4);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7, 1, 4);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 8

	glPushMatrix();
	glTranslated(-5, 1, -2);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 1, -2);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 1, -2);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 9

	glPushMatrix();
	glTranslated(-3, 1, 0);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1, 0);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1, 0);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 10

	glPushMatrix();
	glTranslated(-1, 1, -2);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 1, -2);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 1, -2);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 11

	glPushMatrix();
	glTranslated(-5, 1, -6);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 1, -6);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 1, -6);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 12

	glPushMatrix();
	glTranslated(-3, 1, -8);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1, -8);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1, -8);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	//tile 13

	glPushMatrix();
	glTranslated(0, 1, -9);
	glScaled(8, 1, 16);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1, -9);
	glScaled(10, 1, 10);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1, -9);
	glScaled(16, 1, 8);
	drawCube(0.1, 0.4f, 0.2f, 0.3f);
	glPopMatrix();
}

// Function to draw square pagoda traditional building
void drawSquarePagoda() {
	glPushMatrix();
	glScaled(20, 1, 20);
	drawCube(0.1, 0.4f, 0.2f, 0.1f);

	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1.2, 0);
	glScaled(12, 24, 12);
	drawCube(0.1, 1.0f, 0.9f, 0.8f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2.4, 0);
	glScaled(14, 2, 14);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2.6, 0);
	glScaled(12, 2, 12);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2.8, 0);
	glScaled(10, 2, 10);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 3.6, 0);
	glScaled(8, 16, 8);
	drawCube(0.1, 1.0f, 0.9f, 0.8f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 4.4, 0);
	glScaled(10, 2, 10);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 4.6, 0);
	glScaled(8, 2, 8);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 4.8, 0);
	glScaled(6, 2, 6);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 5.3, 0);
	glScaled(4, 8, 4);
	drawCube(0.1, 1.0f, 0.9f, 0.8f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 5.7, 0);
	glScaled(6, 1, 6);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 5.8, 0);
	glScaled(4, 1, 4);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 5.9, 0);
	glScaled(2, 1, 2);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 6.0, 0);
	glScaled(1.5, 1, 1.5);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 6.05, 0);
	glScaled(1, 1, 1);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 6.1, 0);
	glScaled(0.5, 0.5, 0.5);
	drawCube(0.1, 0.6f, 0.2f, 0.1f);
	glPopMatrix();


}

// Function 0f pond shape
float pondDepth(float x, float y) {
	float dist = sqrt(pow(x / POND_WIDTH, 2) + pow(y / POND_HEIGHT, 2));
	return -0.5f + 0.3f * cos(dist * 5.0f); // Creates natural depth
}

// Function to animate water waves
float waveFunction(float x, float y, float time) {
	return MAX_WAVE_HEIGHT * sin(5.0f * x + time) * cos(5.0f * y + time);
}

// Draws pond water with wave animation
void drawWater() {
	glColor4f(0.0f, 0.5f, 1.0f, 0.7f);  // Semi-transparent water

	for (int i = 0; i < GRID_SIZE - 1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < GRID_SIZE; j++) {
			float x1 = POND_WIDTH * (i / (float)GRID_SIZE - 0.5f);
			float y1 = POND_HEIGHT * (j / (float)GRID_SIZE - 0.5f);
			float x2 = POND_WIDTH * ((i + 1) / (float)GRID_SIZE - 0.5f);
			float y2 = y1;

			float z1 = pondDepth(x1, y1) + waveFunction(x1, y1, timeCounter);
			float z2 = pondDepth(x2, y2) + waveFunction(x2, y2, timeCounter);

			glVertex3f(x1, z1, y1);
			glVertex3f(x2, z2, y2);
		}
		glEnd();
	}
}



// Full Landscape

void drawLandscape() {
	glPushMatrix();
	glTranslatef(0, 1.6, 10);
	glScalef(6, 2, 10);
	drawWater();
	glPopMatrix();

	glPushMatrix();
	drawEntrancePath();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(3, 0, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(0, 0, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(2, 2, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(1, 2, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(2, 0, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(1, 0, 0);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, 2, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, -2, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, 1.6, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, -1.6, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, 1.2, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1.4, -1.2, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1, 1, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-1, -1, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-0.6, 0.9, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(8, 8, 20);
	glTranslatef(-0.6, -0.9, 0);
	drawPinkTree();
	glPopMatrix();

	glPushMatrix();
	drawStoneLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-18, 0, -32);
	drawStoneLantern();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.98, 0.835, 0.647);
	glScalef(4, 1, 4);
	drawLand();
	drawFence();
	glPopMatrix();

	glPushMatrix();
	glScalef(1.5, 1, 1.5);
	glTranslatef(8, 0, -8);
	glPushMatrix();
	drawBushes();
	glPopMatrix();

	glPushMatrix();
	glScalef(1.5, 1, 1.5);
	glTranslatef(8, 0, 8);
	glPushMatrix();
	drawBushes();
	glPopMatrix();

	glPushMatrix();
	glScalef(1.5, 1, 1.5);
	glRotatef(90, 0, 1, 0);
	glTranslatef(-16, 0, 8);
	glPushMatrix();
	drawBushes();
	glPopMatrix();

	glPushMatrix();
	glScalef(1.5, 1, 1.5);
	glRotatef(90, 0, 1, 0);
	glTranslatef(8, 0, 8);
	glPushMatrix();
	drawBushes();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 10);
	glRotatef(-45, 0, 1, 0);
	glScalef(1.5, 1, 1.5);
	drawSteppingStones();



	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 0, -14);
	glRotatef(-25, 0, 1, 0);
	glScalef(1.5, 1, 1.5);
	drawSteppingStones();



	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 3, -9);
	drawPagoda();
	glPopMatrix();



}

// Function to draw the handle of the bridge
void drawHandle() {
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(10, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(11, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(12, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(13, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(14, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(15, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(16, 8, 0);
	drawCylinder(0.1, 0.1, 6, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glTranslated(-8, 0.15, 9.5);
	drawCylinder(0.15, 0.15, 7, 0, 0, 0);
	glPopMatrix();


}

// Function to draw the bridge
void drawBridge() {
	glPushMatrix();
	drawStaircase(5, 1.5, 2, 5, 0.4, 0.2, 0.2);

	glRotated(180, 0, 1, 0);
	glTranslated(-20, 0, 0);
	drawStaircase(-5, 1.5, 2, 5, 0.4, 0.2, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(10, 8, 0);
	glScaled(10, 1, 5);
	drawCube(1, 0.4, 0.2, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 14, -6);
	drawHandle();

	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 14, -10);
	drawHandle();

	glPopMatrix();

}

//function to draw a bench
void bench() {
	glPushMatrix();
	glScaled(4.5, 1, 14);
	glTranslated(0, 0.4, 0);
	drawCube(0.1, 0.3, 0.3, 0.3);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0.1, -0.6, 0);
	glScaled(0.5, 0.5, 8);
	drawCube(0.1, 0.3, 0.3, 0.3);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0.1, 0.6, 0);
	glScaled(0.5, 0.5, 8);
	drawCube(0.1, 0.3, 0.3, 0.3);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(-0.1, -0.6, 0);
	glScaled(0.5, 0.5, 8);
	drawCube(0.1, 0.3, 0.3, 0.3);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(-0.1, 0.6, 0);
	glScaled(0.5, 0.5, 8);
	drawCube(0.1, 0.3, 0.3, 0.3);
	glPopMatrix();
}

void drawRocks() {

	glPushMatrix();
	drawSphere(1, 0, 0, 0);
	glTranslated(0, 0, 1);
	drawSphere(0.5, 0, 0, 0);
	glPopMatrix();
}

// Function to draw grass plant

void plant()
{
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(0.9, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.8, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.7, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.75, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.85, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.725, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.775, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.825, -0.8);
	glVertex2f(0.8, -0.9);
	glVertex2f(0.875, -0.8);
	glVertex2f(0.8, -0.9);
	glEnd();


	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.6, -0.85);
	glVertex2f(0.5, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.4, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.45, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.55, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.425, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.475, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.525, -0.85);
	glVertex2f(0.5, -0.95);
	glVertex2f(0.575, -0.85);
	glVertex2f(0.5, -0.95);
	glEnd();



	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.275, -0.8);
	glVertex2f(0.175, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.075, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.125, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.225, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.1, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.15, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.2, -0.8);
	glVertex2f(0.175, -0.9);
	glVertex2f(0.25, -0.8);
	glVertex2f(0.175, -0.9);
	glEnd();



	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.275, -0.85);
	glVertex2f(-0.175, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.075, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.125, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.225, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.1, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.15, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.2, -0.85);
	glVertex2f(-0.175, -0.95);
	glVertex2f(-0.25, -0.85);
	glVertex2f(-0.175, -0.95);
	glEnd();

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.575, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.475, -0.8);
	glVertex2f(-0.375, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.425, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.525, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.4, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.45, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.5, -0.8);
	glVertex2f(-0.475, -0.9);
	glVertex2f(-0.55, -0.8);
	glVertex2f(-0.475, -0.9);
	glEnd();



	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(-0.9, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.8, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.7, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.75, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.85, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.725, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.775, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.825, -0.85);
	glVertex2f(-0.8, -0.95);
	glVertex2f(-0.875, -0.85);
	glVertex2f(-0.8, -0.95);
	glEnd();

}



// Display function
void display(void) {

	glClearColor(skyColor[0] / 255, skyColor[1] / 255, skyColor[2] / 255, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
	gluLookAt(0.0, 2.0 + camY, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//As time goes by sun moves across the sky
	light_pos0[0] += 0.1;
	if (light_pos0[0] > size)//new day
	{
		light_pos0[0] = -size;//sun go's back to start position to signify a new day
		skyColor[0] = 139;
		skyColor[1] = 90;
		skyColor[2] = 0;
		timeOfDay = 0;

	}

	//Draw the Sun(light0)
	glPushMatrix();
	glTranslatef(light_pos0[0], light_pos0[1], light_pos0[2]);
	glColor3f(1, 1, 0);
	glutSolidSphere(3, 8, 16);
	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);



	// move the scene (all the rendered environment) using keyboard keys
	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);

	//Draw a grid on the xz plane
	if (gridOn == 1) {
		glColor3f(1, 1, 1);
		drawGrid();
	}

	//draw the three axes
	if (axesOn == 1)
		drawAxes();

	setLightingAndShading();


	glTranslatef(sceT, 0, 0);
	glScalef(4, 1, 4);
	glRotated(90, 0, 1, 0);


	glPushMatrix();

	glPushMatrix();
	drawLandscape();
	glPopMatrix();


	glPushMatrix();
	glTranslated(-18, 0, -1);
	drawToriiGate();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, 1, -14);
	glScaled(4, 2, 4);
	drawSquarePagoda();
	glPopMatrix();

	glPushMatrix();
	glScaled(4, 4, 4);
	glTranslated(0, 1.15, 4.2);
	plant();
	glPopMatrix();

	glPushMatrix();
	glRotated(15, 0, 1, 0);
	glTranslated(-10, 0, 8);
	glScaled(0.8, 0.8, 0.4);
	drawBridge();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 1, 15);
	glScaled(2, 2, 2);
	bench();
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glTranslated(-16, 1, -11);
	glScaled(2, 2, 2);
	bench();
	glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
}


void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 1;

	if (key == GLUT_KEY_DOWN)
		camY -= 1;

	if (key == GLUT_KEY_RIGHT)
		sceT += 1;

	if (key == GLUT_KEY_LEFT)
		sceT -= 1;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'l')
		sceRY += 1;

	if (key == 'r')
		sceRY -= 1;

	if (key == 'Z')
		sceTZ += 1;

	if (key == 'z')
		sceTZ -= 1;

	if (key == 's')
		rot += 1;

	if (key == 'w')
		rot -= 1;

	if (key == '!')
		glDisable(GL_LIGHT0); // Light at -x

	if (key == '1')
		glEnable(GL_LIGHT0);

	if (key == '@')
		glDisable(GL_LIGHT1); // Light at +x

	if (key == '2')
		glEnable(GL_LIGHT1);

	//Grids and axes
	if (key == 'G')
		gridOn = 1;
	if (key == 'g')
		gridOn = 0;
	if (key == 'A')
		axesOn = 1;
	if (key == 'a')
		axesOn = 0;

	glutPostRedisplay();
}



void timer(int value)
{
	timeOfDay += 1;
	if (timeOfDay < 500) {
		skyColor[0] += (116 / 500.0);
		skyColor[1] += (165 / 500.0);
		skyColor[2] += (255 / 500.0);

	}
	else {
		skyColor[0] -= (171 / 500.0);
		skyColor[1] -= (144 / 500.0);
		skyColor[2] -= (136 / 500.0);
	}

	timeCounter += 0.005f;
	rotation += rotation >= 360.0 ? -rotation : 2;
	glutTimerFunc(16, timer, 0);//60 fps
	glutPostRedisplay();
}




int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Zen Garden");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutTimerFunc(60.0, timer, 1);

	init();
	glutMainLoop();
	return 0;
}
