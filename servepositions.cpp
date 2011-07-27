#include <iostream>
#include <iomanip>
#include <python2.7/Python.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "cvblob.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/serveposition"

using namespace cvb;
using namespace std;

void cleanup(CvCapture* capture){
  cvReleaseCapture(&capture);
  cvDestroyWindow( "ImageWindow" );
  exit(0);
}

int main( int argc, char** argv ) {
  int s, s2, t, len;
  struct sockaddr_un local, remote;
  char str[100];

  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, SOCK_PATH);
  unlink(local.sun_path);
  len = strlen(local.sun_path) + sizeof(local.sun_family);
  if (bind(s, (struct sockaddr *)&local, len) == -1) {
    perror("bind");
    exit(1);
  }

  if (listen(s, 5) == -1) {
    perror("listen");
    exit(1);
  }
  //end of setup for socket
  

  //setup for opencv
  cvNamedWindow( "ImageWindow", CV_WINDOW_AUTOSIZE );
  CvCapture* capture;
  if (argc==1) {
    capture = cvCreateCameraCapture(int( 0) );
  } else {
    capture = cvCreateCameraCapture(int( argv[1]) );
  }
  assert( capture != NULL );
  IplImage* frame;
  //end of setup for opencv

  while(1) {
    
    int done, n;
    cout << "Waiting for a connection..." << endl;
    t = sizeof(remote);
    if ((s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&t)) == 1) {
      perror("accept");
      exit(1);
    }
    cout << "Connected." << endl;
    done = 0;
    
    //now that we're connected grab a frame, get blobs and return positions
    do {
      //get a frame and find blobs
      frame = cvQueryFrame( capture );
      if( !frame ) break;
      IplImage *gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
      cvCvtColor(frame, gray, CV_BGR2GRAY);
      cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY);
        
      IplImage *labelImg=cvCreateImage(cvGetSize(gray), IPL_DEPTH_LABEL, 1);
	
      CvBlobs blobs;
      unsigned int result= cvLabel(gray, labelImg, blobs);
      cvRenderBlobs(labelImg, blobs, frame, frame);
      //done finding blobs

      //format a string that displays all blobs
      string results;
      char * s = NULL;
      time_t now;
      for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
      {
        //cout << "Blob #" << it->second->label 
          //<< ": Area=" << it->second->area 
          //<< ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")" 
          //<< endl;
        now = time(0);
        asprintf(&s, "$%d:%f:%f:%ld:%f", it->second->label, it->second->centroid.x, it->second->centroid.y, now, it->second->area);
        if (s > 0) results += s;
        free(s);
      }
      //done formatting results
      
      //get stuff from client and print results back

      const char * c_results = results.c_str();

      n = recv(s2, str, 100, 0);
      if(n <= 0) {
        if (n < 0 ) perror("recv");
        done = 1;
      }
      string client_recv;
      for(int i=0;i<n;i++) client_recv += str[i];


      cout << client_recv << endl;
      if( client_recv.compare("q") == 0){
        printf("Recieved q\n");
        cleanup(capture);
      }else if( client_recv.compare("r")==0){
        printf("Recieved r\n");
      }



      if(!done) {
        if (send(s2, c_results, strlen(c_results), 0) < 0){
          perror("send");
          done = 1;
        }
      }
      
      
      cvShowImage( "ImageWindow", frame );
    } while (!done);
    
    close(s2);

  }
  cvReleaseCapture( &capture );
  cvDestroyWindow( "ImageWindow" );
  return 0;
}



