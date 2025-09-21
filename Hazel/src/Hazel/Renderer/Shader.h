#pragma once

#include <string>


namespace Hazel {

    class Shader
    {
    public:
        virtual ~Shader() = default;       //声明在子类实现

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader* Create(const std::string& filepath);     //使用glsl方式的工厂函数
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };

}