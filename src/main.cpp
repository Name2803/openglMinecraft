#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//using namespace glm;

#include "window/window.h"
#include "window/events.h"
#include "graphics/Mesh.h"
#include "graphics/VoxelRenderer.h"
#include "graphics/ShaderCl.h"
#include "graphics/Texture.h"
#include "window/Camera.h"
#include "loaders/png_loading.h"
#include "voxels/voxel.h"
#include "voxels/Chunk.h"
#include "voxels/Chunks.h"

#include "graphics/LineBatch.h"


float vertices[] = {
    // x    y 
    0.0f, -0.01f, 
    0.0f,  0.01f, 
   
    -0.01f, 0.0f, 
     0.01f, 0.0f, 
};


int attrs[] = {
    2, 0
};

const float l_cube_vert[] = {
  //vertices      //color
    0.f, 0.f, 0.f,  1.f, 1.f, 1.f,
    0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
    1.f, 0.f, 1.f,  0.f, 1.f, 1.f,

    1.f, 0.f, 1.f,  0.f, 1.f, 1.f,
    1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
    1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
    0.f, 0.f, 0.f,  1.f, 1.f, 1.f,

    0.f, 0.f, 0.f,  1.f, 1.f, 1.f,
    0.f, 1.f, 0.f,  1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,  1.f, 0.f, 0.f,
    0.f, 1.f, 1.f,  1.f, 1.f, 0.f,
    //----------0
    0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
    0.f, 1.f, 1.f,  1.f, 1.f, 0.f,
    0.f, 1.f, 1.f,  1.f, 1.f, 0.f,
    1.f, 1.f, 1.f,  1.f, 1.f, 1.f,

    1.f, 0.f, 1.f,  0.f, 1.f, 1.f,
    1.f, 1.f, 1.f,  1.f, 1.f, 1.f,
    1.f, 1.f, 1.f,  1.f, 1.f, 1.f,
    1.f, 1.f, 0.f,  1.f, 0.f, 1.f,

    1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
    1.f, 1.f, 0.f,  1.f, 0.f, 1.f,
    1.f, 1.f, 0.f,  1.f, 0.f, 1.f,
    0.f, 1.f, 0.f,  1.f, 0.f, 0.f
};


void EventList(Camera** pcamera, float speed, float delta, float camX, float camY);

bool shouldToDraw = false;

int main(void)
{
    Window::initialize(1280, 720, "Minecraft_C");
    Events::initialize();

    Shader main_shader("../res/main_shader.vs", "../res/main_shader.fs");
    Texture test_texture("../res/block_minecraft.png");

    Shader main_line_shader("../res/main_line_shader.vs", "../res/main_line_shader.fs");
    
    Shader main_voxel_box_shader("../res/main_line_shader.vs", "../res/main_line_shader.fs");

    int arts[] = { 3, 3, 0 };
    Mesh line_cube(l_cube_vert, 36, arts);

    Shader main_crosshair_shader("../res/main_crosshair_shader.vs", "../res/main_crosshair_shader.fs");
    Mesh crosshairMesh(vertices, 4, attrs);

    Camera* camera = new Camera(glm::vec3(0., 5.f, 0.), radians(90.0f));
    VoxelRenderer renderer(1024 * 1024 * 8);
    Chunks chunks(4, 4, 4);
    Mesh** chunksMesh = new Mesh*[chunks.volume];
    for (int i = 0; i < chunks.volume; i++)
    {
        chunksMesh[i] = nullptr;
    }
    
    Chunk* closes[27];
    for (size_t i = 0; i < chunks.volume; i++)
    {
        Chunk* chunk = chunks.chunks[i];
        for (int i = 0; i < 27; i++)
        {
            closes[i] = nullptr;
        }
        for(int j =0;j < chunks.volume; j++)
        {
            Chunk* other = chunks.chunks[j];

            int ox = other->x - chunks.chunks[i]->x;
            int oy = other->y - chunks.chunks[i]->y;
            int oz = other->z - chunks.chunks[i]->z;

            if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1) continue;

            ox += 1;
            oy += 1;
            oz += 1;
            closes[(oy * 3 + oz) * 3 + ox] = other;
        }
        Mesh* mesh = renderer.render(chunk, (const Chunk**)closes, true);
        chunksMesh[i] = mesh;
    }


    LineBatch* voxel_box = new LineBatch(24);
    


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float lastFrame = static_cast<float>(glfwGetTime());
    float deltaTime = 0;

    float camX = 0.0f;
    float camY = 0.0f;

    int temp = 1;


    int attrss [] = { 3, 0 };


    /* Mian Game loop */
    while (!Window::isShouldClose())
    {

        int speed = 13;

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.6f, 0.62f, 0.65f, 1);

        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Events::clicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.8f, 0.4f, 0.2f, 1);
        }

        if (Events::pressed(GLFW_KEY_W)) {
            camera->position += speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_S)) {
            camera->position -= speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_A)) {
            camera->position -= speed * deltaTime * camera->right;
        }
        if (Events::pressed(GLFW_KEY_D)) {
            camera->position += speed * deltaTime * camera->right;
        }
        
        if (Events::jpressed(GLFW_KEY_G)) {
            shouldToDraw = !shouldToDraw;
        }

        if (Events::jpressed(GLFW_KEY_Q)) {
            temp--;
        }
        if (Events::jpressed(GLFW_KEY_E)) {
            temp++;
        }

        if (Events::pressed(GLFW_KEY_SPACE)) {
            camera->position.y += speed * deltaTime * camera->up.y;
        }

        if (Events::pressed(GLFW_KEY_LEFT_SHIFT)) {
            camera->position.y -= speed * deltaTime * camera->up.y;
        }

        if (Events::jpressed(GLFW_KEY_TAB)) {
            Events::toogleCursor();
        }

        /*if (Events::jpressed(GLFW_KEY_T)) {*/
            
        /*}*/


        if (Events::_cursor_locked) {
            camY += -Events::deltaY / Window::height * 2;
            camX += -Events::deltaX / Window::height * 2;

            if (camY < -radians(89.0f)) {
                camY = -radians(89.0f);
            }
            if (camY > radians(89.0f)) {
                camY = radians(89.0f);
            }

            camera->rotation = glm::mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }

        //isolyated block for ray from camera
        {
            vec3 end;       //end vector
            vec3 norm;      //normal vector(for object which is wached)
            vec3 iend;                                                  //ray langh
            voxel* vox = chunks.rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
            if (vox != nullptr) {
                voxel_box->box(iend.x + 0.5f, iend.y + 0.5f, iend.z + 0.5f, 1.005f, 1.005f, 1.005f, 0, 0, 0, 0.5f);

                if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
                    chunks.set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                }
                if (Events::jclicked(GLFW_MOUSE_BUTTON_2)) {
                    chunks.set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), temp);
                }
            }
        }

        Chunk* closes[27];
        for (size_t i = 0; i < chunks.volume; i++)
        {
            Chunk* chunk = chunks.chunks[i];
            if (!chunk->modified) continue;
            chunk->modified = false;

            if (chunksMesh[i] != nullptr)
                delete chunksMesh[i];
           
            for (int i = 0; i < 27; i++)
            {
                closes[i] = nullptr;
            }
            for (int j = 0; j < chunks.volume; j++)
            {
                Chunk* other = chunks.chunks[j];

                int ox = other->x - chunks.chunks[i]->x;
                int oy = other->y - chunks.chunks[i]->y;
                int oz = other->z - chunks.chunks[i]->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1) continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;
            }
            Mesh* mesh = renderer.render(chunk, (const Chunk**)closes, true);
            chunksMesh[i] = mesh;
        }

        

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.4f, 0.8f, 1);

        // Draw VAO
        main_shader.use();
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        main_shader.setMat4("projection", camera->getProjection());
        main_shader.setMat4("view", camera->getView());
        test_texture.bind();

        for (int i = 0; i < chunks.volume; i++)
        {
            main_shader.use();
            model = glm::mat4(1.f);
            model = translate(model, glm::vec3(chunks.chunks[i]->x * CHUNK_W + 0.5f, chunks.chunks[i]->y * CHUNK_H + 0.5f, chunks.chunks[i]->z * CHUNK_D + 0.5f));
            main_shader.setMat4("model", model);
            chunksMesh[i]->draw(GL_TRIANGLES);

            if (shouldToDraw) {
                main_line_shader.use();
                model = glm::mat4(1.0f);
                main_line_shader.setMat4("projection", camera->getProjection());
                main_line_shader.setMat4("view", camera->getView());
                chunks.chunks[i]->ChunkBoxDrawer(shouldToDraw, main_line_shader);
            }
            main_voxel_box_shader.use();
            main_voxel_box_shader.setMat4("projection", camera->getProjection());
            main_voxel_box_shader.setMat4("view", camera->getView());
            main_voxel_box_shader.setMat4("model", glm::mat4(1.f));
            voxel_box->render();
        }

        main_crosshair_shader.use();
        crosshairMesh.draw(GL_LINES);
        std::cout << camera->front.x << "\t" << camera->front.y << "\t" << camera->front.z << "\n";
       

        Window::swapBuffers();
        Events::pullEvents();
    }

    
    
    //Window::terminate();
    return 0;
}