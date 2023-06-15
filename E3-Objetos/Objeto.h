//
// Created by hgallegos on 5/10/2022.
//

#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader_m.h"
using namespace std;
using namespace glm;

class Objeto {
public:
    vec3 distancias;
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    float v0, x0, y0, z0, a0, xt, yt, zt;
    GLuint vao;
    mat4 model;
    bool visible=true;
    GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;
    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
    virtual void actualizarPosicion(float tiempo)=0;
};

class Esfera:public Objeto{
public:
    vec3 centro;
    float radius;
    int slices, stacks;
    vec3 color;



    Esfera() {
        centro = vec3(0.0);
    }
    Esfera(vec3 _centro) {
        centro = _centro;
        x0 = centro.x;
        y0 = centro.y;
        z0 = centro.z;
        float x = 1+rand()%100;
        float y = 1+rand()%100;
        float z = 1+rand()%100;
        color = {x/100,y/100,z/100};
    }
    Esfera(vec3 _centro, float radio) {
        centro = _centro;
        x0 = centro.x;
        y0 = centro.y;
        z0 = centro.z;
        radius = radio;
        float x = 1+rand()%100;
        float y = 1+rand()%100;
        float z = 1+rand()%100;
        color = {x/100,y/100,z/100};
    }
    Esfera(vec3 _centro, float _radius, int _slices, int _stacks) {
        centro = _centro;
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
        float x = 1+rand()%100;
        float y = 1+rand()%100;
        float z = 1+rand()%100;
        color = {x/100,y/100,z/100};

    }
    GLuint setup();

    void display(Shader &sh);
    void actualizarPosicion(float tiempo);
};

#endif //LEARNOPENGL_OBJETO_H
