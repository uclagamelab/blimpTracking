#include "opencv/cv.h"
#include "opencv/highgui.h"

int main( int argc, char** argv ) { 
    cvNamedWindow( "Example2_9", CV_WINDOW_AUTOSIZE );
    CvCapture* capture;
    if (argc==1) {
      capture = cvCreateCameraCapture(int( 0) );
    } else {
      capture = cvCreateCameraCapture(int( argv[1]) );
    }
    assert( capture != NULL );
    //    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
    //    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
    IplImage* frame;

    while(1) {
        frame = cvQueryFrame( capture );
        if( !frame ) break;
        cvShowImage( "Example2_9", frame );
        char c = cvWaitKey(1);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Example2_9" );
}
