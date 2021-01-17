#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>
using namespace std;

// Globals
#define MAX_BUFFER_SIZE     8981

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
/*
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }
*/
int i =0;
GLfloat Lt0pos[] = {1.0f, 1.0f, 5.0f, 1.0f};

// This function is responsible for displaying the object.
void drawScene(void)
{
    

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[i%4]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
    
    int a,c,d,f,g,i;
	glBegin(GL_TRIANGLES);
    for (unsigned int z = 0; z< vecf.size(); z++)
    {
        a = vecf[z][0];
        c = vecf[z][2];
        d = vecf[z][3];
        f = vecf[z][5];
        g = vecf[z][6];
        i = vecf[z][8];
        glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]);
        glVertex3d(vecv[a-1][0], vecv[a-1][1], vecv[a-1][2]);
        glNormal3d(vecn[f-1][0], vecn[f-1][1], vecn[f-1][2]);
        glVertex3d(vecv[d-1][0], vecv[d-1][1], vecv[d-1][2]);
        glNormal3d(vecn[i-1][0], vecn[i-1][1], vecn[i-1][2]);
        glVertex3d(vecv[g-1][0], vecv[g-1][1], vecv[g-1][2]);
        
    }
    glEnd();
    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    
    
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        i++;
        
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
     // Light position
	
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
        Lt0pos[1]+=0.5f;
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position
        Lt0pos[1]-=0.5f;
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position
        Lt0pos[0]-=0.5f;
		
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
        Lt0pos[0]+=0.5f;
		break;
    }
    //cout << "Light position: ("<< Lt0pos[0]<< ", " << Lt0pos[1]<< ", " <<Lt0pos[2]<< ", " <<Lt0pos[3] <<")"<< endl;
	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}


// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{
    char buffer[MAX_BUFFER_SIZE];
	// load the OBJ file here
    while(cin.getline(buffer, MAX_BUFFER_SIZE))
    {
        stringstream ss(buffer);
        Vector3f v;
        vector<unsigned> f;
        string s;
        string delimiter = "/";
        

        ss >> s;       
        if(s=="v")
        {
            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(v);
        }
        if(s=="vn")
        {
            ss >> v[0] >> v[1] >> v[2];
            vecn.push_back(v);
        }
        if(s=="f")
        {
            for (int i = 0; i < 3; i++)
            {
                ss >> s;
                size_t pos = 0;
                string token;
                while((pos = s.find(delimiter)) != std::string::npos)
                {
                    token = s.substr(0,pos);
                    f.push_back(stoi(token));
                    s.erase(0, pos + delimiter.length());
                }
                f.push_back(stoi(s));
            
            }
            vecf.push_back(f);
        }

    }
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();
    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
