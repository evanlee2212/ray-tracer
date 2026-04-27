#include "Texture.h"
#include "../src/png++/png.hpp"

#include <iostream>
#include <vector>
#include <utility>

Texture::Texture(const std::string& filepath)
{
    try {
        png::image<png::rgb_pixel> img;
        img.read(filepath);

        const int w = static_cast<int>(img.get_width());
        const int h = static_cast<int>(img.get_height());

        std::vector<float> data;
        data.reserve(w * h * 3);

        for (int row = 0; row < h; ++row) {
            for (int col = 0; col < w; ++col) {
                const png::rgb_pixel p = img[h - row - 1][col];
                data.push_back(p.red   / 255.0f);
                data.push_back(p.green / 255.0f);
                data.push_back(p.blue  / 255.0f);
            }
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
                     GL_RGB, GL_FLOAT, data.data());
        glBindTexture(GL_TEXTURE_2D, 0);

    } catch (const std::exception& e) {
        std::cerr << "[Texture] Failed to load '" << filepath << "': "
                  << e.what() << '\n';
        m_id = 0;
    }
}

Texture::~Texture() { destroy(); }

Texture::Texture(Texture&& other) noexcept : m_id(other.m_id)
{
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) { destroy(); m_id = other.m_id; other.m_id = 0; }
    return *this;
}

void Texture::destroy()
{
    if (m_id) { glDeleteTextures(1, &m_id); m_id = 0; }
}

void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}