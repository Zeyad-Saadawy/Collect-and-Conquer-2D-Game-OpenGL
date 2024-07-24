#include <stdio.h>
#include <string.h>
#include <corecrt_math.h>
#include <cstdlib>
#include <iostream>
#include < windows.h >
#include <mmsystem.h>
#include <glut.h>

int PlayerX = 500;
int PlayerY = 300;
int playerDirection = 0;  // 0 for up, 1 for down, 2 for left, 3 for right
float playerRotation = 0.0;  // Initial rotation angle
int GameTime = 120;
int playerLives = 5;
void InitializeObstacles();
void CheckCollisions();
int score = 0;
void DrawCollectable();
void CollectCollectable();
int obstacle1X, obstacle1Y, obstacle2X, obstacle2Y, obstacle3X, obstacle3Y, obstacle4X, obstacle4Y, obstacle5X, obstacle5Y;
int obstacleRandomized = 0; // Flag to track if obstacles have been randomized
int speed = 10;
int numCollectables = 5;  // Number of collectables
float collectableX[5];    // X positions of collectables
float collectableY[5];    // Y positions of collectables
bool collectedCollectables[5];  // Track if collectables have been collected
int remainingCollectables;  // Count of remaining collectables
void RandomizeCollectables();

bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;

int speedPowerUpX = -1;  // X-coordinate of the power-up (initialize as -1 to indicate it's not active)
int speedPowerUpY = -1;  // Y-coordinate of the power-up
int speedPowerUpDuration = 10;  // Duration of the speed power-up in GameTime units
bool speedPowerUpActive = false;  // Flag to indicate if the power-up is currently active
bool speedupdone = false;
int speedPowerUptimer = 0;  // Timer to track the duration of the power-up
int targetX;
int targetY;
bool targetcollected = false;
void CollectTarget();
int doubleScoreX = -1;  // X position, -1 indicates inactive
int doubleScoreY = -1;  // Y position, -1 indicates inactive
bool doubleScoreActive = false;  // Whether the power-up is active
bool doubleScoredone = false;
int doubleScoreTimer = 0;
int addingfactor = 10;
float powerUpRotation = 0.0;
float speedPowerUpRotation = 0.0;
float collectableRotation = 0.0;
float targetCollectableRotation = 0.0;
float timeVibrationSpeed = 1.0;  // Adjust the speed of vibration
float timeVibrationAmplitude = 5.0;  // Adjust the amplitude of vibration
bool isBackgroundMusicStarted = false;

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void init() {
	// Initialize OpenGL and other game components here

	// Start background music
	PlaySound(TEXT("back.wav"), NULL, SND_ASYNC | SND_FILENAME);
}


void DrawSceneBorderLineLoop() {
	// Set the color for the scene border
	glColor3f(0, 1.0, 1.0);

	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 1);       // Bottom-left corner
	glVertex2f(1000, 1);    // Bottom-right corner
	glVertex2f(1000, 598);  // Top-right corner
	glVertex2f(1, 598);     // Top-left corner
	glEnd();

	glColor3f(0, 1.0, 1.0);

	glBegin(GL_LINES);
	glVertex2f(1000, 565);  // Top-right corner
	glVertex2f(1, 565);     // Top-left corner
	glEnd();
}



void DrawTriangularBorder() {
	// Set the color for the frame
	glColor3f(1.0, 1.0, 1.0);  // White color

	glBegin(GL_TRIANGLES);

	// Draw the top border frame
	for (int x = 0; x < 999; x += 10) {
		glVertex2f(x, 595);     // Top-left vertex of the triangle
		glVertex2f(x + 10, 595);  // Top-right vertex of the triangle
		glVertex2f(x + 5, 600);  // Bottom-center vertex of the triangle
	}

	// Draw the bottom border frame
	for (int x = 0; x < 999; x += 10) {
		glVertex2f(x, 0);     // Bottom-left vertex of the triangle
		glVertex2f(x + 10, 0);  // Bottom-right vertex of the triangle
		glVertex2f(x + 5, 5);  // Top-center vertex of the triangle
	}

	// Draw the left border frame
	for (int y = 0; y < 598; y += 10) {
		glVertex2f(0, y);     // Bottom-left vertex of the triangle
		glVertex2f(0, y + 10);  // Top-left vertex of the triangle
		glVertex2f(5, y + 5);  // Right-center vertex of the triangle
	}

	// Draw the right border frame
	for (int y = 0; y < 598; y += 10) {
		glVertex2f(999, y);     // Bottom-right vertex of the triangle
		glVertex2f(999, y + 10);  // Top-right vertex of the triangle
		glVertex2f(994, y + 5);  // Left-center vertex of the triangle
	}

	// Draw the taht el kalam border frame
	for (int x = 0; x < 999; x += 10) {
		glVertex2f(x, 565);     // Top-left vertex of the triangle
		glVertex2f(x + 10, 565);  // Top-right vertex of the triangle
		glVertex2f(x + 5, 570);  // Bottom-center vertex of the triangle
	}

	glEnd();
}





void DrawPlayer() {
	// Set the color for the player shape
	glColor3f(1.0, 1.0, 1.0);  // Red color

	// Set the player's position
	float playerX = PlayerX;
	float playerY = PlayerY;

	// Calculate the angle for player rotation based on the direction of motion
	float angle = 0.0;
	if (movingUp) {
		angle = 90.0;  // Rotate 90 degrees for upward motion
	}
	else if (movingDown) {
		angle = -90.0;  // Rotate -90 degrees for downward motion
	}
	else if (movingLeft) {
		angle = 180.0;  // Rotate 180 degrees for leftward motion
	}

	glPushMatrix();  // Save the current transformation matrix
	glTranslatef(playerX, playerY, 0.0);  // Translate to player's position
	glRotatef(angle, 0, 0, 1);  // Rotate the player

	// Draw the player using different primitives
	glBegin(GL_LINES);
	glVertex2f(0, 0);  // Line 1
	glVertex2f(10, 0);
	glVertex2f(10, 0);  // Line 2
	glVertex2f(5, 15);
	glVertex2f(5, 15);  // Line 3
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);  // Triangle 1
	glVertex2f(15, 0);
	glVertex2f(7.5, 15);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(-2, 0);  // Quad
	glVertex2f(17, 0);
	glVertex2f(17, 10);
	glVertex2f(-2, 10);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(7.5, -5);  // Triangle Fan center
	for (int i = 0; i <= 360; i += 45) {
		float radian = i * 3.141592653589 / 180;
		float x = 5 * cos(radian);
		float y = 5 * sin(radian);
		glVertex2f(x, y);  // Triangle Fan outer points
	}
	glEnd();

	glPopMatrix();  // Restore the original transformation matrix
}

void DrawObstacle() {
	// Set the color for the obstacle
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_POLYGON);

	// Define the vertices for the GL_POLYGON part
	glVertex2f(obstacle1X - 20, obstacle1Y - 20);
	glVertex2f(obstacle1X + 20, obstacle1Y - 20);
	glVertex2f(obstacle1X + 20, obstacle1Y + 20);
	glVertex2f(obstacle1X - 20, obstacle1Y + 20);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Define the vertices for the GL_TRIANGLES part
	glVertex2f(obstacle1X - 20, obstacle1Y + 20);
	glVertex2f(obstacle1X, obstacle1Y + 40);
	glVertex2f(obstacle1X + 20, obstacle1Y + 20);

	glEnd();

	//draw obstacle 2
	glBegin(GL_POLYGON);

	// Define the vertices for the GL_POLYGON part
	glVertex2f(obstacle2X - 20, obstacle2Y - 20);
	glVertex2f(obstacle2X + 20, obstacle2Y - 20);
	glVertex2f(obstacle2X + 20, obstacle2Y + 20);
	glVertex2f(obstacle2X - 20, obstacle2Y + 20);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Define the vertices for the GL_TRIANGLES part
	glVertex2f(obstacle2X - 20, obstacle2Y + 20);
	glVertex2f(obstacle2X, obstacle2Y + 40);
	glVertex2f(obstacle2X + 20, obstacle2Y + 20);

	glEnd();

	//draw obstacle 3
	glBegin(GL_POLYGON);

	// Define the vertices for the GL_POLYGON part
	glVertex2f(obstacle3X - 20, obstacle3Y - 20);
	glVertex2f(obstacle3X + 20, obstacle3Y - 20);
	glVertex2f(obstacle3X + 20, obstacle3Y + 20);
	glVertex2f(obstacle3X - 20, obstacle3Y + 20);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Define the vertices for the GL_TRIANGLES part
	glVertex2f(obstacle3X - 20, obstacle3Y + 20);
	glVertex2f(obstacle3X, obstacle3Y + 40);
	glVertex2f(obstacle3X + 20, obstacle3Y + 20);

	glEnd();

	//draw obstacle 4
	glBegin(GL_POLYGON);

	// Define the vertices for the GL_POLYGON part
	glVertex2f(obstacle4X - 20, obstacle4Y - 20);
	glVertex2f(obstacle4X + 20, obstacle4Y - 20);
	glVertex2f(obstacle4X + 20, obstacle4Y + 20);
	glVertex2f(obstacle4X - 20, obstacle4Y + 20);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Define the vertices for the GL_TRIANGLES part
	glVertex2f(obstacle4X - 20, obstacle4Y + 20);
	glVertex2f(obstacle4X, obstacle4Y + 40);
	glVertex2f(obstacle4X + 20, obstacle4Y + 20);

	glEnd();

	//draw obstacle 5
	glBegin(GL_POLYGON);

	// Define the vertices for the GL_POLYGON part
	glVertex2f(obstacle5X - 20, obstacle5Y - 20);
	glVertex2f(obstacle5X + 20, obstacle5Y - 20);
	glVertex2f(obstacle5X + 20, obstacle5Y + 20);
	glVertex2f(obstacle5X - 20, obstacle5Y + 20);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Define the vertices for the GL_TRIANGLES part
	glVertex2f(obstacle5X - 20, obstacle5Y + 20);
	glVertex2f(obstacle5X, obstacle5Y + 40);
	glVertex2f(obstacle5X + 20, obstacle5Y + 20);

	glEnd();






}


void DrawProgressBar(int playerLives) {
	// Set the color for the health bar
	glColor3f(0, 1.0, 0);  // Green color

	if (playerLives > 0) {
		// Calculate the length of the health bar based on the player's lives
		float barLength = playerLives * 20.0;  // Adjust the multiplier as needed

		// Draw the progress bar background
		glBegin(GL_POLYGON);
		glVertex2f(20, 570);
		glVertex2f(20 + barLength, 570);
		glVertex2f(20 + barLength, 580);
		glVertex2f(20, 580);
		glEnd();
	}
}


void InitializeSpeedPowerUp() {
	// Randomize the position of the speed power-up
	speedPowerUpX = rand() % 960 + 20;
	speedPowerUpY = rand() % 530 + 20;
}

void DrawSpeedPowerUp() {

	// Set the color for the speed power-up (you can choose the color you prefer)
	glColor3f(0.0, 1.0, 1.0);

	// Rotate the speed power-up
	speedPowerUpRotation += 2.0;  // Adjust the rotation speed as needed

	// Use glPushMatrix and glPopMatrix to isolate the rotation to this object
	glPushMatrix();

	// Translate to the power-up's position
	glTranslatef(speedPowerUpX, speedPowerUpY, 0);

	// Rotate the power-up
	glRotatef(speedPowerUpRotation, 0, 0, 1);

	// Use GL_TRIANGLE_STRIP to create a triangular shape
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0, 0);  // Adjust the vertices based on the rotation
	glVertex2f(10, 0);
	glVertex2f(5, 20);
	glEnd();

	// Use GL_LINES to add lines to the shape
	glBegin(GL_LINES);
	// Define the lines that connect the vertices
	glVertex2f(0, 0);  // Adjust the vertices based on the rotation
	glVertex2f(10, 0);
	glVertex2f(10, 0);  // Adjust the vertices based on the rotation
	glVertex2f(5, 20);
	glVertex2f(5, 20);  // Adjust the vertices based on the rotation
	glVertex2f(0, 0);
	glEnd();

	// Use GL_POLYGON to create a polygon shape
	glBegin(GL_POLYGON);
	// Define the vertices for the polygon
	glVertex2f(0, 0);  // Adjust the vertices based on the rotation
	glVertex2f(10, 0);
	glVertex2f(15, 10);
	glVertex2f(10, 20);
	glVertex2f(0, 20);
	glEnd();

	// Use GL_QUAD_STRIP to create a quadrilateral strip
	glBegin(GL_QUAD_STRIP);
	// Define the vertices for the quadrilateral strip
	glVertex2f(0, 0);  // Adjust the vertices based on the rotation
	glVertex2f(5, 10);
	glVertex2f(10, 0);
	glVertex2f(15, 10);
	glVertex2f(10, 20);
	glVertex2f(0, 20);
	glEnd();

	// Restore the modelview matrix
	glPopMatrix();

}
void CollectSpeedPowerUp() {
	// Check if the player collects the speed power-up
	float distance = sqrt((PlayerX - speedPowerUpX) * (PlayerX - speedPowerUpX) +
		(PlayerY - speedPowerUpY) * (PlayerY - speedPowerUpY));
	if (distance < 20) {
		// Player collects the speed power-up
		speed = 20;  // Increase the player's speed
		speedPowerUpActive = true;
		speedPowerUptimer = 0;  // Reset the timer
		PlaySound(TEXT("collect.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}
}

void DrawTargetCollectable() {
	// Set the color for the target collectable (you can choose the color you prefer)
	glColor3f(0, 1.0, 0);  // Green color

	// Rotate the target collectable
	targetCollectableRotation += 2.0;  // Adjust the rotation speed as needed

	// Use glPushMatrix and glPopMatrix to isolate the rotation to this object
	glPushMatrix();

	// Translate to the target collectable's position (targetX, targetY)
	glTranslatef(targetX, targetY, 0);

	// Rotate the target collectable
	glRotatef(targetCollectableRotation, 0, 0, 1);

	// Use GL_LINES to create lines
	glBegin(GL_LINES);
	// Define the lines that form the shape
	glVertex2f(-5, 0);
	glVertex2f(5, 0);
	glVertex2f(0, -5);
	glVertex2f(0, 5);
	glEnd();

	// Use GL_LINE_STRIP to create a connected line strip
	glBegin(GL_LINE_STRIP);
	// Define the vertices for the line strip
	glVertex2f(-10, -10);
	glVertex2f(10, -10);
	glVertex2f(10, 10);
	glVertex2f(-10, 10);
	glEnd();

	// Use GL_TRIANGLES to create triangles
	glBegin(GL_TRIANGLES);
	// Define the vertices for the triangles
	glVertex2f(0, -15);
	glVertex2f(-10, 5);
	glVertex2f(10, 5);
	glEnd();

	// Use GL_QUAD_STRIP to create a quadrilateral strip
	glBegin(GL_QUAD_STRIP);
	// Define the vertices for the quadrilateral strip
	glVertex2f(-15, -15);
	glVertex2f(-10, -5);
	glVertex2f(10, -15);
	glVertex2f(15, -5);
	glEnd();

	// Use GL_TRIANGLE_FAN to create a fan of triangles
	glBegin(GL_TRIANGLE_FAN);
	// Define the center point of the fan
	glVertex2f(0, 0);
	// Define the vertices for the fan of triangles
	glVertex2f(0, 10);
	glVertex2f(5, 5);
	glVertex2f(10, 0);
	glVertex2f(5, -5);
	glVertex2f(0, -10);
	glVertex2f(-5, -5);
	glVertex2f(-10, 0);
	glVertex2f(-5, 5);
	glEnd();

	// Restore the modelview matrix
	glPopMatrix();
}
void DrawDoubleScorePowerUp() {

	// Set the color for the power-up
	glColor3f(0, 1.0, 1.0);  // Yellow color

	// Rotate the power-up
	powerUpRotation += 2.0;  // Adjust the rotation speed as needed

	// Draw the power-up with rotation using GL_TRIANGLE_STRIP, GL_LINES, GL_POLYGON, GL_QUAD_STRIP
	glPushMatrix();
	glTranslatef(doubleScoreX, doubleScoreY, 0);
	glRotatef(powerUpRotation, 0, 0, 1);

	glBegin(GL_TRIANGLE_STRIP);

	// Triangle (GL_TRIANGLE_STRIP)
	glVertex2f(0, 20);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);

	glEnd();

	glBegin(GL_LINES);

	// Lines (GL_LINES)
	glVertex2f(-10, 0);
	glVertex2f(0, -20);
	glVertex2f(10, 0);
	glVertex2f(0, -20);

	glEnd();

	glBegin(GL_POLYGON);

	// Polygon (GL_POLYGON)
	for (int i = 0; i < 360; i += 60) {
		float theta = i * 3.141592653589 / 180;
		glVertex2f(20 * cos(theta), 20 * sin(theta));
	}

	glEnd();

	glBegin(GL_QUAD_STRIP);

	// Quad Strip (GL_QUAD_STRIP)
	glVertex2f(-20, -10);
	glVertex2f(-20, -20);
	glVertex2f(20, -10);
	glVertex2f(20, -20);

	glEnd();

	glPopMatrix();

}
void InitializeDoubleScorePowerUp() {
	doubleScoreX = rand() % 960 + 20;
	doubleScoreY = rand() % 530 + 20;
}
void CollectDoubleScorePowerUp() {
	// Check if the player collects the double score power-up
	float distance = sqrt((PlayerX - doubleScoreX) * (PlayerX - doubleScoreX) +
		(PlayerY - doubleScoreY) * (PlayerY - doubleScoreY));
	if (distance < 20) {
		// Player collects the double score power-up
		addingfactor = 20;  // Double the score
		doubleScoreActive = true;  // Set the flag to indicate the power-up is active
		doubleScoreTimer = 0;  // Reset the timer
		PlaySound(TEXT("collect.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}
}

void Display() {

	if (GameTime != 0) {

		if (!isBackgroundMusicStarted) {
			// Start background music
			PlaySound(TEXT("back.wav"), NULL, SND_ASYNC | SND_FILENAME);
			isBackgroundMusicStarted = true;
		}


		glClear(GL_COLOR_BUFFER_BIT);

		DrawSceneBorderLineLoop();
		DrawTriangularBorder();
		DrawPlayer();

		// Draw obstacles 
		DrawObstacle();

		// Check for collisions
		CheckCollisions();
		CollectCollectable();

		// Draw collectable
		DrawCollectable();
		if (!targetcollected) {
			DrawTargetCollectable();
			CollectTarget();
		}
		//score
		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "Score: %d", score);
		print(500, 570, (char*)p0s);


		// time
		glColor3f(1, 0, 0);

		// Calculate the X-coordinate of the "Time" word with vibration
		int x = 700 + timeVibrationAmplitude * sin(timeVibrationSpeed * GameTime);

		char* p1t[20];
		sprintf((char*)p1t, "Time: %d", GameTime);

		// Set the position of the text with the vibrating X-coordinate
		print(x, 570, (char*)p1t);

		//lives
		DrawProgressBar(playerLives);

		//speed power-up
		if (GameTime == 110) {
			InitializeSpeedPowerUp();
		}
		if (GameTime < 110 && !speedPowerUpActive && !speedupdone) {
			DrawSpeedPowerUp();

		}
		CollectSpeedPowerUp();

		//double score power-up
		if (GameTime == 115) {
			InitializeDoubleScorePowerUp();
		}
		if (GameTime < 115 && !doubleScoreActive && !doubleScoredone) {
			DrawDoubleScorePowerUp();
		}

		CollectDoubleScorePowerUp();

	}
	//end screen
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		if (playerLives == 0) {
			glColor3f(1, 0, 0);
			char* p2s[20];
			sprintf((char*)p2s, "GameOver, You Lost all ur lives!");
			print(500, 300, (char*)p2s);
			PlaySound(TEXT("losssound.WAV"), NULL, SND_ASYNC | SND_FILENAME);

		}
		else if (targetcollected) {
			glColor3f(1, 0, 0);
			char* p2s[20];
			sprintf((char*)p2s, "GameOver, You Won!");
			print(500, 300, (char*)p2s);
			PlaySound(TEXT("winsound.WAV"), NULL, SND_ASYNC | SND_FILENAME);

		}
		else {
			glColor3f(1, 0, 0);
			char* p2s[20];
			sprintf((char*)p2s, "GameOver, You ran out of time!");
			print(500, 300, (char*)p2s);
			PlaySound(TEXT("losssound.WAV"), NULL, SND_ASYNC | SND_FILENAME);

		}
		glFlush();
	}

	glFlush();
}

void spe(int k, int x, int y) {
	if (GameTime != 0) {
		if (k == GLUT_KEY_UP) {
			PlayerY += speed;  // Move the player up
			movingUp = true;  // Set the direction of motion
			movingDown = false;
			movingLeft = false;
			movingRight = false;
		}

		if (k == GLUT_KEY_DOWN) {
			PlayerY -= speed;  // Move the player down
			movingUp = false;  // Set the direction of motion
			movingDown = true;
			movingLeft = false;
			movingRight = false;
		}

		if (k == GLUT_KEY_LEFT) {
			PlayerX -= speed;  // Move the player left
			movingUp = false;  // Set the direction of motion
			movingDown = false;
			movingLeft = true;
			movingRight = false;
		}

		if (k == GLUT_KEY_RIGHT) {
			PlayerX += speed;  // Move the player right
			movingUp = false;  // Set the direction of motion
			movingDown = false;
			movingLeft = false;
			movingRight = true;
		}
	}
	glutPostRedisplay();  // Redraw the scene
}


void Counter(int value) {
	if (GameTime != 0)
		GameTime -= 1;
	if (speedPowerUpActive) {
		speedPowerUptimer++;
		if (speedPowerUptimer == 11) {
			speedPowerUpActive = false;
			speedPowerUptimer = 0;
			speed = 10;
			speedupdone = true;
		}
	}

	if (doubleScoreActive) {
		doubleScoreTimer++;
		if (doubleScoreTimer == 11) {
			doubleScoreActive = false;
			doubleScoreTimer = 0;
			addingfactor = 10;
			doubleScoredone = true;
		}
	}
	if (playerLives == 0) {
		GameTime = 0;
	}


	if (GameTime != 0)
		glutTimerFunc(1000, Counter, 0);
	glutPostRedisplay();

}


void RandomizeObstacles() {
	// Randomize obstacle positions for 5 obstacles
	obstacle1X = rand() % 960 + 20;
	obstacle1Y = rand() % 500 + 20;
	obstacle2X = rand() % 960 + 20;
	obstacle2Y = rand() % 500 + 20;
	obstacle3X = rand() % 960 + 20;
	obstacle3Y = rand() % 500 + 20;
	obstacle4X = rand() % 960 + 20;
	obstacle4Y = rand() % 500 + 20;
	obstacle5X = rand() % 960 + 20;
	obstacle5Y = rand() % 500 + 20;

	obstacleRandomized = 1; // Set the flag to indicate randomization
}


void InitializeObstacles() {
	// Check if obstacles have already been randomized
	if (!obstacleRandomized) {
		RandomizeObstacles();
	}
}



void CheckCollisions() {
	// Check collision with boundaries
	if (PlayerX < 10 || PlayerX > 990 || PlayerY < 10 || PlayerY > 550) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
	}

	if ((PlayerX >= obstacle1X - 20 && PlayerX <= obstacle1X + 20) &&
		(PlayerY >= obstacle1Y - 20 && PlayerY <= obstacle1Y + 20)) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
		PlaySound(TEXT("obs.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}

	if ((PlayerX >= obstacle2X - 20 && PlayerX <= obstacle2X + 20) &&
		(PlayerY >= obstacle2Y - 20 && PlayerY <= obstacle2Y + 20)) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
		PlaySound(TEXT("obs.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}

	if ((PlayerX >= obstacle3X - 20 && PlayerX <= obstacle3X + 20) &&
		(PlayerY >= obstacle3Y - 20 && PlayerY <= obstacle3Y + 20)) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
		PlaySound(TEXT("obs.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}

	if ((PlayerX >= obstacle4X - 20 && PlayerX <= obstacle4X + 20) &&
		(PlayerY >= obstacle4Y - 20 && PlayerY <= obstacle4Y + 20)) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
		PlaySound(TEXT("obs.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}

	if ((PlayerX >= obstacle5X - 20 && PlayerX <= obstacle5X + 20) &&
		(PlayerY >= obstacle5Y - 20 && PlayerY <= obstacle5Y + 20)) {
		playerLives--;  // Reduce player lives
		PlayerX = 500;  // Reset player position
		PlayerY = 300;
		PlaySound(TEXT("obs.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}
}



void InitializeCollectables() {
	RandomizeCollectables();
	for (int i = 0; i < numCollectables; i++) {
		collectedCollectables[i] = false;
	}
	remainingCollectables = numCollectables;
}

void RandomizeCollectables() {
	for (int i = 0; i < numCollectables; i++) {
		collectableX[i] = rand() % 960 + 20;  // Random X position between 20 and 980
		collectableY[i] = rand() % 500 + 20;  // Random Y position between 20 and 540
	}
	targetX = rand() % 960 + 20;
	targetY = rand() % 540 + 20;
}

void CollectCollectable() {
	for (int i = 0; i < numCollectables; i++) {
		if (!collectedCollectables[i]) {
			float distance = sqrt((PlayerX - collectableX[i]) * (PlayerX - collectableX[i]) +
				(PlayerY - collectableY[i]) * (PlayerY - collectableY[i]));
			if (distance < 20) {
				// Player collects the collectable
				PlaySound(TEXT("collect.WAV"), NULL, SND_ASYNC | SND_FILENAME);
				score += addingfactor;  // Increase the score
				collectedCollectables[i] = true;  // Mark the collectable as collected
				remainingCollectables--;  // Decrease the count of remaining collectables
			}
		}
	}
}
void CollectTarget() {
	float distance = sqrt((PlayerX - targetX) * (PlayerX - targetX) +
		(PlayerY - targetY) * (PlayerY - targetY));
	if (distance < 20) {
		// Player collects the collectable
		score += addingfactor * 10;  // Increase the score
		targetcollected = true;
		PlaySound(TEXT("collect.WAV"), NULL, SND_ASYNC | SND_FILENAME);

	}
}

void DrawCollectable() {
	for (int i = 0; i < numCollectables; i++) {
		if (!collectedCollectables[i]) {
			// Set the color for the collectable (yellow)
			glColor3f(1, 1, 0);

			// Rotate the collectable
			collectableRotation -= 5.0;  // Adjust the rotation speed as needed

			// Use glPushMatrix and glPopMatrix to isolate the rotation to this object
			glPushMatrix();

			// Translate to the collectable's position
			glTranslatef(collectableX[i], collectableY[i], 0);

			// Rotate the collectable
			glRotatef(collectableRotation, 0, 0, 1);

			// Draw the collectable shape
			glBegin(GL_POLYGON);
			for (int j = 0; j < 360; j += 30) {  // Use fewer points for a custom shape
				float theta = j * 3.141592653589 / 180;
				glVertex2f(10 * cos(theta), 10 * sin(theta));
			}
			glEnd();

			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0, 0);  // Center point
			glVertex2f(10, 0);
			glVertex2f(0, 15);
			glVertex2f(-10, 0);
			glEnd();

			glBegin(GL_LINES);
			for (int j = 0; j < 360; j += 30) {
				float theta = j * 3.141592653589 / 180;
				glVertex2f(0, 0);  // Center point
				glVertex2f(10 * cos(theta), 10 * sin(theta));
			}
			glEnd();

			// Restore the modelview matrix
			glPopMatrix();
		}
	}
}






void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(1000, 600);

	glutCreateWindow("OpenGL - 2D Template");
	glutDisplayFunc(Display);
	glutTimerFunc(0, Counter, 0);
	glutSpecialFunc(spe);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 1000, 0.0, 600);



	srand(time(NULL));
	InitializeObstacles();
	InitializeCollectables();
	glutMainLoop();
}
