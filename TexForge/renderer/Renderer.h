#pragma once

#include <memory>

class Shader;
class Camera;
class Model;
class PaintTexture;

class Renderer
{
public:

    Renderer() = default;
    ~Renderer();

    bool Initialize();

    void Render(
        Camera* camera,
        Model* model,
        PaintTexture* paintTexture
    );

    void Shutdown();

private:

    std::unique_ptr<Shader> shaderProgram;
};