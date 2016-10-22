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

const int hue_slider_max = 100;                                                                                                     
int hue_slider;
const int saturation_slider_max = 100;                                                                                                     
int saturation_slider;
const int intensity_slider_max = 100;                                                                                                     
int intensity_slider;

Mat image;
Mat RGB;
double** HSI;
/**
 * @function on_trackbar
 * @brief Callback for trackbar
 *	hue: 0~360
 *  saturation: 0~1
 *	intensity: 0~1
 */
void on_trackbar( int, void* )
{
	HSI = RGB_2_HSI(image);
	for(int row = 0; row < image.rows; row++)
		for(int col = 0; col < image.cols; col++){
			HSI[2][row*image.cols + col] = (HSI[2][row*image.cols + col]/50)*hue_slider;
			HSI[1][row*image.cols + col] = (HSI[1][row*image.cols + col]/50)*saturation_slider;
			HSI[0][row*image.cols + col] = (HSI[0][row*image.cols + col]/50)*intensity_slider;
		}
	RGB = HSI_2_RGB(HSI, image);
 	imshow( "Output", RGB );
}


int main( int argc, char** argv )
{

	if( argc != 2 )
	{
		std::cout << "./[executable] [input image]" << std::endl;
	  	return -1;
	}
	std::ofstream fout ;
	fout.open("output.txt");
	char* imageName = argv[1];

	image = imread( imageName, 1 );

	// Dump value to file
	/*for(int row = 0; row < image.rows; row++){
		for(int col = 0; col < image.cols; col++){
			fout << std::hex << (uint)image.at<Vec3b>(row, col).val[2] << '|'
				<< (uint)image.at<Vec3b>(row, col).val[1] << '|'
				<< (uint)image.at<Vec3b>(row, col).val[0] << ' ';
		}
		fout << std::endl;
	}*/

	// Convert RGB to HSI and back to RGB
	HSI = RGB_2_HSI(image);
	RGB = HSI_2_RGB(HSI, image);

	/*for(int row = 0; row < image.rows; row++){
		for(int col = 0; col < image.cols; col++){
			fout << HSI[2][row*image.cols + col]*180/M_PI << ' ';
		}
		fout << std::endl;
	}*/
	imwrite( "output.bmp", RGB );


	/// Initialize values
	hue_slider = 50;
	saturation_slider = 50;
	intensity_slider = 50;
	/// Create Windows
	namedWindow("Output", 1); 
	
	/// Create Trackbars
	char TrackbarName[50];
	sprintf( TrackbarName, "Hue x %d", hue_slider_max );
	createTrackbar( TrackbarName, "Output", &hue_slider, hue_slider_max, on_trackbar );
	sprintf( TrackbarName, "Saturation x %d", hue_slider_max );
	createTrackbar( TrackbarName, "Output", &saturation_slider, saturation_slider_max, on_trackbar );
	sprintf( TrackbarName, "Intensity x %d", hue_slider_max );
	createTrackbar( TrackbarName, "Output", &intensity_slider, intensity_slider_max, on_trackbar );
	
	on_trackbar( hue_slider, 0 );
	// waitKey(0);
	while((cv::waitKey() & 0xEFFFFF) != 27);
	
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
