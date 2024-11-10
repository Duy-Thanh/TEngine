#if defined(__MINGW32__) && !defined(__MINGW64__)
    #undef WINVER
    #define WINVER 0x0601
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601
    #endif
    #error This build is not compatible with 32-bit OS and 32-bit compiler. Please use 64-bit OS and 64-bit compiler instead.
#elif defined(__MINGW64__)
    #undef WINVER
    #define WINVER 0x0601
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601
    #endif
    #include <windows.h>
#endif

#include "BuildDate.h"

#include <immintrin.h>  // For _mm_lfence()

#include <deque>
#include <unordered_set>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_freetype.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Easing {
    float linear(float t) { 
        return t; 
    }
    
    float easeInQuad(float t) { 
        return t * t; 
    }
    
    float easeOutQuad(float t) { 
        return t * (2.0f - t); 
    }
    
    float easeInOutQuad(float t) {
        return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
    }
    
    float easeInCubic(float t) { 
        return t * t * t; 
    }
    
    float easeOutCubic(float t) { 
        float t1 = t - 1.0f;
        return 1.0f + t1 * t1 * t1;
    }
    
    float easeInOutCubic(float t) {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }
}

struct CameraShot {
    enum class Type {
        Static,     // Fixed position
        Dolly,      // Forward/backward movement
        Pan,        // Horizontal rotation
        Tilt,       // Vertical rotation
        Crane,      // Vertical movement
        Track,      // Parallel movement
        Orbit,      // Circular movement
        Custom      // Custom path
    };
    
    Type type;
    float startTime;
    float duration;
    glm::vec3 startPos;
    glm::vec3 endPos;
    glm::vec3 startRotation;
    glm::vec3 endRotation;
    float easeInDuration = 0.5f;
    float easeOutDuration = 0.5f;
    bool smoothTransition = true;
    std::function<float(float)> easingFunction = Easing::easeInOutCubic;
    
    // Path control points for custom paths
    std::vector<glm::vec3> controlPoints;
};

float timeMultiplier = 1.0f;
float rotationSpeed = 0.1f;
float currentTime = 0.0f;    // Add this
float earthRotation = 0.0f;  // Add this
float lastFrameTime = 0.0f;

struct ObjectKeyframe {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Keyframe {
    int id;
    float time;
    glm::vec3 cameraPosition;
    glm::vec3 cameraRotation;
    glm::vec3 modelPosition;
    glm::vec3 modelRotation;
    glm::vec3 modelScale;
    std::vector<ObjectKeyframe> objectKeyframes;

    static int nextId;
    Keyframe() : id(nextId++) {}
};

int Keyframe::nextId = 0;

float pixelsPerSecond = 50.0f;
float totalDuration = 30.0f;  // Or whatever default value you want
std::vector<Keyframe> keyframes;

// Add this near the top of your file, with other global variables
bool playing = false;
bool capturing = false;
float currentFrameTime = 0.0f;

// Update timeline
bool timelineNeedsUpdate = false;

// Add these global variables for font handling
ImFont* g_MainFont = nullptr;
ImFont* g_EmojiFont = nullptr;

// Add these global variables at the top of the file, after the other global variables
int WINDOW_WIDTH = 1400;
int WINDOW_HEIGHT = 900;

// Error variables
std::string errorLoadingModelDetails = "";

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

/// PROJECT SUPPORT
struct ProjectData {
    std::vector<std::string> modelPaths;
    std::vector<glm::vec3> modelPositions;
    std::vector<glm::vec3> modelRotations;
    std::vector<glm::vec3> modelScales;
    std::vector<std::vector<glm::vec3>> objectPositions;
    std::vector<std::vector<glm::vec3>> objectRotations;
    std::vector<std::vector<glm::vec3>> objectScales;

    // Camera control values
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraYaw;
    float cameraPitch;
    float cameraFov;
};

ProjectData currentProject;
std::string currentProjectPath;

// Shading mode
enum class ShadingMode {
    Solid,
    Wireframe
};

ShadingMode currentShadingMode = ShadingMode::Solid;

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float roughness;  // New: for controlling specular highlight size
    float metallic;   // New: for controlling metallic appearance
    float ao;         // New: ambient occlusion factor

    Material() : ambient(0.2f, 0.2f, 0.2f), diffuse(0.8f, 0.8f, 0.8f), 
                 specular(1.0f, 1.0f, 1.0f), shininess(32.0f),
                 roughness(0.5f), metallic(0.0f), ao(1.0f) {}
};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

// Shader sources
const char* skyboxVertexShaderSource = R"(
#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    WorldPos = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
)";

const char* proceduralSkyboxFragmentShader = R"(
#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

uniform vec3 topColor = vec3(0.2, 0.5, 1.0);  // Sky color at the top (blue)
uniform vec3 bottomColor = vec3(0.8, 0.9, 1.0); // Sky color at the horizon (lighter blue)

void main()
{
    // Use the y-coordinate of the direction to determine the blend between the top and bottom colors
    float t = TexCoords.y * 0.5 + 0.5;  // Map y from [-1, 1] to [0, 1]
    vec3 skyColor = mix(bottomColor, topColor, t);  // Linear interpolation
    FragColor = vec4(skyColor, 1.0);
}
)";

const char* dynamicSkyboxFragmentShader = R"(
#version 430 core
out vec4 FragColor;

in vec3 TexCoords;
in vec3 WorldPos;

uniform float time;
uniform float earthRotation;

const float PI = 3.14159265359;

vec3 getSunDirection()
{
    float sunRotation = time * 0.1;
    return normalize(vec3(sin(sunRotation), cos(sunRotation), 0.0));
}

vec3 getMoonDirection()
{
    float moonRotation = time * 0.1 + PI;
    return normalize(vec3(sin(moonRotation), cos(moonRotation), 0.0));
}

vec3 getSkyColor(vec3 direction)
{
    vec3 dayTopColor = vec3(0.3, 0.6, 1.0);
    vec3 dayHorizonColor = vec3(0.7, 0.8, 1.0);
    vec3 nightTopColor = vec3(0.02, 0.05, 0.1);
    vec3 nightHorizonColor = vec3(0.1, 0.1, 0.2);

    float t = (direction.y + 1.0) * 0.5;
    t = smoothstep(0.0, 1.0, t);

    vec3 sunDir = getSunDirection();
    float sunFactor = max(dot(direction, sunDir), 0.0);
    float dayFactor = smoothstep(-0.2, 0.2, sunDir.y);

    vec3 dayColor = mix(dayHorizonColor, dayTopColor, t);
    vec3 nightColor = mix(nightHorizonColor, nightTopColor, t);

    return mix(nightColor, dayColor, dayFactor);
}

vec3 getSunColor(vec3 direction)
{
    vec3 sunDir = getSunDirection();
    float sunIntensity = max(dot(direction, sunDir), 0.0);
    sunIntensity = pow(sunIntensity, 256.0);
    return vec3(1.0, 0.9, 0.7) * sunIntensity;
}

vec3 getMoonColor(vec3 direction)
{
    vec3 moonDir = getMoonDirection();
    float moonIntensity = max(dot(direction, moonDir), 0.0);
    moonIntensity = pow(moonIntensity, 512.0);
    return vec3(0.9, 0.9, 1.0) * moonIntensity;
}

void main()
{
    vec3 direction = normalize(WorldPos);
    vec3 skyColor = getSkyColor(direction);
    vec3 sunColor = getSunColor(direction);
    vec3 moonColor = getMoonColor(direction);

    // Simply mix the sky, sun and moon colors without stars
    vec3 finalColor = skyColor + sunColor + moonColor;

    FragColor = vec4(finalColor, 1.0);
}
)";

const char* vertexShaderSource = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

// Update the fragment shader source
const char* fragmentShaderSource = R"(
#version 430
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform int shadingMode;

void main()
{
    if (shadingMode == 0) // Solid shading
    {
        vec3 lightPos = vec3(10.0, 10.0, 10.0);
        vec3 lightColor = vec3(1.0, 1.0, 1.0);
        vec3 objectColor = vec3(0.7, 0.7, 0.7);

        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
    else // Wireframe
    {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green wireframe
    }
}
)";

// Add this function before the main() function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width > 0 && height > 0) {
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;
        glViewport(0, 0, width, height);
    }
}

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// Camera struct
struct Camera {
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float yaw;
    float pitch;
    float distance;
    float fov;
    float nearPlane;
    float farPlane;

    // New cinematic properties
    float shutterSpeed = 1.0f/60.0f; // in seconds
    float sensorWidth = 36.0f;      // in millimeters (full frame)
    float sensorHeight = 24.0f;     // in millimeters (full frame)

    // Camera motion system
    enum class PathType {
        None,
        Linear,
        Bezier,
        CatmullRom,
        Circular
    };

    struct CameraPath {
        PathType type = PathType::None;
        std::vector<glm::vec3> controlPoints;
        float startTime = 0.0f;
        float duration = 0.0f;
        bool loop = false;
    };

    std::vector<CameraShot> shots;
    CameraShot* currentShot = nullptr;
    std::vector<CameraPath> paths;
    CameraPath* currentPath = nullptr;
    float shotTime = 0.0f;
    float pathTime = 0.0f;

    // Lens properties
    float focalLength = 50.0f;      // in millimeters
    float aperture = 2.8f;          // f-stop value
    float focusDistance = 10.0f;    // in world units
    bool depthOfFieldEnabled = false;
    bool motionBlurEnabled = false;
    float shutterAngle = 180.0f;

    Camera() : 
        position(0.0f, 0.0f, 3.0f),
        target(0.0f, 0.0f, 0.0f),
        up(0.0f, 1.0f, 0.0f),
        yaw(-90.0f),
        pitch(0.0f),
        distance(3.0f),
        fov(45.0f),
        nearPlane(0.1f),
        farPlane(100.0f) {}

        // New methods for camera shots
    void addShot(CameraShot::Type type, float duration) {
        CameraShot shot;
        shot.type = type;
        shot.startTime = shots.empty() ? 0.0f : shots.back().startTime + shots.back().duration;
        shot.duration = duration;
        shot.startPos = position;
        shot.startRotation = glm::vec3(pitch, yaw, 0.0f);
        
        // Set end position based on shot type
        switch (type) {
            case CameraShot::Type::Dolly:
                shot.endPos = position + glm::normalize(target - position) * 5.0f;
                shot.endRotation = shot.startRotation;
                break;
            case CameraShot::Type::Pan:
                shot.endPos = position;
                shot.endRotation = shot.startRotation + glm::vec3(0.0f, 90.0f, 0.0f);
                break;
            // Add other shot type calculations...
        }
        
        shots.push_back(shot);
    }

    void updateShot(float time) {
        if (shots.empty()) return;
        
        // Find current shot
        currentShot = nullptr;
        for (auto& shot : shots) {
            if (time >= shot.startTime && time < shot.startTime + shot.duration) {
                currentShot = &shot;
                shotTime = time - shot.startTime;
                break;
            }
        }
        
        if (!currentShot) return;
        
        // Calculate interpolation factor with easing
        float t = shotTime / currentShot->duration;
        float easedT = currentShot->easingFunction(t);
        
        // Update camera based on shot type
        switch (currentShot->type) {
            case CameraShot::Type::Static:
                // No movement
                break;
                
            case CameraShot::Type::Dolly:
                position = glm::mix(currentShot->startPos, currentShot->endPos, easedT);
                break;
                
            case CameraShot::Type::Pan:
                position = currentShot->startPos;
                yaw = glm::mix(currentShot->startRotation.y, 
                              currentShot->endRotation.y, 
                              easedT);
                break;
                
            case CameraShot::Type::Orbit:
                float angle = easedT * glm::two_pi<float>();
                float radius = glm::length(currentShot->startPos - target);
                position.x = target.x + radius * cos(angle);
                position.z = target.z + radius * sin(angle);
                break;
        }
        
        updatePosition();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix(float aspectRatio) {
        if (depthOfFieldEnabled) {
            // Calculate FOV based on focal length and sensor size
            float fovY = 2.0f * atan(sensorHeight / (2.0f * focalLength));
            return glm::perspective(fovY, aspectRatio, nearPlane, farPlane);
        }
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void updatePosition() {
        if (currentPath && currentPath->type != PathType::None) {
            updatePathPosition();
        } else {
            // Original orbital camera behavior
            float x = distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            float y = distance * sin(glm::radians(pitch));
            float z = distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            position = target + glm::vec3(x, y, z);
        }
    }

    // New methods for camera paths
    void updatePathPosition() {
        if (!currentPath || currentPath->controlPoints.empty()) return;

        float t = pathTime / currentPath->duration;
        if (currentPath->loop) {
            t = fmod(t, 1.0f);
        } else {
            t = glm::clamp(t, 0.0f, 1.0f);
        }

        switch (currentPath->type) {
            case PathType::Linear:
                position = calculateLinearPosition(t);
                break;
            case PathType::Bezier:
                position = calculateBezierPosition(t);
                break;
            case PathType::CatmullRom:
                position = calculateCatmullRomPosition(t);
                break;
            case PathType::Circular:
                position = calculateCircularPosition(t);
                break;
            default:
                break;
        }
    }

    glm::vec3 calculateLinearPosition(float t) {
        if (currentPath->controlPoints.size() < 2) return position;
        
        size_t numSegments = currentPath->controlPoints.size() - 1;
        float segmentT = t * numSegments;
        size_t currentPoint = static_cast<size_t>(segmentT);
        float localT = segmentT - currentPoint;
        
        if (currentPoint >= numSegments) {
            return currentPath->controlPoints.back();
        }
        
        return glm::mix(
            currentPath->controlPoints[currentPoint],
            currentPath->controlPoints[currentPoint + 1],
            localT
        );
    }

    glm::vec3 calculateBezierPosition(float t) {
        if (currentPath->controlPoints.size() < 4) return position;
        
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        
        glm::vec3 p0 = currentPath->controlPoints[0];
        glm::vec3 p1 = currentPath->controlPoints[1];
        glm::vec3 p2 = currentPath->controlPoints[2];
        glm::vec3 p3 = currentPath->controlPoints[3];
        
        return uuu * p0 +
               3.0f * uu * t * p1 +
               3.0f * u * tt * p2 +
               ttt * p3;
    }

    glm::vec3 calculateCatmullRomPosition(float t) {
        if (currentPath->controlPoints.size() < 4) return position;
        
        size_t numPoints = currentPath->controlPoints.size();
        float segmentT = t * (numPoints - 3);
        size_t currentPoint = static_cast<size_t>(segmentT);
        float localT = segmentT - currentPoint;
        
        if (currentPoint >= numPoints - 3) {
            return currentPath->controlPoints[numPoints - 2];
        }
        
        return catmullRomSpline(
            currentPath->controlPoints[currentPoint],
            currentPath->controlPoints[currentPoint + 1],
            currentPath->controlPoints[currentPoint + 2],
            currentPath->controlPoints[currentPoint + 3],
            localT
        );
    }

    glm::vec3 calculateCircularPosition(float t) {
        if (currentPath->controlPoints.size() < 2) return position;
        
        // Center and radius from first two control points
        glm::vec3 center = currentPath->controlPoints[0];
        float radius = glm::length(currentPath->controlPoints[1] - center);
        
        float angle = t * 2.0f * glm::pi<float>();
        return center + glm::vec3(
            radius * cos(angle),
            0.0f,
            radius * sin(angle)
        );
    }

    // Method to start a camera path
    void startPath(PathType type, const std::vector<glm::vec3>& points, float duration, bool loop = false) {
        CameraPath path;
        path.type = type;
        path.controlPoints = points;
        path.duration = duration;
        path.loop = loop;
        paths.push_back(path);
        currentPath = &paths.back();
        pathTime = 0.0f;
    }

    // Method to update path time
    void updatePathTime(float deltaTime) {
        if (currentPath) {
            pathTime += deltaTime;
            if (!currentPath->loop && pathTime >= currentPath->duration) {
                currentPath = nullptr;
                pathTime = 0.0f;
            }
        }
    }

    // Method to get depth of field parameters for shaders
    struct DOFParams {
        float focusDistance;
        float aperture;
        float focalLength;
        bool enabled;
    };

    DOFParams getDOFParams() const {
        return {focusDistance, aperture, focalLength, depthOfFieldEnabled};
    }

    glm::vec3 catmullRomSpline(const glm::vec3& p0, const glm::vec3& p1, 
                              const glm::vec3& p2, const glm::vec3& p3, float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        // Catmull-Rom matrix coefficients
        float b0 = -0.5f * t3 + t2 - 0.5f * t;
        float b1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
        float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
        float b3 = 0.5f * t3 - 0.5f * t2;

        return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
    }
};

// Model transformation struct
struct ModelTransform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

// Use this:
Camera camera;

void initCamera() {
    camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
    camera.target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.yaw = -90.0f;
    camera.pitch = 0.0f;
    camera.distance = 10.0f;
    camera.fov = 45.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 100.0f;
}

ModelTransform modelTransform = {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;  // Add this line
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material)
        : vertices(vertices), indices(indices), textures(textures), material(material)
    {
        setupMesh();
    }

    void setupMesh()
    {
        unsigned int VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void Draw(unsigned int shaderProgram)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        // Set material properties
        glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specular));
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.roughness"), material.roughness);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.metallic"), material.metallic);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.ao"), material.ao);

        glUniform1i(glGetUniformLocation(shaderProgram, "hasTexture"), !textures.empty());

        // Draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

struct Object3D {
    std::string name;
    std::vector<Mesh> meshes;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotationAngles = glm::vec3(0.0f);
    glm::quat freeRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
    glm::vec3 scale = glm::vec3(1.0f);
};

class Model
{
public:
    std::string path;
    std::string directory;
    std::vector<Object3D> objects;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotationAngles = glm::vec3(0.0f);
    glm::quat freeRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    Model(const char* path) : path(path)
    {
        loadModel(path);
    }

    ~Model() {
        for (auto& object : objects) {
            for (auto& mesh : object.meshes) {
                glDeleteVertexArrays(1, &mesh.VAO);
            }
        }
    }

    // Add move constructor and assignment operator
    Model(Model&& other) noexcept = default;
    Model& operator=(Model&& other) noexcept = default;
    
    // Delete copy operations to prevent double-deletion of OpenGL resources
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    const std::string& getPath() const { return path; }

    void Draw(unsigned int shaderProgram)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = modelMatrix * glm::mat4_cast(freeRotation); // Only use quaternion rotation
        // Remove these three lines:
        // modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        // modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        // modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, scale);
        
        for(auto& object : objects)
        {
            glm::mat4 objectMatrix = modelMatrix;
            objectMatrix = glm::translate(objectMatrix, object.position);
            objectMatrix = objectMatrix * glm::mat4_cast(object.freeRotation); // Only use quaternion rotation
            // Remove these three lines:
            // objectMatrix = glm::rotate(objectMatrix, glm::radians(object.rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
            // objectMatrix = glm::rotate(objectMatrix, glm::radians(object.rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
            // objectMatrix = glm::rotate(objectMatrix, glm::radians(object.rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
            objectMatrix = glm::scale(objectMatrix, object.scale);
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(objectMatrix));
            
            for(unsigned int i = 0; i < object.meshes.size(); i++)
                object.meshes[i].Draw(shaderProgram);
        }
    }

private:
    void loadModel(std::string path)
    {
        // Add error handling for file operations
        if (!std::filesystem::exists(path)) {
            std::cout << "Model file does not exist: " + path << std::endl;
            return;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            errorLoadingModelDetails = importer.GetErrorString();
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            ImGui::OpenPopup("Error Loading Model");
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        Object3D object;
        object.name = node->mName.C_Str();

        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            object.meshes.push_back(processMesh(mesh, scene));
        }
        
        if (!object.meshes.empty()) {
            objects.push_back(object);
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Material material;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

            // Load colors
            aiColor3D color(0.f, 0.f, 0.f);
            if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
                material.ambient = glm::vec3(color.r, color.g, color.b);
            if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
                material.diffuse = glm::vec3(color.r, color.g, color.b);
            if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
                material.specular = glm::vec3(color.r, color.g, color.b);

            // Load shininess
            float shininess;
            if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
                material.shininess = shininess;

            // Set default values for PBR properties
            material.roughness = 0.5f;  // Medium roughness
            material.metallic = 0.0f;   // Non-metallic by default
            material.ao = 1.0f;         // Full ambient occlusion by default

            // Load textures
            std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures, material);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);  // Use the model's directory
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
        return textures;
    }

    unsigned int TextureFromFile(const char* path, const std::string& directory) {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;  // Resolve the full path

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
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

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
            std::cout << "Texture loaded successfully: " << filename << std::endl;
        } else {
            std::cout << "Texture failed to load at path: " << filename << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

GLFWwindow* initializeGLFWAndCreateWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8); // Request 8x MSAA

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Experience Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    return window;
}

bool initializeGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    return true;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return 0;
    }

    return program;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool enableMSAA = true;

class Scene {
public:
// Lighting system
    struct Light {
        enum class Type { Point, Directional, Spot, Area };
        Type type;
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float radius;  // For area lights
        float angle;   // For spot lights
    };
    std::vector<Light> lights;
    
    // Environment
    struct Environment {
        std::string hdriPath;
        float intensity;
        float rotation;
    };
    Environment environment;
    
    // Post-processing effects
    struct PostProcessing {
        bool bloomEnabled;
        bool ssaoEnabled;
        bool dofEnabled;
        bool motionBlurEnabled;
        float exposure;
        float gamma;
    };
    PostProcessing postProcessing;
    std::vector<std::unique_ptr<Model>> models;
    int selectedModelIndex = -1;
    int selectedObjectIndex = -1;

    void addModel(const char* path) {
        // Check if the model already exists
        auto it = std::find_if(models.begin(), models.end(),
            [path](const std::unique_ptr<Model>& model) {
                return model->getPath() == path;
            });

        if (it == models.end()) {
            models.push_back(std::make_unique<Model>(path));
            currentProject.modelPaths.push_back(path);
        } else {
            std::cout << "Model already exists in the scene: " << path << std::endl;
        }
    }

    void removeModel(int index) {
        if (index >= 0 && index < models.size()) {
            models.erase(models.begin() + index);
            if (selectedModelIndex == index) {
                selectedModelIndex = -1;
            } else if (selectedModelIndex > index) {
                selectedModelIndex--;
            }
        }
    }

    void selectModel(int index) {
        if (index >= -1 && index < static_cast<int>(models.size())) {
            selectedModelIndex = index;
            selectedObjectIndex = -1;  // Reset object selection when selecting a new model
        }
    }

    void drawModels(unsigned int shaderProgram) {
        for (const auto& model : models) {
            model->Draw(shaderProgram);
        }
    }
};

class AnimationSystem {
public:
    struct Track {
        enum class Type {
            Transform,    // Position, rotation, scale
            Material,     // Color, roughness, metallic
            Light,       // Color, intensity
            Camera,      // Position, rotation, FOV
            Morph,       // Blend shapes
            Skeletal     // Bone animations
        };
        
        Type type;
        std::string targetName;
        std::vector<Keyframe> keyframes;
        
        // Interpolation methods
        enum class Interpolation {
            Linear,
            Bezier,
            Hermite,
            Step
        };
        Interpolation interpolation;
    };
    
    std::vector<Track> tracks;
    
    // Animation layers for blending
    struct Layer {
        std::string name;
        float weight;
        std::vector<Track*> tracks;
    };
    std::vector<Layer> layers;
};

// Create a global Scene object
Scene scene;
std::string currentDirectory = std::filesystem::current_path().string();
std::vector<std::string> modelFiles;
bool showFileExplorer = false;
char searchBuffer[256] = "";

void newProject() {
    scene.models.clear();
    scene.selectedModelIndex = -1;
    scene.selectedObjectIndex = -1;
    currentProject = ProjectData();
    currentProjectPath.clear();

    // Clear timeline data
    totalDuration = 30.0f; // Reset to default value
    currentTime = 0.0f;
    pixelsPerSecond = 100.0f; // Reset to default value
    keyframes.clear();
    Keyframe::nextId = 0; // Reset the keyframe ID counter

    timelineNeedsUpdate = true;
}

void saveProject(const std::string& path) {
    json j;
    
    // Clear and repopulate modelPaths to avoid duplications
    currentProject.modelPaths.clear();
    for (const auto& model : scene.models) {
        currentProject.modelPaths.push_back(model->getPath());
    }
    
    j["modelPaths"] = currentProject.modelPaths;
    
    j["modelPositions"] = json::array();
    j["modelRotations"] = json::array();
    j["modelScales"] = json::array();
    j["objectPositions"] = json::array();
    j["objectRotationAngles"] = json::array();
    j["objectScales"] = json::array();

    // Save camera control values
    j["cameraPosition"] = {camera.position.x, camera.position.y, camera.position.z};
    j["cameraTarget"] = {camera.target.x, camera.target.y, camera.target.z};
    j["cameraUp"] = {camera.up.x, camera.up.y, camera.up.z};
    j["cameraYaw"] = camera.yaw;
    j["cameraPitch"] = camera.pitch;
    j["cameraDistance"] = camera.distance;
    j["cameraFov"] = camera.fov;
    j["cameraNearPlane"] = camera.nearPlane;
    j["cameraFarPlane"] = camera.farPlane;

    j["totalDuration"] = totalDuration;
    j["currentTime"] = currentTime;
    j["pixelsPerSecond"] = pixelsPerSecond;

    // Save timeline data
    j["totalDuration"] = totalDuration;  // Make sure totalDuration is declared globally
    j["currentTime"] = currentTime;
    j["pixelsPerSecond"] = pixelsPerSecond;  // Make sure pixelsPerSecond is declared globally

    // Save keyframes
    j["keyframes"] = json::array();
    for (const auto& keyframe : keyframes) {  // Make sure keyframes is declared globally
        json kf;
        kf["id"] = keyframe.id;
        kf["time"] = keyframe.time;
        kf["cameraPosition"] = {keyframe.cameraPosition.x, keyframe.cameraPosition.y, keyframe.cameraPosition.z};
        kf["cameraRotation"] = {keyframe.cameraRotation.x, keyframe.cameraRotation.y, keyframe.cameraRotation.z};
        kf["modelPosition"] = {keyframe.modelPosition.x, keyframe.modelPosition.y, keyframe.modelPosition.z};
        kf["modelRotation"] = {keyframe.modelRotation.x, keyframe.modelRotation.y, keyframe.modelRotation.z};
        kf["modelScale"] = {keyframe.modelScale.x, keyframe.modelScale.y, keyframe.modelScale.z};
        
        kf["objectKeyframes"] = json::array();
        for (const auto& objKf : keyframe.objectKeyframes) {
            json objKfJson;
            objKfJson["position"] = {objKf.position.x, objKf.position.y, objKf.position.z};
            objKfJson["rotation"] = {objKf.rotation.x, objKf.rotation.y, objKf.rotation.z};
            objKfJson["scale"] = {objKf.scale.x, objKf.scale.y, objKf.scale.z};
            kf["objectKeyframes"].push_back(objKfJson);
        }
        
        j["keyframes"].push_back(kf);
    }

    for (size_t i = 0; i < scene.models.size(); ++i) {
        j["modelPositions"].push_back({scene.models[i]->position.x, scene.models[i]->position.y, scene.models[i]->position.z});
        j["modelFreeRotations"].push_back({scene.models[i]->freeRotation.w, scene.models[i]->freeRotation.x, scene.models[i]->freeRotation.y, scene.models[i]->freeRotation.z});
        j["modelRotationAngles"].push_back({scene.models[i]->rotationAngles.x, scene.models[i]->rotationAngles.y, scene.models[i]->rotationAngles.z});
        j["modelScales"].push_back({scene.models[i]->scale.x, scene.models[i]->scale.y, scene.models[i]->scale.z});
        
        json objectPositions, objectRotationAngles, objectScales, objectFreeRotations;
        for (const auto& object : scene.models[i]->objects) {
            objectPositions.push_back({object.position.x, object.position.y, object.position.z});
            objectFreeRotations.push_back({object.freeRotation.w, object.freeRotation.x, object.freeRotation.y, object.freeRotation.z});
            objectRotationAngles.push_back({object.rotationAngles.x, object.rotationAngles.y, object.rotationAngles.z});
            objectScales.push_back({object.scale.x, object.scale.y, object.scale.z});
        }
        j["objectPositions"].push_back(objectPositions);
        j["objectRotationAngles"].push_back(objectRotationAngles);
        j["objectFreeRotations"].push_back(objectFreeRotations);
        j["objectScales"].push_back(objectScales);
    }

    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
        currentProjectPath = path;
        std::cout << "Project saved successfully to: " << path << std::endl;
    } else {
        std::cerr << "Failed to save project to: " << path << std::endl;
    }
}

void openProject(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open project file: " << path << std::endl;
        return;
    }

    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return;
    }

    newProject(); // Clear existing scene

    try {
        currentProject.modelPaths = j.at("modelPaths").get<std::vector<std::string>>();

        for (size_t i = 0; i < currentProject.modelPaths.size(); ++i) {
            // Add the model once
            scene.addModel(currentProject.modelPaths[i].c_str());

            // Set the model's properties
            auto& model = scene.models.back();

            if (i < j["modelPositions"].size()) {
                model->position = glm::vec3(
                    j["modelPositions"][i][0].get<float>(),
                    j["modelPositions"][i][1].get<float>(),
                    j["modelPositions"][i][2].get<float>()
                );
            }

            if (i < j["modelFreeRotations"].size()) {
                model->freeRotation = glm::quat(
                    j["modelFreeRotations"][i][0].get<float>(),
                    j["modelFreeRotations"][i][1].get<float>(),
                    j["modelFreeRotations"][i][2].get<float>(),
                    j["modelFreeRotations"][i][3].get<float>()
                );
            }

            if (i < j["modelRotationAngles"].size()) {
                model->rotationAngles = glm::vec3(
                    j["modelRotationAngles"][i][0].get<float>(),
                    j["modelRotationAngles"][i][1].get<float>(),
                    j["modelRotationAngles"][i][2].get<float>()
                );
            }

            if (i < j["modelScales"].size()) {
                model->scale = glm::vec3(
                    j["modelScales"][i][0].get<float>(),
                    j["modelScales"][i][1].get<float>(),
                    j["modelScales"][i][2].get<float>()
                );
            }

            if (i < j["objectPositions"].size() && !j["objectPositions"][i].empty()) {
                for (size_t k = 0; k < j["objectPositions"][i].size() && k < model->objects.size(); ++k) {
                    model->objects[k].position = glm::vec3(
                        j["objectPositions"][i][k][0].get<float>(),
                        j["objectPositions"][i][k][1].get<float>(),
                        j["objectPositions"][i][k][2].get<float>()
                    );
                }
            }

            if (i < j["objectFreeRotations"].size() && !j["objectFreeRotations"][i].empty()) {
                for (size_t k = 0; k < j["objectFreeRotations"][i].size() && k < model->objects.size(); ++k) {
                    model->objects[k].freeRotation = glm::quat(
                        j["objectFreeRotations"][i][k][0].get<float>(),
                        j["objectFreeRotations"][i][k][1].get<float>(),
                        j["objectFreeRotations"][i][k][2].get<float>(),
                        j["objectFreeRotations"][i][k][3].get<float>()
                    );
                }
            }

            if (i < j["objectRotationAngles"].size() && !j["objectRotationAngles"][i].empty()) {
                for (size_t k = 0; k < j["objectRotationAngles"][i].size() && k < model->objects.size(); ++k) {
                    model->objects[k].rotationAngles = glm::vec3(
                        j["objectRotationAngles"][i][k][0].get<float>(),
                        j["objectRotationAngles"][i][k][1].get<float>(),
                        j["objectRotationAngles"][i][k][2].get<float>()
                    );
                }
            }

            if (i < j["objectScales"].size() && !j["objectScales"][i].empty()) {
                for (size_t k = 0; k < j["objectScales"][i].size() && k < model->objects.size(); ++k) {
                    model->objects[k].scale = glm::vec3(
                        j["objectScales"][i][k][0].get<float>(),
                        j["objectScales"][i][k][1].get<float>(),
                        j["objectScales"][i][k][2].get<float>()
                    );
                }
            }

            if (j.contains("cameraPosition")) {
                camera.position = glm::vec3(
                    j["cameraPosition"][0].get<float>(),
                    j["cameraPosition"][1].get<float>(),
                    j["cameraPosition"][2].get<float>()
                );
            }
            if (j.contains("cameraTarget")) {
                camera.target = glm::vec3(
                    j["cameraTarget"][0].get<float>(),
                    j["cameraTarget"][1].get<float>(),
                    j["cameraTarget"][2].get<float>()
                );
            }
            if (j.contains("cameraUp")) {
                camera.up = glm::vec3(
                    j["cameraUp"][0].get<float>(),
                    j["cameraUp"][1].get<float>(),
                    j["cameraUp"][2].get<float>()
                );
            }
            if (j.contains("cameraYaw")) {
                camera.yaw = j["cameraYaw"].get<float>();
            }
            if (j.contains("cameraPitch")) {
                camera.pitch = j["cameraPitch"].get<float>();
            }
            if (j.contains("cameraDistance")) {
                camera.distance = j["cameraDistance"].get<float>();
            }
            if (j.contains("cameraFov")) {
                camera.fov = j["cameraFov"].get<float>();
            }
            if (j.contains("cameraNearPlane")) {
                camera.nearPlane = j["cameraNearPlane"].get<float>();
            }
            if (j.contains("cameraFarPlane")) {
                camera.farPlane = j["cameraFarPlane"].get<float>();
            }

            camera.updatePosition();

            // Load timeline data
            if (j.contains("totalDuration")) totalDuration = j["totalDuration"].get<float>();
            if (j.contains("currentTime")) currentTime = j["currentTime"].get<float>();
            if (j.contains("pixelsPerSecond")) pixelsPerSecond = j["pixelsPerSecond"].get<float>();

            // Load keyframes
            keyframes.clear();
            if (j.contains("keyframes")) {
                for (const auto& kf : j["keyframes"]) {
                    Keyframe keyframe;
                    keyframe.id = kf["id"].get<int>();
                    keyframe.time = kf["time"].get<float>();
                    keyframe.cameraPosition = glm::vec3(kf["cameraPosition"][0], kf["cameraPosition"][1], kf["cameraPosition"][2]);
                    keyframe.cameraRotation = glm::vec3(kf["cameraRotation"][0], kf["cameraRotation"][1], kf["cameraRotation"][2]);
                    keyframe.modelPosition = glm::vec3(kf["modelPosition"][0], kf["modelPosition"][1], kf["modelPosition"][2]);
                    keyframe.modelRotation = glm::vec3(kf["modelRotation"][0], kf["modelRotation"][1], kf["modelRotation"][2]);
                    keyframe.modelScale = glm::vec3(kf["modelScale"][0], kf["modelScale"][1], kf["modelScale"][2]);

                    for (const auto& objKf : kf["objectKeyframes"]) {
                        ObjectKeyframe objKeyframe;
                        objKeyframe.position = glm::vec3(objKf["position"][0], objKf["position"][1], objKf["position"][2]);
                        objKeyframe.rotation = glm::vec3(objKf["rotation"][0], objKf["rotation"][1], objKf["rotation"][2]);
                        objKeyframe.scale = glm::vec3(objKf["scale"][0], objKf["scale"][1], objKf["scale"][2]);
                        keyframe.objectKeyframes.push_back(objKeyframe);
                    }

                    keyframes.push_back(keyframe);
                }
            }

            // Update the next ID for keyframes
            if (!keyframes.empty()) {
                Keyframe::nextId = std::max(Keyframe::nextId, keyframes.back().id + 1);
            }
        }
    } catch (const json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    } catch (const json::out_of_range& e) {
        std::cerr << "JSON out of range error: " << e.what() << std::endl;
    }

    // After successfully loading the project
    timelineNeedsUpdate = true;
    currentProjectPath = path;
    std::cout << "Project loaded successfully from: " << path << std::endl;
}

struct RecentFile {
    std::string path;
    std::chrono::system_clock::time_point lastAccessed;
};

std::deque<RecentFile> recentFiles;
std::unordered_set<std::string> favoriteDirectories;
constexpr size_t MAX_RECENT_FILES = 10;
std::string errorDetails;

// Application structure
typedef struct _APPLICATION_DATA {
    int argument_count;
    char **arguments;
} APPLICATION_DATA, *PAPPLICATION_DATA;

PAPPLICATION_DATA app_data = nullptr;

const std::unordered_set<std::string> SUPPORTED_FORMATS = {
    ".obj"  // Added more 3D file formats
};

bool isModelFile(const std::string& filename) {
    std::string extension = std::filesystem::path(filename).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return SUPPORTED_FORMATS.find(extension) != SUPPORTED_FORMATS.end();
}

void addToRecentFiles(const std::string& filepath) {
    // Remove if already exists
    auto it = std::find_if(recentFiles.begin(), recentFiles.end(),
        [&filepath](const RecentFile& rf) { return rf.path == filepath; });
    if (it != recentFiles.end()) {
        recentFiles.erase(it);
    }
    
    // Add to front
    recentFiles.push_front({filepath, std::chrono::system_clock::now()});
    
    // Maintain maximum size
    if (recentFiles.size() > MAX_RECENT_FILES) {
        recentFiles.pop_back();
    }
}

void toggleFavoriteDirectory(const std::string& directory) {
    if (favoriteDirectories.count(directory)) {
        favoriteDirectories.erase(directory);
    } else {
        favoriteDirectories.insert(directory);
    }
}

void refreshModelFiles() {
    modelFiles.clear();
    for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) {
        if (entry.is_regular_file() && isModelFile(entry.path().string())) {
            modelFiles.push_back(entry.path().filename().string());
        }
    }
    std::sort(modelFiles.begin(), modelFiles.end());
}

void renderFileExplorer() {
    if (!showFileExplorer) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Enhanced File Explorer: Select the model to load", &showFileExplorer);

    // Left panel - Directory tree and favorites
    ImGui::BeginChild("LeftPanel", ImVec2(200, 0), true);
    
    if (ImGui::CollapsingHeader("Favorites", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (const auto& favDir : favoriteDirectories) {
            if (ImGui::Selectable(std::filesystem::path(favDir).filename().string().c_str())) {
                currentDirectory = favDir;
                refreshModelFiles();
            }
        }
    }

    if (ImGui::CollapsingHeader("Recent Files", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (const auto& recent : recentFiles) {
            if (ImGui::Selectable(std::filesystem::path(recent.path).filename().string().c_str())) {
                try {
                    scene.addModel(recent.path.c_str());
                    showFileExplorer = false;
                } catch (const std::exception& e) {
                    errorDetails = e.what();
                    ImGui::OpenPopup("Error Loading Model");
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel - File browser
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    
    // Current directory and navigation
    ImGui::Text("Current Directory: %s", currentDirectory.c_str());
    ImGui::SameLine();
    if (ImGui::Button("Parent Directory")) {
        std::filesystem::path current(currentDirectory);
        if (current.has_parent_path()) {
            currentDirectory = current.parent_path().string();
            refreshModelFiles();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(favoriteDirectories.count(currentDirectory) ? "Unfavorite" : "Favorite")) {
        toggleFavoriteDirectory(currentDirectory);
    }

    // Search bar with file format filter
    ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    std::string searchStr = std::string(searchBuffer);
    std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);

    static char customPathInput[256] = "";
    ImGui::InputText("Custom Path", customPathInput, sizeof(customPathInput));
    ImGui::SameLine();
    if (ImGui::Button("Go")) {
        std::string customPath = std::string(customPathInput);
        if (std::filesystem::exists(customPath)) {
            currentDirectory = customPath;
            refreshModelFiles();  // Update the directory view
        } else {
            errorDetails = "Invalid path!";
            ImGui::OpenPopup("Error Loading Model");
        }
    }

    // File format filter
    static bool showAllFormats = true;
    if (ImGui::BeginCombo("File Format", showAllFormats ? "All Supported Formats" : "Selected Formats")) {
        if (ImGui::Selectable("All Supported Formats", showAllFormats)) {
            showAllFormats = true;
        }
        for (const auto& format : SUPPORTED_FORMATS) {
            bool selected = !showAllFormats && (format == searchStr);
            if (ImGui::Selectable(format.c_str(), selected)) {
                showAllFormats = false;
                strcpy(searchBuffer, format.c_str());
            }
        }
        ImGui::EndCombo();
    }

    // Grid view of files with thumbnails
    float thumbnailSize = 100.0f;
    float padding = 10.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = static_cast<int>((panelWidth - padding) / (thumbnailSize + padding));
    columns = std::max(1, columns);

    ImGui::BeginChild("FileGrid", ImVec2(0, 0), false);
    int itemCount = 0;

    // Directories first
    for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) {
        if (entry.is_directory()) {
            std::string dirName = entry.path().filename().string();
            
            if (itemCount % columns != 0) {
                ImGui::SameLine();
            }
            
            ImGui::BeginGroup();
            {
                // Define a maximum size for the button to prevent overly large buttons
                float maxButtonWidth = 300.0f;  // Adjust this based on your layout
                float thumbnailSize = 100.0f;   // Default size for the thumbnail

                // Create the combined button text (emoji and directory name)
                std::string buttonText = "📁\n\n" + dirName;

                // Calculate the size of the button text (emoji + directory name)
                ImVec2 textSize = ImGui::CalcTextSize(buttonText.c_str());
                float buttonWidth = std::min(textSize.x + 20.0f, maxButtonWidth);  // Add padding and limit width

                // Calculate horizontal and vertical padding
                float paddingX = (thumbnailSize - textSize.x) / 2.0f;
                float paddingY = (thumbnailSize - textSize.y) / 2.0f;

                // Adjust cursor position for vertical centering
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + paddingY);

                // Use ImGui::PushStyleVar to apply horizontal padding inside the button
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingX, paddingY));

                // Draw the button with the centered emoji and text
                if (ImGui::Button(buttonText.c_str(), ImVec2(thumbnailSize, thumbnailSize))) {
                    currentDirectory = entry.path().string();
                    refreshModelFiles();
                }

                // Restore the style variable after the button
                ImGui::PopStyleVar();
            }
            ImGui::EndGroup();
            
            itemCount++;
        }
    }

    // Then files
    for (const auto& file : modelFiles) {
        std::string lowerFile = file;
        std::transform(lowerFile.begin(), lowerFile.end(), lowerFile.begin(), ::tolower);
        
        if (searchStr.empty() || lowerFile.find(searchStr) != std::string::npos) {
            if (itemCount % columns != 0) {
                ImGui::SameLine();
            }
            
            ImGui::BeginGroup();
            std::string fullPath = (std::filesystem::path(currentDirectory) / file).string();
            
            if (ImGui::Selectable(file.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    try {
                        scene.addModel(fullPath.c_str());
                        addToRecentFiles(fullPath);
                        showFileExplorer = false;
                    } catch (const std::exception& e) {
                        errorDetails = e.what();
                        ImGui::OpenPopup("Error Loading Model");
                    }
                }
            }
            ImGui::EndGroup();
            
            itemCount++;
        }
    }
    ImGui::EndChild();
    
    ImGui::EndChild();
    
    // Enhanced error popup
    if (ImGui::BeginPopupModal("Error Loading Model", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Failed to load the model file.");
        ImGui::Text("Error details:");
        ImGui::TextWrapped("%s", errorDetails.c_str());
        ImGui::Separator();
        ImGui::Text("Please ensure:");
        ImGui::BulletText("The file is a valid 3D model file");
        ImGui::BulletText("The file is not corrupted");
        ImGui::BulletText("You have read permissions");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            errorDetails.clear();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void renderSceneHierarchy() {
    ImGui::Begin("Scene Hierarchy");

    if (ImGui::Button("Deselect All")) {
        scene.selectModel(-1);
    }
    
    for (int i = 0; i < scene.models.size(); i++) {
        std::string modelName = "Model " + std::to_string(i + 1);
        if (ImGui::TreeNode(modelName.c_str())) {
            if (ImGui::IsItemClicked()) {
                scene.selectModel(i);
            }

            for (int j = 0; j < scene.models[i]->objects.size(); j++) {
                std::string objectName = scene.models[i]->objects[j].name;
                if (objectName.empty()) {
                    objectName = "Object " + std::to_string(j + 1);
                }
                if (ImGui::Selectable(objectName.c_str(), scene.selectedModelIndex == i && scene.selectedObjectIndex == j)) {
                    scene.selectModel(i);
                    scene.selectedObjectIndex = j;
                }
            }
            ImGui::TreePop();
        }
    }
    
    ImGui::End();
}

void renderModelProperties() {
    ImGui::Begin("Properties");
    
    if (scene.selectedModelIndex >= 0 && scene.selectedModelIndex < scene.models.size()) {
        // Use reference to avoid copying the unique_ptr
        auto& model = scene.models[scene.selectedModelIndex];
        
        if (scene.selectedObjectIndex >= 0 && scene.selectedObjectIndex < model->objects.size()) {
            auto& object = model->objects[scene.selectedObjectIndex];
            
            ImGui::Text("Object: %s", object.name.c_str());
            
            ImGui::Text("Position");
            ImGui::SliderFloat3("##Position", glm::value_ptr(object.position), -100.0f, 100.0f);
            
            // Add rotation controls
            ImGui::Text("Rotation");
            static float rotationAngle = 0.0f;
            static glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
            
            ImGui::SliderFloat("Rotation Angle", &rotationAngle, -180.0f, 180.0f);
            ImGui::SliderFloat3("Rotation Axis", glm::value_ptr(rotationAxis), -1.0f, 1.0f);
            
            if (ImGui::Button("Apply Rotation")) {
                // Normalize the rotation axis
                rotationAxis = glm::normalize(rotationAxis);
                // Create a quaternion from the angle and axis
                glm::quat newRotation = glm::angleAxis(glm::radians(rotationAngle), rotationAxis);
                // Combine with existing rotation
                object.freeRotation = newRotation * object.freeRotation;
                // Reset the angle for the next rotation
                rotationAngle = 0.0f;
            }
            
            ImGui::Text("Scale");
            ImGui::SliderFloat3("##Scale", glm::value_ptr(object.scale), 0.1f, 2.0f);
        }
        else {
            ImGui::Text("Model: %s", ("Model " + std::to_string(scene.selectedModelIndex + 1)).c_str());
            
            ImGui::Text("Position");
            ImGui::SliderFloat3("##Position", glm::value_ptr(model->position), -100.0f, 100.0f);
            
            // Add rotation controls for model
            ImGui::Text("Rotation");
            static float rotationAngle = 0.0f;
            static glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
            
            ImGui::SliderFloat("Rotation Angle", &rotationAngle, -180.0f, 180.0f);
            ImGui::SliderFloat3("Rotation Axis", glm::value_ptr(rotationAxis), -1.0f, 1.0f);
            
            if (ImGui::Button("Apply Rotation")) {
                rotationAxis = glm::normalize(rotationAxis);
                glm::quat newRotation = glm::angleAxis(glm::radians(rotationAngle), rotationAxis);
                model->freeRotation = newRotation * model->freeRotation;
                rotationAngle = 0.0f;
            }
            
            ImGui::Text("Scale");
            ImGui::SliderFloat3("##Scale", glm::value_ptr(model->scale), 0.1f, 2.0f);
        }
    }
    else {
        ImGui::Text("No model selected.");
        ImGui::Text("Please Load Model first, then select the model or object in Scene Hierarchy");
    }
    
    ImGui::End();
}

void captureKeyframe(float time) {
    // Check if a keyframe already exists at this time
    auto it = std::find_if(keyframes.begin(), keyframes.end(), [time](const Keyframe& k) {
        return std::abs(k.time - time) < 0.001f; // Allow for small floating-point differences
    });

    Keyframe keyframe;
    if (it != keyframes.end()) {
        // Update existing keyframe
        keyframe = *it;
    } else {
        // Create new keyframe
        keyframe.time = time;
    }

    keyframe.cameraPosition = camera.position;
    keyframe.cameraRotation = glm::vec3(camera.yaw, camera.pitch, camera.distance);
    
    if (!scene.models.empty()) {
        auto& model = scene.models[0]; // Capture the first model (you might want to adjust this for multiple models)
        keyframe.modelPosition = model->position;
        keyframe.modelRotation = model->rotationAngles;
        keyframe.modelScale = model->scale;

        // Capture keyframes for each object in the model
        keyframe.objectKeyframes.clear();
        for (const auto& object : model->objects) {
            ObjectKeyframe objKeyframe;
            objKeyframe.position = object.position;
            objKeyframe.rotation = object.rotationAngles;  // Changed from rotation to rotationAngles
            objKeyframe.scale = object.scale;
            keyframe.objectKeyframes.push_back(objKeyframe);
        }
    }

    if (it != keyframes.end()) {
        *it = keyframe; // Update existing keyframe
    } else {
        keyframes.push_back(keyframe); // Add new keyframe
    }

    // Sort keyframes by time
    std::sort(keyframes.begin(), keyframes.end(), [](const Keyframe& a, const Keyframe& b) {
        return a.time < b.time;
    });
}

void updateKeyframeData(Keyframe& keyframe) {
    keyframe.cameraPosition = camera.position;
    keyframe.cameraRotation = glm::vec3(camera.yaw, camera.pitch, camera.distance);
    
    if (!scene.models.empty()) {
        auto& model = scene.models[0]; // Capture the first model (you might want to adjust this)
        keyframe.modelPosition = model->position;
        keyframe.modelRotation = model->rotationAngles; // Changed from rotation to rotationAngles
        keyframe.modelScale = model->scale;

        // Capture keyframes for each object in the model
        keyframe.objectKeyframes.clear();
        for (const auto& object : model->objects) {
            ObjectKeyframe objKeyframe;
            objKeyframe.position = object.position;
            objKeyframe.rotation = object.rotationAngles;
            objKeyframe.scale = object.scale;
            keyframe.objectKeyframes.push_back(objKeyframe);
        }
    }
}

/**
Using Catmull-Rom Splines
Catmull-Rom splines are a popular choice for smooth interpolation because they pass through the control points, making them intuitive for animation.
 */
glm::vec3 catmullRomSpline(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    return 0.5f * ((2.0f * p1) +
                   (-p0 + p2) * t +
                   (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
                   (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
}

void editKeyframe(Keyframe* keyframe) {
    ImGui::Begin("Keyframe Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    bool changed = false;

    // Edit time
    float time = keyframe->time;
    if (ImGui::DragFloat("Time", &time, 0.01f, 0.0f, 30.0f)) {
        keyframe->time = time;
        changed = true;
    }

    // Edit camera properties
    if (ImGui::DragFloat3("Camera Position", glm::value_ptr(keyframe->cameraPosition), 0.1f)) changed = true;
    if (ImGui::DragFloat3("Camera Rotation", glm::value_ptr(keyframe->cameraRotation), 0.1f)) changed = true;

    // Edit model properties
    if (ImGui::DragFloat3("Model Position", glm::value_ptr(keyframe->modelPosition), 0.1f)) changed = true;
    if (ImGui::DragFloat3("Model Rotation", glm::value_ptr(keyframe->modelRotation), 0.1f)) changed = true;
    if (ImGui::DragFloat3("Model Scale", glm::value_ptr(keyframe->modelScale), 0.01f)) changed = true;

    // Edit object properties
    for (size_t i = 0; i < keyframe->objectKeyframes.size(); ++i) {
        if (ImGui::TreeNode(("Object " + std::to_string(i)).c_str())) {
            auto& objKeyframe = keyframe->objectKeyframes[i];
            if (ImGui::DragFloat3("Position", glm::value_ptr(objKeyframe.position), 0.1f)) changed = true;
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(objKeyframe.rotation), 0.1f)) changed = true;
            if (ImGui::DragFloat3("Scale", glm::value_ptr(objKeyframe.scale), 0.01f)) changed = true;
            ImGui::TreePop();
        }
    }

    if (changed) {
        // Sort keyframes by time
        std::sort(keyframes.begin(), keyframes.end(), [](const Keyframe& a, const Keyframe& b) {
            return a.time < b.time;
        });
    }

    ImGui::End();
}

// Helper function to get shot type label
const char* getCameraShotTypeLabel(CameraShot::Type type) {
    switch (type) {
        case CameraShot::Type::Static: return "Static";
        case CameraShot::Type::Dolly: return "Dolly";
        case CameraShot::Type::Pan: return "Pan";
        case CameraShot::Type::Tilt: return "Tilt";
        case CameraShot::Type::Crane: return "Crane";
        case CameraShot::Type::Track: return "Track";
        case CameraShot::Type::Orbit: return "Orbit";
        case CameraShot::Type::Custom: return "Custom";
        default: return "Unknown";
    }
}

void renderTimeline() {
    static float scrollX = 0.0f;  // Add this declaration at the start
    
    ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    // Timeline controls
    if (ImGui::Button(capturing ? "Stop Capture" : "Start Capture")) {
        capturing = !capturing;
        if (!capturing) {
            captureKeyframe(currentTime);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(playing ? "Pause" : "Play")) {
        playing = !playing;
        if (playing) {
            lastFrameTime = glfwGetTime();
        }
    }
    ImGui::SameLine();
    ImGui::Text("Current Time: %.2f s", currentTime);

    // Timeline settings
    const float trackHeight = 40.0f;
    const float timelineWidth = totalDuration * pixelsPerSecond;
    const float timelineHeight = trackHeight * 3; // Camera, Model, and Objects tracks
    const float timeIndicatorHeight = 20.0f;
    const float keyframeTrackHeight = 30.0f;

    ImVec2 timelinePos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float visibleWidth = ImGui::GetContentRegionAvail().x;
    float totalWidth = timelineWidth * (totalDuration / 30.0f);

    // Remove duplicate declarations and use the existing scrollX variable
    ImGui::SetNextItemWidth(visibleWidth);
    ImGui::DragFloat("##timelineScroll", &scrollX, 1.0f, 0.0f, totalWidth - visibleWidth);

    ImGui::Text("Camera Shots");
    float trackY = timelinePos.y + 60.0f;
    drawList->AddRectFilled(
        ImVec2(timelinePos.x, trackY),
        ImVec2(timelinePos.x + visibleWidth, trackY + 30),
        IM_COL32(40, 40, 40, 255)
    );

    // Draw shots
    for (const auto& shot : camera.shots) {
        float startX = timelinePos.x + (shot.startTime / totalDuration) * timelineWidth - scrollX;
        float endX = timelinePos.x + ((shot.startTime + shot.duration) / totalDuration) * timelineWidth - scrollX;
        
        if (endX >= timelinePos.x && startX <= timelinePos.x + visibleWidth) {
            // Shot background
            ImU32 shotColor;
            switch (shot.type) {
                case CameraShot::Type::Dolly:
                    shotColor = IM_COL32(100, 150, 200, 255);
                    break;
                case CameraShot::Type::Pan:
                    shotColor = IM_COL32(150, 200, 100, 255);
                    break;
                case CameraShot::Type::Orbit:
                    shotColor = IM_COL32(200, 100, 150, 255);
                    break;
                default:
                    shotColor = IM_COL32(150, 150, 150, 255);
            }
            
            drawList->AddRectFilled(
                ImVec2(startX, trackY + 2),
                ImVec2(endX, trackY + 28),
                shotColor
            );
            
            // Shot label
            const char* shotLabel = getCameraShotTypeLabel(shot.type);
            ImVec2 textSize = ImGui::CalcTextSize(shotLabel);
            float textX = startX + (endX - startX) / 2 - textSize.x / 2;
            drawList->AddText(
                ImVec2(textX, trackY + 8),
                IM_COL32(0, 0, 0, 255),
                shotLabel
            );
        }
    }

    // Mouse wheel scrolling
    if (ImGui::IsWindowHovered()) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0) {
            scrollX -= wheel * 30.0f; // Adjust the 30.0f to change scroll speed
        }
    }

    // Custom scrolling logic (middle mouse button drag)
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle) && ImGui::IsWindowHovered()) {
        scrollX -= ImGui::GetIO().MouseDelta.x;
    }

    // Smooth scrolling when playing
    if (playing) {
        float playheadX = currentTime * pixelsPerSecond;
        float scrollMargin = visibleWidth * 0.1f; // 10% margin

        if (playheadX < scrollX + scrollMargin || playheadX > scrollX + visibleWidth - scrollMargin) {
            float targetScrollX = playheadX - visibleWidth / 2;
            scrollX = scrollX + (targetScrollX - scrollX) * 0.1f; // Smooth interpolation
        }
    }

    // Clamp scrollX
    scrollX = std::max(0.0f, std::min(scrollX, timelineWidth - visibleWidth));

    // Draw timeline background
    drawList->AddRectFilled(timelinePos, 
        ImVec2(timelinePos.x + visibleWidth, timelinePos.y + keyframeTrackHeight + timelineHeight + timeIndicatorHeight), 
        IM_COL32(50, 50, 50, 255));

    // Draw keyframe track
    drawList->AddRectFilled(
        ImVec2(timelinePos.x, timelinePos.y),
        ImVec2(timelinePos.x + visibleWidth, timelinePos.y + keyframeTrackHeight),
        IM_COL32(60, 60, 60, 255));

    // Draw tracks
    const char* trackNames[] = {"Camera", "Model", "Objects"};
    for (int i = 0; i < 3; i++) {
        float y = timelinePos.y + keyframeTrackHeight + i * trackHeight;
        drawList->AddRectFilled(
            ImVec2(timelinePos.x, y), 
            ImVec2(timelinePos.x + visibleWidth, y + trackHeight), 
            IM_COL32(70, 70, 70, 255));
        drawList->AddText(ImVec2(timelinePos.x + 5, y + 5), IM_COL32(200, 200, 200, 255), trackNames[i]);
    }

    // Set the clip rect to create a scrolling effect
    drawList->PushClipRect(timelinePos, ImVec2(timelinePos.x + visibleWidth, 
        timelinePos.y + keyframeTrackHeight + timelineHeight + timeIndicatorHeight), true);

    // Draw keyframes
    static Keyframe* selectedKeyframe = nullptr;
    const float keyframeRadius = 6.0f;
    const float keyframeHitboxRadius = 10.0f;

    for (auto& keyframe : keyframes) {
        float x = timelinePos.x + (keyframe.time / totalDuration) * timelineWidth - scrollX;
        if (x >= timelinePos.x - keyframeRadius && x <= timelinePos.x + visibleWidth + keyframeRadius) {
            ImVec2 keyframePos(x, timelinePos.y + keyframeTrackHeight / 2);
            
            bool isHovered = ImGui::IsMouseHoveringRect(
                ImVec2(keyframePos.x - keyframeHitboxRadius, keyframePos.y - keyframeHitboxRadius),
                ImVec2(keyframePos.x + keyframeHitboxRadius, keyframePos.y + keyframeHitboxRadius));

            ImU32 keyframeColor = (&keyframe == selectedKeyframe) ? IM_COL32(255, 165, 0, 255) :
                                  (isHovered ? IM_COL32(255, 255, 0, 255) : IM_COL32(200, 200, 0, 255));

            drawList->AddCircleFilled(keyframePos, keyframeRadius, keyframeColor);
            drawList->AddCircle(keyframePos, keyframeRadius, IM_COL32(0, 0, 0, 255), 0, 2.0f);

            if (ImGui::IsMouseClicked(0) && isHovered) {
                selectedKeyframe = &keyframe;
                currentTime = keyframe.time;
            }

            char timeLabel[16];
            snprintf(timeLabel, sizeof(timeLabel), "%.2f", keyframe.time);
            ImVec2 textSize = ImGui::CalcTextSize(timeLabel);
            drawList->AddText(ImVec2(keyframePos.x - textSize.x / 2, keyframePos.y + keyframeRadius + 2), IM_COL32(255, 255, 255, 255), timeLabel);
        }
    }

    // Draw playhead
    float playheadX = timelinePos.x + (currentTime / totalDuration) * timelineWidth - scrollX;
    drawList->AddLine(
        ImVec2(playheadX, timelinePos.y), 
        ImVec2(playheadX, timelinePos.y + keyframeTrackHeight + timelineHeight + timeIndicatorHeight), 
        IM_COL32(255, 0, 0, 255), 2.0f);

    // Draw time indicators
    float timeIndicatorY = timelinePos.y + keyframeTrackHeight + timelineHeight;
    for (int i = 0; i <= totalDuration; i++) {
        float x = timelinePos.x + (i / totalDuration) * timelineWidth - scrollX;
        if (x >= timelinePos.x && x <= timelinePos.x + visibleWidth) {
            drawList->AddLine(
                ImVec2(x, timeIndicatorY), 
                ImVec2(x, timeIndicatorY + timeIndicatorHeight), 
                IM_COL32(100, 100, 100, 255));
            drawList->AddText(
                ImVec2(x - 10, timeIndicatorY + 2), 
                IM_COL32(200, 200, 200, 255), 
                std::to_string(i).c_str());
        }
    }

    drawList->PopClipRect();

    // Handle timeline interaction
    ImGui::SetCursorScreenPos(timelinePos);
    ImGui::InvisibleButton("timeline", ImVec2(visibleWidth, keyframeTrackHeight + timelineHeight + timeIndicatorHeight));
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
        float clickedTime = ((ImGui::GetIO().MousePos.x - timelinePos.x + scrollX) / timelineWidth) * totalDuration;
        currentTime = std::max(0.0f, std::min(clickedTime, totalDuration));
        selectedKeyframe = nullptr; // Deselect keyframe when clicking on timeline
    }

    // Keyframe editing
    if (selectedKeyframe) {
        editKeyframe(selectedKeyframe);
    }

    // Add zoom controls
    if (ImGui::Button("-")) {
        pixelsPerSecond = std::max(10.0f, pixelsPerSecond - 5.0f);
    }
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        pixelsPerSecond = std::min(200.0f, pixelsPerSecond + 5.0f);
    }
    ImGui::SameLine();
    ImGui::Text("Zoom: %.0f px/s", pixelsPerSecond);

    ImGui::End();
}

void playAnimation(float time) {
    if (scene.models.empty() || keyframes.size() < 2) {
        return;
    }
    
    // Validate time bounds
    time = std::clamp(time, 0.0f, totalDuration);
    
    if (keyframes.size() < 2) return;  // We need at least two keyframes to interpolate

    // Find the two keyframes to interpolate between
    Keyframe* startKeyframe = nullptr;
    Keyframe* endKeyframe = nullptr;
    for (size_t i = 0; i < keyframes.size() - 1; ++i) {
        if (time >= keyframes[i].time && time <= keyframes[i + 1].time) {
            startKeyframe = &keyframes[i];
            endKeyframe = &keyframes[i + 1];
            break;
        }
    }

    if (!startKeyframe || !endKeyframe) return;

    // Calculate interpolation factor
    float t = (time - startKeyframe->time) / (endKeyframe->time - startKeyframe->time);

    // Interpolate camera properties
    camera.position = glm::mix(startKeyframe->cameraPosition, endKeyframe->cameraPosition, t);
    camera.yaw = glm::mix(startKeyframe->cameraRotation.x, endKeyframe->cameraRotation.x, t);
    camera.pitch = glm::mix(startKeyframe->cameraRotation.y, endKeyframe->cameraRotation.y, t);
    camera.distance = glm::mix(startKeyframe->cameraRotation.z, endKeyframe->cameraRotation.z, t);

    // Interpolate model properties
    if (scene.selectedModelIndex >= 0 && scene.selectedModelIndex < scene.models.size()) {
        auto& model = scene.models[scene.selectedModelIndex];
        model->position = glm::mix(startKeyframe->modelPosition, endKeyframe->modelPosition, t);
        model->rotationAngles = glm::mix(startKeyframe->modelRotation, endKeyframe->modelRotation, t);
        model->scale = glm::mix(startKeyframe->modelScale, endKeyframe->modelScale, t);

        // Interpolate object properties
        for (size_t i = 0; i < model->objects.size() && i < startKeyframe->objectKeyframes.size() && i < endKeyframe->objectKeyframes.size(); ++i) {
            model->objects[i].position = glm::mix(startKeyframe->objectKeyframes[i].position, endKeyframe->objectKeyframes[i].position, t);
            model->objects[i].rotationAngles = glm::mix(startKeyframe->objectKeyframes[i].rotation, endKeyframe->objectKeyframes[i].rotation, t);  // Changed from rotation to rotationAngles
            model->objects[i].scale = glm::mix(startKeyframe->objectKeyframes[i].scale, endKeyframe->objectKeyframes[i].scale, t);
        }
    }
}

int main(int argc, char** argv) {
    // Force CPU to wait until all previous instructions complete
    _mm_lfence();

    app_data = new APPLICATION_DATA;

    if (app_data == nullptr) {
        std::cout << "Failed to allocate memory!" << std::endl;
        system("pause");

        exit(0);
    }

    if (argc == 0 && sizeof(argv) <= 0) { std::cerr << "Unknown error" << std::endl; int *p = nullptr; *p = 42; }

    app_data->argument_count = argc;
    app_data->arguments = argv;

    if (app_data->argument_count == 1 || argc == 1) {
        std::cout << "[ACK]" << std::endl;
    } else {
        std::cout << "The application has started using the specialized parameters. They will be listed below" << std::endl;
        std::cout << "WARNING: Use command-line option carefully! They will be changed the behaviour of the application.";
        std::cout << " If you occurred any errors or weird behaviour, remove each command-line option until it resolved.";
        std::cout << " If doesn't help, please consider to run application without any command-line option!" << std::endl;
        
        for (int i = 0; i < app_data->argument_count; i++) {
            std::cout << "argv[" << i << "] = " << app_data->arguments[i] << std::endl;
        }
    }

    GLFWwindow* window = initializeGLFWAndCreateWindow();
    if (!window) return -1;
    if (!initializeGLEW()) return -1;

    Material material;

    material.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    material.shininess = 32.0f;
    material.roughness = 0.5f;
    material.metallic = 0.0f;
    material.ao = 1.0f;

    initCamera();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    // Check for valid window size?
    int width1, height1;
    glfwGetFramebufferSize(window, &width1, &height1);
    if (width1 > 0 && height1 > 0) {
        ImGui::GetIO().DisplaySize = ImVec2((float)width1, (float)height1);
    }

    // Configure FreeType
    ImFontAtlas* atlas = io.Fonts;
    ImFontConfig config, config_emoji;
    config_emoji.OversampleH = config.OversampleV = 1;
    config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    config_emoji.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    
    // Load fonts
    static const ImWchar ranges[] = {
        0x1, 0x1FFFF, 0
    };
    
    // Load main font
    config.MergeMode = false;
    ImFont* mainFont = io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 19.0f, &config, ranges);
    if (mainFont == nullptr) {
        std::cerr << "Error: Failed to load main font!" << std::endl;
    }
    
    // Load emoji font
    config_emoji.MergeMode = true;
    // config_emoji.GlyphOffset = ImVec2(0, 3.0f);
    ImFont* emojiFont = io.Fonts->AddFontFromFileTTF("noto-untouchedsvg.ttf", 40.0f, &config_emoji, ranges);
    if (emojiFont == nullptr) {
        std::cerr << "Error: Failed to load emoji font!" << std::endl;
    }
    
    // Build font atlas
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    io.Fonts->Build();

    ImGuiStyle& style = ImGui::GetStyle();
    style.TabRounding = 8.f;
    style.FrameRounding = 8.f;
    style.GrabRounding = 8.f;
    style.WindowRounding = 8.f;
    style.PopupRounding = 8.f;

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!vertexShader || !fragmentShader) return -1;

    // Create shader program
    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    if (!shaderProgram) return -1;

    GLuint skyboxVertexShader = compileShader(GL_VERTEX_SHADER, skyboxVertexShaderSource);
    GLuint skyboxFragmentShader = compileShader(GL_FRAGMENT_SHADER, dynamicSkyboxFragmentShader);
    if (!skyboxVertexShader || !skyboxFragmentShader) return -1;

    // Create skybox shader program
    GLuint skyboxShader = createShaderProgram(skyboxVertexShader, skyboxFragmentShader);
    if (!skyboxShader) return -1;

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(skyboxVertexShader);
    glDeleteShader(skyboxFragmentShader);

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up view and projection matrices
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (WINDOW_HEIGHT > 0) ? ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT) : 1.0f, 
        0.1f, 1000.0f);

    // Get uniform locations
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint shadingModeLoc = glGetUniformLocation(shaderProgram, "shadingMode");
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        int w_width, w_height;
        glfwGetFramebufferSize(window, &w_width, &w_height);
        if (w_width == 0 || w_height == 0) {
            glfwWaitEvents();
            continue;
        } else {
            ImGui::GetIO().DisplaySize = ImVec2((float)w_width, (float)w_height);
        }

        // Input handling
        processInput(window);

        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Update current time only if playing
        if (playing) {
            currentTime += deltaTime;
            if (currentTime > 30.0f) {  // Assuming 30 seconds total duration
                currentTime = 0.0f;  // Loop the animation
            }
            std::cout << "Playing animation, currentTime: " << currentTime << std::endl;
            playAnimation(currentTime);
        }

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderTimeline();

        // ImGui window for camera control
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f),
            ImGuiCond_FirstUseEver);

        ImGui::Begin("Camera Control");
        if (ImGui::SliderFloat3("Target", glm::value_ptr(camera.target), -10.0f, 10.0f)) {
            camera.updatePosition();
        }
        if (ImGui::SliderFloat("Distance", &camera.distance, 0.1f, 990.0f)) {
            camera.updatePosition();
        }
        if (ImGui::SliderFloat("Yaw", &camera.yaw, -180.0f, 180.0f)) {
            camera.updatePosition();
        }
        if (ImGui::SliderFloat("Pitch", &camera.pitch, -89.0f, 89.0f)) {
            camera.updatePosition();
        }
        ImGui::SliderFloat("FOV", &camera.fov, 1.0f, 120.0f);
        ImGui::SliderFloat("Near Plane", &camera.nearPlane, 0.1f, 10.0f);
        ImGui::SliderFloat("Far Plane", &camera.farPlane, 10.0f, 1000.0f);

        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", 
                camera.position.x, camera.position.y, camera.position.z);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f),
            ImGuiCond_FirstUseEver);

        ImGui::Begin("Options");
        ImGui::Checkbox("Enable MSAA", &enableMSAA);
        ImGui::End();

        // Apply MSAA setting based on user input
        if (enableMSAA) {
            glEnable(GL_MULTISAMPLE);
        } else {
            glDisable(GL_MULTISAMPLE);
        }

        ImGui::Begin("Material Properties");
        ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Ambient Occlusion", &material.ao, 0.0f, 1.0f);
        ImGui::ColorEdit3("Ambient", glm::value_ptr(material.ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(material.diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(material.specular));
        ImGui::SliderFloat("Shininess", &material.shininess, 1.0f, 256.0f);
        ImGui::End();

        // Inside your main loop, where you render the ImGui interface
        ImGui::Begin("Light Controls");
        ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -10.0f, 10.0f);
        ImGui::End();
        
        renderSceneHierarchy();
        renderModelProperties();

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Shading Mode");
        if (ImGui::RadioButton("Solid", currentShadingMode == ShadingMode::Solid))
            currentShadingMode = ShadingMode::Solid;
        ImGui::SameLine();
        if (ImGui::RadioButton("Wireframe", currentShadingMode == ShadingMode::Wireframe))
            currentShadingMode = ShadingMode::Wireframe;
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f), ImGuiCond_FirstUseEver);

        renderFileExplorer();

        ImGui::Begin("Menu");
        if (ImGui::Button("New Project")) {
            ImGui::OpenPopup("New Project");
        }
        ImGui::SameLine();
        if (ImGui::Button("Open Project")) {
            ImGui::OpenPopup("Open Project");
        }
        ImGui::SameLine();
        static char projectPath[256] = "";
        static bool showOverwriteConfirmation = false;
        if (ImGui::Button("Save Project As")) {
            ImGui::OpenPopup("Save Project");
            showOverwriteConfirmation = false;
            memset(projectPath, 0, sizeof(projectPath));
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Model")) {
            showFileExplorer = true;
            refreshModelFiles();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            glDeleteVertexArrays(1, &skyboxVAO);
            glDeleteBuffers(1, &skyboxVBO);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwTerminate();
            exit(0);
        }

        // New Project popup
        if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to start a new project?");
            ImGui::Text("Any unsaved changes will be lost.");
            if (ImGui::Button("Yes")) {
                newProject();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Open Project popup
        if (ImGui::BeginPopupModal("Open Project", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char projectPath[256] = "";
            ImGui::InputText("Project Path", projectPath, IM_ARRAYSIZE(projectPath));
            if (ImGui::Button("Open")) {
                openProject(projectPath);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Save Project popup
        if (ImGui::BeginPopupModal("Save Project", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Project Path", projectPath, IM_ARRAYSIZE(projectPath));
            
            if (showOverwriteConfirmation) {
                ImGui::Text("File already exists. Overwrite?");
                if (ImGui::Button("Yes")) {
                    saveProject(projectPath);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    showOverwriteConfirmation = false;
                }
            } else {
                if (ImGui::Button("Save")) {
                    if (std::filesystem::exists(projectPath)) {
                        showOverwriteConfirmation = true;
                    } else {
                        saveProject(projectPath);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        ImGui::End();

        // Clear the color and depth buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShader);
        
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Remove translation
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 
                            1, 
                            GL_FALSE, 
                            glm::value_ptr(skyboxView));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 
                            1, 
                            GL_FALSE, 
                            glm::value_ptr(projection));
        
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default

        // Activate shader program
        glUseProgram(shaderProgram);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.position));

        // Update view matrix based on camera settings
        glm::vec3 cameraPos = glm::vec3(
            camera.distance * cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)),
            camera.distance * sin(glm::radians(camera.pitch)),
            camera.distance * sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch))
        );
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Update projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Create and set model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, modelTransform.position);
        model = glm::rotate(model, glm::radians(modelTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(modelTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(modelTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, modelTransform.scale);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
        glUniform1i(shadingModeLoc, static_cast<int>(currentShadingMode));

        if (currentShadingMode == ShadingMode::Wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        scene.drawModels(shaderProgram);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    //
    delete app_data;
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
