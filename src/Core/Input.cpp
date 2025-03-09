#include "Input.h"
#include "Renderer.h"
#include "Logger.h"

bool Input::firstMouse = true;
float Input::lastX = 400.0f;
float Input::lastY = 300.0f;
float Input::mouseSensitivity = 0.1f;
float Input::moveSpeed = 2.5f;  // Units per second
float Input::lastFrame = 0.0f;  // For frame-independent movement

Input* Input::instance = nullptr;

Input::Input(Renderer* renderer) : renderer(renderer) {
    instance = this;  // Store instance pointer
}

Input::~Input(){
    // Don't delete renderer here as it's managed elsewhere
}

void Input::HandleObjectSelection(GLFWwindow* window, int button, int action, int mods) {
    auto& logger = Logger::GetInstance();
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (!renderer) {
        logger.Critical("Renderer instance is null!");
        return;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        // Get window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Get viewport dimensions
        int viewportX = *renderer->PanelX;
        int viewportY = *renderer->PanelY;
        int viewportWidth = width - viewportX;
        int viewportHeight = height - viewportY;

        // Check if click is within viewport
        if (mouseX < viewportX || mouseX > width || 
            mouseY < viewportY || mouseY > height - viewportY) {
            logger.Debug("Click outside viewport");
            return;
        }

        // Adjust coordinates for viewport offset
        mouseX -= viewportX;
        mouseY -= viewportY;

        // Convert to normalized device coordinates (-1 to 1)
        float x = (2.0f * mouseX) / viewportWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / viewportHeight;
        
        Scene* scene = renderer->getActiveScene();
        
        // Debug: Print number of objects in scene
        auto sceneNodes = scene->GetSceneNodes();
        logger.Debug("Number of objects in scene: ", sceneNodes.size());
        
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
        logger.Debug("Ray Casting Debug:");
        logger.Debug("Click position (viewport): ", mouseX, ", ", mouseY);
        logger.Debug("NDC Coordinates: ", x, ", ", y);
        logger.Debug("Ray Origin: ", rayOrigin.x, ", ", rayOrigin.y, ", ", rayOrigin.z);
        logger.Debug("Ray Direction: ", rayDir.x, ", ", rayDir.y, ", ", rayDir.z);
        
        // Debug: Print all object positions
        logger.Debug("Scene Objects:");
        for (auto* obj : sceneNodes) {
            Mesh* mesh = static_cast<Mesh*>(obj);
            glm::vec3 pos = mesh->GetPosition();
            glm::vec3 scale = mesh->GetScale();
            logger.Debug("Object at position: ", pos.x, ", ", pos.y, ", ", pos.z, 
                        " with scale: ", scale.x, ", ", scale.y, ", ", scale.z);
        }
        
        // Perform picking
        SceneObject* picked = scene->PickMesh(rayOrigin, rayDir);
        if (picked) {
            scene->SetActiveMesh(picked);
            logger.Info("Picked mesh at ", picked);
            
            // Get picked object's position for debugging
            Mesh* mesh = static_cast<Mesh*>(picked);
            glm::vec3 pos = mesh->GetPosition();
            logger.Debug("Picked mesh position: ", pos.x, ", ", pos.y, ", ", pos.z);
        } else {
            scene->SetActiveMesh(nullptr);
            logger.Info("No mesh picked - Ray missed all objects");
        }
        
        logger.Debug("------------------------");
    }
}

void Input::HandleCameraZoom(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        float zoomDelta = static_cast<float>(yoffset) * 0.1f;
        renderer->getActiveScene()->AdjustCameraZoom(zoomDelta);
    }
}

void Input::HandleCameraRotation(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        firstMouse = true;
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
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

void Input::UpdateCameraMovement(GLFWwindow* window) {
    // Calculate delta time
    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    glm::vec3 movement(0.0f);
    float speed = moveSpeed * deltaTime; // Frame-independent movement speed
    
    Scene* scene = instance->renderer->getActiveScene();
    glm::vec3 cameraPos = scene->GetCameraPosition();
    glm::vec3 cameraTarget = scene->GetCameraTarget();
    glm::vec3 forward = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Forward/Backward movement along camera direction
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= forward;

    // Left/Right movement perpendicular to camera direction
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += right;

    // Up/Down movement along world up vector
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        movement -= up;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        movement += up;

    // Sprint modifier
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed *= 2.0f;

    // Apply movement if any key was pressed
    if (movement != glm::vec3(0.0f)) {
        movement = glm::normalize(movement) * speed;
        scene->MoveCamera(movement);
    }

    // Handle escape key to close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Input::InitializeInputCallbacks(GLFWwindow* window) {
    auto& logger = Logger::GetInstance();
    logger.Info("Initializing input callbacks...");
    
    // Set up callbacks - note we're not setting window user pointer here anymore
    glfwSetScrollCallback(window, HandleCameraZoom);
    glfwSetCursorPosCallback(window, HandleCameraRotation);
    glfwSetMouseButtonCallback(window, HandleObjectSelection);
    
    logger.Info("Callbacks initialized successfully");
}
