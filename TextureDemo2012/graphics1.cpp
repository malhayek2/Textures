// CS 3600 Graphics Programming
// Spring, 2012
//
// This project shows how to import a .tga file and use it as a texture map.

#include <cmath>
#include <cstring>
#include <ctype.h>
#include <conio.h>
#include <iostream>
using namespace std;
#include "graphics.h"
#include <ctime>
#include "rat.h"
#include "maze.h"

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
bool gMouseLeft = false;
bool gMouseMiddle = false;
bool gMouseRight = false;
enum viewtype { top_view, perspective_view, rat_view };
viewtype current_view = perspective_view;
int mstart;
int mendx;
int mendy;
int mM;
int mN;
bool top = true;
bool bottom = true;
bool left = true;
bool right = true;
Maze gMaze;
Rat mRat;

// Textures
const int num_textures = 4;
static GLuint texName[num_textures];
/*Helper Functions*/

double GetTime() {
	static clock_t start_time = clock();
	clock_t end_time = clock();
	return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}
double getDeltaTime() {
	static clock_t starttime = clock();//built in function clock() problem default is 17 clicks a second while we need about 100 so we made changes
	static int currentframe = 0;
	clock_t currenttime = clock();
	currentframe += 1;
	double totaltime = double(currenttime - starttime) / CLOCKS_PER_SEC;//CLOCKS_PER_SEC is a built in function
	if (totaltime == 0)//prevents dividing by zero
	{
		totaltime = .0001;
	}
	//cout << "current frame" << currentframe << endl;
	double framespersecond = (double)currentframe / totaltime;
	double DT = 1.0 / framespersecond;
	return DT;

}
void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 30.0);
	glMatrixMode(GL_MODELVIEW);
}

void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, M + world_margin_x,
		-world_margin_y, N + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}
// tga image loader code.
#include "tga.h"
gliGenericImage *readTgaImage(char *filename)
{
  FILE *file;
  gliGenericImage *image;

  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: could not open \"%s\"\n", filename);
    return NULL;
  }
  image = gliReadTGA(file, filename);
  fclose(file);
  if (image == NULL) {
    printf("Error: could not decode file format of \"%s\"\n", filename);
    return NULL;
  }
  return image;
}


// Generic image loader code.
gliGenericImage *readImage(char *filename)
{
	size_t size = strlen(filename);
	if(toupper(filename[size-3]) == 'T' && toupper(filename[size-2]) == 'G' && toupper(filename[size-1]) == 'A')
	{
		gliGenericImage * result = readTgaImage(filename);
		if(!result)
		{
			cerr << "Error opening " << filename << endl;
			_getch();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown Filetype!\n";
		_getch();
		exit(1);
	}
}

// This resets the edges of the texture image to a given "border color".
// You must call this for clamped images that do not take up the whole polygon.
// Otherwise, the texture edges will smear outward across the rest
// of the polygon.
void SetBorder(gliGenericImage * image)
{
	// set a border color.
	unsigned int border_r=50;
	unsigned int border_g=50;
	unsigned int border_b=255;
	int x,y;
	y=0;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-2;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}

	x=0;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=1;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=image->width-1;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
	x=image->width-2;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//


// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(0,0,0); //forground color

	double eye[3] = { -3, -3, 7 }; // pick a nice vantage point.
	double at[3] = { 3, 3, 0 };

	
	if (current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 0, 1); // Z is up!
	}
	else if (current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		double z_level = .25;
		double x = mRat.getX();
		double y = mRat.getY();
		double dx = cos(mRat.getDirection() / 180 * 3.14);
		double dy = sin(mRat.getDirection() / 180 * 3.14);
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = z_level;
		gluLookAt(x, y, z_level, at_x, at_y, at_z, 0, 0, 1);
	}
	//glLoadIdentity();
	if (gMouseLeft) {
		mRat.spinLeft();
	}

	if (gMouseRight) {
		mRat.spinRight();
	}

	if (gMouseMiddle) {
		mRat.move();
	}

	gMaze.Draw();

	mRat.Draw();

	// Draw quads with texture
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2d(0,0);
	glTexCoord2f(1,0); glVertex2d(10,0);
	glTexCoord2f(1,1); glVertex2d(10,6);
	glTexCoord2f(0,1); glVertex2d(0,6);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		case 't':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			/*Remove or construct top wall of the current cell */
			// if (top){
			// 	gMaze.removeTopWall(mRat.getX(), mRat.getY(), false);
			// 	mRat.setMaze(&gMaze);
			// 	top = false;
			// } else {
			// 	gMaze.removeTopWall(mRat.getX(), mRat.getY(), true);
			// 	mRat.setMaze(&gMaze);
			// 	top = true;
			// }
			break;
		case 'r':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'p':
			/*When the user clicks ‘p’, see the world from a perspective viewpoint of your choosing.*/
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'w':
			mRat.move();
			break;
		case 'a':
			mRat.spinLeft();
			break;
		case 'd':
			mRat.spinRight();
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}




// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gMouseLeft = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		gMouseLeft = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		gMouseMiddle = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		gMouseMiddle = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gMouseRight = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		gMouseRight = false;
	}
	glutPostRedisplay();
}

// Return true if h is a perfect power of 2 (up to 4096)
bool PowerOf2(int h)
{
	if(h!= 2 && h!=4 && h!=8 && h!=16 && h!=32 && h!=64 && h!=128 && 
				h!=256 && h!=512 && h!=1024 && h!=2048 && h!=4096)
		return false;
	else
		return true;
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	gMaze.Init();
	mRat.setMaze(&gMaze);
	mstart = gMaze.getStart();
	mendx = gMaze.getEndX();
	mendy = gMaze.getEndY();
	mM = gMaze.getM();
	mN = gMaze.getN();
	mRat.Init(double(mstart));

	gliGenericImage *image[num_textures];
	int n=0;
	image[n++] = readImage("fruit.tga");
	image[n++] = readImage("sky.tga");
	image[n++] = readImage("cookies.tga");
	image[n++] = readImage("Seattle.tga");
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	if(n!=num_textures)
	{
		printf("Error: Wrong number of textures\n");
		_getch();
		exit(1);;
	}

	glGenTextures(num_textures, texName);

	for(int i=0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		int repeats = false;
		int needs_border = false; // Needed if clamping and not filling the whole polygon.
		if(repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		if(needs_border)
		{
			// set a border.
			SetBorder(image[i]);
		}

		bool mipmaps = false;
		if(!PowerOf2(image[i]->height) || !PowerOf2(image[i]->width))
		{
			// WARNING: Images that do not have width and height as 
			// powers of 2 MUST use mipmaps.
			mipmaps = true; 
		}

		if (mipmaps) 
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
					image[i]->width, image[i]->height,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					//GL_LINEAR_MIPMAP_LINEAR);
					GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
					//GL_LINEAR);
					GL_NEAREST);
		} 
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
					image[i]->width, image[i]->height, 0,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Textures Demo");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(.2, .5, .1, 0);	//background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
