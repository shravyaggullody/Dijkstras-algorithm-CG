#include <glut.h>
#include <string.h>
#include<stdio.h>
#include<math.h>
#define MAX_NODES 20
#define MAX_EDGES 100
int sourceNodeIndex = -1;
int destinationNodeIndex = -1;
int isDijkstraApplied = 0;
int isHelpScreen = 0;
int shapeIndex = 0;
int currentState = 0;
typedef struct Node 
{
    float x;
    float y;
	float color;
	int index;
} Node;

// Edge structure
typedef struct Edge 
{
    int nodeIndex1;
    int nodeIndex2;
    float cost;
    char color[8];
    int thickness;
} Edge;
struct Edge coloredEdges[MAX_EDGES];
int coloredEdgeCount = 0;

// Node and Edge lists
Node nodes[20];
Edge edges[100];
int nodeCount = 0;
int edgeCount = 0;
int firstClickedNodeIndex = -1;
void edgethick();
void *currentfont;
void background();
void drawstring(float x, float y, float z, char *string);
void display();
void mouse_click(int button, int state, int x, int y);
void DrawOval(float cx, float cy, float r, int num_segments) ;
void dijkstra();
void keyboard();
void delay()
{
	int i,j;
	for (i = 0; i < 30000; i++) 
		for (j = 0; j < 60000; j++); //delay
}
void setFont(void *font)
{
    currentfont = font;
}
void drawstring(float x, float y, float z, char *string)
{

    int len = (int) strlen(string);
    int i;
    glRasterPos3f(x, y, z);
    for(i = 0; i < len; i++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glutBitmapCharacter(currentfont, string[i]);
    }
}

void frontscreen(void)
{
	isHelpScreen = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setFont(GLUT_BITMAP_TIMES_ROMAN_24);
	//setFont(GLUT_BITMAP_9_BY_15);
	background();
	glColor3f(0, 0, 1.0); //blue
	drawstring(-41, 80,0.0, "SDM INSTITUTE OF TECHNOLOGY, UJIRE-574240");
	
	glColor3f(0, 0.7, 0); // dark green
	drawstring(-49, 70,0.0, "DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");

	glColor3f(1.0, 0.2, 0.2); // red
	drawstring(-55, 60,0.0 ,"COMPUTER GRAPHICS LABORATORY WITH MINI PROJECT(18CSL67)");
	
	glColor3f(0.8, 0.4, 0.1); //dark yellow
	drawstring(-20, 45,0.0, "A MINI PROJECT ON");
	
	glColor3f(0.6, 0.0, 1.0); // purple
	drawstring(-29, 30,0.0, " ''''DIJKSTRA'S ALGORITHM''''");
	
	glColor3f(1, 0, 0); // red
	drawstring(-90, -32, 0.0, "SUBMITTED BY:");

	drawstring(-90, -42, 0.0, "SANTHRUPTHI G GOWDA");
	drawstring(-90, -52, 0.0, "SHARANYA B M");
	drawstring(-90, -62, 0.0, "SHRAVANI SHASHIKANT JABADE");
	drawstring(-90, -72, 0.0, "SHRAVYA GULLODY");

	drawstring(-38, -42, 0.0, " 4SU20CS087");
	drawstring(-38, -52, 0.0, " 4SU20CS089");
	drawstring(-38, -62, 0.0, " 4SU20CS095");
	drawstring(-38, -72, 0.0, " 4SU20CS096");

	glColor3f(1, 0, 0); // red
	drawstring(-90,-92, 0.0, "<<< PRESS ''C'' TO CLOSE ");

	glColor3f(1, 0, 0); // red
	drawstring(50, -32, 0.0, "GUIDE:");

	drawstring(50, -42, 0.0, "Mr. ARJUN K");
	drawstring(50, -52, 0.0, "ASSISTANT PROFESSOR");
	drawstring(50, -62, 0.0, "DEPT OF CSE");

	glColor3f(1, 0, 0); // red
	drawstring(50,-92, 0.0, "PRESS ''M'' TO CONTINUE>>>");

	glColor3f(0, 0, 0);//black
	drawstring(12,3,0.0,"HELP(h)");
	
	glColor3f(0.8, 0.4, 0.1); //dark yellow
	glBegin(GL_LINE_LOOP);
	glVertex2f(8,0);
	glVertex2f(8,10);
	glVertex2f(28,10);
	glVertex2f(28,0);
	glEnd();

	glColor3f(0.8, 0.6,1.0 );//purple
	glBegin(GL_LINE_LOOP);
	glVertex2f(7,-1);
	glVertex2f(7,11);
	glVertex2f(29,11);
	glVertex2f(29,-1);
	glEnd();

	glColor3f(0.8, 0.6,1.0 );//purple
	glBegin(GL_LINE_LOOP);
	glVertex2f(7.5,-1);
	glVertex2f(7.5,11);
	glVertex2f(28.5,11);
	glVertex2f(28.5,-1);
	glEnd();

	glColor3f(0, 0, 0);//black
	drawstring(-34,3,0.0,"ABOUT US(a)");

	glColor3f(0.8, 0.4, 0.1); //dark yellow
	glBegin(GL_LINE_LOOP);
	glVertex2f(-12,0);
	glVertex2f(-12,10);
	glVertex2f(-37,10);
	glVertex2f(-37,0);
	glEnd();

	glColor3f(0.8, 0.6,1.0 );//purple
	glBegin(GL_LINE_LOOP);
	glVertex2f(-11,-1);
	glVertex2f(-11,11);
	glVertex2f(-38,11);
	glVertex2f(-38,-1);
	glEnd();

	glColor3f(0.8, 0.6,1.0 );//purple
	glBegin(GL_LINE_LOOP);
	glVertex2f(-11.5,-1);
	glVertex2f(-11.5,11);
	glVertex2f(-37.5,11);
	glVertex2f(-37.5,-1);
	glEnd();

    glFlush();
}
void myinit()
{
	glClearColor(1.0, 1.0, 0.8, 0);//light yellow
    glOrtho(-100.0, 100.0, -100.0, 100.0, -50.0, 50.0);
}
void background()
{
	glBegin(GL_QUADS);
    glColor3f(0.8f, 0.6f, 1.0f); // lighter shade of purple
    glVertex2f(-100, 100);
    glColor3f(1.0f, 0.8f, 0.7f); // lighter shade of red
    glVertex2f(-100, -100);
    glColor3f(0.7f, 0.95f, 0.8f); // lighter shade of green
    glVertex2f(100, -100);
    glColor3f(1.0f, 1.0f, 0.7f); // lighter shade of yellow
    glVertex2f(100, 100);
    glEnd();
}

void menu()
{
	isDijkstraApplied = 0;
	isHelpScreen = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();

    glColor3f(1.0, 0.0, 0.0); //red
    drawstring(-17, 70, 0.0,"DIJIKSTRA  ALGORITHM");

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-20,68);
	glVertex2f(25,68);
	glEnd(); 
    
    glColor3f(0.4, 0.0, 0.0);
    drawstring(-6.5, 41,0.0, "START(s)");

	glColor3f(0.8, 0.6,1.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-8,38);
	glVertex2f(-8,48);
	glVertex2f(8,48);
	glVertex2f(8,38);
	glEnd();

	glColor3f(0.4,0.0,0.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-9,37);
	glVertex2f(-9,49);
	glVertex2f(9,49);
	glVertex2f(9,37);
	glEnd();

	glColor3f(0.4, 0.0, 0.0);
    drawstring(-7, 21,0.0, "ABOUT(i)");

	glColor3f(0.8, 0.6,1.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-8,18);
	glVertex2f(-8,28);
	glVertex2f(8,28);
	glVertex2f(8,18);
	glEnd();

	glColor3f(0.4,0.0,0.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-9,17);
	glVertex2f(-9,29);
	glVertex2f(9,29);
	glVertex2f(9,17);
	glEnd();

    glColor3f(0.4, 0.0, 0.0);
    drawstring(-11, 1,0.0, "FLOWCHART(f)");

	glColor3f(0.8, 0.6,1.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-12,-2);
	glVertex2f(-12,8);
	glVertex2f(13,8);
	glVertex2f(13,-2);
	glEnd();

	glColor3f(0.4,0.0,0.0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(-13,-3);
	glVertex2f(-13,9);
	glVertex2f(14,9);
	glVertex2f(14,-3);
	glEnd();

	glColor3f(1, 0, 0); // red
	drawstring(50,-92, 0.0, "PRESS ''C'' TO CLOSE>>>");
    glColor3f(1, 0, 0); // red
	drawstring(-92,-92, 0.0, "<<<PRESS ''B'' TO GO BACK");
    glFlush();
    //glutSwapBuffers();
}
void About()
{
	isHelpScreen = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.8, 0.4, 0.1); // dark yellow
	glVertex2f(-90, 82);
	glVertex2f(-10, 82);
	glVertex2f(-10, 2);
	glVertex2f(-90, 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.6, 0.4, 0.8 );// DARK purple
	glVertex2f(-91, 81);
	glVertex2f(-11, 81);
	glVertex2f(-11, 1);
	glVertex2f(-91, 1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.8, 0.4, 0.1); // dark yellow
	glVertex2f(-90, -8);
	glVertex2f(-10, -8);
	glVertex2f(-10, -88);
	glVertex2f(-90, -88);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.6, 0.4, 0.8 );// DARK purple
	glVertex2f(-91, -9);
	glVertex2f(-11, -9);
	glVertex2f(-11, -89);
	glVertex2f(-91, -89);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.8, 0.4, 0.1); // dark yellow
	glVertex2f(10, 82);
	glVertex2f(90, 82);
	glVertex2f(90, 2);
	glVertex2f(10, 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.6, 0.4, 0.8 );// DARK purple
	glVertex2f(9, 81);
	glVertex2f(89, 81);
	glVertex2f(89, 1);
	glVertex2f(9, 1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.8, 0.4, 0.1); // dark yellow
	glVertex2f(10, -8);
	glVertex2f(90, -8);
	glVertex2f(90, -88);
	glVertex2f(10, -88);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.6, 0.4, 0.8 );// DARK purple
	glVertex2f(9, -9);
	glVertex2f(89, -9);
	glVertex2f(89, -89);
	glVertex2f(9, -89);
	glEnd();
	
	glColor3f(1, 0, 0); // red
	drawstring(50,-96, 0.0, "PRESS ''C'' TO CLOSE>>>");
    glColor3f(1, 0, 0); // red
	drawstring(-92,-96, 0.0, "<<<PRESS ''B'' TO GO BACK");
	glColor3f(0, 0, 1.0); //blue
    drawstring(-14, 90, 0.0,"---ABOUT US---");
	glFlush();
	
	delay();
	glColor3f(1, 0, 0); // red
    drawstring(-87, 70, 0.0, "NAME");
	glColor3f(1, 0.5, 0); // orange
    drawstring(-87, 60, 0.0, "BRANCH");
	glColor3f(0.6, 0.2, 0.8); // purple
    drawstring(-87, 50, 0.0, "USN");
	glColor3f(0, 0.5, 0); // green
    drawstring(-87, 40, 0.0, "CONTACT");
	glColor3f(0, 0, 1); // blue
	drawstring(-87, 30, 0.0, "ADDRESS");
	glColor3f(1, 0, 0); // red
    drawstring(-70, 70, 0.0, ": SANTHRUPTHI G GOWDA");
	glColor3f(1, 0.5, 0); // orange
    drawstring(-70, 60, 0.0, ": CSE");
	glColor3f(0.6, 0.2, 0.8); // purple
    drawstring(-70, 50, 0.0, ": 4SU20CS087");
	glColor3f(0, 0.5, 0); // green
    drawstring(-70, 40, 0.0, ": 8197522975");
	glColor3f(0, 0, 1); // blue
	drawstring(-70, 30,0.0, ":  Masthikatte , Hosanagara taluk ,");
	glColor3f(0, 0, 1); // blue
	drawstring(-70, 20,0.0, "   Shivamogga district - 577448");
	glFlush();
	
	delay();
	glColor3f(1, 0, 0); // red
	drawstring(13, 70, 0.0, "NAME");
	glColor3f(1, 0.5, 0); // orange
	drawstring(13, 60, 0.0, "BRANCH");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(13, 50, 0.0, "USN");
	glColor3f(0, 0.5, 0); // green
	drawstring(13,40, 0.0, "CONTACT");
	glColor3f(0, 0, 1); // blue
	drawstring(13, 30, 0.0, "ADDRESS");
	glColor3f(1, 0, 0); // red
	drawstring(30, 70, 0.0, ": SHARANYA B M");
	glColor3f(1, 0.5, 0); // orange
	drawstring(30, 60, 0.0, ": CSE");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(30, 50, 0.0, ": 4SU20CS089");
	glColor3f(0, 0.5, 0); // green
	drawstring(30, 40, 0.0, ": 9900224640");
	glColor3f(0, 0, 1); // blue
	drawstring(30, 30,0.0, ":  Bychanahally Village and Post ,");
	glColor3f(0, 0, 1); // blue
	drawstring(30, 20,0.0, "   Kasaba Hobli , Arkalagudu Taluk ,");
	glColor3f(0, 0, 1); // blue
	drawstring(30, 10,0.0, "   Hassan District - 573102");
	glFlush();
	
	delay();
	glColor3f(1, 0, 0); // red
	drawstring(-87, -20,0.0, "NAME");
	glColor3f(1, 0.5, 0); // orange
	drawstring(-87, -30,0.0, "BRANCH");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(-87, -40,0.0, "USN");
	glColor3f(0, 0.5, 0); // green
	drawstring(-87, -50,0.0, "CONTACT");
	glColor3f(0, 0, 1); // blue
	drawstring(-87, -60, 0.0, "ADDRESS");
	glColor3f(1, 0, 0); // red
	drawstring(-70, -20,0.0, ": SHRAVANI SHASHIKANT JABADE");
	glColor3f(1, 0.5, 0); // orange
	drawstring(-70, -30,0.0, ": CSE");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(-70, -40,0.0, ": 4SU20CS095");
	glColor3f(0, 0.5, 0); // green
	drawstring(-70, -50,0.0, ": 6360431962");
	glColor3f(0, 0, 1); // blue
	drawstring(-70, -60,0.0, ": A/p Jatrat , Taluka Nipani ,");
	glColor3f(0, 0, 1); // blue
	drawstring(-70, -70,0.0, "  Belgaum - 591237");
	glFlush();
	
	delay();
	glColor3f(1, 0, 0); // red
	drawstring(13, -20, 0.0, "NAME");
	glColor3f(1, 0.5, 0); // orange
	drawstring(13, -30, 0.0, "BRANCH");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(13, -40, 0.0, "USN");
	glColor3f(0, 0.5, 0); // green
	drawstring(13, -50, 0.0, "CONTACT");
	glColor3f(0, 0, 1); // blue
	drawstring(13, -60, 0.0, "ADDRESS");
	glColor3f(1, 0, 0); // red
    drawstring(30, -20,0.0, ": SHRAVYA GULLODY");
	glColor3f(1, 0.5, 0); // orange
	drawstring(30, -30,0.0, ": CSE");
	glColor3f(0.6, 0.2, 0.8); // purple
	drawstring(30, -40,0.0, ": 4SU20CS095");
	glColor3f(0, 0.5, 0); // green
	drawstring(30, -50,0.0, ": 8618840388");
	glColor3f(0, 0, 1); // blue
	drawstring(30, -60,0.0, ": 'Janaki Rama' house , Kuvempu Nagara ,");
	glColor3f(0, 0, 1); // blue
	drawstring(30, -70,0.0, "   Belal road , Ujire , Belthangady Tq");
	glColor3f(0, 0, 1); // blue
	drawstring(30, -80,0.0, "   Dakshina Kannada -574240");
	glFlush();
}

//animation
void Square() 
{
    glBegin(GL_POLYGON);
    glVertex2f(-80, 60);
    glVertex2f(-80, 40);
    glVertex2f(-60, 40);
    glVertex2f(-60, 60);
    glEnd();
}
void Triangle() 
{
    glBegin(GL_POLYGON);
    glVertex2f(-70, 30);
    glVertex2f(-80, 5);
    glVertex2f(-60, 5);
    glEnd();
}
void hexagon() 
{
    glBegin(GL_POLYGON);
    glVertex2f(-75, -5);
    glVertex2f(-80, -15);
    glVertex2f(-75, -25);
    glVertex2f(-65, -25);
    glVertex2f(-60, -15);
    glVertex2f(-65, -5);
    glEnd();
}
void DrawRobot() 
{
    glColor3f(1.0, 0.0, 0.0);

    // (square)
    Square();

    // (triangle)
    glColor3f(0.0, 0.0, 1.0);
    //glTranslatef(-70, 60, 0);
    Triangle();

    // (hexagon)
    glColor3f(0.0, 1.0, 0.0);
    //glTranslatef(0, -40, 0);
    hexagon();

    glFlush();
}

void display_ani() 
{
	if (!isHelpScreen) 
	{
        // Skip animation when help screen  is not displayed
        return;
    }
    if (shapeIndex == 0) 
	{
        glColor3f(1.0, 0.6, 0.6); // Light pink color
        hexagon();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, -17, 0, "ZONE");
    }
	else if (shapeIndex == 1) 
	{
        glColor3f(0.6, 0.8, 0.6); // Mint green color
        Square();
	    glColor3f(0.0, 0.0, 0.0);
		drawstring(-73, 47, 0, "CG");
    }
	else if (shapeIndex == 2) 
	{
        glColor3f(0.6, 0.6, 1.0); // Lavender color
        Triangle();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, 11, 0, "HELP");
    } 
	else if (shapeIndex == 3) 
	{
        glColor3f(1.0, 1.0, 0.6); // Pale yellow color
        hexagon();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, -17, 0, "ZONE");
    }
	else if (shapeIndex == 4) 
	{
        glColor3f(1.0, 0.6, 1.0); // Orchid color
        Square();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-73, 47, 0, "CG");
    }
	else if (shapeIndex == 5) 
	{
        glColor3f(0.6, 0.8, 0.8); // Aqua color
        Triangle();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, 11, 0, "HELP");
    }
	else if (shapeIndex == 6) 
	{
        glColor3f(1.0, 0.8, 0.4); // Peach color
        hexagon();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, -17, 0, "ZONE");
    }
	else if (shapeIndex == 7) 
	{
        glColor3f(0.8, 0.4, 0.8); // Lilac color
        Square();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-73, 47, 0, "CG");
    }
	else if (shapeIndex == 8) 
	{
        glColor3f(0.4, 0.6, 1.0); // Sky blue color
        Triangle();
		glColor3f(0.0, 0.0, 0.0);
		drawstring(-74, 11, 0, "HELP");
    }
    glFlush();
}

void update(int value) 
{
    shapeIndex = (shapeIndex + 1) % 9; // Cycle through shapes
    glutPostRedisplay();
    glutTimerFunc(100, update, 0); // Wait for 1 second before switching shapes
}
//end of animation

void Help()
{
	isHelpScreen = 1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	glColor3f(0, 0.7, 0); // dark green
	drawstring(-8, 85,0.0, "--- HELP ---");
	glColor3f(1.0, 0.0, 0.0); //red
    drawstring(-15, 65, 0.0,"DIJIKSTRA  ALGORITHM");
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-18,58);
	glVertex2f(25,58);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-18,59);
	glVertex2f(25,59);
	glEnd();

	glColor3f(0, 0, 1.0); // blue
	drawstring(-20, 32, 0.0, "KEYS");
	drawstring(-18, 20, 0.0, "M");
	drawstring(-18, 12, 0.0, "B");
	drawstring(-18, 4, 0.0, "C");
	drawstring(-18, -4, 0.0, "A");
	drawstring(-18, -12, 0.0, "H");
	drawstring(-18, -20, 0.0, "S");
	drawstring(-18, -28, 0.0, "I");
	drawstring(-18, -36, 0.0, "F");
	drawstring(-18, -44, 0.0, "Y");
	
	drawstring(-4, 20, 0.0, "-");
	drawstring(-4, 12, 0.0, "-");
	drawstring(-4, 4, 0.0, "-");
	drawstring(-4, -4, 0.0, "-");
	drawstring(-4, -12, 0.0, "-");
	drawstring(-4, -20, 0.0, "-");
	drawstring(-4, -28, 0.0, "-");
	drawstring(-4, -36, 0.0, "-");
	drawstring(-4, -44, 0.0, "-");

	glColor3f(0, 0, 1.0); // blue
	drawstring(10, 32, 0.0, "FUNCTIONS");
	drawstring(10, 20, 0.0, "To Continue");
	drawstring(10, 12, 0.0, "To Go Back");
	drawstring(10, 4, 0.0, "To Quit");
	drawstring(10, -4, 0.0, "To Get Developer Info");
	drawstring(10, -12, 0.0, "To Get Help");
	drawstring(10, -20, 0.0, "To Start");
	drawstring(10, -28, 0.0, "To Get Algorithm");
	drawstring(10, -36, 0.0, "To Get Flowchart");
	drawstring(10, -44, 0.0, "To Get Shortest Path");

	glColor3f(1, 0, 0); // red
	drawstring(50,-92, 0.0, "PRESS ''C'' TO CLOSE>>>");
    glColor3f(1, 0, 0); // red
	drawstring(-92,-92, 0.0, "<<<PRESS ''B'' TO GO BACK");
	glutTimerFunc(0, update, 0);
	glutDisplayFunc(display_ani);
	glFlush();
}

void about()
{
	isHelpScreen = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	
	glColor3f(1, 0, 0); // red
	drawstring(50,-92, 0.0, "PRESS ''C'' TO CLOSE>>>");
    glColor3f(1, 0, 0); // red
	drawstring(-92,-92, 0.0, "<<<PRESS ''B'' TO GO BACK");
	glFlush();
	
	delay();
	glColor3f(1.0, 0.0, 0.0); //red
    drawstring(-16, 80, 0.0,"DIJKSTRA'S  ALGORITHM");
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(-18,78);
	glVertex2f(25,78);
	glEnd();
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, 60, 0.0, "Dijkstra's algorithm allows us to find the shortest path between any two vertices of a graph.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, 50, 0.0, "It differs from the minimum spanning tree because the shortest distance between two vertices might not include all the  		vertices of the graph.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, 40, 0.0, "Dijkstra's algorithm is widely used in the routing protocols required by the routers to update their forwarding table.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, 30, 0.0, "The algorithm provides the shortest cosfft path from the source router to other routers in the network.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.0, 0.0);
	drawstring(-89, 14, 0.0,"Algorithm:");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, 0, 0.0,"1) Create a set sptSet (shortest path tree set) that keeps track of vertices included in the shortest-path tree, i.e.,");
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-86.5,-8,0.0," whose minimum distance from the source is calculated and finalized. Initially, this set is empty.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, -16, 0.0,"2) Assign a distance value to all vertices in the input graph. Initialize all distance values as INFINITE.");
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-86.5, -24, 0.0," Assign distance value as 0 for the source vertex so that it is picked first.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-89, -32, 0.0,"3) While sptSet doesn’t include all vertices");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-79, -42, 0.0,"a) Pick a vertex u which is not there in sptSet and has a minimum distance value.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-79, -52, 0.0,"b) Include u to sptSet.");
	glFlush();
	
	delay();
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-79, -62, 0.0,"c) Update distance value of all adjacent vertices of u. To update the distance values, iterate through all adjacent vertices.");
	glColor3f(0.0, 0.3, 0.4);
	glFlush();
	
	delay();
	drawstring(-79, -72, 0.0,"   For every adjacent vertex v, if the sum of distance value of u (from source) and weight of edge u-v, is less than the distance value of v,");
	glColor3f(0.0, 0.3, 0.4);
	drawstring(-79, -80, 0.0,"   then update the distance value of v.");
	glFlush();
}
void start()
{
	glClear(GL_COLOR_BUFFER_BIT);
	background();
	glColor3f(1.0,0.0,0.0);
	drawstring(-16,92,0.0,"DIJKSTRA'S ALGORITHM");
	drawstring(-17,90,0.0,"...................................................");
	glColor3f(1, 0, 0); // red
	drawstring(50,-92, 0.0, "PRESS ''C'' TO CLOSE>>>");
    glColor3f(1, 0, 0); // red
	drawstring(-92,-92, 0.0, "<<<PRESS ''B'' TO GO BACK");
	glFlush();
	delay();
	drawstring(-90,80,0.0,"# Clicking the left button allows you to create a new node");
	glFlush();
	delay();
	drawstring(-90,72,0.0,"# Clicking on two nodes allows you to establish an edge between two node");
	glFlush();
	delay();
	drawstring(-90,64,0.0,"# Click the right button to select source node and destination node.");
	glFlush();
	delay();
	drawstring(-90,56, 0.0,"# Press ''Y'' to find shortest path");
	glFlush();
	delay();
	glColor3f(0.0f, 1.0f, 0.0f);
	DrawOval(50 ,81, 3,50);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawstring(53,80, 0.0,"--Source node");
	glFlush();
	delay();
	glColor3f(0.0f, 0.0f, 1.0f);
	DrawOval(50 ,74, 3,50);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawstring(53,72, 0.0,"--Destination node");
	glFlush();
	glutDisplayFunc(display);
    glutMouseFunc(mouse_click);
	glutKeyboardFunc(keyboard);
	glFlush();
}

void flowchart()
{
	int i,j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	
	glColor3f(1, 0, 0); // Red
	drawstring(50, -92, 0.0, "PRESS ''C'' TO CLOSE>>>");
	glColor3f(1, 0, 0); // Red
	drawstring(-92, -92, 0.0, "<<<PRESS ''B'' TO GO BACK");
	glColor3f(1, 0, 0); // Red
	drawstring(-14, 85, 0.0, "Flow Diagram of Dijkstra's");
	drawstring(-15, 80, 0.0, "...............................................");
	glFlush();

	delay();
	// Oval
	glColor3f(0, 0, 0); // Black
	DrawOval(0, 68, 8, 30);
	glColor3f(1, 1, 1); // White
	drawstring(-5, 65, 0.0, "START");
	glFlush();

	delay();
	// Arrow
	glBegin(GL_LINES);
	glVertex2f(0, 60);
	glVertex2f(0, 49);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 50);
	glVertex2f(1, 52);
	glVertex2f(-1, 52);
	glEnd();
	glFlush();

	delay();
	// Box
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); // Black
	glVertex2f(25, 50);
	glVertex2f(-25, 50);
	glVertex2f(-25, 33);
	glVertex2f(25, 33);
	glEnd();
	glColor3f(1, 1, 1); // White
	drawstring(-19, 43, 0.0, "Root is set at the local node &");
	glColor3f(1, 1, 1); // White
	drawstring(-20, 37, 0.0, "then forward to the tentative list");
	glFlush();

	delay();
	// Arrow
	glColor3f(0, 0, 0); // Black
	glBegin(GL_LINES);
	glVertex2f(0, 33);
	glVertex2f(0, 25);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 25);
	glVertex2f(1, 27);
	glVertex2f(-1, 27);
	glEnd();
	glFlush();

	delay();
	// Rombus
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); // Black
	glVertex2f(0, 25);
	glVertex2f(25, 12);
	glVertex2f(0, 1);
	glVertex2f(-25, 12);
	glEnd();
	glColor3f(1, 1, 1); // White
	drawstring(-10, 14, 0.0, "is tentative list");
	glColor3f(1, 1, 1); // White
	drawstring(-5, 8, 0.0, "empty?");
	glFlush();

	delay();
	// Arrow
	glBegin(GL_LINES);
	glVertex2f(24, 12);
	glVertex2f(40, 12);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 12);
	glVertex2f(39, 10);
	glVertex2f(39, 14);
	glEnd();
	glFlush();

	delay();
	drawstring(27, 13, 0.0, "No");
	glFlush();

	delay();
	DrawOval(48, 13, 8, 30); // Oval
	glColor3f(1, 1, 1); // White
	drawstring(44, 11, 0.0, "STOP");
	glFlush();

	delay();
	// Arrow
	glColor3f(0, 0, 0); // Black
	glBegin(GL_LINES);
	glVertex2f(0, 1);
	glVertex2f(0, -9);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(0, -8);
	glVertex2f(1, -6);
	glVertex2f(-1, -6);
	glEnd();
	glFlush();

	delay();
	drawstring(1, -5, 0.0, "Yes");
	glFlush();

	delay();
	// Box2
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); // Black
	glVertex2f(-25, -25);
	glVertex2f(-25, -8);
	glVertex2f(25, -8);
	glVertex2f(25, -25);
	glEnd();
	glColor3f(1, 1, 1); // White
	drawstring(-22, -16, 0.0, "Move the node with shortest path");
	glColor3f(1, 1, 1); // White
	drawstring(-14, -22, 0.0, "to the permanent list.");
	glFlush();

	delay();
	// Arrow
	glBegin(GL_TRIANGLES);
	glVertex2f(0, -36);
	glVertex2f(1, -34);
	glVertex2f(-1, -34);
	glEnd();
	glColor3f(0, 0, 0); // Black
	glBegin(GL_LINES); // Last line
	glVertex2f(0, -25);
	glVertex2f(0, -36);
	glEnd();
	glFlush();

	delay();
	// Box
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); // Black
	glVertex2f(-25, -53);
	glVertex2f(-25, -36);
	glVertex2f(25, -36);
	glVertex2f(25, -53);
	glEnd();
	glColor3f(1, 1, 1); // White
	drawstring(-20, -43, 0.0, "If the node has larger cost then");
	glColor3f(1, 1, 1); // White
	drawstring(-20, -49, 0.0, "the next node replace it with it.");
	glFlush();
	for (i = 0; i < 10000; i++) 
		for (j = 0; j < 40000; j++); //delay
	// Arrow
	glColor3f(0, 0, 0); // Black
	glBegin(GL_LINES);
	glVertex2f(-25, -44.5);
	glVertex2f(-45, -44.5);
	glEnd();
	glFlush();
	for (i = 0; i < 10000; i++) 
		for (j = 0; j < 40000; j++); //delay
	glBegin(GL_LINES);
	glVertex2f(-45, -44.5);
	glVertex2f(-45, 12);
	glEnd();
	glFlush();
	for (i = 0; i < 10000; i++) 
		for (j = 0; j < 40000; j++); //delay
	glBegin(GL_LINES);
	glVertex2f(-25, 12);
	glVertex2f(-45, 12);
	glEnd();
	glFlush();
	glBegin(GL_TRIANGLES);
	glVertex2f(-26, 10);
	glVertex2f(-25, 12);
	glVertex2f(-26, 14);
	glEnd();
	glFlush();
}

void DrawOval(float cx, float cy, float r, int num_segments)
{
    int ii;
	float temp;
    float theta = 2 * 3.1415926 / num_segments;
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    float x = r;
    float y = 0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (ii = 0; ii <= num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);
        temp = x;
        x = cosTheta * x - sinTheta * y;
        y = sinTheta * temp + cosTheta * y;
    }
    glEnd();
}

void reset() 
{
    nodeCount = 0;
    edgeCount = 0;
    firstClickedNodeIndex = -1;
	sourceNodeIndex = -1;
    destinationNodeIndex = -1;
}

void  keyboard(unsigned char key, int x, int y) 
{
    if (key == 'm' || key == 'M') 
	{
        currentState = 0;
		reset();
        menu();
    } 
	else if (key == 'c' || key == 'C') 
	{
        exit(0);
    } 
	else if (key == 'a' || key == 'A') 
	{
		currentState = 0;
		reset();
        About();
    }
	else if (key == 'h' || key == 'H') 
	{
		currentState = 0;
		reset();
        Help();
    }
	else if (key == 'i' || key == 'I') 
	{
		currentState = 1;
        about();
    }
	else if (key == 's' || key == 'S') 
	{
        currentState = 2;
        start();
		display();
    }
	else if (key == 'f' || key == 'F') 
	{
		currentState = 1;
        flowchart();
    }
	else if (key == 'b' || key == 'B') 
	{
        if (currentState == 1) 
		{
            // If in start screen, move to menu screen
            currentState = 0;
			reset();
            menu();
        } 
		else if (currentState == 0) 
		{
            // If in menu screen, move to frontscreen
            currentState = 1;
			reset();
            frontscreen();
        }
		else if (currentState == 2) 
		{
            currentState = 0;
			reset();
            menu();
        }
    }
	else if (key == 'y' || key == 'Y') 
	{
		dijkstra();
    }
}
// Draw a circle at the given position with a number label
void draw_circle(float x, float y, float radius, int number) 
{
    char label[3];
    int numSegments = 100;
    float angle;
    int i;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= numSegments; i++) 
	{
        angle = 2.0 * 3.141 * i / numSegments;
        glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
    }
    glEnd();
	if (number >= 1 && number <= 20) {
		char label[3];
		sprintf(label, "%d", number);
		glColor3f(1.0, 1.0, 1.0); // White color
		drawstring(x - 2.0, y - 1.0, 0.0, label);
	}
}
//square nodes
/*void draw_circle(float x, float y, float sideLength, int number) 
{
    char label[3];
    int i;

    glBegin(GL_QUADS);
    glVertex2f(x - sideLength/2, y - sideLength/2);
    glVertex2f(x + sideLength/2, y - sideLength/2);
    glVertex2f(x + sideLength/2, y + sideLength/2);
    glVertex2f(x - sideLength/2, y + sideLength/2);
    glEnd();
    
    if (number >= 1 && number <= 20) {
        sprintf(label, "%d", number);
        glColor3f(1.0, 1.0, 1.0); // White color
        drawstring(x - 2.0, y - 1.0, 0.0, label);
    }
}*/

void draw_line(Node node1, Node node2) {
    glBegin(GL_LINES);
    glVertex2f(node1.x, node1.y);
    glVertex2f(node2.x, node2.y);
    glEnd();
}

void mouse_click(int button, int state, int x, int y) {
    int i;
    Node newNode;
    Edge newEdge;
    int clickedNodeIndex = -1;

    if (isDijkstraApplied) {
        return; // Ignore mouse clicks if Dijkstra's algorithm has been applied
    }

    if (currentState == 0 || currentState == 1) {
        // In menu state, do not create nodes
        return;
    }
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && nodeCount < 20) {
        // Left button click: Create a new node

        // Convert mouse coordinates to OpenGL coordinates
        float convertedX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 200 - 100;
        float convertedY = 100 - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 200;

        // Check if the clicked position is within the region of an existing node
        for (i = 0; i < nodeCount; i++) {
            float dx = convertedX - nodes[i].x;
            float dy = convertedY - nodes[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= 5.0) {
                // Clicked within the region of an existing node
                clickedNodeIndex = i;
                break;
            }
        }
        
        if (clickedNodeIndex != -1) {
            if (firstClickedNodeIndex == -1) {
                // First node clicked
                firstClickedNodeIndex = clickedNodeIndex;
            } else {
                // Second node clicked
                int nodeIndex1 = firstClickedNodeIndex;
                int nodeIndex2 = clickedNodeIndex;
                
                // Calculate the distance between the nodes
                float dx = nodes[nodeIndex2].x - nodes[nodeIndex1].x;
                float dy = nodes[nodeIndex2].y - nodes[nodeIndex1].y;
                float distance = sqrt(dx * dx + dy * dy);
                
                // Create an edge between the nodes
                newEdge.nodeIndex1 = nodeIndex1;
                newEdge.nodeIndex2 = nodeIndex2;
                newEdge.cost = distance;
                edges[edgeCount++] = newEdge;
                firstClickedNodeIndex = -1;
                glutPostRedisplay();
            }
        } else {
            // Check if the clicked position is too close to any existing node
            for (i = 0; i < nodeCount; i++) {
                float dx = convertedX - nodes[i].x;
                float dy = convertedY - nodes[i].y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance <= 5.0 * 2.5) {
                    // Clicked position is too close to an existing node, do not create a new node
                    return;
                }
            }
            
            // Create a new node at the clicked position
            newNode.x = convertedX;
            newNode.y = convertedY;
            nodes[nodeCount++] = newNode;
        }
        glutPostRedisplay();
    }
        else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right button click: Update node colors
        
        // Convert mouse coordinates to OpenGL coordinates
        float convertedX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 200 - 100;
        float convertedY = 100 - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 200;
        
        // Check if the clicked position is within the region of an existing node
        for (i = 0; i < nodeCount; i++) {
            float dx = convertedX - nodes[i].x;
            float dy = convertedY - nodes[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= 5.0) {
                // Clicked within the region of an existing node
                clickedNodeIndex = i;
                break;
            }
        }
        
        if (clickedNodeIndex != -1) {
            // Right-clicked on a node
            if (sourceNodeIndex == -1) {
                // First node clicked
                sourceNodeIndex = clickedNodeIndex;
            } else if (destinationNodeIndex == -1) {
                // Second node clicked
                destinationNodeIndex = clickedNodeIndex;
            } else {
                sourceNodeIndex = clickedNodeIndex;
                destinationNodeIndex = -1; // Reset the destination node index
            }
            
            glutPostRedisplay();
        }
    }
}

void thickenEdges(int* shortestPathEdges, int shortestPathEdgeCount, int sourceNodeIndex, int destinationNodeIndex) {
    int i;
    // Draw the edges
    for (i = 0; i < shortestPathEdgeCount; i++) {
        int edgeIndex = shortestPathEdges[i];
        Node node1 = nodes[edges[edgeIndex].nodeIndex1];
        Node node2 = nodes[edges[edgeIndex].nodeIndex2];
        draw_line(node1, node2);
    }
}


void dijkstra() {
    float distanceFromSource[MAX_NODES];
    int previousNode[MAX_NODES];
    int visited[MAX_NODES];
    int i, j;
    int path[MAX_NODES];
    int pathCount = 0;
    int currentNode;
    int shortestPathEdges[MAX_EDGES];
    int shortestPathEdgeCount = 0;
    char shortestpath[100];
    char shortestPathString[100];
    char nodeString[10];

    for (i = 0; i < nodeCount; i++) {
        distanceFromSource[i] = 99999;
        previousNode[i] = -1;
        visited[i] = 0;
    }

    distanceFromSource[sourceNodeIndex] = 0;

    for (i = 0; i < nodeCount; i++) {
        int current = -1;
        float shortestDistance = 99999;

        for (j = 0; j < nodeCount; j++) {
            if (!visited[j] && distanceFromSource[j] < shortestDistance) {
                current = j;
                shortestDistance = distanceFromSource[j];
            }
        }

        if (current == -1) {
            break;
        }

        visited[current] = 1;

        for (j = 0; j < edgeCount; j++) {
            if (edges[j].nodeIndex1 == current) {
                int neighborIndex = edges[j].nodeIndex2;
                float distance = distanceFromSource[current] + edges[j].cost;

                if (distance < distanceFromSource[neighborIndex]) {
                    distanceFromSource[neighborIndex] = distance;
                    previousNode[neighborIndex] = current;
                }
            } else if (edges[j].nodeIndex2 == current) {
                int neighborIndex = edges[j].nodeIndex1;
                float distance = distanceFromSource[current] + edges[j].cost;

                if (distance < distanceFromSource[neighborIndex]) {
                    distanceFromSource[neighborIndex] = distance;
                    previousNode[neighborIndex] = current;
                }
            }
        }
    }

	// Print the shortest distance and path
	sprintf(shortestpath, "Shortest distance from node %d to node %d: %.2f\n", sourceNodeIndex + 1, destinationNodeIndex + 1, distanceFromSource[destinationNodeIndex]);
	glColor3f(0.0, 0.0, 0.0);
	drawstring(35, -70, 0, shortestpath);

	currentNode = destinationNodeIndex;
	while (currentNode != sourceNodeIndex) {
		path[pathCount++] = currentNode;
		currentNode = previousNode[currentNode];
	}
	strcpy(shortestPathString, "Shortest path: ");
	sprintf(nodeString, "%d", sourceNodeIndex + 1);
	strcat(shortestPathString, nodeString);
	for (i = pathCount - 1; i >= 0; i--)
	{
		sprintf(nodeString, "-->%d", path[i] + 1);
		strcat(shortestPathString, nodeString);
		// Draw arrow between nodes in the shortest path
		if (i > 0)
		{
			Node currentNode = nodes[path[i]];
			Node nextNode = nodes[path[i - 1]];
			draw_line(currentNode, nextNode);
		}
	}
	glColor3f(0.0, 0.0, 0.0);
	drawstring(35, -80, 0, shortestPathString);
    // Populate the shortestPathEdges array with the correct edges
    currentNode = destinationNodeIndex;
    while (currentNode != sourceNodeIndex) {
        int previous = previousNode[currentNode];
        for (j = 0; j < edgeCount; j++) {
            if ((edges[j].nodeIndex1 == currentNode && edges[j].nodeIndex2 == previous) ||
                (edges[j].nodeIndex1 == previous && edges[j].nodeIndex2 == currentNode)) {
                shortestPathEdges[shortestPathEdgeCount++] = j;
                break;
            }
        }
        currentNode = previous;
    }
	glColor3f(1.0,0.0,0.0);
	glLineWidth(5.0);
    thickenEdges(shortestPathEdges, shortestPathEdgeCount, sourceNodeIndex, destinationNodeIndex);
	glLineWidth(1.0);
	isDijkstraApplied = 1;
	display();
	glColor3f(1.0, 0.0, 0.0); //red
    drawstring(10, -60, 0.0,"DIJKSTRA'S ALGORITHM IMPLEMENTED SUCCESSFULLY");
    glFlush();
}

// Display function
void display() 
{
    int i, j;
    // Draw the edges

    glColor3f(0.0, 0.0, 1.0); // Blue color
    for (i = 0; i < edgeCount; i++) 
	{
        Node node1 = nodes[edges[i].nodeIndex1];
        Node node2 = nodes[edges[i].nodeIndex2];
        draw_line(node1, node2);
    }
	    // Draw all the nodes
    for (i = 0; i < nodeCount; i++) 
    {
        glPushMatrix();
        glTranslatef(nodes[i].x, nodes[i].y, 0.0);
        if (i == sourceNodeIndex) {
            glColor3f(0.0f, 1.0f, 0.0f); // Green color for the source node
        } else if (i == destinationNodeIndex) {
            glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the destination node
        } else {
            glColor3f(1.0f, 0.0f, 0.0f); // Red color for other nodes
        }
        draw_circle(0.0, 0.0, 5.0, i + 1); // Draw a circle node with radius 5 and label
        glPopMatrix();
    }
	// Print cost on edges
    glColor3f(0.0f, 0.0f, 0.0f);
	for (i = 0; i < edgeCount; i++) 
	{
		float textHeight ;
		float textWidth ;
		int nodeIndex1 = edges[i].nodeIndex1;
		int nodeIndex2 = edges[i].nodeIndex2;
		float cost = edges[i].cost;
		float midX = (nodes[nodeIndex1].x + nodes[nodeIndex2].x) / 2.0f;
		float midY = (nodes[nodeIndex1].y + nodes[nodeIndex2].y) / 2.0f;
		char costStr[10];
		sprintf(costStr, "%.2f", cost); // Format integer cost as string
		textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)costStr);
		textHeight = 12.0f; // Height of GLUT_BITMAP_HELVETICA_12
		glRasterPos2f(midX, midY);
		for (j = 0; costStr[j] != '\0'; j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, costStr[j]);
		}
	}
    glFlush();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1600,800);
    glutCreateWindow("DIJKSTRA ALGORITHM");
	myinit();
    glutDisplayFunc(frontscreen);
	glutKeyboardFunc(keyboard);
	glutFullScreen(); // make the window full screen
    glutMainLoop();
	return 0;
}