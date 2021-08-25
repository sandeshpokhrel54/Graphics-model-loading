#include<iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

glm::mat4 myperspective(float angleInRadians,float aspectRatio, float zNear, float zFar)
{
    float matarray[16] = {0}; //4*4 matrix array
    matarray[0] = 1/(aspectRatio * tan(angleInRadians/2.0f));
    matarray[5] = 1/tan(angleInRadians/2.0f);
    matarray[11] = -1.0f; 

    if (GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE)
    {
        matarray[10] = -zFar/(zFar-zNear);
        matarray[14] = -(zFar*zNear)/(zFar-zNear);
    }
    else
    {
        matarray[10] = -(zFar+zNear)/(zFar-zNear);
        matarray[14] =  -(2.0f * zFar* zNear)/(zFar-zNear);
    }

    glm::mat4 projection = glm::make_mat4(matarray);
    // std::cout<<glm::to_string(projection);
    return projection;
}


glm::mat4 mat_scale(glm::mat4 mat1, glm::vec3 vec1)
{
    glm::mat4 scaledMatrix;

    scaledMatrix[0] = mat1[0] * vec1[0];
    scaledMatrix[1] = mat1[1] * vec1[1];
    scaledMatrix[2] = mat1[2] * vec1[2];
    scaledMatrix[3] = mat1[3];

    // std::cout<<"Scaled Matrix: \n";
    // std::cout<<glm::to_string(scaledMatrix);

    return scaledMatrix;
}

glm::mat4 mat_rotate(glm::mat4 mat1, float angle, glm::vec3 vec1)
{
    const float theta = angle; //glm::radians(angle) if input is in degree
    const float cos_theta = cos(angle);
    const float sin_theta = sin(theta);

    glm:: vec3 axis = normalize(vec1);  // vec1/sqrt(vec1[0]*vec1[0] + vec1[1]*vec1[1] + vec1[2]*vec1[2])
    glm::vec3 temp = (1 - cos_theta) *axis;

    glm::mat4 Rotate;  //Generating composite transformation matrix

        Rotate[0][0] = cos_theta + temp[0] * axis[0];
        Rotate[0][1] = temp[0] * axis[1] + sin_theta * axis[2];
        Rotate[0][2] = temp[0] * axis[2] - sin_theta * axis[1];

        Rotate[1][0] = temp[1] * axis[0] - sin_theta * axis[2];
        Rotate[1][1] = cos_theta + temp[1] * axis[1];
        Rotate[1][2] = temp[1] * axis[2] + sin_theta * axis[0];

        Rotate[2][0] = temp[2] * axis[0] + sin_theta * axis[1];
        Rotate[2][1] = temp[2] * axis[1] - sin_theta * axis[0];
        Rotate[2][2] = cos_theta + temp[2] * axis[2];

    glm::mat4 Result_Mat;   //obtain rotated matrix by multiplying with CTM
        Result_Mat[0] = mat1[0] * Rotate[0][0] + mat1[1] * Rotate[0][1] + mat1[2] * Rotate[0][2];
        Result_Mat[1] = mat1[0] * Rotate[1][0] + mat1[1] * Rotate[1][1] + mat1[2] * Rotate[1][2];
        Result_Mat[2] = mat1[0] * Rotate[2][0] + mat1[1] * Rotate[2][1] + mat1[2] * Rotate[2][2];
        Result_Mat[3] = mat1[3];


    // std::cout<<"Rotated Matrix: \n";
    // std::cout<<glm::to_string(Result_Mat);

    return Result_Mat;

}


glm::mat4 mat_Translate( glm::mat4 mat1, glm:: vec3 vec1)
{
    glm::mat4 Result_Mat;           // cant do mat[0] + vec[0], mat[1] + vec[1], mat[2] + vec[2]
        Result_Mat[0] = mat1[0];
        Result_Mat[1] = mat1[1];
        Result_Mat[2] = mat1[2];
        Result_Mat[3] = mat1[0] * vec1[0] + mat1[1] * vec1[1] + mat1[2] * vec1[2] + mat1[3];


    // std::cout<<"Translated Matrix: \n";
    // std::cout<<glm::to_string(Result_Mat);

    return Result_Mat;
}

//look at  glm::lookAt