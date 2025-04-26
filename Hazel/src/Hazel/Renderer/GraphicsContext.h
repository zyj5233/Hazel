#pragma once

namespace Hazel {

    class GraphicsContext           //图形上下文
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;     //避免画面闪烁
    };

}