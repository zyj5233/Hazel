#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Hazel {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        HZ_CORE_ASSERT(windowHandle, "Window handle is null!")      //检查wondowHandel是否为空
    }

    void OpenGLContext::Init()
    {
        HZ_PROFILE_FUNCTION();
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);        //获取OpengGL函数地址
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        if (status) {
            HZ_CORE_INFO("OpenGL Info:");
            HZ_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));       //类型转换，不然会报错
            HZ_CORE_INFO("  显卡: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            HZ_CORE_INFO("  版本: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
    }

    void OpenGLContext::SwapBuffers()
    {
        HZ_PROFILE_FUNCTION();
        glfwSwapBuffers(m_WindowHandle);
    }

}