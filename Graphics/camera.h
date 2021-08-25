#ifndef CAMERA_H
#define CAMERA_H
#include<vector>
#include "glad/include/glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    //constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    //constructor with scalars
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 WorldUp;
    glm::vec3 Up;
    glm::vec3 Right;

    //euler angles
    float Yaw;
    float Pitch;

    //camera options
    float Zoom;
    float MouseSensitivity;
    float MovementSpeed;
    glm::mat4 getViewMatrix();
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yoffset);
    glm::mat4 lookAtObject(glm::vec3 objectPosition);

private:
    void updateCameraVectors();
};



#endif