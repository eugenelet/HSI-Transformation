#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <math.h>
#include <algorithm>

using namespace cv;

double** RGB_2_HSI(Mat img);
Mat HSI_2_RGB(double** HSI, Mat img);

Mat image;
Mat RGB;
double** HSI;


int main( int argc, char** argv )
{

	if( argc != 2 )
	{
		std::cout << "./[executable] [input image]" << std::endl;
		return -1;
	}
	char* imageName = argv[1];

	image = imread( imageName, 1 );

	// Convert RGB to HSI
	HSI = RGB_2_HSI(image);

	// Scan through image to convert specific pixels
	for(int row = 0; row < image.rows; row++)
		for(int col = 0; col < image.cols; col++){
			double hue = HSI[2][row*image.cols + col]*180/M_PI;
			if(hue<120 && hue>60){
				// Shift Hue towards red
				HSI[2][row*image.cols + col] = (hue - 50.0)*M_PI/180;
				// Increase Saturation of grass to increase constrast
				HSI[1][row*image.cols + col] *= 1.2 ;
				// Reduce intensity to provide a "dawn" feel
				HSI[0][row*image.cols + col] *= 0.9 ;
			}
			else if(hue>160 && hue<300){
				// Shift hue towards dark blue
				HSI[2][row*image.cols + col] = (hue + 50.0)*M_PI/180;
				// Reduce saturation to have a dull feel
				HSI[1][row*image.cols + col] *= 0.5 ;
				// Reduce intensity to provide a dim lighting
				HSI[0][row*image.cols + col] *= 0.6 ;
			}

		}
	// Convert HSI to RGB
	RGB = HSI_2_RGB(HSI, image);
	imwrite( "trans.bmp", RGB );

	imshow("input", image);
 	imshow( "output", RGB );

	waitKey(0);
	
	return 0;
		
}

double** RGB_2_HSI(Mat RGB){
	double **HSI;
	HSI = new double*[3];
	for(int i = 0; i < 3; i++)
		HSI[i] = new double[RGB.rows* RGB.cols];

	for(int row = 0; row < RGB.rows; row++)
		for(int col = 0; col < RGB.cols; col++){
			double R = ((double)RGB.at<Vec3b>(row, col).val[2])/255;
			double G = ((double)RGB.at<Vec3b>(row, col).val[1])/255;
			double B = ((double)RGB.at<Vec3b>(row, col).val[0])/255;
			double i = R + G + B;
			HSI[0][row*RGB.cols + col] = i/3;
			double r = R/i;
			double g = G/i;
			double b = B/i;
			if(R==G && G==B){
				HSI[1][row*RGB.cols + col] = 0;
				HSI[2][row*RGB.cols + col] = 0;
			}
			else{
				double w = 0.5*(2*R - G -B)/std::sqrt((R-G)*(R-G)+(R-B)*(G-B));
				if(w > 1)
					w = 1;
				if(w < -1)
					w = -1;
				HSI[2][row*RGB.cols + col] = std::acos(w);
				if(B > G)
					HSI[2][row*RGB.cols + col] = 2*M_PI - HSI[2][row*RGB.cols + col];
				
				if(r<=g && r<=b)	HSI[1][row*RGB.cols + col] = 1 - 3*r;
				if(g<=r && g<=b)	HSI[1][row*RGB.cols + col] = 1 - 3*g;
				if(b<=r && b<=g)	HSI[1][row*RGB.cols + col] = 1 - 3*b;

			}
		}
	return HSI;
}


Mat HSI_2_RGB(double** HSI, Mat img){
	Mat RGB(img.rows, img.cols, img.type());
	for(int row = 0; row < img.rows; row++)
		for(int col = 0; col < img.cols; col++){
			double H = HSI[2][row*img.cols + col];
			double S = HSI[1][row*img.cols + col];
			double I = HSI[0][row*img.cols + col];
			double R, G, B, r, g, b;
			if(S > 1) S = 1;
			if(I > 1) I = 1;
			if(S==0)
				R = G = B = I;
			else{
				if((H>=0) && (H < 2*M_PI/3)){
					b = (1-S)/3;
					r = (1+S*std::cos(H)/std::cos(M_PI/3 - H)) / 3;
					g = 1 - r - b;
				}
				else if((H>=2*M_PI/3) && (H<4*M_PI/3)){
					H = H - 2*M_PI/3;
					r = (1-S)/3;
					g = (1+S*std::cos(H)/std::cos(M_PI/3 - H)) / 3;
					b = 1 - r - g;
				}
				else if((H>=4*M_PI/3) && (H<2*M_PI)){
					H = H - 4*M_PI/3;
					g = (1-S)/3;
					b = (1+S*std::cos(H)/std::cos(M_PI/3 - H)) / 3;
					r = 1 - b - g;
				}

				if(r < 0)	r = 0;
				if(g < 0)	g = 0;
				if(b < 0)	b = 0;
				R = 3*I*r;	G = 3*I*g; B = 3*I*b;
				if(R>1)	R=1;
				if(G>1)	G=1;
				if(B>1)	B=1;
				RGB.at<Vec3b>(row, col).val[2] = R * 255;
				RGB.at<Vec3b>(row, col).val[1] = G * 255;
				RGB.at<Vec3b>(row, col).val[0] = B * 255;
			}


		}
		return RGB;
}








