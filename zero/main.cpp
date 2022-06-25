#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>
using namespace std;

struct Face {
    unsigned a,b,c,d,e,f,g,h,i;

    Face(
        vector<unsigned> point1, 
        vector<unsigned> point2,
        vector<unsigned> point3) {
            a = point1[0];
            b = point1[1];
            c = point1[2];
            d = point2[0];
            e = point2[1];
            f = point2[2];
            g = point3[0];
            h = point3[1];
            i = point3[2];
        }

    void draw(vector<Vector3f> vecv, vector<Vector3f> vecn) {
        glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]);
        glVertex3d(vecv[a-1][0], vecv[a-1][1], vecv[a-1][2]);
        glNormal3d(vecn[f-1][0], vecn[f-1][1], vecn[f-1][2]);
        glVertex3d(vecv[d-1][0], vecv[d-1][1], vecv[d-1][2]);
        glNormal3d(vecn[i-1][0], vecn[i-1][1], vecn[i-1][2]);
        glVertex3d(vecv[g-1][0], vecv[g-1][1], vecv[g-1][2]);

    }
};

// Globals
const char Space = ' ';
const char Slash = '/';

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<Face> vecf;

int colorIndex = 0;
float lightXOffset, lightYOffset = 0.0f;

// You will need more global variables to implement color and position changes

// These are convenience functions which allow us to call OpenGL
// methods on Vec3d objects
inline void glVertex(const Vector3f &a)
{
    glVertex3f(a.x(), a.y(), a.z());
}

inline void glNormal(const Vector3f &a)
{
    glNormal3f(a.x(), a.y(), a.z());
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        colorIndex += 1;
        glutPostRedisplay();
        cout << "Changing color" << endl;
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;
    }

    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // add code to change light position
        cout << "Unhandled key press: up arrow." << endl;
        lightYOffset += 0.5;
        break;
    case GLUT_KEY_DOWN:
        // add code to change light position
        cout << "Unhandled key press: down arrow." << endl;
        lightYOffset -= 0.5;
        break;
    case GLUT_KEY_LEFT:
        // add code to change light position
        cout << "Unhandled key press: right arrow." << endl;
        lightXOffset -= 0.5;
        break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
        cout << "Unhandled key press: left arrow." << endl;
        lightXOffset += 0.5;
        break;
    }

    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

void drawShape()
{
    glBegin(GL_TRIANGLES);
    for (auto &face : vecf) // access by reference to avoid copying
    {  
        face.draw(vecv, vecn);
    }
    glEnd();
}

// This function is responsible for displaying the object.
void drawScene(void)
{

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
    glLoadIdentity();           // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

    // Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = {{0.5, 0.5, 0.9, 1.0},
                                {0.9, 0.5, 0.5, 1.0},
                                {0.5, 0.9, 0.3, 1.0},
                                {0.3, 0.8, 0.9, 1.0}};

    // Here we use the first color entry as the diffuse color
    int colorIndexChoosen = colorIndex % 4;
    GLfloat *color = diffColors[colorIndexChoosen];
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0, 1.0, 1.0, 1.0};
    // Light position
    GLfloat Lt0pos[] = {1.0f + lightXOffset, 1.0f + lightYOffset, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    // This GLUT method draws a teapot.  You should replace
    // it with code which draws the object you loaded.
    drawShape();
    

    // Dump the image to the screen.
    glutSwapBuffers();
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST); // Depth testing must be turned on
    glEnable(GL_LIGHTING);   // Enable lighting calculations
    glEnable(GL_LIGHT0);     // Turn on light #0.
}

void setViewPort(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h)
    {
        glViewport((w - h) / 2, 0, h, h);
    }
    else
    {
        glViewport(0, (h - w) / 2, w, w);
    }
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    setViewPort(w, h);

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

Vector3f createVertice(string xCoordinateString, string yCoordinateString, string zCoordinateString)
{
    float xCoordinate = stof(xCoordinateString);
    float yCoordinate = stof(yCoordinateString);
    float zCoordinate = stof(zCoordinateString);
    Vector3f newVertice(xCoordinate, yCoordinate, zCoordinate);
    return newVertice;
}

Vector3f createVertice(string data)
{
    size_t indexOfSpace = data.find(Space);
    string xCoordinateString = data.substr(0, indexOfSpace);
    string yzString = data.substr(indexOfSpace + 1);
    size_t index2OfSpace = yzString.find(Space);
    string yCoordinateString = yzString.substr(0, index2OfSpace);
    string zCoordinateString = yzString.substr(index2OfSpace + 1);
    Vector3f newVertice = createVertice(xCoordinateString, yCoordinateString, zCoordinateString);
    return newVertice;
}

void parseVertice(string data) {
    Vector3f newVertice = createVertice(data);
    vecv.push_back(newVertice);
}

void parseNormal(string data)
{
    Vector3f newVertice = createVertice(data);
    vecn.push_back(newVertice);
}

vector<unsigned> createFacePoint(string data) {
    size_t indexOfSlash = data.find(Slash);
    size_t indexOfSlash2 = data.find_last_of(Slash);
    string index1String = data.substr(0, indexOfSlash);
    string index2String = data.substr(indexOfSlash + 1, indexOfSlash2 - indexOfSlash - 1);
    string index3String = data.substr(indexOfSlash2 + 1);
    unsigned index1 = stoi(index1String);
    unsigned index2 = stoi(index2String);
    unsigned index3 = stoi(index3String);

    vector<unsigned> newPoint;
 
    newPoint.push_back(index1);
    newPoint.push_back(index2);
    newPoint.push_back(index3);

    return newPoint;
}

Face parseFace(string data)
{
    size_t indexOfSpace = data.find(Space);
    size_t indexOfSpace2 = data.find_last_of(Space);
    string point1String = data.substr(0, indexOfSpace);
    string point2String = data.substr(indexOfSpace + 1, indexOfSpace2 - indexOfSpace - 1);
    string point3String = data.substr(indexOfSpace2 + 1);
    vector<unsigned> point1 = createFacePoint(point1String);
    vector<unsigned> point2 = createFacePoint(point2String);
    vector<unsigned> point3 = createFacePoint(point3String);
    Face newFace(point1, point2, point3);
    return newFace;
}

void parseLine(string line)
{
    size_t lineLength = line.length();
    bool lineTooSmall = lineLength <= 1;
    if (lineTooSmall)
    {
        return;
    }
    size_t indexOfSpace = line.find(Space);

    string prefix = line.substr(0, indexOfSpace);
    string data = line.substr(indexOfSpace + 1);
    bool isAVertice = prefix == "v";
    bool isANormal = prefix == "vn";
    bool isAFace = prefix == "f";
    if (isAVertice)
    {
        parseVertice(data);
    }
    else
    {
        if (isANormal)
        {
            parseNormal(data);
        }
        else
        {
            if (isAFace)
            {
                Face newFace = parseFace(data);
                vecf.push_back(newFace);
            }
            else
            {
                cout << "Type o line is unknown line: " << line << "\n";
            }
        }
    }
}

void readModel(FILE *filePointer)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, filePointer)) != -1)
    {
        string lineString(line);
        parseLine(lineString);
    }
    if (line)
    {
        free(line);
    }
}

void loadInput(char *fileName)
{
    FILE *filePointer = fopen(fileName, "r");
    bool couldNotFindFile = filePointer == NULL;
    if (couldNotFindFile)
    {
        cout << "Could not find the file " << fileName << "\n";
        return;
    }
    readModel(filePointer);
    fclose(filePointer);
}

char *getFileName(int argc, char **argv)
{
    if (argc < 2)
    {
        return NULL;
    }
    char *fileName = argv[1];
    return fileName;
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char **argv)
{
    char *fileName = getFileName(argc, argv);
    if (fileName)
    {
        string message("Is going to open the file:");
        message.append(fileName);
        cout << message << "\n";
    }
    else
    {
        cerr << "Impossible obtain fileName of the object \n";
        cerr << "Sintax: a0 fileName.obj \n";
        return 0;
    }
    loadInput(fileName);

    glutInit(&argc, argv);

    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(360, 360);
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up the callback function for resizing windows
    glutReshapeFunc(reshapeFunc);

    // Call this whenever window needs redrawing
    glutDisplayFunc(drawScene);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0; // This line is never reached.
}
