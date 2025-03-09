
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Camera.h"
#include "../HeaderFiles/Sphere.h"
#include "../HeaderFiles/Model.h"


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const uint SCR_WIDTH=800;
const uint SCR_HEIGHT=600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//light position
glm::vec3 lightPos(1.2f, 0.3f, 2.0f);
float lightSpeed=1.5f;
float rotationAngle = 0.0f;
float radius = 2.0f;

//head size
float headsize=0.01f;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sphere Orbiting Face", NULL, NULL);
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

    // build and compile shaders
    // -------------------------
    Shader LightShader("./Shaders/light_sphere.vs", "./Shaders/light_sphere.fs");
    Shader HeadShader("./Shaders/head.vs", "./Shaders/head.fs");

    // load models
    // -----------
    Sphere sphere(20,20);
    Model Woman("./head_obj/woman1.obj");

    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sphere
        LightShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        rotationAngle += lightSpeed * deltaTime;
        //smoother than glfwGetTime
        lightPos.x = radius * cos(rotationAngle);
        // Y position now moves up and down more slowly and affects the radius
        lightPos.y = 0.9f * sin(rotationAngle * 0.3f);
        lightPos.z = radius * sin(rotationAngle);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        LightShader.setMat4("projection", projection);
        LightShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos); 
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        LightShader.setMat4("model", model);
        sphere.Draw();

        // Woman
        HeadShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //same projection and view since they depend on the camera and not the object
        HeadShader.setMat4("projection", projection);
        HeadShader.setMat4("view", view);
        // render the loaded model
        glm::mat4 modelW = glm::mat4(1.0f);
        modelW = glm::translate(modelW, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it so it's at the center of the scene
        modelW = glm::scale(modelW, glm::vec3(headsize, headsize, headsize));//scale it up/down
        HeadShader.setMat4("model", modelW);
        HeadShader.setVec3("viewPos", camera.Position);
        HeadShader.setVec3("lightPos", lightPos);
        Woman.Draw(HeadShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //-----------------
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        lightSpeed = glm::max(0.0f, lightSpeed - 0.05f);
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        lightSpeed += 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
        headsize+=0.0001f;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        if(headsize<=0.01f)
            return;
        else
            headsize-=0.0001f;
    
    }
    //-----------------------
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
