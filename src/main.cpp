#include <imgui.h>
#include <math.h>
#include <cstddef>
#include <cstdlib>
#include "glm/fwd.hpp"
#include "texture.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <VAO.hpp>
#include "../src-common/glimac/FreeFlyCamera.hpp"
#include "../src-common/glimac/TrackballCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "Boid.hpp"
#include "GroupBoid.hpp"
#include "VBO.hpp"
#include "doctest/doctest.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "object.hpp"
#include "random.hpp"

float cohesion        = 0.3f;
float separation      = 0.15f;
float forceCohesion   = 10.f;
float forceSeparation = 1.f;
float mur             = 0.2f;
float forceMur        = 1.f;
float alignement      = 0.3f;
float forceAlignement = 50.f;
int   nbrBoids        = 40;
float size            = 0.02f;
float intensity       = 03.f;

void sliders()
{
    ImGui::Begin("facteurs");
    ImGui::SliderFloat("cohesion", &cohesion, 0.0f, 1.0f);
    ImGui::SliderFloat("separation", &separation, 0.0f, 1.0f);
    ImGui::SliderFloat("force cohesion", &forceCohesion, 0.0f, 100.0f);
    ImGui::SliderFloat("force separation", &forceSeparation, 0.0f, 100.0f);
    ImGui::SliderFloat("distance au mur", &mur, 0.0f, 1.0f);
    ImGui::SliderFloat("force du mur", &forceMur, 0.0f, 100.0f);
    ImGui::SliderFloat("alignement", &alignement, 0.0f, 1.0f);
    ImGui::SliderFloat("force alignement", &forceAlignement, 0.0f, 100.0f);
    ImGui::End();

    ImGui::Begin("Boids");
    ImGui::InputInt("Number of boids wanted", &nbrBoids);
    ImGui::SliderFloat("size", &size, 0.0f, 0.1f);
    ImGui::SliderFloat("intensity light", &intensity, 0.0f, 5.f);
    ImGui::End();
}

int main()
{
    GroupBoid group(nbrBoids);
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    /************ CAMERA ***************/
    TrackballCamera trackBallCamera;
    FreeFlyCamera   personCamera;

    /************ SHADER ************/
    const p6::Shader shader = p6::load_shader(
        "../src/shaders/3D.vs.glsl",
        "../src/shaders/tex3D.fs.glsl"
    );

    shader.use();

    /************ OBJECTS **************/
    GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    GLint uTexture = glGetUniformLocation(shader.id(), "uTexture");

    Object objBoid;
    objBoid.setInputfile("jellyfishvf");
    objBoid.load();

    Object cube;
    cube.setInputfile("cube");
    cube.load();

    Object person;
    person.setInputfile("hollowknight");
    person.load();

    Object decor;
    decor.setInputfile("decor");
    decor.load();

    /************ TEXTURE **************/
    Texture textureBoid;
    textureBoid.bind();
    textureBoid.set("jellyfishText");
    textureBoid.debind();

    Texture textureCube;
    textureCube.bind();
    textureBoid.set("cube");
    textureBoid.debind();

    Texture texturePerson;
    texturePerson.bind();
    texturePerson.set("HKText");

    Texture textureDecor;
    textureDecor.bind();
    textureDecor.set("textureDecor");

    /************* LIGHTS **************/

    // directionnelle
    GLint uKdDir             = glGetUniformLocation(shader.id(), "uKdDir");
    GLint uKsDir             = glGetUniformLocation(shader.id(), "uKsDir");
    GLint uShininessDir      = glGetUniformLocation(shader.id(), "uShininessDir");
    GLint uLightDir_vs       = glGetUniformLocation(shader.id(), "uLightDir_vs");
    GLint uLightIntensityDir = glGetUniformLocation(shader.id(), "uLightIntensityDir");

    // ponctuelle
    GLint uLightPos_vs       = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLint uKdPos             = glGetUniformLocation(shader.id(), "uKdPos");
    GLint uKsPos             = glGetUniformLocation(shader.id(), "uKsPos");
    GLint uShininessPos      = glGetUniformLocation(shader.id(), "uShininessPos");
    GLint uLightIntensityPos = glGetUniformLocation(shader.id(), "uLightIntensityPos");

    // person
    GLint uLightPerson_vs       = glGetUniformLocation(shader.id(), "uLightPerson_vs");
    GLint uKdPerson             = glGetUniformLocation(shader.id(), "uKdPerson");
    GLint uKsPerson             = glGetUniformLocation(shader.id(), "uKsPerson");
    GLint uShininessPerson      = glGetUniformLocation(shader.id(), "uShininessPerson");
    GLint uLightIntensityPerson = glGetUniformLocation(shader.id(), "uLightIntensityPerson");

    glEnable(GL_DEPTH_TEST);

    // VBO
    VBO vboBoid;
    vboBoid.bind();
    vboBoid.sendData(objBoid.getVertices());
    vboBoid.debind();

    VBO vboCube;
    vboCube.bind();
    vboCube.sendData(cube.getVertices());
    vboCube.debind();

    VBO vboPerson;
    vboPerson.bind();
    vboPerson.sendData(person.getVertices());
    vboPerson.debind();

    VBO vboDecor;
    vboDecor.bind();
    vboDecor.sendData(decor.getVertices());
    vboDecor.debind();

    // VAO

    static constexpr GLuint vertex_attr_position  = 0;
    static constexpr GLuint vertex_attr_normal    = 1;
    static constexpr GLuint vertex_attr_texCoords = 2;

    VAO vaoBoid;
    vaoBoid.bind();

    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vboBoid.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vboBoid.debind();

    vaoBoid.debind();

    VAO vaoCube;
    vaoCube.bind();

    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vboCube.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vboCube.debind();

    vaoCube.debind();

    VAO vaoPerson;
    vaoPerson.bind();

    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vboPerson.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vboPerson.debind();

    vaoPerson.debind();

    VAO vaoDecor;
    vaoDecor.bind();

    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vboDecor.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vboDecor.debind();

    vaoDecor.debind();

    // last position mouse
    float lastX = 0;
    float lastY = 0;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ctx.background(p6::hex(000513));

        /************ CAMERA ***************/
        ctx.mouse_dragged = [&](p6::MouseDrag drag) {
            float deltaX = drag.position.x - lastX;
            float deltaY = drag.position.y - lastY;

            if (lastX != 0 && lastY != 0)
            {
                personCamera.rotateLeft(-deltaX * 50.f);
                personCamera.rotateUp(deltaY * 50.f);
            }

            lastX = drag.position.x;
            lastY = drag.position.y;
        };

        glm::vec2 mousePos = ctx.mouse_delta();
        trackBallCamera.rotateLeft(-mousePos.x * 50.f);
        trackBallCamera.rotateUp(mousePos.y * 50.f);

        if (!ctx.mouse_button_is_pressed(p6::Button::Right))
        {
            lastX = 0;
            lastY = 0;
        }

        // camera autour d'un object
        ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
            trackBallCamera.moveFront(-scroll.dy);
        };
        // camera libre
        // avancer
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            personCamera.moveFront(0.01f);
        }
        // gauche
        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            personCamera.moveLeft(0.01f);
        }
        // reculer
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            personCamera.moveFront(-0.01f);
        }
        // droite
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            personCamera.moveLeft(-0.01f);
        }

        /*** VIEW & PROJ MATRIX ***/

        glm::mat4       viewMatrixTrackball = trackBallCamera.getViewMatrix();
        glm::mat4       viewMatrix          = personCamera.getViewMatrix();
        const glm::mat4 projMatrix          = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        /******* LIGHTS *******/

        // lumière directionnelle
        glm::vec4 lightDir{1.f, 1.f, 1.f, 0.f};
        glm::vec3 lightDir_vs = glm::vec3(viewMatrix * lightDir);

        glUniform3f(uKdDir, 0.f, 0.f, 0.f);
        glUniform3f(uKsDir, 2.f, 2.f, 2.f);
        glUniform1f(uShininessDir, 4.f);
        glUniform3f(uLightDir_vs, lightDir_vs.x, lightDir_vs.y, lightDir_vs.z);
        glUniform3f(uLightIntensityDir, 4.f, 4.f, 1.4f);

        // lumière ponctuelle
        glm::vec4 lightPos{0.2f, -0.1f, -0.5f, 1.f};
        glm::vec3 lightPos_vs = glm::vec3(viewMatrix * lightPos);

        glUniform3f(uKdPos, 2.f, 2.f, 2.f);
        glUniform3f(uKsPos, 2.f, 2.f, 2.f);
        glUniform1f(uShininessPos, 5.f);
        glUniform3f(uLightPos_vs, lightPos_vs.x, lightPos_vs.y, lightPos_vs.z);
        glUniform3f(uLightIntensityPos, 0.8f, 1.f, 1.5f);

        // lumière person
        glm::vec4 lightPerson{0.01, 0., -0.2, 1.};
        auto      lightPerson_vs = glm::vec3(lightPerson);

        glUniform3f(uKdPerson, 0.2f, 0.2f, 0.2f);
        glUniform3f(uKsPerson, 0.f, 0.f, 0.f);
        glUniform1f(uShininessPerson, 2.f);
        glUniform3f(uLightPerson_vs, lightPerson_vs.x, lightPerson_vs.y, lightPerson_vs.z);

        glUniform3f(uLightIntensityPerson, intensity, intensity, intensity);

        /************ DECOR ************/

        vaoDecor.bind();

        textureDecor.send(uTexture);
        textureDecor.bind();

        // matrix model
        glm::mat4 modelMatrix = glm::translate(glm::mat4{1.}, {0., -1., -0.});
        modelMatrix           = glm::rotate(modelMatrix, 0.5f, {0., 1., 0.});
        modelMatrix           = glm::scale(modelMatrix, glm::vec3(0.07f));

        // matrix model-vue
        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        // matrix MVP
        glm::mat4 MVPMatrix = projMatrix * modelViewMatrix;

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        decor.draw();

        vaoDecor.debind();

        /*********** CUBE ***********/

        vaoCube.bind();

        textureCube.send(uTexture);
        textureCube.bind();

        // matrix model
        modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));

        // matrix model-vue
        modelViewMatrix = viewMatrix * modelMatrix;

        // matrix MVP
        MVPMatrix = projMatrix * modelViewMatrix;

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        // draw
        cube.draw();

        vaoCube.debind();

        /************ PERSON ************/

        vaoPerson.bind();

        texturePerson.send(uTexture);
        texturePerson.bind();

        // matrix model

        modelMatrix = glm::inverse(viewMatrix);
        modelMatrix = glm::translate(modelMatrix, {0., -0.05, -0.12});
        modelMatrix = glm::rotate(modelMatrix, 1.5f, {0., 1., 0.});
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));

        // matrix model-vue
        modelViewMatrix = viewMatrix * modelMatrix;

        // matrix MVP
        MVPMatrix = projMatrix * modelViewMatrix;

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        person.draw();

        vaoPerson.debind();

        /*********** BOIDS *************/
        int length = group.getSize();
        if (nbrBoids > length)
        {
            group.addBoid(1);
        }

        if (nbrBoids < length)
        {
            group.removeBoid(1);
        }

        vaoBoid.bind();

        textureBoid.send(uTexture);
        textureBoid.bind();

        sliders();
        group.moveBoids(cohesion, separation, forceCohesion, forceSeparation, mur, forceMur, alignement, forceAlignement);

        for (Boid& boid : group.getGroup())
        {
            /**********MATRICES BOID**********/
            // matrix model
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), boid.getPosition());
            modelMatrix           = glm::scale(modelMatrix, glm::vec3(size));

            // matrix model-vue
            glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

            // matrix MVP
            glm::mat4 MVPMatrix = projMatrix * modelViewMatrix;

            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

            // draw boid
            objBoid.draw();
        }

        vaoBoid.debind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}