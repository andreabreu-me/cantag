/**
 * $Header$
 */
#include <Config.hh>
#include <QuadTangle.hh>
#include <opencv/cv.h>
#include <cmath>

#define COMPARE_THRESH 1

#define QUADTANGLE_DEBUG

QuadTangle::QuadTangle() {
  m_fitted = false;
}

QuadTangle::QuadTangle(float* points, int numpoints, bool prev_fitted) {
  
  if (!prev_fitted && points) {
    CvMemStorage* seqstore = cvCreateMemStorage(0);
    CvSeq* contour = cvCreateSeq(CV_SEQ_POLYGON,sizeof(CvSeq),sizeof(CvPoint),seqstore);
    for(int i=0;i<numpoints*2;i+=2) {
      CvPoint p = cvPoint((int)(points[i]*1000),(int)(points[i+1]*1000));
      cvSeqPush(contour,&p);    
    }

    //CvPoint poly2[contour->total];
    //    cvCvtSeqToArray( contour, poly2 , cvSlice(0,contour->total));
    //for( int pt = 0; pt < contour->total; pt++ ) {
    //      std::cerr << poly2[pt].x << " " << poly2[pt].y << std::endl;
    //    }

#ifdef QUADTANGLE_DEBUG
    PROGRESS("Copied points back into opencv sequence");
#endif
    CvMemStorage* store = cvCreateMemStorage(0);
    CvSeq *result = cvApproxPoly( contour, sizeof(CvContour), store,
				  CV_POLY_APPROX_DP, 
				  cvContourPerimeter(contour)*0.02, 0 );
    
    //    CvPoint poly[result->total];
    //    cvCvtSeqToArray( result, poly , cvSlice(0,result->total));
    //    for( int pt = 0; pt < result->total; pt++ ) {
    //      std::cerr << "0 0 " << poly[pt].x << " " << poly[pt].y << std::endl;
    //    }
    //    std::cerr<<"-"<<std::endl;

#ifdef QUADTANGLE_DEBUG
    PROGRESS("Applied polygon approximation");
    PROGRESS("Vertices = " << result->total);
    PROGRESS("Convex = " << (cvCheckContourConvexity(result) ? "yes" : "no"));
    PROGRESS("Area = " << fabs(cvContourArea(result,CV_WHOLE_SEQ)));
#endif

    // Check for 4 vertices
    // Check for a convex contour
    
    if( result->total == 4 &&
	cvCheckContourConvexity(result) &&
	fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000) {    
      CvPoint corners[4];
      cvCvtSeqToArray(result,corners,cvSlice(0,4));
#ifdef QUADTANGLE_DEBUG
      PROGRESS("Accepting polygon with points " <<
	       "("<<corners[0].x<<","<<corners[0].y<<") "<<
	       "("<<corners[1].x<<","<<corners[1].y<<") "<<
	       "("<<corners[2].x<<","<<corners[2].y<<") "<<
	       "("<<corners[3].x<<","<<corners[3].y<<")");

#endif

      m_x0 = (float)corners[0].x/1000.f;
      m_y0 = (float)corners[0].y/1000.f;
      m_x1 = (float)corners[1].x/1000.f;
      m_y1 = (float)corners[1].y/1000.f;
      m_x2 = (float)corners[2].x/1000.f;
      m_y2 = (float)corners[2].y/1000.f;
      m_x3 = (float)corners[3].x/1000.f;
      m_y3 = (float)corners[3].y/1000.f;
      sort_points();
      m_fitted = true;
    }
    else {
#ifdef QUADTANGLE_DEBUG
      PROGRESS("Rejecting polygon");
#endif
      m_fitted = false;
    }
  
    cvReleaseMemStorage(&seqstore);
    cvReleaseMemStorage(&store);
  }
  else {
    m_fitted = false;
  }
}

QuadTangle::QuadTangle(float x0, float y0,float x1, float y1,float x2, float y2,float x3, float y3) : m_x0(x0), m_y0(y0),m_x1(x1), m_y1(y1),m_x2(x2), m_y2(y2),m_x3(x3), m_y3(y3), m_fitted(true) {}

float dist(float x0, float y0, float x1, float y1) {
  return sqrt( (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1) );
}

bool QuadTangle::Compare(const QuadTangle& o) const {
  return ((dist(m_x0,m_y0,o.m_x0,o.m_y0) < COMPARE_THRESH) &&
	  (dist(m_x1,m_y1,o.m_x1,o.m_y1) < COMPARE_THRESH) &&
	  (dist(m_x2,m_y2,o.m_x2,o.m_y2) < COMPARE_THRESH) &&
	  (dist(m_x3,m_y3,o.m_x3,o.m_y3) < COMPARE_THRESH));
}

inline void QuadTangle::compute_central_point() {
  /*
   * Find the central point of this quadtangle (A,B,C,D)
   *
   *     A +--------------+ B
   *       |              |
   *       |              |
   *       |              |
   *       |              |
   *       |              |
   *       |              |
   *       |              |
   *     D +--------------+ C
   *
   *
   *       A + AC*a = B + BD*b
   *
   *    a*AC - b*BD = B - A
   *
   *  (ACx -BDx)(a) = (ABx) 
   *  (ACy -BDy)(b)   (ABy)
   *
   *            (a) = 1/( ACy*BDx - ACx*BDy ) * (-BDy  BDx)(ABx)
   *            (b)                             (-ACy  ACx)(ABy)
   *
   * Central point  = A+AC*a
   *
   */
  float Ax = m_x0;
  float Ay = m_y0;

  float Bx = m_x1;
  float By = m_y1;

  float Cx = m_x2;
  float Cy = m_y2;

  float Dx = m_x3;
  float Dy = m_y3;

  float ACx = Cx-Ax;
  float ACy = Cy-Ay;

  float BDx = Dx-Bx;
  float BDy = Dy-By;

  float ABx = Bx-Ax;
  float ABy = By-Ay;

  float b = (BDx*ABy - BDy*ABx)/(ACy*BDx - ACx*BDy);

  m_xc = Ax+ACx*b;
  m_yc = Ay+ACy*b;        
}


float QuadTangle::find_angle(float x, float y, float cx, float cy) {
  if ((x >= cx) && (y >= cy)) {
    return M_PI/2+ atan( (y-cy) / (x-cx) );
  }
  else if ((x >= cx) && (y < cy)) {
    return atan( (cy-y) / (x-cx) );
  }
  else if ((x < cx) && (y < cy)) {
    return 3*M_PI/2 + atan ( (cy-y) / (cx-x) );
  }
  else if ((x < cx) && (y >= cy)) {
    return M_PI + atan( (y-cy) / (cx-x));
  }
}

void QuadTangle::swap( float *a, float *b) {
  float t = *a;
  *a = *b;
  *b = t;
}

void QuadTangle::sort_points()
{
  // sort the points into clockwise order.
  float angles[4];
  angles[0] = find_angle(m_x0,m_y0,m_xc,m_yc);
  angles[1] = find_angle(m_x1,m_y1,m_xc,m_yc);
  angles[2] = find_angle(m_x2,m_y2,m_xc,m_yc);
  angles[3] = find_angle(m_x3,m_y3,m_xc,m_yc);
  //  PROGRESS("Centre (" << m_xc << "," << m_yc <<")");
  //  PROGRESS("Original order " << std::endl <<
  //	   "(" << m_x0 << "," << m_y0 << ") @ "<< angles[0] << std::endl <<
  //	   "(" << m_x1 << "," << m_y1 << ") @ "<< angles[1] << std::endl <<
  //	   "(" << m_x2 << "," << m_y2 << ") @ "<< angles[2] << std::endl <<
  //	   "(" << m_x3 << "," << m_y3 << ") @ "<< angles[3]);
  float tx,ty; 
  for(int i=0;i<4;i++) {
    if (angles[0] > angles[1]) {
      swap(&m_x0,&m_x1);
      swap(&m_y0,&m_y1);
      swap(&angles[0],&angles[1]);
    }

    if (angles[1] > angles[2]) {
      swap(&m_x1,&m_x2);
      swap(&m_y1,&m_y2);
      swap(&angles[1],&angles[2]);
    }

    if (angles[2] > angles[3]) {
      swap(&m_x2,&m_x3);
      swap(&m_y2,&m_y3);
      swap(&angles[2],&angles[3]);
    }
  }
  
  //  PROGRESS("Final order " << std::endl <<
  //	   "(" << m_x0 << "," << m_y0 << ") @ "<< angles[0] << std::endl <<
  //	   "(" << m_x1 << "," << m_y1 << ") @ "<< angles[1] << std::endl <<
  //	   "(" << m_x2 << "," << m_y2 << ") @ "<< angles[2] << std::endl <<
  //	   "(" << m_x3 << "," << m_y3 << ") @ "<< angles[3]);
}

  