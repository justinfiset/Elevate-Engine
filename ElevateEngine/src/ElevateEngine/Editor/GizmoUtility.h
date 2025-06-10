#pragma once
#include <string>
#include "ImGuizmo.h"

namespace Elevate::Editor {
    std::string GuizmoOperationToString(int tool)
    {
        switch (tool)
        {
        case 7: return "Translate";
        case 120: return "Rotate";
        case 896: return "Scale";
        case 14463: return "Universal";
        default: return "Unknown";
        }
    }
}