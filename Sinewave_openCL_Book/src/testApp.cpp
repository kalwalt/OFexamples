#include "testApp.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>

#ifdef __GNUC__
#include <GL/glx.h>
#endif


GLuint vbo = 0;
int vbolen;
int imWidth = 0;
int imHeight = 0;

cl_kernel kernel = 0;
cl_mem cl_vbo_mem;
cl_context context = 0;
cl_command_queue commandQueue = 0;
cl_program program = 0;

float anim = 0.0;
float rotateX    = 0.0;
float rotateY    = 0.0;
float translateZ = -3.0;

const unsigned int meshWidth  = 512; //256;
const unsigned int meshHeight = 512; //256;
///
// Forward declarations
void Cleanup();
cl_int computeVBO();


void renderVBO( int vbolen )
{

    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    // Draw VBO containing the point list coordinates, to place GL_POINTS at feature locations
    // bind VBOs for vertex array and index array
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);         // for vertex coordinates
    glEnableClientState(GL_VERTEX_ARRAY);             // activate vertex coords array

    glVertexPointer(4, GL_FLOAT, 0, 0);

    // draw lines with endpoints given in the array
   // glDrawArrays(GL_LINES, 0, vbolen*2);
    //draw points for the wave mesh
    glColor3f(1.0, 0.0, 0.0);
    glDrawArrays(GL_POINTS, 0, meshWidth * meshHeight);

    glDisableClientState(GL_VERTEX_ARRAY);            // deactivate vertex array
    // flip backbuffer to screen
    glutSwapBuffers();

    // bind with 0, so, switch back to normal pointer operation
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

///
// Main rendering call for the scene
//
void renderScene(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	computeVBO();

	renderVBO( vbolen );
	glutSwapBuffers();
}

GLuint initVBO(int vbolen )
{
    GLint bsize;

    GLuint vbo_buffer;
    // generate the buffer
    glGenBuffers(1, &vbo_buffer);

    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer);
	if( glGetError() != GL_NO_ERROR ) {
		std::cerr<<"Could not bind buffer"<<std::endl;
	}



    // create the buffer, this basically sets/allocates the size
	// for our VBO we will hold 2 line endpoints per element
	unsigned int size = meshWidth * meshHeight * 4 * sizeof( float);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

	if( glGetError() != GL_NO_ERROR ) {
		std::cerr<<"Could not bind buffer"<<std::endl;
	}
    // recheck the size of the created buffer to make sure its what we requested
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);

    if ((GLuint)bsize != size) {
        printf("Vertex Buffer object (%d) has incorrect size (%d).\n", (unsigned)vbo_buffer, (unsigned)bsize);
    }

    // we're done, so unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	if( glGetError() != GL_NO_ERROR ) {
		std::cerr<<"Could not bind buffer"<<std::endl;
	}
	return vbo_buffer;
}

cl_int computeVBO()
{
	cl_int errNum;

    // Set the kernel arguments, send the cl_mem object for the VBO
    errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_vbo_mem);
    errNum = clSetKernelArg(kernel, 1, sizeof(cl_int), &meshWidth);
    errNum = clSetKernelArg(kernel, 2, sizeof(cl_int), &meshHeight);
    errNum = clSetKernelArg(kernel, 3, sizeof(cl_float), &anim);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error setting kernel arguments." << std::endl;
        Cleanup();
        return 1;
    }

    size_t globalWorkSize[] = {meshWidth, meshHeight};


	// Acquire the GL Object
	// Note, we should ensure GL is completed with any commands that might affect this VBO
	// before we issue OpenCL commands
	glFinish();
	errNum = clEnqueueAcquireGLObjects(commandQueue, 1, &cl_vbo_mem, 0, NULL, NULL );

    // Queue the kernel up for execution across the array
    errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL,
                                    globalWorkSize, NULL,
                                    0, NULL, NULL);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Error queuing kernel for execution." << std::endl;
    }

   //
	// Release the GL Object
	// Note, we should ensure OpenCL is finished with any commands that might affect the VBO
	errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &cl_vbo_mem, 0, NULL, NULL );
	clFinish(commandQueue);

	return 0;
}


///
//  Create an OpenCL context on the first available platform using
//  either a GPU or CPU depending on what is available.
//
cl_context CreateContext()
{
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    // First, select an OpenCL platform to run on.  For this example, we
    // simply choose the first available platform.  Normally, you would
    // query for all available platforms and select the most appropriate one.
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
        return NULL;
    }

    // Next, create an OpenCL context on the platform.  Attempt to
    // create a GPU-based context, and if that fails, try to create
    // a CPU-based context.
    cl_context_properties contextProperties[] =
    {
#ifdef _WIN32
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
		CL_GL_CONTEXT_KHR,
		(cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR,
		(cl_context_properties)wglGetCurrentDC(),
#elif defined( __GNUC__)
		CL_CONTEXT_PLATFORM, (cl_context_properties)firstPlatformId,
		CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
#elif defined(__APPLE__)
		//todo
#endif
        0



    };
	cl_uint uiDevCount;
    cl_device_id* cdDevices;
	// Get the number of GPU devices available to the platform
    errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_GPU, 0, NULL, &uiDevCount);

    // Create the device list
    cdDevices = new cl_device_id [uiDevCount];
    errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_GPU, uiDevCount, cdDevices, NULL);


    context = clCreateContext(contextProperties, 1, &cdDevices[0], NULL, NULL, &errNum);


    if (errNum != CL_SUCCESS)
    {
        std::cout << "Could not create GPU context, trying CPU..." << std::endl;
        context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU,
                                          NULL, NULL, &errNum);
        if (errNum != CL_SUCCESS)
        {
            std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
            return NULL;
        }
    }

    return context;
}

///
//  Create a command queue on the first device available on the
//  context
//
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
    cl_int errNum;
    cl_device_id *devices;
    cl_command_queue commandQueue = NULL;
    size_t deviceBufferSize = -1;

    // First get the size of the devices buffer
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
        return NULL;
    }

    if (deviceBufferSize <= 0)
    {
        std::cerr << "No devices available.";
        return NULL;
    }

    // Allocate memory for the devices buffer
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Failed to get device IDs";
        return NULL;
    }

    // In this example, we just choose the first available device.  In a
    // real program, you would likely use all available devices or choose
    // the highest performance device based on OpenCL device queries
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
    if (commandQueue == NULL)
    {
        std::cerr << "Failed to create commandQueue for device 0";
        return NULL;
    }

    *device = devices[0];
    delete [] devices;
    return commandQueue;
}

///
//  Create an OpenCL program from the kernel source file
//
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
    cl_int errNum;
    cl_program program;

    std::ifstream kernelFile(fileName, std::ios::in);
    if (!kernelFile.is_open())
    {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return NULL;
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();

    std::string srcStdStr = oss.str();
    const char *srcStr = srcStdStr.c_str();
    program = clCreateProgramWithSource(context, 1,
                                        (const char**)&srcStr,
                                        NULL, NULL);
    if (program == NULL)
    {
        std::cerr << "Failed to create CL program from source." << std::endl;
        return NULL;
    }

    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (errNum != CL_SUCCESS)
    {
        // Determine the reason for the error
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);

        std::cerr << "Error in kernel: " << std::endl;
        std::cerr << buildLog;
        clReleaseProgram(program);
        return NULL;
    }

    return program;
}

///
//  Create memory objects used as the arguments to kernels in OpenCL
//  The memory objects are created from existing OpenGL buffers and textures
//
bool CreateMemObjects(cl_context context,  GLuint vbo, cl_mem *p_cl_vbo_mem )
{
	cl_int errNum;

	*p_cl_vbo_mem = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, vbo, &errNum );
	if( errNum != CL_SUCCESS )
	{
		std::cerr<< "Failed creating memory from GL buffer." << std::endl;
		return false;
	}

    return true;
}

///
//  Cleanup any created OpenCL resources
//
void Cleanup()
{
    if (commandQueue != 0)
        clReleaseCommandQueue(commandQueue);

    if (kernel != 0)
        clReleaseKernel(kernel);

    if (program != 0)
        clReleaseProgram(program);

    if (context != 0)
        clReleaseContext(context);

	if( cl_vbo_mem != 0 )
		clReleaseMemObject(cl_vbo_mem);

	// after we have released the OpenCL references, we can delete the underlying OpenGL objects
	if( vbo != 0 )
	{
		glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
		glDeleteBuffers(1, &vbo);
	}

	exit(0);
}


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	 cl_device_id device = 0;

    vbolen = meshWidth * meshHeight * 4;
    GLuint lungVBO;
    lungVBO = meshWidth * meshHeight * 4;

	vbo = initVBO(vbolen);

    // Create an OpenCL context on first available platform
    context = CreateContext();
    if (context == NULL)
    {
        std::cerr << "Failed to create OpenCL context." << std::endl;
        return ;
    }

    // Create a command-queue on the first device available
    // on the created context
    commandQueue = CreateCommandQueue(context, &device);
    if (commandQueue == NULL)
    {
        Cleanup();
        return ;
    }

    // Create OpenCL program from GLinterop.cl kernel source
    program = CreateProgram(context, device, "sinewave.cl");
    if (program == NULL)
    {
        Cleanup();
        return ;
    }

    // Create OpenCL kernel
    kernel = clCreateKernel(program, "simpleGL", NULL);
    if (kernel == NULL)
    {
        std::cerr << "Failed to create kernel" << std::endl;
        Cleanup();
        return ;
    }

    // Create memory objects that will be used as arguments to
    // kernel
    if (!CreateMemObjects(context, vbo, &cl_vbo_mem))

    {
        Cleanup();
        return ;
    }

	// Perform some queries to get information about the OpenGL objects
	//performQueries();

    std::cout << std::endl;
    std::cout << "Executed program succesfully." << std::endl;
   // Cleanup();

}

//--------------------------------------------------------------
void testApp::update(){
     anim += 0.01f;
}

//--------------------------------------------------------------
void testApp::draw(){



    glDisable( GL_DEPTH_TEST);
    cam.begin();
    //ofTranslate(ofGetWidth()*0.5,ofGetHeight()*0.5,0);
    ofScale(100,100,100);
    renderScene();
    cam.end();

    ofSetColor(255,255,255);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
