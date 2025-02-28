#include "Input.h"

Input::Input(Renderer * renderer) : renderer(renderer)
{

}

Input::~Input(){
    delete(renderer);
}
void Input::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        std::cout <<this->renderer->getActiveScene()->GetSceneNodes().front() <<std::endl;
        // glm::vec3 rayDirection = ScreenToWorldRay(mouseX, mouseY, screenWidth, screenHeight, viewMatrix, projectionMatrix);
        // Mesh* selectedMesh = PickMesh(cameraPosition, rayDirection, sceneMeshes);

        // if (selectedMesh) {
        //     std::cout << "Mesh selected!" << std::endl;
        // }
        std::cout << mouseX << " " << mouseY << std::endl ;
    }
}
