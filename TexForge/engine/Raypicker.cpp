#include "RayPicker.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cfloat>


static bool RayTriangleIntersect(
    const Ray& ray,
    glm::vec3 v0,
    glm::vec3 v1,
    glm::vec3 v2,
    float& distance,
    glm::vec3& barycentric)
{
    const float EPSILON = 0.000001f;


    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;


    glm::vec3 h = glm::cross(
        ray.direction,
        edge2
    );


    float a = glm::dot(
        edge1,
        h
    );


    if (a > -EPSILON && a < EPSILON)
        return false;


    float f = 1.0f / a;


    glm::vec3 s =
        ray.origin - v0;


    float u =
        f * glm::dot(
            s,
            h
        );


    if (u < 0.0f || u > 1.0f)
        return false;



    glm::vec3 q =
        glm::cross(
            s,
            edge1
        );


    float v =
        f * glm::dot(
            ray.direction,
            q
        );


    if (v < 0.0f || u + v > 1.0f)
        return false;



    distance =
        f * glm::dot(
            edge2,
            q
        );


    if (distance > EPSILON)
    {
        barycentric =
            glm::vec3(
                1.0f - u - v,
                u,
                v
            );

        return true;
    }


    return false;
}





Ray RayPicker::ScreenPointToRay(
    double mouseX,
    double mouseY,
    int screenWidth,
    int screenHeight,
    const Camera& camera)
{
    // Mouse -> Normalized Device Coordinates

    float x =
        (2.0f * static_cast<float>(mouseX))
        / screenWidth - 1.0f;


    float y =
        1.0f -
        (2.0f * static_cast<float>(mouseY))
        / screenHeight;



    // Clip space

    glm::vec4 rayClip(
        x,
        y,
        -1.0f,
        1.0f
    );



    // Clip -> Eye space

    glm::vec4 rayEye =
        glm::inverse(
            camera.GetProjectionMatrix()
        )
        *
        rayClip;



    // We want a direction

    rayEye.z = -1.0f;
    rayEye.w = 0.0f;



    // Eye -> World space

    glm::vec3 rayWorld =
        glm::vec3(
            glm::inverse(
                camera.GetViewMatrix()
            )
            *
            rayEye
        );


    rayWorld =
        glm::normalize(rayWorld);



    Ray ray;

    ray.origin =
        camera.Position;

    ray.direction =
        rayWorld;


    return ray;
}






RaycastHit RayPicker::Raycast(
    const Ray& ray,
    const Model& model)
{

    RaycastHit closestHit;


    float closestDistance =
        FLT_MAX;



    for (const Mesh& mesh : model.meshes)
    {

        const auto& vertices =
            mesh.GetVertices();


        const auto& indices =
            mesh.GetIndices();



        for (size_t i = 0; i < indices.size(); i += 3)
        {

            Vertex vert0 =
                vertices[indices[i]];


            Vertex vert1 =
                vertices[indices[i + 1]];


            Vertex vert2 =
                vertices[indices[i + 2]];



            float distance;

            glm::vec3 barycentric;



            bool hit =
                RayTriangleIntersect(
                    ray,
                    vert0.Position,
                    vert1.Position,
                    vert2.Position,
                    distance,
                    barycentric
                );



            if (hit && distance < closestDistance)
            {

                closestDistance =
                    distance;



                closestHit.hit =
                    true;


                closestHit.distance =
                    distance;



                closestHit.position =
                    ray.origin +
                    ray.direction * distance;



                // --------------------------
                // UV interpolation
                // --------------------------

                closestHit.uv =
                    vert0.TexCoords *
                    barycentric.x +

                    vert1.TexCoords *
                    barycentric.y +

                    vert2.TexCoords *
                    barycentric.z;



                // --------------------------
                // Normal interpolation
                // --------------------------

                closestHit.normal =
                    glm::normalize(

                        vert0.Normal *
                        barycentric.x +

                        vert1.Normal *
                        barycentric.y +

                        vert2.Normal *
                        barycentric.z
                    );

            }
        }
    }


    return closestHit;
}