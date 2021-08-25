#define GLFw_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "shaderClass/shader.h"
#include "camera.h"
#include "model.h"
#include "utility.h"
#include "audio.h"

#define POINT_LIGHTS 7

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//light info
glm::vec3 sunPos(-100.0f,30.0f,-30.0f); //-100 30 0
glm::vec3 sunLight(1.0f,1.0f,1.0f);

//two point lights
glm::vec3 pointLightColor(0.96f, 0.72f, 0.28f);

// glm::vec3 pointLight1(-18.3517f, -2.134f, 25.47f);
// glm::vec3 pointLight2(-24.0f, -0.6f, -34.0f);

glm::vec3 pointLight[POINT_LIGHTS];
  
//loading cubemap
unsigned int loadCubemap(std::vector<std::string> faces)
{ 
    unsigned int textureID; 
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width,height,nrChannels;
    unsigned char *data;
    for(unsigned int i = 0; i<faces.size(); i++)
    {
        if(data)
        {
            data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout<<"Cubemap texture load failed"<<faces[i]<<std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse  = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Castle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //complie shaders
    Shader skyboxShader("cubemap.vs","cubemap.fs");
    Shader modelShader("mshader.vs","mshader.fs");
    Shader lightSourceSun("sun.vs", "sun.fs");
    Shader terrainShader("terrain.vs", "terrain.fs");

    //load models
    // Model  withTerrain("./resources/withTerrain.obj");
    Model onlyTerrain("./resources/island3.obj");
    Model Hilltopcastle("./resources/buildings3.obj");




//initialize light points
pointLight[0] = glm::vec3(-18.46f, -2.48f, -25.91f);
pointLight[1] = glm::vec3(-29.74, -3.063f, -25.64f);
pointLight[2] = glm::vec3(-24.44f, -2.55f, -16.96f);
pointLight[3] = glm::vec3(-26.395f, -4.40f, 19.83f);
pointLight[4] = glm::vec3(-17.034f, -1.37f, 25.90f);
pointLight[5] = glm::vec3(-19.46f, -1.34f, 25.826f);
pointLight[6] = glm::vec3(-11.38f, -4.31f, 24.73f);

//light object cube
float lightCubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

//light cube configuration
unsigned int VAflightCube, VBflightcube;
glGenVertexArrays(1, &VAflightCube);
glGenBuffers(1, &VBflightcube);

glBindVertexArray(VAflightCube);
glBindBuffer(GL_ARRAY_BUFFER, VBflightcube);
glBufferData(GL_ARRAY_BUFFER, sizeof(lightCubeVertices), lightCubeVertices, GL_STATIC_DRAW);

glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

//skybox configuration  
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

unsigned int VAfskybox, VBfskybox;
glGenVertexArrays(1,&VAfskybox);
glGenBuffers(1,&VBfskybox);

glBindVertexArray(VAfskybox);

glBindBuffer(GL_ARRAY_BUFFER, VBfskybox);
glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
//position attribute
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

//.........................cubemap texture........
    std::vector<std::string> texture_faces = {
        "./resources/skybox/right.jpg",
        "./resources/skybox/left.jpg",
        "./resources/skybox/top.jpg",
        "./resources/skybox/bottom.jpg",
        "./resources/skybox/front.jpg",
        "./resources/skybox/back.jpg"
    };

    unsigned int cubemapTexID = loadCubemap(texture_faces);

    //use skybox shader
    skyboxShader.use();
    //set skybox uniform
    skyboxShader.setInt("skybox",0);

    camera.Position = glm::vec3(-7.34f,4.32f,-51.98f);
    // render loop

    //play sound
        Init(); 

    int fogcontrol = 0;

    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        // std::cout<<"fps"<<1.0f/deltaTime<<std::endl;

        //fog control
        if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            fogcontrol = 1;
            
        }

        if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            fogcontrol = 0;
        }



        //inputs
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        //model part
        modelShader.use();
        modelShader.setVec3("sunPos",sunPos); //set sun position uniform    
        modelShader.setVec3("sunColor",sunLight); 
        modelShader.setVec3("pLightColor", pointLightColor);
        modelShader.setInt("fogbool", fogcontrol);
        glUniform3fv(glGetUniformLocation(modelShader.ID, "pLights"), sizeof(pointLight), glm::value_ptr(pointLight[0])); //setting array of vectors uniform
        
        float aRad = glm::radians(camera.Zoom);
        float aspectR = (float)SCR_WIDTH/(float)SCR_HEIGHT;

        glm::mat4 projection = myperspective(aRad, aspectR, 0.1f, 80.0f);
        // std::cout<<"checkvalues \n";
        // std::cout<<aRad<<" radians"<<endl;
        // std::cout<<aspectR<<" asR"<<endl;
        // std::cout<<glm::to_string(projection)<<endl;
        // std::cout<<"endcheck";      
        glm::mat4 view = camera.getViewMatrix();
        modelShader.setVec3("viewerPos",camera.Position);
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        
        model = mat_Translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = mat_scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        
        modelShader.setMat4("model", model);
        Hilltopcastle.Draw(modelShader);

        
        terrainShader.use();
        terrainShader.setVec3("sunPos", sunPos);
        terrainShader.setVec3("sunColor",sunLight);
        terrainShader.setVec3("pLightColor", pointLightColor);
        terrainShader.setInt("fogbool", fogcontrol);
        glUniform3fv(glGetUniformLocation(terrainShader.ID, "pLights"), sizeof(pointLight), glm::value_ptr(pointLight[0]));

        terrainShader.setVec3("viewerPos",camera.Position);
        terrainShader.setMat4("projection",projection);
        terrainShader.setMat4("view", view);
        glm::mat4 terrainMod = glm::mat4(1.0f);
        terrainMod = glm::scale(terrainMod, glm::vec3(0.05f,0.05f, 0.05f));
        terrainShader.setMat4("model",terrainMod);
        onlyTerrain.Draw(terrainShader);



        
        //sun
        lightSourceSun.use();
        lightSourceSun.setVec3("lightColor",sunLight); 
        lightSourceSun.setMat4("projection", projection);
        lightSourceSun.setMat4("view",view);
        model = glm::mat4(1.0f);
        model = mat_Translate(model,sunPos);
        model = mat_scale(model,glm::vec3(0.4f, 0.4f, 0.4f));
        lightSourceSun.setMat4("model", model);
        glBindVertexArray(VAflightCube);
        glDrawArrays(GL_TRIANGLES,0,36);


        //point lights
        lightSourceSun.setVec3("lightColor",pointLightColor);
        for(unsigned int i=0; i<POINT_LIGHTS; i++)
        {
            model = glm::mat4(1.0f);
            model = mat_Translate(model, pointLight[i]);
            model = mat_scale(model, glm::vec3(0.05, 0.05,0.05));
            lightSourceSun.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES,0, 36);
        }

        //cubemap part
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("projection", projection);
        // camera/view transformation
        view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setInt("fogbool", fogcontrol);

        glBindVertexArray(VAfskybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,cubemapTexID);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAfskybox);
    glDeleteBuffers(1, &VBfskybox);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}