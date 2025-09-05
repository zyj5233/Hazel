#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Hazel {

    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);   //顶点着色器和片段着色器字符串
        ~Shader();

        void Bind() const;      //激活着色器
        void Unbind() const;    //解绑着色器

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


    private:
        uint32_t m_RendererID;      //着色器唯一id
    };

}