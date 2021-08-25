#include"camera.h"

Camera::Camera(glm::vec3 position , glm::vec3 up, float yaw, float pitch) :  Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{   
    Position = position;
    WorldUp = up;
    Pitch = pitch;
    Yaw = yaw;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    // return glm::lookAt(Position, Position + Front, Up);
    glm::vec3 f((Position+Front) - Position);
    glm::vec3 s(normalize(cross(f,Up)));
    glm::vec3 u(cross(s,f));

    glm::mat4 Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] =-f.x;
	Result[1][2] =-f.y;
	Result[2][2] =-f.z;
    Result[3][0] =-dot(s,Position);
    Result[3][1] =-dot(u,Position);
    Result[3][2] = dot(f,Position);
    return Result;

}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if(direction == FORWARD)
        Position += Front * velocity;
    if(direction == BACKWARD)
        Position -= Front * velocity;
    if(direction == RIGHT)
        Position += Right * velocity;
    if(direction == LEFT)
        Position -= Right * velocity;
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if(Zoom < 1.0f)
        Zoom = 1.0f;
    if(Zoom > 45.0f)
        Zoom = 45.0f;

}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if(constrainPitch)
    {
        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();

}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
    

}

glm::mat4 Camera::lookAtObject(glm::vec3 objectPostion)
{
    glm::vec3 front = glm::normalize(objectPostion);
    glm::vec3 f((Position+front) - Position);
    glm::vec3 s(normalize(cross(f,Up)));
    glm::vec3 u(cross(s,f));

    glm::mat4 Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] =-f.x;
	Result[1][2] =-f.y;
	Result[2][2] =-f.z;
    Result[3][0] =-dot(s,Position);
    Result[3][1] =-dot(u,Position);
    Result[3][2] = dot(f,Position);
    
    return Result;

}