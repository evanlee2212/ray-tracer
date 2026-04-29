#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices,
           const std::vector<unsigned int>& indices,
           bool hasTexCoords)
    : m_indexed(true)
{
    upload(vertices, &indices, hasTexCoords);
}

Mesh::Mesh(const std::vector<float>& vertices, bool hasTexCoords)
    : m_indexed(false)
{
    upload(vertices, nullptr, hasTexCoords);
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo)
    , m_vertexCount(other.m_vertexCount), m_indexCount(other.m_indexCount)
    , m_indexed(other.m_indexed)
{
    other.m_vao = other.m_vbo = other.m_ebo = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        destroy();
        m_vao = other.m_vao; m_vbo = other.m_vbo; m_ebo = other.m_ebo;
        m_vertexCount = other.m_vertexCount;
        m_indexCount  = other.m_indexCount;
        m_indexed     = other.m_indexed;
        other.m_vao = other.m_vbo = other.m_ebo = 0;
    }
    return *this;
}

Mesh::~Mesh() { destroy(); }

void Mesh::destroy()
{
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
}

void Mesh::upload(const std::vector<float>& vertices,
                  const std::vector<unsigned int>* indices,
                  bool hasTexCoords)
{
    const int stride = hasTexCoords ? 8 : 6;
    m_vertexCount = static_cast<int>(vertices.size()) / stride;

    // VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // EBO
    if (indices) {
        m_indexCount = static_cast<int>(indices->size());
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_indexCount * sizeof(unsigned int),
                     indices->data(),
                     GL_STATIC_DRAW);
    }

    // VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    if (m_ebo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const GLsizei byteStride = stride * sizeof(float);

    // attrib 0 — position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)0);

    // attrib 1 — normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, byteStride,
                          (void*)(3 * sizeof(float)));

    // attrib 2 — texcoord
    if (hasTexCoords) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, byteStride,
                              (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(0);
}

void Mesh::draw() const
{
    glBindVertexArray(m_vao);
    if (m_indexed)
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}