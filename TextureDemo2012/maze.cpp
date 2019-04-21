#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include "glut.h"
#include "maze.h"

//extern viewtype current_view;
void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}
Cell::Cell()
{
	mLeft = true;
	mBottom = true;
	mRight = true;
	mTop = true;
	mVisited = false;
}

bool Cell::getTop()
{
	return this->mTop; 
}

bool Cell::getBottom()
{
	return this->mBottom; 
}

bool Cell::getLeft()
{
	return this->mLeft; 
}

bool Cell::getRight()
{
	return this->mRight; 
}

void Cell::SetTop(bool state)
{
	this->mTop = state;
}

void Cell::SetBottom(bool state)
{
	this->mBottom = state;
}

void Cell::SetRight(bool state)
{
	this->mRight = state;
}

void Cell::SetLeft(bool state)
{
	this->mLeft = state;
}

void Cell::Draw(int i, int j, double texfile1, double texfile2, double texfile3, double texfile4)
{
	int x = i;
	int y = j;

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texfile1);
	static double h = 0.0;
	glBegin(GL_QUADS);
	glTexCoord2d(0, h); glVertex3d(-M, -N, 2);
	glTexCoord2d(M / (M - 2), h); glVertex3d(M*1.5, -N, 2);
	glTexCoord2d(M / (M - 2), h + N / (N - 2)); glVertex3d(M*1.5, N*1.5, 2);
	glTexCoord2d(0, h + N / (N - 2)); glVertex3d(-M, N*1.5, 2);
	glEnd();
	h += .00000007;
	glDisable(GL_TEXTURE_2D);



	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texfile1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(0, 0, 0);
	glTexCoord2f(M, 0); glVertex3d(0, N, 0);
	glTexCoord2f(M, N); glVertex3d(M, N, 0);
	glTexCoord2f(0, N); glVertex3d(M, 0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	*/

	
	if (mTop)
	{

		DrawLine(i, j + 1, i + 1, j + 1);

		if (i % 2 == 0) {
			
			/*topleft->right->bot->left*/

			/*
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texfile4);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x, y + 0.5, 0);
			glTexCoord2d(1, 0); glVertex3f(x +0.5, y + 0.5, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 0.5, y +0.5, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y + 0.5, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			*/
			
		}
		
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texfile1);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x, y + 1, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y + 1, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y + 1, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y + 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		

	}
	if (mLeft)
	{

		DrawLine(i, j, i, j + 1);


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texfile3);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex3f(x, y, 0);
		glTexCoord2d(1, 0); glVertex3f(x, y + 1, 0);
		glTexCoord2d(1, 1); glVertex3f(x, y + 1, 1);
		glTexCoord2d(0, 1); glVertex3f(x, y, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);


	}
	if (mRight)
	{

		DrawLine(i + 1, j, i + 1, j + 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texfile4);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex3f(x + 1, y, 0);
		glTexCoord2d(1, 0); glVertex3f(x + 1, y + 1, 0);
		glTexCoord2d(1, 1); glVertex3f(x + 1, y + 1, 1);
		glTexCoord2d(0, 1); glVertex3f(x + 1, y, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);


	}
	
	if (mBottom)
	{

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texfile1);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3d(i, j, 0);
			glTexCoord2d(1, 0); glVertex3d(i + 1, j, 0);
			glTexCoord2d(1, 1); glVertex3d(i + 1, j, 1);
			glTexCoord2d(0, 1); glVertex3d(i, j, 1);
			

			glEnd();
			glDisable(GL_TEXTURE_2D);

			DrawLine(i,j, i+1,j);
		
	}


}

void Cell::SetVisited(bool state)
{
	this->mVisited = state;
}

bool Cell::GetVisited()
{
	return this->mVisited;
}

Maze::Maze()
{
}

void Maze::Init()
{
	
	// Find a start cell:
	RemoveWallsR(0, 0);
	mStartI = rand()%M;
	mEndI = rand()%M;
	this->mCells[mStartI][0].SetBottom(false);
	this->mCells[mEndI][N-1].SetTop(false);
	//RemoveWallsR(mStartI,0);
	//RemoveWallsT(mEndI,3);
	
}

bool Maze::isItWall(double x_in, double y_in, double radius_in)
{
	int tempX = int(x_in);
	int tempY = int(y_in);
	//wall up, down, right, left
	if (mCells[tempX][tempY].getTop() && (y_in + radius_in) >= (tempY+1))
	{
		return true;
	}
	if (mCells[tempX][tempY].getBottom() && (y_in - radius_in) <= (tempY))
	{
		return true;
	}
	if (mCells[tempX][tempY].getRight() && (x_in + radius_in) >= (tempX+1))
	{
		return true;
	}
	if (mCells[tempX][tempY].getLeft() && (x_in - radius_in) <= (tempX))
	{
		return true;
	}
	//corners upl, upr, dl, dr
	//distance x_in - tempx
	//distance y_in - tempy
	//bottom right
	//if (r> offsetx && r>1-offsettx)return true
	double offsetX = x_in - tempX;
	double offsetY = y_in - tempY;
	if (radius_in > offsetY && radius_in > (1-offsetX)){
		return true;
	}
	if (radius_in > offsetY && radius_in > offsetX){
		return true;
	}
	if (radius_in > (1-offsetX) && radius_in > (1-offsetY)){
		return true;
	}
	if (radius_in > offsetX && radius_in > (1-offsetY)){
		return true;
	}
	return false;
}

bool Maze::isValid(int i_in, int j_in)
{
	//0 up
	//1 down
	//2 left
	//3 right
	if (i_in < 0 || j_in < 0){
		return false;
	}
	if (i_in >= M || j_in >= N){
		return false;
	}
	if (mCells[i_in][j_in].GetVisited()){
		return false;
	}
	return true;
}

void Maze::Drawfloor(double texname)
{
	//glColor3d(.1, .5, .3);
	
	
	for (int i = 1; i <= M; i++) {
		for (int j = 1; j <= N; j++) {

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texname);
			glBegin(GL_QUADS);
			/*topleft->right->bot->left*/
			glTexCoord2f(0, 0); glVertex3d(i-1, j-1, 0);
			glTexCoord2f(1, 0); glVertex3d(i-1, j, 0);
			glTexCoord2f(1, 1); glVertex3d(i, j, 0);
			glTexCoord2f(0, 1); glVertex3d(i, j-1, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	
	/*
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texname);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(0, 0, 0);
	glTexCoord2f(1, 0); glVertex3d(0, N, 0);
	glTexCoord2f(1, 1); glVertex3d(M, N, 0);
	glTexCoord2f(0, 1); glVertex3d(M, 0, 0);
	//cout << "printing floor" << endl;
	glDisable(GL_TEXTURE_2D);
	*/


	//glEnd();
}
void Maze::RemoveWallsR(int i, int j)
{
	this->mCells[i][j].SetVisited(true);
	vector<int> options;
	if (isValid(i,j+1)){
		options.push_back(0);
	}
	if (isValid(i,j-1)){
		options.push_back(1);
	}
	if (isValid(i-1,j)){
		options.push_back(2);
	}
	if (isValid(i+1,j)){
		options.push_back(3);
	}

	//shuffle the options
	random_shuffle(options.begin(), options.end());
	

	for (unsigned int m=0;m<options.size();m++){
		
		if (options[m] == 0){
			//up is next
			if (isValid(i,j+1)){
				mCells[i][j].SetTop(false);
				mCells[i][j+1].SetBottom(false);
				RemoveWallsR(i,j+1);
			}
		} else if (options[m] == 1){
			//down
			if (isValid(i,j-1)){
				mCells[i][j-1].SetTop(false);
				mCells[i][j].SetBottom(false);
				RemoveWallsR(i,j-1);
			}
		} else if (options[m] == 2){
			//left
			if (isValid(i-1,j)){
				mCells[i][j].SetLeft(false);
				mCells[i-1][j].SetRight(false);
				RemoveWallsR(i-1,j);
			}
		} else if (options[m] == 3){
			//right
			if (isValid(i+1,j)){
				mCells[i+1][j].SetLeft(false);
				mCells[i][j].SetRight(false);
				RemoveWallsR(i+1,j);
			}
		}
	}	
	
}

int Maze::getStart()
{
	return this->mStartI;
}

int Maze::getEndX()
{
	return this->mEndI;
}

int Maze::getEndY()
{
	return N-1;
}

int Maze::getM()
{
	return M;
}
int Maze::getN()
{
	return N;
}

void Maze::removeTopWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetTop(bool_in);
	this->mCells[tempx][tempy+1].SetBottom(bool_in);
}

void Maze::removeBottomWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetBottom(bool_in);
	this->mCells[tempx][tempy-1].SetTop(bool_in);
}

void Maze::removeRightWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetRight(bool_in);
	this->mCells[tempx+1][tempy].SetLeft(bool_in);
}

void Maze::removeLeftWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetLeft(bool_in);
	this->mCells[tempx-1][tempy].SetRight(bool_in);
}

void Maze::Draw(double texfile1, double texfile2, double texfile3, double texfile4)
{
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			/**/
			mCells[i][j].Draw(i, j, texfile1, texfile2, texfile3, texfile4);

}


