#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
#include "glut_ply.h"
#include <iostream>
#include "BoundingVolume.h"
#include <random>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido = 0.0f;

// lighting
glm::vec3 lightPos(0, 0, 0);

Esfera esfera(vec3(0),2., 20, 20);
vector<float> radios = {0.5,1,1.5,2,3};
vector<Objeto*> esferas;
Objeto *pEsfera = new Esfera(vec3(0),2, 50, 50);
Model_PLY modelo;
vector<Objeto*> objetos;
float offset=0;
float velocidad_usuario = 1;
bool boton_presionado = false;
bool boton_presionado_up = false;
bool boton_presionado_down = false;

int main() {
    char *archivo = "../models/pumpkin.ply";
    modelo.Load(archivo);

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)     {
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))     {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader lightingShader("../2.2.basic_lighting.vs", "../2.2.basic_lighting.fs");
    //Shader lightCubeShader("../2.2.light_cube.vs", "../2.2.light_cube.fs");

    for(auto i: radios){
        auto esf = new Esfera(vec3(0),i, 20, 20);
        esf->setup();
        esferas.push_back(esf);
    }

    //esfera.setup();
    //pEsfera->setup();
    pEsfera->vao = esfera.vao;
    modelo.setup();

    //objetos.emplace_back(pEsfera);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        tiempoTranscurrido = currentFrame - tiempoInicial -offset; //static_cast<float>(glfwGetTime());
        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);        lightingShader.setMat4("model", model);

        //esfera.display(lightingShader);
        //pEsfera->display(lightingShader);
        int aux=1;
        for (auto &obj : objetos) {
            Esfera* esferaPtr = dynamic_cast<Esfera*>(obj);
           // cout<<"EXEE  "<<aux<<" "<<esferaPtr->centro.x<<" "<<esferaPtr->centro.y<<" "<<esferaPtr->centro.z<<" "<<endl;

            obj->actualizarPosicion(velocidad_usuario * tiempoTranscurrido);
            lightingShader.setVec3("objectColor", esferaPtr->color);
            obj->display(lightingShader);
            aux++;

        }
        aux = 1;
        vector<Objeto*> colisionan;

        for (auto &x : objetos) {
            for (auto &y : objetos) {
                if (x == y){
                    continue;
                }
                Esfera* esfera1 = dynamic_cast<Esfera*>(x);
                Esfera* esfera2 = dynamic_cast<Esfera*>(y);
                auto BS1 = BoundingSphere(esfera1->centro,esfera1->radius);
                auto BS2 = BoundingSphere(esfera2->centro,esfera2->radius);
                if(BS1.intersecta(BS2))
                {
                    colisionan.push_back(x);
                    colisionan.push_back(y);
                }
            }

        }
        vector<Objeto*> aux_v;
        if (colisionan.size() != 0) {
            for (auto &x: objetos) {
                bool colisiono = false;
                for (auto &y: colisionan) {
                    if (x == y) {
                        colisiono = true;
                    }
                }
                if (!colisiono) {
                    aux_v.push_back(x);
                }
            }
            objetos = aux_v;
        }
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        modelo.display(lightingShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
 //   glDeleteVertexArrays(1, &cubeVAO);
 //   glDeleteVertexArrays(1, &lightCubeVAO);
 //   glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        boton_presionado = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE){
        if (boton_presionado) {
            float x = 5+rand()%30;
            float y = 5+rand()%30;
            float z = 5+rand()%30;
            float v = 1 + rand()%5;
            int radio = rand()%5;
            //Objeto *pE = new Esfera(glm::vec3(0),2,0,0);

            auto pE_aux = dynamic_cast<Esfera*>(esferas[radio]);
            Objeto *pE = new Esfera(glm::vec3(0),pE_aux->radius,0,0);

            pE->v0 = v;
            pE->a0 = 40 + rand() % 20;
            pE->x0 = 0;
            pE->y0 = 0;
            pE->z0 = 0;
            pE->distancias.x = x;
            pE->distancias.y = y;
            pE->distancias.z = z;
            pE->vao = pE_aux->vao;
//            pE->vao = esfera.vao;
            pE->indices_size = pE_aux->indices_size;
            //pE->indices_size = esfera.indices_size;
            objetos.emplace_back(pE);
            std::cout<<objetos.size()<<endl;
            //objetos = {pE};
            boton_presionado = false;

            //cout << endl << x << " " << y << " " << z << " " << pE->a0;
            //etiempoInicial = static_cast<float>(glfwGetTime());
        }
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        boton_presionado_up = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
        if (boton_presionado_up) {
            boton_presionado_up = false;
            velocidad_usuario*=1.25;

        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        boton_presionado_down = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
        if (boton_presionado_down) {
            boton_presionado_down = false;
            velocidad_usuario/=1.25;

        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
