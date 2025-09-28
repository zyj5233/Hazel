#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "Hazel/Core/Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        // 验证ImGui头文件与编译链接的库版本是否匹配
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;     //会启用自动状态保存
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // 键盘控制
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // 手柄控制
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // 启用停靠功能
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // 启用多视口
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;      //禁止窗口合并
        //io.IniFilename = nullptr;         //恢复初始化
        // 设置浅色风格
        ImGui::StyleColorsLight();
        //ImGui::StyleColorsClassic();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 10.0f;                    //直角窗口
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;       //完全不透明
        }

        Application& app = Application::Get();      //app是s_Instance的别名
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());   //获取m_Window

        //初始化
        ImGui_ImplGlfw_InitForOpenGL(window, true);     //与glfw之间的桥梁
        ImGui_ImplOpenGL3_Init("#version 410");     
    }

    void ImGuiLayer::OnDetach()     //清除
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin()        //准备帧
    {
        ImGui_ImplOpenGL3_NewFrame();   //重置渲染状态
        ImGui_ImplGlfw_NewFrame();      //处理输入事件
        ImGui::NewFrame();              //创建新的imgui上下文
    }

    void ImGuiLayer::End()          //渲染帧
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
        // 核心渲染函数，生成绘制数据
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());     //渲染
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)      //确保多视口渲染时上下文正确切换
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender()    //自定义的ImGui渲染接口
    {
        static bool show = true;        //确保生命周期
        ImGui::ShowDemoWindow(&show);   
    }

  
}