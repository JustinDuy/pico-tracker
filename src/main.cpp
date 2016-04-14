

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QWidget>
#include <QImageReader>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/structured_light.hpp>

#include <iostream>
#define ___DEBUG 1
using namespace cv;
using namespace std;
String captured_path = "../captured/";

int main( int argc, char** argv )
{
    QApplication a(argc, argv);

    //load Kinect calibration file
    /*FileStorage fs( "../data/cam_proj_trans.yaml", FileStorage::READ );
    if( !fs.isOpened() )
    {
      cout << "Failed to open Camera Calibration Data File." << endl;
      return -1;
    }


    // Loading calibration parameters
    Mat R1, u1;

    fs["R1"] >> R1;
    fs["u1"] >> u1;
    cout << "R1" << endl << R1 << endl;
    cout << "u1" << endl << u1 << endl;*/


    const size_t CAMERA_WIDTH = 640;
    const size_t CAMERA_HEIGHT = 480;

    //tracking a red dot on the camera
    // Open kinect rgb cam, using openni
    VideoCapture capture( CV_CAP_OPENNI );
    if( !capture.isOpened() )
    {
      // check if cam opened
      cout << "kinect not opened!" << endl;
      return -1;
    }
    /*namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
    */
    int thresh = 100;
    int max_thresh = 255;
    while (true)
    {
        Mat imgOriginal, gray;
        capture.grab();
        capture.retrieve( imgOriginal, CV_CAP_OPENNI_BGR_IMAGE );

        int largest_area=0;
        int largest_contour_index=0;

        vector< vector<Point> > contours; // Vector for storing contour
        vector<Vec4i> hierarchy;

        cvtColor(imgOriginal, gray, CV_BGR2GRAY);

        Mat canny_output;
        /// Detect edges using canny
        Canny( gray, canny_output, thresh, thresh*2, 3 );
        findContours( canny_output, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image

        for( int i = 0; i< contours.size(); i++ ) {// iterate through each contour.
           double a=contourArea( contours[i],false);  //  Find the area of contour
           if(a>largest_area){
               largest_area=a;
               largest_contour_index=i;                //Store the index of largest contour
               //bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
           }
        }

        drawContours( gray, contours, largest_contour_index, Scalar(255), CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index

        imshow("Original", imgOriginal); //show the original image
         imshow("Largest contour", gray); //show the original image
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

