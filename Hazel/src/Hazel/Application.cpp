#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"
#include "Hazel/Log.h"

#include "glad/glad.h"
namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {     
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }


    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )        //逆向遍历处理事件
        {
            (*--it)->OnEvent(e);        //这个调用的是当前层的onevent
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();      //是层级的onupdata

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
