/*
* cl.exe /c /EHsc /I "D:\\Code\\sukrutdatar\\RTR\\glew\\include" window.c
* link.exe window.obj user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
*/

// header file inclusion
#include <windows.h>    // windows header files
#include <stdio.h>      // for file IO
#include <stdlib.h>     // for exit

// openGL headers
#include <gl/glew.h>    // this must be before <gl/GL.h> inclusion
#include <gl/GL.h>

#include "OGL.h"
#include "vmath.h"
#include "..\include\Aakaar.h"
using namespace vmath;

// link with win32 library
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// link with openGL library
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")


#define LOG_MESSAGE(msg) LogMessage(__FILE__, __func__, __LINE__, msg)
#define LOG_ERROR(msg) LogErrorMessage(__FILE__, __func__, __LINE__, msg, GetLastError())

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations / prototypes / signatures
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LogMessage(const char *, const char *, const int, const char *);
void LogErrorMessage(const char *, const char *, const int, const char *, int);

// global variable declarations
FILE *gpFile = NULL;

HWND ghwnd = NULL;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;

// openGL related global variables
HDC ghdc = NULL;        // device context
HGLRC ghrc = NULL;      // rendering context

GLuint gShaderProgramObject = 0;
enum
{
    AMC_ATTRIBUTE_POSITION = 0
};

GLuint vao = 0;
GLuint vbo_position = 0;
GLuint mvpMatrixUniform = 0;
mat4 perspectiveProjectionMatrix;

float* cube_position = NULL;
GLfloat angleCube = 0.0f;

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT iCmdShow)
{
    // function declarations / signatures / prototype
    int initialize(void);
    void uninitialize(void);
    void display(void);
    void update(void);


    // variable declarations
    HWND hwnd;
    WNDCLASSEX wndClass;
    MSG msg;
    TCHAR szAppName[] = TEXT("SSDWindow");
    int iResult = 0;
    BOOL bDone = FALSE;

    const int iWindowWidth = 800;
    const int iWindowHeight = 600;
    const int iWindowCenterX = iWindowWidth / 2;
    const int iWindowCenterY = iWindowHeight / 2;

    const int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    const int iScreenCenterX = (iScreenWidth / 2) - iWindowCenterX;
    const int iScreenCenterY = (iScreenHeight / 2) - iWindowCenterY;

    // code

    if (fopen_s(&gpFile, "log.txt", "w") != 0)
    {
        MessageBox(NULL, TEXT("Log file cannot be opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
        exit(0);
    }

    // WNDCLASSEX initialization
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY_INITIAL_ICON));
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpszClassName = szAppName;
    wndClass.lpszMenuName = NULL;
    wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY_INITIAL_ICON));

    // Register WNDCLASSEX
    RegisterClassEx(&wndClass);

    // Create window
    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                        szAppName,
                        TEXT("Aakaar library client"),
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                        iScreenCenterX,
                        iScreenCenterY,
                        WIN_WIDTH,
                        WIN_HEIGHT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Failed to create window"), TEXT("Window error"), MB_OK);
        exit(0);
    }

    ghwnd = hwnd;

    // initialization
    iResult = initialize();
    if (iResult != 0)
    {
        MessageBox(hwnd, TEXT("initialize() failed."), TEXT("Error"), MB_OK | MB_ICONERROR);
        DestroyWindow(hwnd);
    }

    // show the window
    ShowWindow(hwnd, iCmdShow);

    SetForegroundWindow(hwnd);

    SetFocus(hwnd);

    LOG_MESSAGE("Window created, starting message loop.");

    // game loop: heart of the program
    while (bDone == FALSE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                LOG_MESSAGE("WM_QUIT received, exiting game loop.");
                bDone = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (gbActiveWindow == TRUE)
            {
                // render
                display();

                // update
                update();
            }
        }
    }

    // un-initialization
    uninitialize();

    return ((int)msg.wParam);
}

// function definitions
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // function declarations / prototype / signature
    void ToggleFullScreen(void);
    void resize(int, int);

    // code
    switch (msg)
    {
        case WM_CREATE:
            LOG_MESSAGE("WM_CREATE received.");
            break;
        case WM_LBUTTONDOWN:
            LOG_MESSAGE("Mouse left button pressed.");
            break;

        case WM_KEYDOWN:
            switch (LOWORD(wParam))
            {
                case VK_ESCAPE:
                    DestroyWindow(hwnd);
                    break;
            }
            break;

        case WM_CHAR:
            switch(LOWORD(wParam))
            {
                case 'F':
                case 'f':
                    if (gbFullScreen == FALSE)
                    {
                        ToggleFullScreen();
                        gbFullScreen = TRUE;
                        LOG_MESSAGE("Going to full screen mode");
                    }
                    else
                    {
                        ToggleFullScreen();
                        gbFullScreen = FALSE;
                        LOG_MESSAGE("Exiting full screen mode");
                    }
                    break;
            }
            break;

        case WM_DESTROY:
            LOG_MESSAGE("WM_DESTROY received, posting WM_QUIT message.");
            PostQuitMessage(0);
            break;

        case WM_SETFOCUS:
            gbActiveWindow = TRUE;
            break;

        case WM_KILLFOCUS:
            gbActiveWindow = FALSE;
            break;

        case WM_SIZE:
            resize(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_ERASEBKGND:
            return (0);
    }

    return (DefWindowProc(hwnd, msg, wParam, lParam));
}

void LogMessage(const char* file, const char* function, const int lineNumber, const char *logEntry)
{
    if (gpFile)
    {
        fprintf(gpFile, "[%s:%s][%s::%s(%d)] %s\n", __DATE__, __TIME__, file, function, lineNumber, logEntry);
        fflush(gpFile);
    }
}

void LogErrorMessage(const char* file, const char* function, const int lineNumber, const char *logEntry, int errorCode)
{
    if (gpFile)
    {
        fprintf(gpFile, "[%s:%s][%s::%s(%d)] %s: %d\n", __DATE__, __TIME__, file, function, lineNumber, logEntry, errorCode);
        fflush(gpFile);
    }
}

void ToggleFullScreen(void)
{
    // variable declarations
    MONITORINFO mi = {sizeof(MONITORINFO)};

    // code
    if (gbFullScreen == FALSE)
    {
        // get current window style
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

        // check if style has WS_OVERLAPPEDWINDOW
        if (dwStyle & WS_OVERLAPPEDWINDOW)
        {
            // get current window placement and save it
            // MONITORINFOF_PRIMARY is same as MONITOR_DEFAULTTOPRIMARY
            if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
            {
                // remove WS_OVERLAPPEDWINDOW from style
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,
                            HWND_TOP,
                            mi.rcMonitor.left,                          // left top of screen (x)
                            mi.rcMonitor.top,                           // left top of screen (y)
                            mi.rcMonitor.right - mi.rcMonitor.left,     // width of the window in pixels
                            mi.rcMonitor.bottom - mi.rcMonitor.top,     // heigth of the window in pixels
                            SWP_NOZORDER | SWP_FRAMECHANGED);           // parent z order var parinam hou deu nakos and majhya pan | frame screen chya baher taak (WM_NCCALCSIZE msg send karto)

                // hide the cursor
                ShowCursor(FALSE);
            }
        }
    }
    else
    {
        // add WS_OVERLAPPEDWINDOW in the window style
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

        // set window placement back to it's original location
        SetWindowPlacement(ghwnd, &wpPrev);

        // SetWindowPos() has highest priorty (more than SetWindowPlacement() and style) -- z-order
        // SetWindowPlacement() has greater priority than SetWindowPos() in case of setting x, y, width, height
        SetWindowPos(ghwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

        // show cursor again
        ShowCursor(TRUE);
    }
}

int initialize(void)
{
    // function declarations / prototype / signature
    void resize(int, int);
    void printGLInfo(void);
    void uninitialize();

    // variable declarations
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;

    // code

    // STEP 1: initialization of PIXELFORMATDESCRIPTOR
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    // draw to window or printer or bitmap |
    // draw using openGL (real time, immediate mode) and not retained mode
    // asa pfd ahe jo DOUBLE_BUFFER la support karel
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

    pfd.iPixelType = PFD_TYPE_RGBA;

    // Pixel type RGBA ahe, so each color la kiti bit vaparaiche?
    // tar total 32 bit vapar, mhanje
    // R = 8 bit, G = 8 bit, B = 8 bit, Alpha = 8 bit
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    // for depth
    pfd.cDepthBits = 32;


    // STEP 2: get the handle to device context for pixel format descriptor selection in next step
    // BeginPaint() ni dilela DC fakta WM_PAINT sathi ch vaaparu shakto
    // mhanun GetDC(), ni milalela DC kontya pan kama sathi vaparta yeto
    ghdc = GetDC(ghwnd);
    if (ghdc == NULL)
    {
        LOG_ERROR("Failed to get device context, GetDC() failed.");
        return -1;
    }

    // STEP 3: get best matching pixel format descriptor index from OS
    // which resembles to our pfd
    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
    if (iPixelFormatIndex == 0)
    {
        LOG_ERROR("Failed to get pixel format description index, ChoosePixelFormat() failed.");
        return -2;
    }

    // STEP 4: set obtained pixel format to the device context
    if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
    {
        LOG_ERROR("Failed to set pixel format, SetPixelFormat() failed.");
        return -3;
    }

    // STEP 5: tell Windows Graphics Library (WGL) (Bridging Library) to get openGL compatible context from this device context (ghdc)
    ghrc = wglCreateContext(ghdc);
    if (ghrc == NULL)
    {
        LOG_ERROR("Failed to get rendering context, wglCreateContext() failed.");
        return -4;
    }

    // from here control will be transfered from device context to rendering context
    // make the rendering context current
    if (wglMakeCurrent(ghdc, ghrc) == FALSE)
    {
        LOG_ERROR("Failed to make rendering context current, wglMakeCurrent() failed.");
        return -5;
    }


    // initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        LOG_ERROR("Failed to initialize GLEW.\n");
        fprintf(gpFile, "Error: %s\n", glewGetErrorString(err));
        return -6;
    }

    // print GL information
    //printGLInfo();

    // vertex shader
    const GLchar* vertexShaderSourceCode =
        "#version 460 core"             \
        "\n"                            \
        "in vec4 aPosition;"            \
        "uniform mat4 uMVPMatrix;"      \
        "void main(void)"               \
        "{"                             \
        "gl_Position=uMVPMatrix * aPosition;"        \
        "}";

    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

    glCompileShader(vertexShaderObject);
    
    GLint status = 0;
    GLint infoLogLength = 0;
    GLchar* szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
                fprintf(gpFile, "Vertex shader compilation failed: %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        glDeleteShader(vertexShaderObject);
        return -7;
    }

    // fragment shader
    const GLchar* fragmentShaderSourceCode =
        "#version 460 core"             \
        "\n"                            \
        "out vec4 FragColor;"           \
        "void main(void)"               \
        "{"                             \
        "FragColor = vec4(1.0, 1.0, 1.0, 1.0);"           \
        "}";

    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

    glCompileShader(fragmentShaderObject);

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
                fprintf(gpFile, "Fragment shader compilation failed: %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        glDeleteShader(vertexShaderObject);
        glDeleteShader(fragmentShaderObject);
        return -8;
    }

    // shader program
    gShaderProgramObject = glCreateProgram();

    glAttachShader(gShaderProgramObject, vertexShaderObject);
    glAttachShader(gShaderProgramObject, fragmentShaderObject);

    // bind attribute location (pre-linking binding)
    glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

    // link shader program
    glLinkProgram(gShaderProgramObject);
    glDeleteShader(vertexShaderObject);
    glDeleteShader(fragmentShaderObject);

    glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(gShaderProgramObject, infoLogLength, NULL, szInfoLog);
                fprintf(gpFile, "Shader linking failed: %s\n", szInfoLog);
                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        return -9;
    }

    // get shader uniform locations
    // must be done after linking
    mvpMatrixUniform = glGetUniformLocation(gShaderProgramObject, "uMVPMatrix");

    // declare position and color arrays
    if (GetCubeVertices(&cube_position) == AAKAAR_FAILED) {
      LOG_ERROR("\nFailed to get cube vertices.\n");
      return -10;
    }

    LOG_MESSAGE("Fetched cube vertices successfully from the aakaar library.");

    // VAO - vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO (vertex buffer object) for position
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, CUBE_VERTICES_SIZE,
                 cube_position,
                 GL_STATIC_DRAW);

    // cube_position[] hya array varna, 3 vertices create kar, mhanun parameter 2 ha ahe 3
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    // unbind with vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind with vao
    glBindVertexArray(0);

    // jasa glClearColor() apan dilelya colour ni ColourBuffer clear karto
    // tasa, glClearDepth() dileli value, DepthBuffer madhlya saglya bits la set kar
    glClearDepth(1.0f);

    // state machine madhye rasterizer ha kahi tests execute karto, tyatli ek aste depth test
    // so ti test enable karavi laagte
    glEnable(GL_DEPTH_TEST);

    // depth test cha konta function enable karu? mhanje vaapru testing sathi
    // GL_LEQUAL mhanje less than or equal to - he function vapar
    // mhanje ashya potential pixel cha fragment banav, jyache pixel che z-coordinates (depth) hi dilelya
    // glClearDepth() madhlya value peksha less than or equal to ahe
    glDepthFunc(GL_LEQUAL);


    // set the clear color of the window to blue
    // Retained mode (RGB) --> (0 to 255) --> 256 color range --> 2^8
    // Immediate mode (RGB) --> (0 to 1.0) --> 2^32 range
    // mhanje STEP 1 madhe dilele pfd.cColorBits
    // alpha --> 1.0f mhanje opaque nahiye
    // here openGL starts
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // initialize perspective project matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();

    resize(WIN_WIDTH, WIN_HEIGHT);

    return (0);
}

void printGLInfo(void)
{
    // variable declarations
    GLint numExtensions;
    GLint i;

    // code
    fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // listing of supported extensions
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for (i = 0; i < numExtensions; i++)
    {
        fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
    }
}

void resize(int width, int height)
{
    // code

    if (height <= 0)
    {
        height = 1;
    }

    // window chya kontya area var focus karaicha?
    // tar atta majha focus purna window var ahe, (0 ,0) to width and height
    // viewport mhanje durbin (binocular)
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);


    // set perspective project matrix
    perspectiveProjectionMatrix = vmath::perspective(
        45.0f,
        (GLfloat)width / (GLfloat)height,
        0.1f,
        100.f);
}

void display(void)
{
    // code

    // window la clear color ni saaf kar
    // ha clear color apan initialize madhe set kela ahe
    // depth value hi initialize madhe set keli ahe
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gShaderProgramObject);

    // tranformation
    mat4 modelViewMatrix = vmath::mat4::identity();
    mat4 translationMatrix = vmath::translate(0.0f, 0.0f, -8.0f);
    mat4 rotationMatrix = vmath::mat4::identity();
    mat4 rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
    mat4 rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
    mat4 rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

    rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
    modelViewMatrix = translationMatrix * rotationMatrix;

    mat4 modelViewProjectionMatrix = vmath::mat4::identity();
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
    
    // push above model view projection matrix into vertex shader's uniform location
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(vao);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    glBindVertexArray(0);
    glUseProgram(0);

    // double buffering madhe ahe apan
    // tar OS, buffer swap kar
    // pan konte buffer?
    SwapBuffers(ghdc);
}

void update(void)
{
    // code

    angleCube = angleCube + 0.5f;
    if (angleCube >= 360.0f) {
      angleCube = angleCube - 360.0f;
    }
}

void uninitialize(void)
{
    // function declarations / prototype / signature
    void ToggleFullScreen(void);

    // code

    // uninitialize shader object and shader program
    if (gShaderProgramObject)
    {
        glUseProgram(gShaderProgramObject);

        GLint numShaders = 0;
        glGetProgramiv(gShaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
        
        if (numShaders > 0)
        {
            GLuint* pShaders = (GLuint*)malloc(sizeof(GLuint) * numShaders);
            if (pShaders != NULL)
            {
                glGetAttachedShaders(gShaderProgramObject, numShaders, NULL, pShaders);
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(gShaderProgramObject, pShaders[i]);
                    glDeleteShader(pShaders[i]);
                    pShaders[i] = 0;
                }

                free(pShaders);
                pShaders = NULL;
            }
        }

        glDeleteProgram(gShaderProgramObject);
        gShaderProgramObject = 0;
    }

    // delete VBO (vertex buffer object) of position
    if (cube_position) {
      free(cube_position);
      cube_position = NULL;
    }

    if (vbo_position)
    {
        glDeleteBuffers(1, &vbo_position);
        vbo_position = 0;
    }

    // delete VAO (vertex array object)
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    // make the hdc as current DC if current context is ghrc
    if (wglGetCurrentContext() == ghrc)
    {
        wglMakeCurrent(NULL, NULL);
    }

    // destroy / delete rendering context
    if (ghrc)
    {
        wglDeleteContext(ghrc);
        ghrc = NULL;
    }

    // release the HDC
    if (ghdc)
    {
        ReleaseDC(ghwnd, ghdc);
        ghdc = NULL;
    }

    // log file close kara
    if (gpFile)
    {
        fclose(gpFile);
        gpFile = NULL;
    }

    if (gbFullScreen == TRUE)
    {
        // if application is exiting in full screen
        DestroyWindow(ghwnd);
    }
}
