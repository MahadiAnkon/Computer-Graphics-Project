//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;
float angle = 0.0;
// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool rotate_around = false;
float rotate_fan = 0.0;
float rotatedegree = 0.0;
bool dooropen = false;
bool doortonext = false;
float slide=0;
unsigned int wood_tex;
unsigned int sofa_tex1;
unsigned int sofa_tex2;
unsigned int sofa_tex3;
unsigned int tv_tex;
unsigned int shelf_tex;
unsigned int showcase_tex;
unsigned int showcase_door;
unsigned int shelf_tex2;
unsigned int wall_tex1;
unsigned int wall_tex2;
unsigned int wall_tex3;
unsigned int blacktex;
unsigned int floor_tex;
unsigned int painting_tex;
unsigned int painting_tex2;
unsigned int painting_tex3;
unsigned int bed_frame;
unsigned int bedsheet;
unsigned int bedframe;
unsigned int pillow;
unsigned int table_tex;
unsigned int brick_wood;
bool box=false;
bool box2 = false;
bool box3 = false;
bool box4 = false;
float shamne1=0.0;
float shamne2 = 0.0;
float shamne3 = 0.0;
float shamne4 = 0.0;
float aquarium;
//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = -2.0, eyeY = 1, eyeZ = -4.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
glm::vec3 LightPositions[] = {
    glm::vec3(1.50f,  1.50f,  -4.0f),
    glm::vec3(1.50f,  1.50f,  1.0f),
    glm::vec3(-5.5f,  1.5f,  -2.0f),
    glm::vec3(-1.5f,  1.5f,  1.0f),
};
DirLight dirlight(
    LightPositions[0].x, LightPositions[0].y, LightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight(

    LightPositions[1].x, LightPositions[1].y, LightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight1(

    LightPositions[2].x, LightPositions[2].y, LightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    LightPositions[3].x, LightPositions[3].y, LightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);


// light settings
bool pointLightOn1 = false;
bool pointLightOn2 = true;
bool dirLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = false;
bool emissionToggle = true;




// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transform(float tr_x, float tr_y, float tr_z, int rot_x, int rot_y, int rot_z, float scal_x, float scal_y, float scal_z) {
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tr_x, translate_Y + tr_y, translate_Z + tr_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y + rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * scal_x, scale_Y * scal_y, scale_Z * scal_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}

void Sofa(Cube& cube, Shader& lightingShader, Shader& lightingShader2,  glm::mat4 model)
{
    cube.setTextureProperty(sofa_tex1,sofa_tex1, 32.0f);
    glm::mat4 a = transform(-0.8, -0.5, -5.5, 0, 0, 0, 4, 0.2, 1.5);
    cube.drawCubeWithTexture(lightingShader, model*a);


    a = transform(-0.8, -0.5 + 0.1, -5.5, 0, 0, 0, 2, 0.25, 1.5);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(.2, -0.5 + 0.1, -5.5, 0, 0, 0, 2, 0.25, 1.5);
    cube.drawCubeWithTexture(lightingShader, model * a);



    a = transform(-0.8, -0.28, -5.5, 0, 0, 0, 4, 1.2, 0.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-0.8, -0.28, -5.4, 0, 0, 0, 0.2, 0.6, 1.3);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-0.8 + 1.9, -0.28, -5.4, 0, 0, 0, 0.2, 0.6, 1.3);
    cube.drawCubeWithTexture(lightingShader, model * a);


    a = transform(-0.8, -0.5, -5.5, 0, 0, 0, 0.1, -0.3, 1.5);
    cube.drawCubeWithTexture(lightingShader, model * a);


    a = transform(-0.8 + 1.95, -.5, -5.5, 0, 0, 0, 0.1, -0.3, 1.5);
    cube.drawCubeWithTexture(lightingShader, model * a);


}
void table(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(sofa_tex2, sofa_tex2, 32.0f);
    glm::mat4 a = transform(-0.45, -.2, -4.045, 0, 0, 0, 2, 0.2, 1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-0.45, -.6, -4.0, 0, 0, 0, 0.06, 0.8, 0.06);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);



    a = transform(-0.45 + 0.95, -.6, -4.0, 0, 0, 0, 0.06, 0.8, 0.06);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-0.45, -.6, -4.0 + 0.4, 0, 0, 0, 0.06, 0.8, 0.06);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-0.45 + 0.965, -.6, -4.0 + 0.4, 0, 0, 0, 0.06, 0.8, 0.06);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
}

void shelf(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(shelf_tex, shelf_tex, 32.0f);
    glm::mat4 a = transform(-2.44, -.78, -1, 0, 90, 0, 1.7, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.78, -1, 0, 00, 0, 0.9, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.78, -1.9, 0, 00, 0, 0.9, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    cube.setTextureProperty(shelf_tex2, shelf_tex2, 32.0f);
    a = transform(-2.44, -.7, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);


    a = transform(-2.44, -.2, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, 0.4, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);


    cube.setTextureProperty(shelf_tex, shelf_tex, 32.0f);
    a = transform(-2.44, 0.98, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);
}


void showcase(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(shelf_tex, shelf_tex, 32.0f);
    glm::mat4 a = transform(-2.44, -.78, -1, 0, 90, 0, 1.7, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.78, -1, 0, 00, 0, 0.9, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);


    a = transform(-2.40, -.78, -1.44, 0, 00, 0, 0.9, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.78, -1.9, 0, 00, 0, 0.9, 3.5, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.7, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, -.2, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, 0.4, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.44, 0.97, -1.85, 90, 00, 0, 0.9, 1.7, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);
}


void dooropenclose(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(showcase_door, showcase_door, 32.0f);
    glm::mat4 a = transform(-2.00, -.7, -1.0, 0, 90, 0, 0.78, 3.25, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);
}


void dooropenclose2(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(showcase_door, showcase_door, 32.0f);   
    //glm::mat4 tr = transform(2.00, .7, 1.44, 0,0,0,1,1,1); // to origin
    //glm::mat4 tr2 = transform(0, 0, 0, 0, min(angle, 180.0f), 0, 1, 1, 1); // rotate
   //glm::mat4 tr3 = transform(-2.00, -.7, -1.44, 0, 0, 0, 0.84, 3.25, 0.1); // back
    glm::mat4 a = transform(-2.00, -.7, -1.44, 0, 90, 0, 0.84, 3.25, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);
}

void book(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model, float r, float g, float b)
{
    glm::mat4 a = transform(-2.44, -.21, -2.70, -20, 0, 0, 0.6, 0.8, .2);
    cube.setMaterialisticProperty(glm::vec3(r, g, b));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
}

void Fan(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, a;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.1f, 0.5f));
    a = scaleMatrix * translateMatrix;
    cube.setMaterialisticProperty(glm::vec3(0.82, 0.51, 0.263));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));//,translate_X, translate_Y, translate_Z
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.1f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    a = rotateYMatrix * scaleMatrix * translateMatrix;
    //moveMatrix = rotateZMatrix * moveMatrix;
    cube.setMaterialisticProperty(glm::vec3(0.82, 0.51, 0.263));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.5f));//,
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.1f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    a = rotateYMatrix * scaleMatrix * translateMatrix;
    //moveMatrix = rotateZMatrix * moveMatrix;
    cube.setMaterialisticProperty(glm::vec3(0.82, 0.51, 0.263));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    a = transform(-0.1, -0.07, -0.2, 0, 0, 0, 1, 0.4, 1);
    cube.setMaterialisticProperty(glm::vec3(0.678, 0.318, 0.031));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
}

void wall(Cube& cube, Cube& tiles_cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    //floor
    tiles_cube.setTextureProperty(floor_tex, floor_tex, 32.0f);
    glm::mat4 a = transform(-2.5, -.8, -6, 0, 0, 0, 10, 0.1, 16);
    tiles_cube.drawCubeWithTexture(lightingShader, model * a);

    //1st wall
    a = transform(-2.5, -.8, -6, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.2, 1.45, -5.97, 0, 0, 0, 8.8, .5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //2nd wall
    a = transform(-2.5, -.8, 2, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.1, 1.45, 1.6, 0, 0, 0, 8.8, 0.5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);


    //3rd wall
    a = transform(-2.5, -.8, -3.5, 0, 0, 0, .1, 5, 11);
    cube.setMaterialisticProperty(glm::vec3(0.941, 0.776, 0.761));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.5, -.8, -6, 0, 0, 0, .1, 5, 3);
    cube.setMaterialisticProperty(glm::vec3(0.941, 0.776, 0.761));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.5, 0.7, -4.5, 0, 0, 0, .1, 1.5, 2);
    cube.setMaterialisticProperty(glm::vec3(0.941, 0.776, 0.761));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.5, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);


    //4th wall
    a = transform(2.5, -.8, -6, 0, 0, 0, .1, 5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.988, 0.796, 0.78));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(2.1, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //celling
    a = transform(-2.5, 1.7, -6, 0, 0, 0, 10, 0.1, 16);
    cube.setMaterialisticProperty(glm::vec3(0.902, 0.624, 0.408));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

}
void wall2(Cube& cube, Cube& tiles_cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    //floor
    glm::mat4 a = transform(-2.5, -.8, -4.5, 0, 0, 0, 10, 0.1, 13);
    tiles_cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-1.1, -.8, -6, 0, 0, 0, 7.5, 0.1, 13);
    tiles_cube.drawCubeWithTexture(lightingShader, model * a);

    //1st wall

    a = transform(-2.5, -.8, -6, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.2, 1.45, -5.97, 0, 0, 0, 8.8, .5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //2nd wall
    a = transform(-2.5, -.8, 2, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.1, 1.45, 1.6, 0, 0, 0, 8.8, 0.5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //3rd wall
    a = transform(-2.5, -.8, -6, 0, 0, 0, .1, 5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.941, 0.776, 0.761));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.5, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //4th wall


    a = transform(2.1, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
    //celling

    a = transform(-2.5, 1.7, -6, 0, 0, 0, 10, 0.1, 16);
    cube.setMaterialisticProperty(glm::vec3(0.902, 0.624, 0.408));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
}

void wall3(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    //floor
    glm::mat4 a = transform(-2.5, -.8, -6, 0, 0, 0, 10, 0.1, 16);
    cube.setMaterialisticProperty(glm::vec3(0.902, 0.624, 0.408));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //1st wall

    a = transform(-2.5, -.8, -6, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.2, 1.45, -5.97, 0, 0, 0, 8.8, .5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //2nd wall
    a = transform(-2.5, -.8, 2, 0, 0, 0, 10, 5, 0.1);
    cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.1, 1.45, 1.6, 0, 0, 0, 8.8, 0.5, 0.8);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //3rd wall
    a = transform(-2.5, -.8, -6, 0, 0, 0, .1, 5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.941, 0.776, 0.761));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.5, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    //4th wall
    a = transform(2.5, -.8, -6, 0, 0, 0, .1, 5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.988, 0.796, 0.78));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(2.1, 1.45, -6, 0, 0, 0, 0.8, 0.5, 16);
    cube.setMaterialisticProperty(glm::vec3(0.961, 0.596, 0.576));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);


}

void ladder(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    glm::mat4 a = transform(-2.0, -.8, -4, 0, 0, 0, .15, 5.3, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);


    a = transform(-1.4, -.8, -4, 0, 0, 0, .15, 5.3, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, -.4, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, 0, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, 0.4, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, 0.8, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, 1.2, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

    a = transform(-2.0, 1.6, -4, 0, 0, 0, 1.2, 0.15, .15);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);
}


void dressingtable(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    cube.setTextureProperty(table_tex, table_tex, 32.0f);
    glm::mat4 a = transform(-2.0, -.8, -4, 0, 0, 0, 3.5, 2.75, 0.1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.0, -.8, -4, 0, 0, 0, 0.1, 1.25, 1.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-0.3, -.8, -4, 0, 0, 0, 0.1, 1.25, 1.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-1.15, -.8, -4, 0, 0, 0, 0.1, 1.25, 1.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-1.15, -.5, -4, 0, 0, 0, 1.7, 0.1, 1.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform(-2.0, -.8, -3.4, 0, 0, 0, 1.7, 1.25, 0.1);
    //  drawCube(cubeVAO, lightingShader, model * a, 1, 0, 0);

    a = transform(-2, -.2, -4, 0, 0, 0, 3.5, 0.1, 1.2);
    cube.drawCubeWithTexture(lightingShader, model * a);

}


void bed(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    float baseHeight = 0.6;
    float width = 2.5;
    float length = 5;
    float pillowWidth = 0.3 * 3;
    float pillowLength = 0.15 * 4;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.6;

    //base
    cube.setTextureProperty(bed_frame, bed_frame, 32.0f);
    glm::mat4 a = transform(-6, -0.75, 0.6, 0, 0, 0, length, baseHeight, width);
    cube.drawCubeWithTexture(lightingShader, model * a);

    //foam
    cube.setTextureProperty(bedsheet, bedsheet, 32.0f);
    a = transform(-6, -0.45, 0.6, 0, 0, 0, length, baseHeight - 0.3, width);
    cube.drawCubeWithTexture(lightingShader, model * a);

    //pillow 1
    cube.setTextureProperty(pillow, pillow, 32.0f);
    a = transform(-5.9, -0.3, 0.7, 0, 0, 0, pillowLength, 0.1, pillowWidth);
    cube.drawCubeWithTexture(lightingShader, model * a);

    ////pillow 2
    a = transform(-5.9, -0.3, 1.3, 0, 0, 0, pillowLength, 0.1, pillowWidth);
    cube.drawCubeWithTexture(lightingShader, model * a);

    ////blanket
    //model = alTogether * translate2 * scale * translate;
    //drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);

    ////head
    cube.setTextureProperty(bed_frame, bed_frame, 32.0f);
    a = transform(-6.05, -0.75, 0.6, 0, 0, 0, 0.1, baseHeight + 0.9, width);
    cube.drawCubeWithTexture(lightingShader, model * a);

}

void doortonextroom(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{
    glm::mat4 a = transform(-2.45, -0.75, -4.5, 0, 90, 0, 2.0, 2.9, .1);
    cube.setMaterialisticProperty(glm::vec3(1, 0, 0));
    cube.drawCubeWithMaterialisticProperty(lightingShader2, model * a);

}

glm::mat4 transform2(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, a;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    a = translateMatrix  * rotateXMatrix * rotateYMatrix* rotateZMatrix * scaleMatrix;
    return a;
}


void waredrove(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model,unsigned int b)
{

    cube.setTextureProperty(b, b, 32.0f);
    glm::mat4 a = transform2(-0.3, -0.75, -2.5, 0, 0, 0, 2.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);
    
    a = transform2(-0.3, 0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73+0.375, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73+0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 1.125, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33, -2.499 + shamne1, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.375, -2.499 + shamne2, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.75, -2.499 + shamne3, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 1.125, -2.499 + shamne4, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

}

void bedsidetable(Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{

    cube.setTextureProperty(bedframe, bedframe, 32.0f);
    glm::mat4 a = transform2(-0.3, -0.75, -2.5, 0, 0, 0, 2.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, 0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 0.375, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 1.125, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33, -2.499 + shamne1, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.375, -2.499 + shamne2, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.75, -2.499 + shamne3, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 1.125, -2.499 + shamne4, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);


}

void table2 (Cube& cube, Shader& lightingShader, Shader& lightingShader2, glm::mat4 model)
{

    cube.setTextureProperty(shelf_tex, shelf_tex, 32.0f);
    glm::mat4 a = transform2(-0.3, -0.75, -2.5, 0, 0, 0, 2.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.75, -2, 0, 90, 0, 1.0, 3, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, 0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73, -2.5, 90, 0, 0, 2.1, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 0.375, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 0.75, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.3, -0.73 + 1.125, -2.5, 90, 0, 0, 2, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.73 + .65, -2.5, 90, 0, 0, 5, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.75+.45, -2.5, 0, 0, 0, 5.0, 1.8, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.7+3.85, -0.8, -2, 0, 90, 0, 1, 1.4, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(0.7, -0.73 + .45, -2.5, 90, 0, 0, 5, 1, .1);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33, -2.499 + shamne1, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.375, -2.499 + shamne2, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 0.75, -2.499 + shamne3, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);

    a = transform2(-0.25, -0.73 + 0.33 + 1.125, -2.499 + shamne4, 90, 0, 0, 1.9, 1, 0.7);
    cube.drawCubeWithTexture(lightingShader, model * a);
}




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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
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

    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");


    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";
    string f_tiles = "floor_tiles.jpg";
    string wood = "wood.png";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    sofa_tex1 = loadTexture("sofatex1.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sofa_tex2 = loadTexture("sofatex2.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    wood_tex = loadTexture(wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    blacktex = loadTexture("black.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    shelf_tex = loadTexture("shelf.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    shelf_tex2 = loadTexture("shelf2.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    showcase_tex = loadTexture("showcasered.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    showcase_door = loadTexture("showcasedoor.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floor_tex = loadTexture("floor.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    painting_tex = loadTexture("painting.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    painting_tex2 = loadTexture("painting2.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    painting_tex3 = loadTexture("painting3.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    bed_frame = loadTexture("bed_frame_wood.png", GL_REPEAT, GL_REPEAT, 6, 2);
    bedframe = loadTexture("bedframe.jpeg", GL_REPEAT, GL_REPEAT, 6, 2);
    bedsheet = loadTexture("bedsheet.jpeg", GL_REPEAT, GL_REPEAT, 6, 2);
    pillow = loadTexture("cushion.jpg", GL_REPEAT, GL_REPEAT, 6, 2);
    table_tex = loadTexture("table wood.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    brick_wood = loadTexture("brick_wood.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    string s;
    for (int i = 0; i < 75; i++)
    {
        s = "aquarium/giphy-"+to_string(i)+".png";
        aquarium = loadTexture(s.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        if (i == 74)
            i = 0;
    }




    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube tiles_cube = Cube(floor_tex, floor_tex, 32.0, 0.0f, 0.0f, 20.0f, 20.0f);
    //Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    pointlight1.turnOff();
    pointlight1.turnSpecularOff();
    pointlight2.turnSpecularOff();
    dirlight.turnSpecularOff();
    spotlight.turnSpecularOff();



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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);
        
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);
       
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        dirlight.setUpPointLight(lightingShaderWithTexture);
        spotlight.setUpPointLight(lightingShaderWithTexture);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.42, 0.11, 8.18f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;

        cube.setTextureProperty(diffMap, specMap, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        





        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.76, 0.059, -9.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.0f, 1.0f));//translate_X, translate_Y, translate_Z
        //middle sofa
        model = transform(-3.3, -0.1, -0.5, 0, -90, 0, 0.8, 1, 1);
       // Sofa(cube, lightingShaderWithTexture,lightingShader, model);//,translate_X, translate_Y, translate_Z
 







        model = transform(-10.2, 0, 2.8, 0, 180, 0, 1.2, 1, 1.6);
        bed(cube, lightingShaderWithTexture, lightingShader, model);






        //room1_wall
        model = transform(0, 0, 0, 0, 0, 0, 1, 1, 1);
        wall(cube,tiles_cube, lightingShaderWithTexture, lightingShader, model);

        //room2_wall
        model = transform(-5.0, 0, 0, 0, 0, 0, 1, 1, 1);
        wall2(cube, tiles_cube, lightingShaderWithTexture, lightingShader, model);

        //room3_wall
        model = transform(-5.0, -2.5, 0, 0, 0, 0, 1, 1, 1);
        wall3(cube, lightingShaderWithTexture, lightingShader, model);

        //sofa
        model = transform(-3.3, -0.1, -0.5, 0, -90, 0, 0.8, 1, 1);
        Sofa(cube, lightingShaderWithTexture, lightingShader, model);

        model = transform(-1.8, -0.1, -1.4, 0, -130, 0, 0.5, 1, 0.8);
        Sofa(cube, lightingShaderWithTexture, lightingShader, model);

        model = transform(-2.0, -0.1, 0.6, 0, 310, 0, 0.5, 1, 0.8);
        Sofa(cube, lightingShaderWithTexture, lightingShader, model);


        //Mat
        glm::mat4 a = transform(.7, -.8, -1.2, 0, -90, 0, 3, 0.2, 2);
        cube.setMaterialisticProperty(glm::vec3(0.89, 0.694, 0.694));
        cube.drawCubeWithMaterialisticProperty(lightingShader, model* a);

        //table
        model = transform(5.0, -0.2, -0.5, 0, 90, 0, 1.5, 1, 1.3);
        table(cube, lightingShaderWithTexture, lightingShader, model);

        //tv
        //cout << s << endl;
        cube.setTextureProperty(aquarium, aquarium, 32.0f);
        a = transform(-2.45, -.4, 1.0, 0, 90, 0, 4.5, 2.2, 0.1);
        cube.drawCubeWithTexture(lightingShaderWithTexture, a);
        //cube.setMaterialisticProperty(glm::vec3(0, 0, 0));
        //cube.drawCubeWithMaterialisticProperty(lightingShader, a);

        //painting
        cube.setTextureProperty(painting_tex3, painting_tex3, 32.0);
        a = transform(-0.35, 0.2, 1.9, 0, 00, 0, 1.4, 1.8, 0.02);
        cube.drawCubeWithTexture(lightingShaderWithTexture, a);

        //painting2
        cube.setTextureProperty(painting_tex2, painting_tex2, 32.0);
        a = transform(0.4, -.0, 1.9, 0, 00, 0, 1.4, 1.8, 0.02);
        cube.drawCubeWithTexture(lightingShaderWithTexture, a);

        //painting3
        cube.setTextureProperty(painting_tex, painting_tex, 32.0);
        a = transform(-1.1, -.0, 1.9, 0, 00, 0, 1.4, 1.8, 0.02);
        cube.drawCubeWithTexture(lightingShaderWithTexture, a);


        //shelf
        model = transform(0, 0, -1, 0, 0, 0, 1, 1, 1);
        shelf(cube, lightingShaderWithTexture, lightingShader, model);


        //waredrove
        model = transform(-5.1, 0, -2, 0, -90, 0, 1, 1, 1);
        waredrove(cube, lightingShaderWithTexture, lightingShader, model,brick_wood);
        if(box)
        {
            for (int i = 0;i<10;i++)
            {
                shamne1 += 0.00013;

                shamne1 = min(shamne1+0.0 , 0.4);
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                shamne1 -= 0.00013;

                shamne1 = max(shamne1 + 0.0, 0.0);
            }
        }



        if (box2)
        {
            for (int i = 0; i < 10; i++)
            {
                shamne2 += 0.00013;

                shamne2 = min(shamne2 + 0.0, 0.4);
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                shamne2 -= 0.00013;

                shamne2 = max(shamne2 + 0.0, 0.0);
            }
        }

        if (box3)
        {
            for (int i = 0; i < 10; i++)
            {
                shamne3 += 0.00013;

                shamne3 = min(shamne3 + 0.0, 0.4);
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                shamne3 -= 0.00013;

                shamne3 = max(shamne3 + 0.0, 0.0);
            }
        }

        if (box4)
        {
            for (int i = 0; i < 10; i++)
            {
                shamne4 += 0.00013;

                shamne4 = min(shamne4 + 0.0, 0.4);
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                shamne4 -= 0.00013;

                shamne4 = max(shamne4 + 0.0, 0.0);
            }
        }

            //box


        //bed_side_table
        model = transform(-4.15, -.45, -0.6, 0, -90, 0, 0.35, 0.4, 0.5);
        bedsidetable(cube, lightingShaderWithTexture, lightingShader, model);

        //door to next room
        if (!doortonext)
        {
            model = transform(0, 0, min(slide,1.0f), 0, 0, 0, 1, 1, 1);
            doortonextroom(cube, lightingShaderWithTexture, lightingShader, model);
        }



        //book
        float r = 0, g = 0, b = 0, x = 0;
        for (int i = 0; i < 7; i++)
        {
            r += 0.2, g += 0.1, b += 0.3;
            model = transform(0, 0, x, 0, 0, 0, 1, 1, 1);
            book(cube, lightingShaderWithTexture, lightingShader, model,r,g,b);
            x += 0.1;
        }
        r = 0, g = 0, b = 0, x = 0;
        for (int i = 0; i < 7; i++)
        {
            r += 0.3, g += 0.2, b += 0.1;
            model = transform(0, -0.9, -0.9 - x, 50, 0, 0, 1, 1, 0.7);
            book(cube, lightingShaderWithTexture, lightingShader, model, r, g, b);
            x += 0.1;
        }
        r = 0, g = 0, b = 0, x = 0;
        for (int i = 0; i < 7; i++)
        {
            r += 0.1, g += 0.2, b += 0.3;
            model = transform(0, -2, -0.9 - x, 50, 0, 0, 1, 1, 0.7);
            book(cube, lightingShaderWithTexture, lightingShader, model, r, g, b);
            x += 0.1;
        }

        //showcase
        model = transform(-5, 0, -3, 0, -90, 0, 1, 1, 3.5);
        showcase(cube, lightingShaderWithTexture, lightingShader, model);


        if (dooropen)
        {
            a = transform(-6.5, 0, -3.5, 0, -90, 0, 1, 1, 1);
            dooropenclose(cube, lightingShaderWithTexture, lightingShader, a*model);
            a = transform(6.5, 0, -3.5, 0, 90, 0, 1, 1, 1);
            dooropenclose2(cube, lightingShaderWithTexture, lightingShader, a*model);
        }
        else
        {
            dooropenclose(cube, lightingShaderWithTexture, lightingShader, model);
            dooropenclose2(cube, lightingShaderWithTexture, lightingShader, model);
        }

        //FAN
        model = transform(-0.1, 1, 0.5, 0, rotate_fan, 0, 0.7, 1, 1);
        Fan(cube, lightingShaderWithTexture, lightingShader, model);
        rotate_fan = rotate_fan + rotatedegree;
        a = transform(-.1, 1.1, 0.5, 0, 0, 0, 0.2, 1.3, 0.2);
        cube.setMaterialisticProperty(glm::vec3(0.451, 0.208, 0.012));
        cube.drawCubeWithMaterialisticProperty(lightingShader,  a);


        model = transform(-4.95, -3.85, -1.43, 20, 0, 0, 1, 1, 1);
        ladder(cube, lightingShaderWithTexture, lightingShader, model);


        //table

        model = transform2(-4.3 , -0.2, -1, 0, 90, 0, 0.4, 0.7, 1.2);
        table2(cube, lightingShaderWithTexture, lightingShader, model);

        model = transform(-3.15, 0, -3.5, 0, 90, 0, 1, 1, 1);
       // dressingtable(cube, lightingShaderWithTexture, lightingShader, model);









        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        

        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);
        dirlight.setUpPointLight(lightingShader);
        spotlight.setUpPointLight(lightingShader);

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.065, 4.86783));
        
        //Table(cube, lightingShader, translateMatrix);


        

        // also draw the lamp object(s)
        
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::vec3 LightColor[] = {
        glm::vec3(0.8f, 0.8f, 0.8f),// White - Dir
        glm::vec3(1.0f,  0.0f,  0.0f),//Red - Spot Light
        glm::vec3(0.0f,  1.0f,  0.0f),//Green - Point Light 1
        glm::vec3(0.0f,  0.0f,  1.0f)//Blue - Point Light 1
        };

        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, LightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
            cube.drawCube(ourShader, model);
        }
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        rotatedegree += 2.0;
        if (rotatedegree == 6.0)
        {
            rotatedegree = 0.0;
            rotate_fan = 0.0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!rotate_around) {
            rotate_around = true;
        }
        else {
            rotate_around = false;
        }
    }



}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (dirLightOn)
        {
            dirlight.turnOff();
            dirLightOn = !dirLightOn;
        }
        else
        {
            dirlight.turnOn();
            dirLightOn = !dirLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn1)
        {
            pointlight1.turnOff();
            pointLightOn1 = !pointLightOn1;
        }
        else
        {
            pointlight1.turnOn();
            pointLightOn1 = !pointLightOn1;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (pointLightOn2)
        {
            pointlight2.turnOff();
            pointLightOn2 = !pointLightOn2;
        }
        else
        {
            pointlight2.turnOn();
            pointLightOn2 = !pointLightOn2;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (spotLightOn)
        {
            spotlight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotlight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOff();
            if (pointLightOn2)
                pointlight2.turnAmbientOff();
            spotlight.turnAmbientOff();
            if (dirLightOn)
                dirlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOn();
            if (pointLightOn2)
                pointlight2.turnAmbientOn();
            if (spotLightOn)
                spotlight.turnAmbientOn();
            if (dirLightOn)
                dirlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOff();
            if (pointLightOn2)
                pointlight2.turnDiffuseOff();
            if (dirLightOn)
                dirlight.turnDiffuseOff();
            if (spotLightOn)
                spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOn();
            if (pointLightOn2)
                pointlight2.turnDiffuseOn();
            if (dirLightOn)
                dirlight.turnDiffuseOn();
            if (spotLightOn)
                spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOff();
            if (pointLightOn2)
                pointlight2.turnSpecularOff();
            if (dirLightOn)
                dirlight.turnSpecularOff();
            if (spotLightOn)
                spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOn();
            if (pointLightOn2)
                pointlight2.turnSpecularOn();
            if (dirLightOn)
                dirlight.turnSpecularOn();
            if (spotLightOn)
                spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        //angle += 1;
        if (!dooropen) {
            dooropen = true;
        }
        else {
            dooropen = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if (!box) {

            box = true;
            //            shamne = min(shamne + 0.0013, 0.4);
        }
        else {
            box = false;
        }
    }


    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        if (!box2) {

            box2 = true;
            //            shamne = min(shamne + 0.0013, 0.4);
        }
        else {
            box2 = false;
        }
    }


    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (!box3) {

            box3 = true;
            //            shamne = min(shamne + 0.0013, 0.4);
        }
        else {
            box3 = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        if (!box4) {

            box4 = true;
            //            shamne = min(shamne + 0.0013, 0.4);
        }
        else {
            box4 = false;
        }
    }
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

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



