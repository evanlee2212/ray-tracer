#include "TriMesh.h"
#include "glm/glm.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

TriMesh::TriMesh(const std::string& filepath)
    : m_mesh(loadFromFile(filepath), false)
{
    m_valid = true;
}

void TriMesh::draw() const { m_mesh.draw(); }

std::vector<float> TriMesh::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[TriMesh] Failed to open: " << filepath << '\n';
        return {};
    }

    std::vector<glm::vec3> positions;
    std::string tx, ty, tz;

    while (std::getline(file, tx, ',') &&
           std::getline(file, ty, ',') &&
           std::getline(file, tz, ','))
    {
        try {
            positions.emplace_back(std::stof(tx), std::stof(ty), std::stof(tz));
        } catch (const std::invalid_argument&) {
            std::cerr << "[TriMesh] Skipping bad token: "
                      << tx << ' ' << ty << ' ' << tz << '\n';
        }
    }
    file.close();

    std::cout << "[TriMesh] Triangles read: " << positions.size() / 3 << '\n';

    std::vector<glm::vec3> normals(positions.size(), glm::vec3(0.0f));

    for (size_t i = 0; i + 2 < positions.size(); i += 3) {
        const glm::vec3& v0 = positions[i];
        const glm::vec3& v1 = positions[i + 1];
        const glm::vec3& v2 = positions[i + 2];

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        normals[i]     += faceNormal;
        normals[i + 1] += faceNormal;
        normals[i + 2] += faceNormal;
    }

    std::vector<float> vbo;
    vbo.reserve(positions.size() * 6);

    for (size_t i = 0; i < positions.size(); ++i) {
        const glm::vec3  v = positions[i];
        const glm::vec3  n = glm::normalize(normals[i]);

        vbo.push_back(v.x); vbo.push_back(v.y); vbo.push_back(v.z);
        vbo.push_back(n.x); vbo.push_back(n.y); vbo.push_back(n.z);
    }

    return vbo;
}