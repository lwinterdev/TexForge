// Raypicker.cpp
// Verantwortlich für Raycasting / Picking von 3D-Modellen.
// Enthält:
// - Implementation des Möller–Trumbore Dreiecks-Ray-Intersektionstests
// - Umrechnung eines Bildschirmpunkts in einen Welt-Ray (ScreenPointToRay)
// - Durchlauf aller Dreiecke eines Modells zur Bestimmung des nächsten Schnittpunktes
// Kommentare sind auf Deutsch verfasst und erklären Zweck und Algorithmen.

#include "RayPicker.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cfloat>


// Führt den Möller–Trumbore Ray-Triangle-Intersection-Test durch.
// Parameter:
// - ray: Ray mit Ursprung und Richtung (Weltkoordinaten).
// - v0, v1, v2: Eckpunkte des Dreiecks (Weltkoordinaten).
// - distance: Ausgabeparameter für die Distanz entlang des Rays bis zum Schnittpunkt.
// - barycentric: Ausgabeparameter für die baryzentrischen Koordinaten des Schnittpunkts.
//
// Rückgabe:
// - true, wenn ein Schnittpunkt (vorne am Ray) gefunden wurde, false sonst.
//
// Anmerkungen:
// - EPSILON verhindert numerische Probleme bei parallelen Fällen.
// - u und v sind baryzentrische Komponenten; Überprüfung stellt sicher, dass Punkt im Dreieck liegt.
// - distance > EPSILON stellt sicher, dass Schnittpunkt vor dem Ray-Origin liegt (kein "hinteres" Treffen).
static bool RayTriangleIntersect(
    const Ray& ray,
    glm::vec3 v0,
    glm::vec3 v1,
    glm::vec3 v2,
    float& distance,
    glm::vec3& barycentric)
{
    const float EPSILON = 0.000001f;

    // Kanten des Dreiecks
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;

    // Berechne den Vektor perpendicular zur Kante edge2 und Ray-Richtung
    glm::vec3 h = glm::cross(
        ray.direction,
        edge2
    );

    // Skalarprodukt zur Bestimmung von Parallelität
    float a = glm::dot(
        edge1,
        h
    );

    // Wenn a nahezu 0 ist, sind Ray und Dreieck parallel -> kein Schnitt
    if (a > -EPSILON && a < EPSILON)
        return false;

    float f = 1.0f / a;

    // Vektor vom Dreieckseck zum Ray-Origin
    glm::vec3 s =
        ray.origin - v0;

    // Berechne baryzentrische Koordinate u
    float u =
        f * glm::dot(
            s,
            h
        );

    // Punkt liegt links oder rechts vom Dreieck -> kein Schnitt
    if (u < 0.0f || u > 1.0f)
        return false;

    // Hilfsvektor q
    glm::vec3 q =
        glm::cross(
            s,
            edge1
        );

    // Berechne baryzentrische Koordinate v
    float v =
        f * glm::dot(
            ray.direction,
            q
        );

    // Prüfe ob Punkt innerhalb des Dreiecks liegt (u + v <= 1)
    if (v < 0.0f || u + v > 1.0f)
        return false;

    // Berechne Distanz entlang des Rays zum Schnittpunkt
    distance =
        f * glm::dot(
            edge2,
            q
        );

    // Wenn distance > EPSILON, ist der Schnittpunkt vor dem Ursprung des Rays
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

    // Schnittpunkt liegt hinter dem Ray-Origin oder numerisch ungültig
    return false;
}

// Wandelt Bildschirmkoordinaten (Mausposition) in einen Ray im Weltkoordinatensystem um.
// Eingabe:
// - mouseX, mouseY: Mausposition in Pixeln (Fensterkoordinaten).
// - screenWidth, screenHeight: Auflösung des Render-Ziels.
// - camera: Kamera, von der View- und Projektionsmatrizen stammen.
//
// Ablauf:
// 1. Bildschirm -> Normalized Device Coordinates (NDC).
// 2. NDC -> Clip Space (rayClip).
// 3. Clip -> Eye Space (mit Inverser Projektionsmatrix).
// 4. Setze Richtungskomponenten für Eye Space (z = -1, w = 0).
// 5. Eye -> World Space (mit Inverser Viewmatrix), normalisieren.
// 6. Ray-Origin = Kamera-Position, Ray-Richtung = berechneter Welt-Richtungsvektor.
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

    // Clip space (Hinweis: z = -1 -> näher zur Kamera in OpenGL-Konvention)
    glm::vec4 rayClip(
        x,
        y,
        -1.0f,
        1.0f
    );

    // Clip -> Eye space (verwende Inverse Projektionsmatrix)
    glm::vec4 rayEye =
        glm::inverse(
            camera.GetProjectionMatrix()
        )
        *
        rayClip;

    // Wir wollen eine Richtung im Eye-Space (kein Positionsvektor)
    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    // Eye -> World space (verwende Inverse Viewmatrix)
    glm::vec3 rayWorld =
        glm::vec3(
            glm::inverse(
                camera.GetViewMatrix()
            )
            *
            rayEye
        );

    // Normiere die Richtung
    rayWorld =
        glm::normalize(rayWorld);

    Ray ray;

    // Ausgangspunkt ist die Kameraposition im Welt-Koordinatensystem
    ray.origin =
        camera.Position;

    ray.direction =
        rayWorld;

    return ray;
}

// Führt Raycasting auf ein gesamtes Modell aus und findet das nächste Schnittdreieck.
// Rückgabe:
// - RaycastHit mit Informationen über Treffer, Distanz, Position, interpolierte UVs und Normalen.
RaycastHit RayPicker::Raycast(
    const Ray& ray,
    const Model& model)
{
    RaycastHit closestHit;

    // Initialisiere mit maximaler Distanz
    float closestDistance =
        FLT_MAX;

    // Iteriere alle Meshes des Modells
    for (const Mesh& mesh : model.meshes)
    {
        const auto& vertices =
            mesh.GetVertices();

        const auto& indices =
            mesh.GetIndices();

        // Dreiecksweise über die Indexliste iterieren (je 3 Indices ein Dreieck)
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

            // Prüfe Schnitt mit aktuellem Dreieck
            bool hit =
                RayTriangleIntersect(
                    ray,
                    vert0.Position,
                    vert1.Position,
                    vert2.Position,
                    distance,
                    barycentric
                );

            // Falls Treffer und näher als bisher gefundene Treffer, aktualisiere Ergebnis
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
                // UV-Interpolation mittels baryzentrischer Koordinaten
                // --------------------------
                closestHit.uv =
                    vert0.TexCoords *
                    barycentric.x +

                    vert1.TexCoords *
                    barycentric.y +

                    vert2.TexCoords *
                    barycentric.z;

                // --------------------------
                // Normalen-Interpolation und Normierung
                // Hinweis: lineare Interpolation kann bei stark gekrümmten Flächen approximativ sein.
                // Falls Modelle mit transformierten Normalen verwendet werden, sollten diese vorher richtig transformiert werden.
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