#include <iostream>
#include <iomanip>
#include <python2.7/Python.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "cvblob.h"
using namespace cvb;
using namespace std;

int main( int argc, char** argv ) { 
    cvNamedWindow( "Example2_9", CV_WINDOW_AUTOSIZE );
    CvCapture* capture;
    if (argc==1) {
      capture = cvCreateCameraCapture(int( 0) );
    } else {
      capture = cvCreateCameraCapture(int( argv[1]) );
    }
    assert( capture != NULL );
    //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
    //cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
    IplImage* frame;
    CvTracks tracks;

    while(1) {
      frame = cvQueryFrame( capture );
      if( !frame ) break;
      IplImage *gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
      cvCvtColor(frame, gray, CV_BGR2GRAY);
      cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY);
        
      IplImage *labelImg=cvCreateImage(cvGetSize(gray), IPL_DEPTH_LABEL, 1);
	
      CvBlobs blobs;

      unsigned int result= cvLabel(gray, labelImg, blobs);
      //cvFilterByArea(blobs, 1, 100);
      //cvRenderBlobs(labelImg, blobs, frame, frame);
      cvUpdateTracks(blobs, tracks, 5., 10);
      cvRenderTracks(tracks, frame, frame, CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);
      cvReleaseBlobs(blobs);
      cvShowImage( "Example2_9", frame );
      char c = cvWaitKey(1);
      if( c == 27 ) break;
    }
    cvReleaseTracks(tracks);
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Example2_9" );
}
