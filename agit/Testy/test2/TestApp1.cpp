// Autor: S.E.Härtel
// www.zserve.de
// edited: 20.01.2015


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "cvcam.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;

bool drawGrid, initialize, play, setMarker, passThrough, gameEnd, playAgain = false;
bool showMessageIni, playerMove, WinGame, toggelIniOrPlay = true;
int weightPointAverage, showGrid, weightPointAverageMove = 0;
int baseGridPointX, baseGridPointY, bGPX, bGPY, mgpX, mgpY =0;

int x, y, mark;

int bGPArrayX1[9] = { -75, -25, +25, -75, -25, +25, -75, -25, +25 };//base for matrixgrid
int bGPArrayY1[9] = { -75, -75, -75, -25, -25, -25, +25, +25, +25 };//to set the area of 
int bGPArrayX2[9] = { -25, +25, +75, -25, +25, +75, -25, +25, +75 };//point compare between
int bGPArrayY2[9] = { -25, -25, -25, +25, +25, +25, +75, +75, +75 };//3D and 2D coordinates

int bgpResX1[9], bgpResY1[9], bgpResX2[9], bgpResY2[9] = {0};//store calculated cornerpoints of grid

int tttMPointX[9] = { 108, 180, 252, 108, 180, 252, 108, 180, 252};//centerpoints of the ttt 2D Field
int tttMPointY[9] = { 72, 72, 72, 144, 144, 144, 216, 216, 216};//as base for drawing O or X

int hueThreshold = 345;//track for red, red great dot or square needed to fit in the TTT Grid

//char Area[9] = {'1','2','3','4','5','6','7','8','9'};
char Area[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
//char playerMoveMark(' ');

int main()
{
	CvCapture* capture = 0;
	capture = cvCaptureFromCAM(0); 	

	// capture width and height from camoutput
	int width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);

	// create picture datastructur
	IplImage* HSV = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	IplImage* H_Plane = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage* Mask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage* tttPlayField = cvLoadImage("TicTacToeFeldFlat.jpg",1);

	// create output-window
	cvNamedWindow("current",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Mask",CV_WINDOW_AUTOSIZE);//uncomment to adjust for other color/dont forget to cvShowImage uncomment ceil 321
	cvCreateTrackbar("hue", "Mask", &hueThreshold, 360, 0);
	cvNamedWindow("TicTacToe",CV_WINDOW_AUTOSIZE);
		
	cout << "Place Grid and set Stone in the middle of Grid" << endl;
	cout << "-> press Enter to initialize Gamefield" << endl << endl;
	cout << endl;
	showMessageIni = false;
	
	while(1)
	{
		IplImage* orgFrame = cvQueryFrame(capture);//wrong mirrored
		IplImage* currentFrame;
		currentFrame = cvCloneImage(orgFrame);
		cvFlip(orgFrame, currentFrame,0);//right direction
		if(currentFrame == 0)
		{
			break;
		}

		//convert to HSV
		cvCvtColor(currentFrame, HSV, CV_BGR2HSV);
		//Divide multi-channel array into several single-channel array
		cvSplit(HSV, H_Plane, 0,0,0);

		// Segmentation
		int minHue = hueThreshold - 10;
		int maxHue = hueThreshold + 10;

		if(minHue < 0) minHue += 360;
		if(maxHue > 360) maxHue -= 360;

		maxHue /= 2;
		minHue /= 2;

		if(maxHue > minHue) cvInRangeS(H_Plane, cvScalar(minHue), cvScalar(maxHue), Mask);
		else
		{
			cvInRangeS(H_Plane,cvScalar(maxHue), cvScalar(minHue), Mask);
			cvSubRS(Mask, cvScalar(255), Mask);
		}

		// Mask filter
		cvErode(Mask, Mask);
		cvDilate(Mask, Mask);		

		//weightpoint of white pixel
		int xSum = 0;
		int ySum = 0;
		int counter = 0;
		for(int x = 0; x<width; x++)
		{
			for(int y=0; y<height; y++)
			{
				CvScalar pixel = cvGet2D(Mask, y, x);
				
				if(pixel.val[0] > 0)
				{
					xSum += x;
					ySum += y;
					counter++;
				}
			}
		}
		
		if(counter > 0)
		{
			xSum /= counter;
			ySum /= counter;
		}

		// mark weightpoint
		cvLine(currentFrame, cvPoint(xSum, ySum-10), cvPoint(xSum, ySum+10), cvScalar(0, 255, 0), 2);
		cvLine(currentFrame, cvPoint(xSum-10, ySum), cvPoint(xSum+10, ySum), cvScalar(0, 255, 0), 2);
		
		

		if(initialize)		
		{	
			if(weightPointAverage < 10)//calculate average of centerpoint as base for grid
			{
				baseGridPointX += xSum;
				baseGridPointY += ySum;

				if(weightPointAverage == 9)
				{
					baseGridPointX /= 10;
					baseGridPointY /= 10;
					drawGrid = true;
					cout << endl << "baseGridPointX: " << baseGridPointX << " baseGridPointY: " << baseGridPointY << endl;
				}		
			}
			weightPointAverage++;
		
			if(drawGrid)//show Grid for seconds
			{			
				for(int i=0;i<9;i++)
				{							
					cvRectangle(currentFrame,//Show grid in range 
						cvPoint(baseGridPointX+bGPArrayX1[i], baseGridPointY+bGPArrayY1[i]),
						cvPoint(baseGridPointX+bGPArrayX2[i], baseGridPointY+bGPArrayY2[i]),
						cvScalar(0,0,255,0),
						1,8,0);	//   * cvScalar -> color of token / gaming piece RGB (set to blue)
							//  -* cvScalar(255,0,0,0) red doesn't do it well, while the function capture also the amount of color 
							// --* from the Hand for the average calculation of the weightpoint and lead to wrong positioning
					bgpResX1[i] = (baseGridPointX+bGPArrayX1[i]);//store Point X1 in Range for comparison
					bgpResY1[i] = (baseGridPointY+bGPArrayY1[i]);//store Point Y1 in Range
					bgpResX2[i] = (baseGridPointX+bGPArrayX2[i]);//store Point X2 in Range
					bgpResY2[i] = (baseGridPointY+bGPArrayY2[i]);//store Point Y2 in Range
				}
				if ( showGrid > 30) 
				{
					drawGrid = false;
					initialize = false;
					showGrid = 0;
					weightPointAverage = 0;	
					toggelIniOrPlay = false;
					//play = true;
					cout << endl << "Press enter again to start game" << endl << endl<<"\t\t  ";
				}
				showGrid++;			
			}		
		}

		//Gamelogic -- hier 
		
		
		if(play)
		{
			if(weightPointAverageMove < 10)//calculate average of placed playitem 
										   //as base for sectoridentifier
			{
				mgpX += xSum;
				mgpY += ySum;
				if(weightPointAverageMove == 9)
				{
					mgpX /= 10;
					mgpY /= 10;					
					//cout << "mgpX: " << mgpX << " mgpY: " << mgpY << endl;
					passThrough = true;
				}		
				weightPointAverageMove++;
			}
			
			if(passThrough)//only if the weightpoint from calculated center of playstone exist
			{				
				for(int j=0;j<9;j++)
				{
					if((mgpX > bgpResX1[j] && mgpX < bgpResX2[j] && mgpY > bgpResY1[j] && mgpY < bgpResY2[j])&& (Area[j] == ' '))
					{
						x = j;
						y = j;
						mark = j;			
						setMarker = true;						
						break;
					}
				}
				passThrough = false;
				weightPointAverageMove = 0;				
			}
			
			if(setMarker)
			{
				if(playerMove)
				{
					cvLine(tttPlayField, cvPoint(tttMPointX[x]-36,tttMPointY[y]-36), cvPoint(tttMPointX[x]+36,tttMPointY[y]+36), cvScalar(0, 255, 0), 2);
					cvLine(tttPlayField, cvPoint(tttMPointX[x]-36,tttMPointY[y]+36), cvPoint(tttMPointX[x]+36,tttMPointY[y]-36), cvScalar(0, 255, 0), 2);
					Area[mark] = 'X';				
					playerMove = false;	
					cout << "Area[" << x+1 << "] = " << Area[x] << endl;
				}
				else
				{
					cvCircle(tttPlayField,
					cvPoint(tttMPointX[x],tttMPointY[y]), 35,
					cvScalar(0, 0, 255),
					2,8,0);
					Area[mark] = 'O';					
					playerMove = true;
					cout << "Area[" << x+1 << "] = " << Area[x] << endl;
				}			
				setMarker = false;
				for(int l=0;l<9;l++) //Output of used Area
					cout << Area[l] << " ";
				
				mgpX = 0;
				mgpY = 0;
			}	

			//look for -> 8 wincondition and 1 draw
			if (Area[0] != ' ') 
			{
				if (Area[1] == Area[0] && Area[2] == Area[0]) 
				{
					gameEnd = true;					
				}
				if (Area[3] == Area[0] && Area[6] == Area[0]) 
				{
					gameEnd = true;					
				}				
			}
			if (Area[4] != ' ') 
			{
				if (Area[0] == Area[4] && Area[8] == Area[4]) 
				{
					gameEnd = true;
				}
				if (Area[1] == Area[4] && Area[7] == Area[4]) 
				{
					gameEnd = true;
				}
				if (Area[3] == Area[4] && Area[5] == Area[4]) 
				{
					gameEnd = true;
				}
				if (Area[2] == Area[4] && Area[6] == Area[4]) 
				{
					gameEnd = true;
				}				
			}
			if (Area[8] != ' ') 
			{
				if (Area[2] == Area[8] && Area[5] == Area[8]) 
				{
					gameEnd = true;
				}
				if (Area[6] == Area[8] && Area[7] == Area[8]) 
				{
					gameEnd = true;
				}				
			}
			if (Area[0] != ' ' && Area[1] != ' ' && Area[2] != ' ' &&
				Area[3] != ' ' && Area[4] != ' ' && Area[5] != ' ' &&
				Area[6] != ' ' && Area[7] != ' ' && Area[8] != ' ' && !gameEnd)
				{
					gameEnd = true;
					WinGame = true;
				}
			if(gameEnd)
			{
				if(!WinGame)
				{
					cout << endl << "\t\t  " << "Player "<< Area[mark] << " wins!" << endl << endl;
					play = false;
					gameEnd = false;
				}
				else
				{
					cout << endl << "\t\t  " << "Nobody Wins" << endl << endl;
					play = false;
					gameEnd = false;
				}
				cout << endl << "Press Backspace to play again / ESC to exit" << endl << endl <<"\t\t  ";
			}
		}
		char c = cvWaitKey(60);	
		if( c == 8 )//push Backspace to play again
			playAgain=true;

		if(playAgain)
		{
			for(int n=0;n<9;n++)
			{
				Area[n] = ' ';//array set back to default
			}			
			play = true;
			playAgain=false;
			tttPlayField=cvLoadImage("TicTacToeFeldFlat.jpg");			
		}
		// show image
		cvShowImage("current", currentFrame);
		//cvShowImage("Mask", Mask);//uncomment to show the image of hue
		cvShowImage("TicTacToe", tttPlayField);	
		
		if (toggelIniOrPlay)
		{
			if ( c == 13) //wait for enter-key to initialize the Grid
			{
				initialize = true;
				drawGrid = true;
				play = false;
				//toggelIniOrPlay = false;
				
			}	
		}
		else
		{
			if ( c == 13) //wait for enter-key to start Game
			{
				initialize = false;
				drawGrid = false;
				play = true;
			}
		}
		
		if( c == 27 )
        goto _cleanup_;

	}
_cleanup_:

  cvReleaseImage( &HSV );
  cvReleaseImage( &H_Plane );
  cvReleaseImage( &Mask );
  cvReleaseImage( &tttPlayField );
  cvReleaseCapture( &capture );

  cvDestroyWindow("current");
  cvDestroyWindow("Mask");
  cvDestroyWindow("TicTacToe");

  return 0;
	
}

