#include "Tema1.h"

#include <vector>
#include <iostream>
#include <cmath>

#include <Core/Engine.h>
#include "Transform2D1.h"
#include "Object2D1.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);


    glm::vec3 corner = glm::vec3(0, 0, 0);
    arrowCoordinates = glm::vec2(100, 350);
    archCoordinates = glm::vec2(100, 350);
    baloonCoordinates = glm::vec2(0, -100);
    shurikCoordinates = glm::vec2(resolution.x, 100);

    arrow_len = 100;

    stop_dragging = -100;
    drag = 0;
    scale = 1;
    count = 0;

    arch_len = 180;
    press_left_click = 0;
    move_forward = 0;

    something = 1;

    score = 0;
    lifes = 3;
    shown = 0;

    // vectori ce memoreaza pozitiile baloanelor si shuriken-elor
    for (int i = 0; i < 9; i++) {
        redBaloons.push_back(400 + i*100);
        redBaloons.push_back(-(100+ rand() % 1999));
    }

    for (int i = 0; i < 9; i++) {
        yellowBaloons.push_back(410 + i * 100);
        yellowBaloons.push_back(-(100 + rand() % 1789));
    }

    for (int i = 0; i < 3; i++) {
        shurikens.push_back(resolution.x + i * 160);
        shurikens.push_back(100 + rand() % 720 );

        //vector care numara coliziunile pentru fiecare shuriken cu arcul
        shurikCollCount.push_back(0);
    }

    direction = 0;

    // crearea tuturor mesh-urilor
    Mesh* arrow = Object2D1:: CreateArrow("arrow", corner, arrow_len, glm::vec3(0.420, 0.557, 0.13));
    AddMeshToList(arrow);

    Mesh* shurik = Object2D1::CreateShuriken("shurik", corner, 100, glm::vec3(0.824, 0.412, 0.118));
    AddMeshToList(shurik);

    Mesh* baloon = Object2D1::CreateBaloon("red_baloon", corner, 100, 130, 100, glm::vec3(0.9, 0, 0));
    AddMeshToList(baloon);

    Mesh* ybaloon = Object2D1::CreateBaloon("yellow_baloon", corner, 100, 130, 100, glm::vec3(1, 0.902, 0));
    AddMeshToList(ybaloon);

    Mesh* arch = Object2D1::CreateArch("arch", corner, 130, arch_len, 100, glm::vec3(1, 1, 1));
    AddMeshToList(arch);

    Mesh* rectangle = Object2D1::CreateRectangle("score_r", corner, 5, 200, glm::vec3(0.690, 0.878, 0.902), true);
    AddMeshToList(rectangle);

    Mesh* life = Object2D1::CreateBaloon("life", corner, 50, 60, 100, glm::vec3(0.698, 0.133, 0.133));
    AddMeshToList(life);

    
    Mesh* f_rec = Object2D1::CreateRectangle("rfin", corner, 400, 800, glm::vec3(0.502, 0.502, 0.000), false);
    AddMeshToList(f_rec);
    

    Mesh* G = Object2D1::StrG("g", corner, 120,glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(G);

    Mesh* A = Object2D1::StrA("a", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(A);

    Mesh* M = Object2D1::StrM("m", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(M);

    Mesh* E = Object2D1::StrE("e", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(E);

    Mesh* O = Object2D1::StrO("o", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(O);

    Mesh* V = Object2D1::StrV("v", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(V);

    Mesh* R = Object2D1::StrR("r", corner, 120, glm::vec3(0.502, 0.502, 0.000));
    AddMeshToList(R);
}

void Tema1::FrameStart()
{
    glClearColor(0.902, 0.902, 0.980, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    // calculeaza unghiul format intre vectorii formati intre directia mouse-ului si ale arcului
    radians = atan2(( mouseCoordinates.y - archCoordinates.y ) ,  mouseCoordinates.x - archCoordinates.x );

    // deseneaza balonasele ce sugereaza vietile ramase
    {
        for (int j = 0; j < lifes; j++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D1::Translate(70 + j * 60,680);
            RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
        }
    }
    
    if (lifes > 0) {
        // deseneaza bara de viteza
        {
            if (press_left_click != 0)
            {
                scale++;
                modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D1::Scale(scale * 0.2, 5);
                RenderMesh2D(meshes["score_r"], shaders["VertexColor"], modelMatrix);
            }
        }

        // deseneaza si directioneaza
        {
            modelMatrix = glm::mat3(1);

            // daca s-a oprit din tras, s-a eliberat butonul stang al mouse-ului
            if (stop_dragging == 1) {
                move_forward = drag;
                arrowCoordinates += move_forward * glm::normalize(mouseCoordinates - archCoordinates) * deltaTimeSeconds;
                scale = 1;
            }

            // daca sageata a iesit din coordonatele ecranului, atunci mareste "fortat" coordonatele sagetii
            if (arrowCoordinates.x < 0 || arrowCoordinates.x > resolution.x ||
                arrowCoordinates.y < 0 || arrowCoordinates.y > resolution.y) {
                arrowCoordinates.x = resolution.x + 1;
            }

            // daca e la inceput sau sageata a iesit din scena
            if (arrowCoordinates.x > resolution.x || arrowCoordinates.x < 0 || arrowCoordinates.y >resolution.y) {
                stop_dragging = -100;
                drag = 0;
                arrowCoordinates.x = archCoordinates.x;
                arrowCoordinates.y = archCoordinates.y;
                count = 0;
            }
            modelMatrix *= Transform2D1::Translate(arrowCoordinates.x, arrowCoordinates.y);
            modelMatrix *= Transform2D1::Rotate((radians));
            RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
        }

        // deseneaza arcul
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D1::Translate(archCoordinates.x, archCoordinates.y);
            modelMatrix *= Transform2D1::Rotate(RADIANS(180));
            modelMatrix *= Transform2D1::Translate(0, (arrow_len / 200));
            modelMatrix *= Transform2D1::Rotate((radians));
            modelMatrix *= Transform2D1::Translate(0, -(arrow_len / 200));
            RenderMesh2D(meshes["arch"], shaders["VertexColor"], modelMatrix);
        }

        // deseneaza shuriken-ul
        {

            for (int j = 0; j < 3; j++) {

                shurikens[2 * j] -= deltaTimeSeconds * 200;

                // daca a iesit din scena
                if (shurikens[2 * j] <= -200) {
                    shurikens[2 * j] = shurikCoordinates.x;
                    shurikens[2 * j + 1] = 1 + rand() % 720;

                    // daca s-a produs deja o coliziune cu arcul, reseteaza valoarea
                    shurikCollCount[j] = 0;
                }

                modelMatrix = glm::mat3(1);
                something += deltaTimeSeconds * 200;
                modelMatrix *= Transform2D1::Translate(shurikens[2 * j], shurikens[2 * j + 1]);
                modelMatrix *= Transform2D1::Rotate(RADIANS(something));
                RenderMesh2D(meshes["shurik"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
    else {
        // deseneaza scena finala GAME OVER

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(350, 385);
        RenderMesh2D(meshes["g"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(500, 385);
        RenderMesh2D(meshes["a"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(650, 385);
        RenderMesh2D(meshes["m"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(800, 385);
        RenderMesh2D(meshes["e"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(360, 185);
        RenderMesh2D(meshes["o"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(500, 185);
        RenderMesh2D(meshes["v"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(655, 185);
        RenderMesh2D(meshes["e"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(781, 185);
        RenderMesh2D(meshes["r"], shaders["VertexColor"], modelMatrix);
   
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D1::Translate(resolution.x / 2 - 420, resolution.y / 2 - 180);
        RenderMesh2D(meshes["rfin"], shaders["VertexColor"], modelMatrix);

        if (shown == 0) {
            std::cout << "\n\n\n";
            std::cout << " ______________________________________________\n";
            std::cout << "|                                              |\n";
            std::cout << "|       C O N G R A T U L A T I O N S          |\n";
            std::cout << "|                                              |\n";
            std::cout << "|       Your final score is : " << score << "              |\n";
            std::cout << "|______________________________________________|\n";
            shown = 1;
        }
    }

    
    int x = 0;
    int y = 0;
    //deseneaza baloanele rosii
    {        
        for (int j = 0; j < 9; j++) {

            // calculeaza factorul de viteza
            if (j <= 4) {
                factor = 2 + j / 6;
            }
            else {
                factor = 1.2 + j / 6;
            }
            redBaloons[2 * j + 1] += deltaTimeSeconds * 100 * factor;
            
            if (redBaloons[2 * j + 1] - 100 >= resolution.y) {
                
                redBaloons[2 * j + 1] = (baloonCoordinates.y - rand() % 689) * factor;
            }

            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D1::Translate(redBaloons[2 * j], redBaloons[2 * j + 1]);
            RenderMesh2D(meshes["red_baloon"], shaders["VertexColor"], modelMatrix);
        }
    }

    //deseneaza baloanele galbene
    {
        for (int j = 0; j < 9; j++) {
            factor = 1.2 + j / 4;
            yellowBaloons[2 * j + 1] += deltaTimeSeconds * 100 * factor;;

            if (yellowBaloons[2 * j + 1] - 100 >= resolution.y) {

                yellowBaloons[2 * j + 1] = (baloonCoordinates.y - rand() % 795) * factor;
            }

            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D1::Translate(yellowBaloons[2 * j], yellowBaloons[2 * j + 1]);
            RenderMesh2D(meshes["yellow_baloon"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        glm::vec3 archCollision = { archCoordinates.x, archCoordinates.y, arch_len / 2 };

        // coliziunea cu baloanele rosii
        glm::vec3 redBaloonCollision;
        for (int j = 0; j < 9; j++) {
            
            redBaloonCollision.x = redBaloons[2 * j];
            redBaloonCollision.y = redBaloons[2 * j + 1];
            redBaloonCollision.z = 65;

            int dx = arrowCoordinates.x - redBaloonCollision.x;
            int dy = arrowCoordinates.y - redBaloonCollision.y;

            float distance = sqrt(pow(dx, 2) + pow(dy, 2));

            // spargerea balonului rosu adauga 10 puncte
            if (distance < redBaloonCollision.z) {
                redBaloons[2 * j + 1] = -100;
                score += 10;

                std::cout << "S C O R E  :  " << score << "\n";
            }
        }

        // coliziunea cu baloanele galbene
        glm::vec3 yellowBaloonCollision;
        for (int j = 0; j < 9; j++) {
            yellowBaloonCollision.x = yellowBaloons[2 * j];
            yellowBaloonCollision.y = yellowBaloons[2 * j + 1];
            yellowBaloonCollision.z = 65;
            
            int dx = arrowCoordinates.x - yellowBaloonCollision.x;
            int dy = arrowCoordinates.y - yellowBaloonCollision.y;

            float distance = sqrt(pow(dx, 2) + pow(dy, 2));

            // la spargerea balonului galben scorul scade cu doua unitati
            if (distance < yellowBaloonCollision.z && shown == 0) {
                yellowBaloons[2 * j + 1] = -100;
                score -= 2;

                std::cout << "S C O R E  :  " << score << "\n";
            }
        }

        //coliziunile cu shurikenul
        glm::vec3 shurikCollision;
        for (int j = 0; j < 3; j++) {
            shurikCollision.x = shurikens[2 * j];
            shurikCollision.y = shurikens[2 * j + 1];
            shurikCollision.z = 100;

            // coliziunea arc shuriken
            int dx = archCollision.x - shurikCollision.x;
            int dy = archCollision.y - shurikCollision.y;

            float distance = sqrt(pow(dx, 2) + pow(dy, 2));

            //la coliziunea arcului cu shuriken-ul se scad 5 puncte
            if (distance < shurikCollision.z + archCollision.z && shurikCollCount[j] == 0 ) {
                lifes--;
                score -= 5;
                shurikCollCount[j] = 1;

                std::cout << "S C O R E  :  " << score << "\n";
            }


            // coliziune sageata shuriken
            if (arrowCoordinates.x != archCoordinates.x) {
                dx = arrowCoordinates.x - shurikCollision.x;
                dy = arrowCoordinates.y - shurikCollision.y;

                distance = sqrt(pow(dx, 2) + pow(dy, 2));

                // coliziunea sagetii cu shuriken-ul adauga 15 puncte scorului
                if (distance < shurikCollision.z ) {
                    shurikens[2 * j] = resolution.x + rand() % 200;
                    shurikens[2 * j + 1] = 1 + rand() % 720;
                    score += 15;

                    std::cout << "S C O R E  :  " << score << "\n";
                }
            }
        }

    }

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    int cons = 10;
    
    if (window->KeyHold(GLFW_KEY_W)) {
        
        if (archCoordinates.y < resolution.y - arch_len/2 - cons)
        {
            archCoordinates.y += 150 * deltaTime;
            arrowCoordinates.y += 150 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (archCoordinates.y > arch_len / 2 + cons)
        {
            archCoordinates.y -= 150 * deltaTime;
            arrowCoordinates.y -= 150 * deltaTime;
        }
    }
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
    {
        drag += 30;
        press_left_click = 1;
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    // add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (stop_dragging != 1) {
        mouseCoordinates = glm::vec2(mouseX, -mouseY + resolution.y);
    }
 }

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // add mouse button release event
   if ( IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
       press_left_click = 0;
       stop_dragging = 1;
       move_forward = 1;
    }
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
