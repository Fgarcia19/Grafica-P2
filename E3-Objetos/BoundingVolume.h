//
// Created by hgallegos on 7/06/2023.
//

#ifndef E3_OBJETOS_BOUNDINGVOLUME_H
#define E3_OBJETOS_BOUNDINGVOLUME_H
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
class BoundingVolume {
public:
    virtual bool intersecta(BoundingVolume &bv)=0;
};

class BoundingSphere : public BoundingVolume{
public:
    vec3 centro; float radio;
    BoundingSphere() { centro = vec3(0); radio = 0; }
    BoundingSphere(vec3 _centro, float _radio){
        centro = _centro; radio = _radio;
    }
    bool intersecta(BoundingVolume &bv){}
    bool intersecta(BoundingSphere &bs){
        double distancia = sqrt(pow(centro.x - bs.centro.x, 2) + pow(centro.y - bs.centro.y, 2) + pow(centro.z - bs.centro.z, 2));

        //cout<<"Distancia "<<(bs.centro - centro).length()<<endl;
        //cout<<"DIST "<< distancia<<endl;
        //eeeecout<<"Radios "<< bs.radio + radio<<endl;
        return distancia <= bs.radio + radio;
    }
};


#endif //E3_OBJETOS_BOUNDINGVOLUME_H