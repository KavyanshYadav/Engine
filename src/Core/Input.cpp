#include "Input.h"
#include "Renderer.h"

bool Input::firstMouse = true;
float Input::lastX = 400.0f;
float Input::lastY = 300.0f;
float Input::mouseSensitivity = 0.1f;

Input::Input(Renderer* renderer) : renderer(renderer)
{

}

Input::~Input(){
    delete(renderer);
}

void Input::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        // Get window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Adjust coordinates for viewport offset
        mouseX -= *renderer->PanelX;
        mouseY -= *renderer->PanelY;
        width -= *renderer->PanelX;
        height -= *renderer->PanelY;

        // Convert to normalized device coordinates (-1 to 1)
        float x = (2.0f * mouseX) / width - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / height;
        
        Scene* scene = renderer->getActiveScene();
        glm::mat4 projection = scene->GetProjectionMatrix();
        glm::mat4 view = scene->GetViewMatrix();
        
        // Create ray in clip space
        glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
        
        // Convert to eye space
        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        
        // Convert to world space
        glm::vec4 rayWorld = glm::inverse(view) * rayEye;
        glm::vec3 rayDir = glm::normalize(glm::vec3(rayWorld));
        
        // Get camera position as ray origin
        glm::vec3 rayOrigin = scene->GetCameraPosition();
        
        // Debug output
        std::cout << "Mouse: " << mouseX << ", " << mouseY << std::endl;
        std::cout << "Ray Origin: " << rayOrigin.x << ", " << rayOrigin.y << ", " << rayOrigin.z << std::endl;
        std::cout << "Ray Dir: " << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << std::endl;
        
        // Perform picking
        SceneObject* picked = scene->PickMesh(rayOrigin, rayDir);
        if (picked) {
            scene->SetActiveMesh(picked);
            std::cout << "Picked mesh at " << picked << std::endl;
        } else {
            scene->SetActiveMesh(nullptr);
            std::cout << "No mesh picked" << std::endl;
        }
    }
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Get the user pointer which should be set to the Renderer
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        // Adjust zoom based on scroll direction
        // yoffset is positive when scrolling up/away from user
        float zoomDelta = static_cast<float>(yoffset) * 0.1f;  // Adjust sensitivity as needed
        renderer->getActiveScene()->AdjustCameraZoom(zoomDelta);
    }
}

void Input::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        return;  // Only rotate when right mouse button is pressed
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->getActiveScene()->RotateCamera(xoffset * mouseSensitivity, yoffset * mouseSensitivity);
    }
}

void Input::ProcessInput(GLFWwindow* window) {
    glm::vec3 movement(0.0f);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        movement.y -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        movement.y += 1.0f;

    if (movement != glm::vec3(0.0f)) {
        renderer->getActiveScene()->MoveCamera(glm::normalize(movement));
    }
}

void Input::SetupCallbacks(GLFWwindow* window) {
    // Set up scroll callback
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetCursorPosCallback(window, MouseMoveCallback);
}
