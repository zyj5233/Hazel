#include <Hazel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override        //sandboxapp层级
    {
        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            HZ_TRACE("A key is pressed (poll)!");
    }
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Hazel::Event& event) override
    {
        if (event.GetEventType() == Hazel::EventType::KeyPressed)
        {
            Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;     //将基类 Event 引用转换为派生类 KeyPressedEvent 引用
            if (e.GetKeyCode() == HZ_KEY_TAB)
                HZ_TRACE("Tab key is pressed (event)!");
            HZ_TRACE("{0}", (char)e.GetKeyCode());
        }
    }

};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());      //创建一个example的实例压入栈
    }

    ~Sandbox()
    {

    }

};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}