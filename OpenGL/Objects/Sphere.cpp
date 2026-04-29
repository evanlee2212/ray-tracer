#include "Sphere.h"
#include <numbers>
#include <cmath>

Sphere::Sphere(glm::vec3 center, float radius, int slices, int stacks)
    : m_center(center), m_radius(radius)
    , m_mesh([&]{
        std::vector<float>        vbo;
        std::vector<unsigned int> idx;
        buildVertices(center, radius, slices, stacks, vbo);
        buildIndices (slices, stacks, idx);
        return Mesh(vbo, idx, /*hasTexCoords=*/true);
    }())
{}

void Sphere::draw() const { m_mesh.draw(); }

void Sphere::buildVertices(glm::vec3 center, float radius, int slices, int stacks,
                           std::vector<float>& vbo)
{
    vbo.clear();
    vbo.reserve((stacks + 1) * (slices + 1) * 8);

    for (int i = 0; i <= stacks; ++i) {
        const float theta = i * std::numbers::pi_v<float> / stacks;
        const float sinT  = std::sin(theta);
        const float cosT  = std::cos(theta);

        for (int j = 0; j <= slices; ++j) {
            const float phi  = j * 2.0f * std::numbers::pi_v<float> / slices;
            const float sinP = std::sin(phi);
            const float cosP = std::cos(phi);

            // Local position on unit sphere, then scale + offset
            const float lx = sinT * cosP;
            const float ly = cosT;
            const float lz = sinT * sinP;

            // Position
            vbo.push_back(center.x + radius * lx);
            vbo.push_back(center.y + radius * ly);
            vbo.push_back(center.z + radius * lz);

            // Normal (same as unit-sphere direction)
            vbo.push_back(lx);
            vbo.push_back(ly);
            vbo.push_back(lz);

            // UV — longitude maps to U, latitude maps to V
            vbo.push_back(static_cast<float>(j) / slices);
            vbo.push_back(static_cast<float>(i) / stacks);
        }
    }
}

void Sphere::buildIndices(int slices, int stacks,
                          std::vector<unsigned int>& indices)
{
    indices.clear();
    indices.reserve(stacks * slices * 6);

    for (int i = 0; i < stacks; ++i) {
        const int row1 = i       * (slices + 1);
        const int row2 = (i + 1) * (slices + 1);

        for (int j = 0; j < slices; ++j) {
            // Triangle 1
            indices.push_back(row1 + j);
            indices.push_back(row1 + j + 1);
            indices.push_back(row2 + j + 1);
            // Triangle 2
            indices.push_back(row1 + j);
            indices.push_back(row2 + j + 1);
            indices.push_back(row2 + j);
        }
    }
}