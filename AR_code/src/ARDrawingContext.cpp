/*****************************************************************************
*   Markerless AR desktop application.
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

////////////////////////////////////////////////////////////////////
// File includes:
#include "ARDrawingContext.hpp"

////////////////////////////////////////////////////////////////////
// Standard includes:
// #include <gl/gl.h>
// #include <gl/glu.h>
#include <gl.h>
#include <glu.h>
#include <cmath>
void ARDrawingContextDrawCallback(void* param)
{
    ARDrawingContext * ctx = static_cast<ARDrawingContext*>(param);
    if (ctx)
    {
        ctx->draw();
    }
}

ARDrawingContext::ARDrawingContext(std::string windowName, cv::Size frameSize, const CameraCalibration& c)
  : m_isTextureInitialized(false)
  , m_calibration(c)
  , m_windowName(windowName)
{
    this->turn = 0;//转向变量
    // Create window with OpenGL support
    cv::namedWindow(windowName, cv::WINDOW_OPENGL);

    // Resize it exactly to video size
    cv::resizeWindow(windowName, frameSize.width, frameSize.height);

    // Initialize OpenGL draw callback:
    cv::setOpenGlContext(windowName);
    cv::setOpenGlDrawCallback(windowName, ARDrawingContextDrawCallback, this);
}

ARDrawingContext::~ARDrawingContext()
{
    cv::setOpenGlDrawCallback(m_windowName, 0, 0);
}

void ARDrawingContext::updateBackground(const cv::Mat& frame)
{
  frame.copyTo(m_backgroundImage);
}

void ARDrawingContext::updateWindow()
{
    cv::updateWindow(m_windowName);
}

void ARDrawingContext::draw()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear entire screen:
  drawCameraFrame();                                  // Render background
  drawAugmentedScene();                               // Draw AR
  glFlush();
}


void ARDrawingContext::drawCameraFrame()
{
  // Initialize texture for background image
  if (!m_isTextureInitialized)
  {
    glGenTextures(1, &m_backgroundTextureId);
    glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_isTextureInitialized = true;
  }

  int w = m_backgroundImage.cols;
  int h = m_backgroundImage.rows;

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);

  // Upload new texture data:
  if (m_backgroundImage.channels() == 3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_backgroundImage.data);
  else if(m_backgroundImage.channels() == 4)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_backgroundImage.data);
  else if (m_backgroundImage.channels()==1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_backgroundImage.data);

  const GLfloat bgTextureVertices[] = { 0, 0, w, 0, 0, h, w, h };
  const GLfloat bgTextureCoords[]   = { 1, 0, 1, 1, 0, 0, 0, 1 };
  const GLfloat proj[]              = { 0, -2.f/w, 0, 0, -2.f/h, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1 };

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(proj);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);

  // Update attribute values.
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, bgTextureVertices);
  glTexCoordPointer(2, GL_FLOAT, 0, bgTextureCoords);

  glColor4f(1,1,1,1);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_TEXTURE_2D);
}

void ARDrawingContext::drawAugmentedScene()
{
  // Init augmentation projection
  Matrix44 projectionMatrix;
  int w = m_backgroundImage.cols;
  int h = m_backgroundImage.rows;
  buildProjectionMatrix(m_calibration, w, h, projectionMatrix);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projectionMatrix.data);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (isPatternPresent)
  {
    // Set the pattern transformation
    Matrix44 glMatrix = patternPose.getMat44();
    glLoadMatrixf(reinterpret_cast<const GLfloat*>(&glMatrix.data[0]));

    // Render model
    drawCoordinateAxis();
    drawCubeModel();
  }
}

void ARDrawingContext::buildProjectionMatrix(const CameraCalibration& calibration, int screen_width, int screen_height, Matrix44& projectionMatrix)
{
  float nearPlane = 0.01f;  // Near clipping distance
  float farPlane  = 100.0f;  // Far clipping distance

  // Camera parameters
  float f_x = calibration.fx(); // Focal length in x axis
  float f_y = calibration.fy(); // Focal length in y axis (usually the same?)
  float c_x = calibration.cx(); // Camera primary point x
  float c_y = calibration.cy(); // Camera primary point y

  projectionMatrix.data[0] = -2.0f * f_x / screen_width;
  projectionMatrix.data[1] = 0.0f;
  projectionMatrix.data[2] = 0.0f;
  projectionMatrix.data[3] = 0.0f;

  projectionMatrix.data[4] = 0.0f;
  projectionMatrix.data[5] = 2.0f * f_y / screen_height;
  projectionMatrix.data[6] = 0.0f;
  projectionMatrix.data[7] = 0.0f;

  projectionMatrix.data[8] = 2.0f * c_x / screen_width - 1.0f;
  projectionMatrix.data[9] = 2.0f * c_y / screen_height - 1.0f;    
  projectionMatrix.data[10] = -( farPlane + nearPlane) / ( farPlane - nearPlane );
  projectionMatrix.data[11] = -1.0f;

  projectionMatrix.data[12] = 0.0f;
  projectionMatrix.data[13] = 0.0f;
  projectionMatrix.data[14] = -2.0f * farPlane * nearPlane / ( farPlane - nearPlane );        
  projectionMatrix.data[15] = 0.0f;
}


void ARDrawingContext::drawCoordinateAxis()//绘制坐标轴
{
  static float lineX[] = {0,0,0,1,0,0};
  static float lineY[] = {0,0,0,0,1,0};
  static float lineZ[] = {0,0,0,0,0,1};

  glLineWidth(2);

  glBegin(GL_LINES);

//  glColor3f(1.0f, 0.0f, 0.0f);
//  glVertex3fv(lineX);
//  glVertex3fv(lineX + 3);
//
//  glColor3f(0.0f, 1.0f, 0.0f);
//  glVertex3fv(lineY);
//  glVertex3fv(lineY + 3);
//
//  glColor3f(0.0f, 0.0f, 1.0f);//
//  glVertex3fv(lineZ);
//  glVertex3fv(lineZ + 3);

  glEnd();
}

void ARDrawingContext::drawCubeModel() {
  static const GLfloat LightAmbient[] = {0.25f, 0.25f, 0.25f, 1.0f};    // Ambient Light Values
  static const GLfloat LightDiffuse[] = {0.1f, 0.1f, 0.1f, 1.0f};    // Diffuse Light Values
  static const GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};    // Light Position

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT);

  glColor4f(0.4f, 0.8f, 1.0f, 0.5f);         // Full Brightness, 50% Alpha ( NEW )背景颜色
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);       // Blending Function For Translucency Based On Source Alpha
  glEnable(GL_BLEND);

  glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  glEnable(GL_COLOR_MATERIAL);

  glScalef(0.25, 0.25, 0.25);
  glTranslatef(0, 0, 1);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//画面
  //glBegin(GL_QUADS);

  /**一般箭头**/
//  glBegin(GL_POLYGON);
//  glNormal3f(0.0f,0.0f,1.0f);
//  glVertex3f(-1.0f,0.0f,-2.0f);
//  glVertex3f(-3.0f,0.0f,-2.0f);
//  glVertex3f(0.0f,3.0f,-2.0f);
//  glVertex3f(3.0f,0.0f,-2.0f);
//  glVertex3f(1.0f,0.0f,-2.0f);
//  glVertex3f(1.0f,-4.0f,-2.0f);
//  glVertex3f(-1.0f,-4.0f,-2.0f);
  // Front Face
//  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
  // Back Face贴近于平面的面
//  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
  // Top Face
//  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
  // Bottom Face
//  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
  // Right face
//  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
  // Left Face
//  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
  /*改进箭头*/

//  float ax[10]={1,1,1.1,1.,1.12,1.14,1.2,1.4,1.7,3};
//  float ay[10]={1,2,3,4,5,6,7,8,9,10};
//  float bx[10]={-1,-1,-0.9,-0.89,-0.88,-0.86,-0.8,-0.6,-0.3,-1};
//  float by[10]={1,2,3,4.5,6,7.5,9.5,11,13,16};

  /* int j for test*/

 //int j = 5;

  float ax[6][5]={{1,1,1,1,1},
                  {1,1.2,1.4,1.7,2.1},
                  {1,0.875,0.5,0.2,-0.4},
                  {1,1.05,1.1,1.4,1.8},
                  {1,0.96,0.92,0.85,0.7},
                  {1,0.0,0.5,0.0,-0.5}};
  float ay[6][5]={{1,2,3,4,5},
                  {1,2,3,3.8,4.6},
                  {1,1.8,2.6,3.5,4},
                  {1,1.25,1.65,1.75,2},
                  {1,2.1,3.2,4.3,5.4},
                  {1,0.0,1.9,0.0,2.5}};
  float bx[6][5]={{-1,-1,-1,-1,-1},
                  {-1,-0.8,-0.6,-0.1,0.5},
                  {-1,-1.075,-1.1,-1.3,-1.4},
                  {-1,-0.4,0.4,1.2,1.8},
                  {-1,-1.04,-1.08,-1.13,-1.3},
                  {-1,0.0,-1.05,0.0,-1.2}};
  float by[6][5]={{1,2,3,4,5},
                  {1,2.2,3.5,4.5,5.4},
                  {1,1.55,2.2,2.6,3},
                  {1,2.2,3,3.8,4},
                  {1,1.9,2.8,3.6,4.6},
                  {1,0.0,1.4,0.0,2}};
  for(int i=0;i<5;i++){
    drawArrow(ax[this->turn][i],ay[this->turn][i],bx[this->turn][i],by[this->turn][i],this->turn);
  }
//  for (int i = 0; i < 10; i++) {
//    glBegin(GL_POLYGON);
//    glVertex3f(0.0f, 0.5f + 1.5 * i, 0.0f);
//    glNormal3f(0.0f, 1.0f, 0.0f);//放在第一个下面有奇效
//    glVertex3f(-0.5f, 0.0f + 1.5 * i, 0.0f);
//    glVertex3f(-1.0f, 0.0f + 1.5 * i, 0.0f);
//    glVertex3f(0.0f, 1.0f + 1.5 * i, 0.0f);
//    glVertex3f(1.0f, 0.0f + 1.5 * i, 0.0f);
//    glVertex3f(0.5f, 0.0f + 1.5 * i, 0.0f);
//    glEnd();
//  }

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//画线
//  glColor4f(0.0f,0.65f,0.3f,0.0f); // Full Brightness, 50% Alpha ( NEW )-- (0.2f,0.65f,0.3f,0.35f)隐藏线
//  glBegin(GL_QUADS);
//  // Front Face
//  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
//  // Back Face
//  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
//  // Top Face
//  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
//  // Bottom Face
//  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
//  // Right face
//  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
//  // Left Face
//  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
//  glEnd();

  glPopAttrib();
}
void ARDrawingContext::drawArrow(float ax,float ay,float bx,float by,int turn){
  if(ax!=0.0f||ay!=0.0f||bx!=0.0f||by!=0.0f){
  glBegin(GL_POLYGON);
  float mx = (ax+bx)/2;
  float my=(ay+by)/2;
  float ky=sqrt((ax-bx)*(ax-bx)/((ax-bx)*(ax-bx)+(ay-by)*(ay-by)));
  float kx=sqrt((ay-by)*(ay-by)/((ax-bx)*(ax-bx)+(ay-by)*(ay-by)));
  if(ay>by){
    kx=-kx;
  }
  float x[6][6]={{0.5,-0.5,-1.0,1.0,1.0,0.5},
                 {0.5,-0.5,-1.0,1.0,1.0,0.5},
                 {0.25,-0.25,-0.5,0.5,0.5,0.25},
                 {0.25,-0.25,-0.5,0.5,0.5,0.25},
                 {0.5,-0.5,-1.0,1.0,1.0,0.5},
                 {0.25,-0.25,-0.5,0.5,0.5,0.25}};
  float y[6][6]={{0.5,0.5,1.0,1.0,-1.0,-0.5},
                 {0.5,0.5,1.0,1.0,-1.0,-0.5},
                 {0.25,0.25,0.5,0.5,-0.5,-0.25},
                 {0.25,0.25,0.5,0.5,-0.5,-0.25},
                 {0.5,0.5,1.0,1.0,-1.0,-0.5},
                 {0.25,0.25,0.5,0.5,-0.5,-0.25}};
  printf("mx: %f\nmy: %f\n:ky: %f\nkx: %f",mx,my,ky,kx);
//  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f(mx+x[turn][0]*kx,my+y[turn][0]*ky, 0.0f);
  glNormal3f(0.0f, 1.0f, 0.0f);//放在第一个下面有奇效
  glVertex3f(mx+x[turn][1]*ky,my+y[turn][1]*kx, 0.0f);
  glVertex3f(mx+x[turn][2]*ky,my+y[turn][2]*kx, 0.0f);
  glVertex3f(mx+x[turn][3]*kx,my+y[turn][3]*ky, 0.0f);
  glVertex3f(mx+x[turn][4]*ky,my+y[turn][4]*kx, 0.0f);
  glVertex3f(mx+x[turn][5]*ky,my+y[turn][5]*kx, 0.0f);
//    glVertex3f(-0.5f, 0.0f, 0.0f);
//    glVertex3f(-1.0f, 0.0f, 0.0f);
//    glVertex3f(0.0f, 1.0f, 0.0f);
//    glVertex3f(1.0f, 0.0f, 0.0f);
//    glVertex3f(0.5f, 0.0f, 0.0f);
  glEnd();
  }
}