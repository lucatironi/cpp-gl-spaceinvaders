#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "shader.hpp"

struct Character
{
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer
{
    public:
        TextRenderer(Shader shader);
        ~TextRenderer();

        void LoadFont(std::string font, GLuint fontSize);
        void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

    private:
        Shader shader;
        GLuint VBO, quadVAO;
        std::map<GLchar, Character> Characters;

        void initRenderData();
};

#endif