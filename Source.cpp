#define STB_IMAGE_IMPLEMENTATION
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include "Glut.h"
#include "stb_image.h"


int windowWidth = 1600;
int windowHeight = 1200;
int score = 0;
float diverX = 0.0f;
float diverY = 0.0f;
float diverSpeed = 0.03f;
bool isFlipped = false;
int diverLives = 3;
GLuint diverTexture;
GLuint heartTexture;
float fishX = 0.5;
float fishY = 0.5;
float fishSpeed = 0.0001;
bool isFishFlipped = true;
float fishDirectionX = 0.0;
float fishDirectionY = 0.0;
float fishAcceleration = 0.00005;
float fishDeceleration = 0.0001;
float fishMaxSpeed = 0.001;
float fishDirectionChangeRange = 0.5;
GLuint fishTexture;
float sharkX = -0.5;
float sharkY = -0.5;
float sharkSpeed = 0.0002;
bool isSharkFlipped = false;
float sharkDirectionX = 0.0;
float sharkDirectionY = 0.0;
float sharkAcceleration = 0.0001;
float sharkDeceleration = 0.00005;
float sharkMaxSpeed = 0.0005;
float sharkDirectionChangeRange = 0.3;
GLuint sharkTexture;
int level = 1;
int scoreToIncreaseLevel = 10;
GLuint gameOverTexture;
bool gameOver = false;

// Function to load an image file as a texture
GLuint loadTexture(const char* filename) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

	if (image) {
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(image);
	}
	else {
		printf("Failed to load texture: %s\n", filename);
		stbi_image_free(image);
		return 0;
	}

	return textureID;
}

void initialize() {
	srand(static_cast<unsigned>(time(0)));
	// Omogucava da se transparentne slike lepo loaduju
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
}




void drawDiver() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, diverTexture);

	glPushMatrix();
	glTranslatef(diverX, diverY, 0.0);

	if (isFlipped) {
		glScalef(-1.0, 1.0, 1.0); 
	}

	float scale = 0.25;
	glScalef(scale, scale, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawFish() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fishTexture);

	glPushMatrix();
	glTranslatef(fishX, fishY, 0.0);

	if (isFishFlipped) {
		glScalef(-1.0, 1.0, 1.0); 
	}

	float scale = 0.15;
	glScalef(scale, scale, 1.0);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


void drawShark() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sharkTexture);

	glPushMatrix();
	glTranslatef(sharkX, sharkY, 0.0);

	if (isSharkFlipped) {
		glScalef(-1.0, 1.0, 1.0);
	}

	float scale = 0.5;
	glScalef(scale, scale, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void displayScore() {
	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1.0, 1.0, 1.0);
	char scoreText[100];
	sprintf_s(scoreText, "Score: %d  Level: %d", score, level); 


	glRasterPos2f(-0.95, 0.9); 
	for (int i = 0; scoreText[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]); 
	}
	glPopAttrib();
}


void drawHearts() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, heartTexture);

	glPushMatrix();
	glTranslatef(-0.05, 0.9, 0.0);

	float scale = 0.1;
	glScalef(scale, scale, 1.0);

	for (int i = 0; i < diverLives; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
		glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
		glEnd();

		glTranslatef(1.0, 0.0, 0.0);
	}

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}



void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	
	drawFish();
	drawShark();
	drawDiver();
	displayScore();
	drawHearts();
	
	

	if (gameOver) {
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, gameOverTexture);
		
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
		glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
		glEnd();
		

	}


	glutSwapBuffers();


}


void reshapeWindow(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

float getRandomDirection(float range) {
	return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0 - 1.0) * range;
}



void update() {
	

		float fishSpeedX = fishSpeed * fishDirectionX;
		float fishSpeedY = fishSpeed * fishDirectionY;
		float fishSpeedMagnitude = sqrt(fishSpeedX * fishSpeedX + fishSpeedY * fishSpeedY);

		
		if (fishSpeedMagnitude < fishMaxSpeed) {
			fishSpeedMagnitude += fishAcceleration;
		}

		
		if (fabs(fishDirectionX) < 0.001 && fabs(fishDirectionY) < 0.001) {
			fishSpeedMagnitude -= fishDeceleration;
			if (fishSpeedMagnitude < 0.0) {
				fishSpeedMagnitude = 0.0;
			}
		}

		
		fishSpeedX = (fishSpeedX / fishSpeedMagnitude) * fishSpeed;
		fishSpeedY = (fishSpeedY / fishSpeedMagnitude) * fishSpeed;

		
		fishX += fishSpeedX;
		fishY += fishSpeedY;

		
		if (fishX > 1.0 || fishX < -1.0 || fishY > 1.0 || fishY < -1.0) {
			fishDirectionX = -fishDirectionX;
			fishDirectionY = -fishDirectionY;
			isFishFlipped = !isFishFlipped;
			fishDirectionX += getRandomDirection(fishDirectionChangeRange);
			fishDirectionY += getRandomDirection(fishDirectionChangeRange);
		}

		
		float sharkSpeedX = sharkSpeed * sharkDirectionX;
		float sharkSpeedY = sharkSpeed * sharkDirectionY;
		float sharkSpeedMagnitude = sqrt(sharkSpeedX * sharkSpeedX + sharkSpeedY * sharkSpeedY);

		
		if (sharkSpeedMagnitude < sharkMaxSpeed) {
			sharkSpeedMagnitude += sharkAcceleration;
		}

		
		if (fabs(sharkDirectionX) < 0.001 && fabs(sharkDirectionY) < 0.001) {
			sharkSpeedMagnitude -= sharkDeceleration;
			if (sharkSpeedMagnitude < 0.0) {
				sharkSpeedMagnitude = 0.0;
			}
		}

		
		sharkSpeedX = (sharkSpeedX / sharkSpeedMagnitude) * sharkSpeed;
		sharkSpeedY = (sharkSpeedY / sharkSpeedMagnitude) * sharkSpeed;

		
		sharkX += sharkSpeedX;
		sharkY += sharkSpeedY;

		// gen ajkulin direction nakon udarca u zid 
		if (sharkX > 1.0 || sharkX < -1.0 || sharkY > 1.0 || sharkY < -1.0) {
			sharkDirectionX = -sharkDirectionX;
			sharkDirectionY = -sharkDirectionY;
			isSharkFlipped = !isSharkFlipped;
			sharkDirectionX += getRandomDirection(sharkDirectionChangeRange);
			sharkDirectionY += getRandomDirection(sharkDirectionChangeRange);
		}
		// kolizija izmedju ribe i ronioca
		if (fabs(fishX - diverX) < 0.1 && fabs(fishY - diverY) < 0.1) {
			score += 1;


			
			fishX = getRandomDirection(1.0);
			fishY = getRandomDirection(1.0);
			fishDirectionX = getRandomDirection(fishDirectionChangeRange);
			fishDirectionY = getRandomDirection(fishDirectionChangeRange);

			//Proverava da li je dostignut odredjen skor i onda povecava level
			if (score % scoreToIncreaseLevel == 0) {
				level++;
				sharkSpeed += 0.0002; 
				diverSpeed -= 0.0005;   
			}
		}


		// kolizija ajkula - ronilac
		if (fabs(sharkX - diverX) < 0.2 && fabs(sharkY - diverY) < 0.2) {

			diverLives--;


			
			diverX = getRandomDirection(1.0);
			diverY = getRandomDirection(1.0);
			isFlipped = false; 

			
			fishX = 0.5f;
			fishY = 0.5f;
			fishDirectionX = getRandomDirection(fishDirectionChangeRange);
			fishDirectionY = getRandomDirection(fishDirectionChangeRange);
		}
		if (diverLives <= 0) {
			gameOver = true;
		}

	glutPostRedisplay();
	
}


void keyboardInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		diverY += diverSpeed;
		break;
	case 'a':
	case 'A':
		diverX -= diverSpeed;
		isFlipped = false;
		break;
	case 's':
	case 'S':
		diverY -= diverSpeed;
		break;
	case 'd':
	case 'D':
		diverX += diverSpeed;
		isFlipped = true;
		break;
	case 'r':
	case 'R':
		if (gameOver) {
			gameOver = false;
			score = 0;
			diverLives = 3;
			level = 1;
			fishX = 0.5;
			fishY = 0.5;
			fishDirectionX = getRandomDirection(fishDirectionChangeRange);
			fishDirectionY = getRandomDirection(fishDirectionChangeRange);
			sharkX = -0.5;
			sharkY = -0.5;
			sharkDirectionX = getRandomDirection(sharkDirectionChangeRange);
			sharkDirectionY = getRandomDirection(sharkDirectionChangeRange);
			sharkSpeed = 0.0002;
			glutPostRedisplay();
		}
		break;
	case 27: exit(0);
		break;

	}


	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Shark Jaws");


	diverTexture = loadTexture("C:/Users/Mladen/Downloads/scuba2.png");
	fishTexture = loadTexture("C:/Users/Mladen/Downloads/fish2.png");
	sharkTexture = loadTexture("C:/Users/Mladen/Downloads/shark2.png");
	heartTexture = loadTexture("C:/Users/Mladen/Downloads/heart.png");
	gameOverTexture = loadTexture("C:/Users/Mladen/Downloads/gameover.png");



	if (diverTexture == 0 || fishTexture == 0 || sharkTexture == 0 || heartTexture == 0 || gameOverTexture == 0) {
		return 1; 
	}

	// random seed za fish i shark movement
	srand(static_cast<unsigned int>(time(NULL)));
	fishDirectionX = static_cast<float>(rand() % 3 - 1); 
	fishDirectionY = static_cast<float>(rand() % 3 - 1);
	sharkDirectionX = static_cast<float>(rand() % 3 - 1); 
	sharkDirectionY = static_cast<float>(rand() % 3 - 1);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeWindow);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboardInput);
	initialize();
	glClearColor(0, 0.1, 0.1, 1.0); 

	glutMainLoop();

	return 0;
}