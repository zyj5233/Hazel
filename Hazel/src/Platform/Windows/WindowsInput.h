#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel {

    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;        //检测长按

        virtual bool IsMouseButtonPressedImpl(int button) override;
        //获取坐标
        virtual std::pair<float, float> GetMousePositionImpl() override;
        //获取x，y坐标
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };

}