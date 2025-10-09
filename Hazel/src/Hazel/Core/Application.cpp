#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"

#include "Hazel/Core/Log.h"

#include "Hazel/Renderer/Renderer.h"
#include "Input.h"

#include <glfw/glfw3.h>

namespace Hazel { 

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;     //一次只能有一个实例

    Application::Application()
    {    
        HZ_PROFILE_FUNCTION();
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;      //this指向application实例

        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }

    Application::~Application()
    {
        HZ_PROFILE_FUNCTION();
        //Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        HZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));        //不需要显示调用，触发自动调用
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )        //逆向遍历处理事件
        {
            (*--it)->OnEvent(e);        //这个调用的是当前层的onevent
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        HZ_PROFILE_FUNCTION();

        while (m_Running)
        { 
            HZ_PROFILE_SCOPE("RunLoop");

            float time = (float)glfwGetTime();      //获取当前帧时间
            Timestep timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;
            
            if (!m_Minimized)
            {

                {
                    HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {
                    HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();
        if (e.GetWidth() == 0 || e.GetHeight() == 0)    //窗口是否最小化
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight()); 

        return false;
    }

    }
