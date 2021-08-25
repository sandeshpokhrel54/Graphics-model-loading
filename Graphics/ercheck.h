
#include"glad/include/glad/glad.h"
#include<iostream>
void GlClearError();

// get all the errors
void _GlGetError(const char *function, const char *file, int line);

#define glCheckError(x) GlClearError();\
    x;\
    _GlGetError(#x, __FILE__, __LINE__);


void GlClearError(){
    while (glGetError() != GL_NO_ERROR);
}

void _GlGetError(const char *function, const char *file, int line) {
    while(GLenum errorCode = glGetError()){
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION"; break;
       /* case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW"; break;*/
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << "[Opengl Error] (" << error << "):\nFunction: " << function <<"\nFile: "<< file << "\nLine:" << line << std::endl;
    }
}