#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

// Custom preprocessor macros
#define KEY_PRESSED 1

#define NUM0 48
#define NUM1 49
#define NUM2 50
#define NUM3 51
#define NUM4 52
#define NUM5 53
#define NUM6 54
#define NUM7 55
#define NUM8 56
#define NUM9 57
#define KB_A 65
#define KB_B 66
#define KB_C 67
#define KB_D 68
#define KB_E 69
#define KB_F 70
#define KB_G 71
#define KB_H 72
#define KB_I 73
#define KB_J 74
#define KB_K 75
#define KB_L 76
#define KB_M 77
#define KB_N 78
#define KB_O 79
#define KB_P 80
#define KB_Q 81
#define KB_R 82
#define KB_S 83
#define KB_T 84
#define KB_U 85
#define KB_V 86
#define KB_W 87
#define KB_X 88
#define KB_Y 89
#define KB_Z 90
#define KB_BACKSPACE 259
#define KB_LSHIFT 340
#define KB_ENTER 257
#define KB_SLASH 47
#define KB_MINUS 45
#define KB_EQUAL 61
#define KB_HOME 268
#define KB_ESCAPE 256

//namespace Process {
//    static bool firstMouse = false;
//    static float deltaTime = 0.0f;
//    static float lastFrame = 0.0f;
//}
//
//enum Camera_Movement {
//    FORWARD,
//    BACKWARD,
//    LEFT,
//    RIGHT,
//};
//
//class Camera {
//private:
//    float yaw = -90.f;
//    float pitch = 0.0f;
//    float lastX = 400;
//    float lastY = 300;
//    float fov = 45.0f;
//    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//public:
//    Camera(glm::vec3 vector=glm::vec3(0.0f,0.0f,0.0f))
//        : cameraPos{ vector }{ }
//    float cameraSpeed = 1.0f;
//    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//
//    float getYaw() const { return yaw; }
//    float getPitch() const { return pitch; }
//    float getFOV() const { return fov; }
//    float getLastX() const{ return lastX; }
//    float getLastY() const { return lastY; }
//    const glm::vec3& getCameraPos() const { return cameraPos; }
//    const glm::vec3& getCameraFront() const { return cameraFront; }
//    const glm::vec3& getCameraUp() const { return cameraUp; }
//
//    void setLastX(float& set) { lastX = set; }
//    void setLastY(float& set) { lastY = set; }
//    void setYaw(float& set) { yaw = set; }
//    void setPitch(float& set) { pitch = set; }
//    const void setCameraFront(glm::vec3 vector) { this->cameraFront = vector; }
//    const void setCameraPos(glm::vec3 vector) { this->cameraPos = vector; }
//};
//
//void ProcessKeyboard(Camera_Movement direction, float deltaTime, Camera* camera);
//void processInput(GLFWwindow* window, Camera* camera);

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif