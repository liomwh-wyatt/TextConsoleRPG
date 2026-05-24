#pragma once

namespace FUIConfig 
{
    // 1280x720 비율
    constexpr int ConsoleWidth = 167;
    constexpr int ConsoleHeight = 47;

    // 상단 패널 (+2)
    constexpr int StatusX = 2;
    constexpr int StatusY = 1;
    constexpr int StatusW = 40;
    constexpr int StatusH = 29;

    constexpr int ViewportX = 43;
    constexpr int ViewportY = 1;
    constexpr int ViewportW = 91;
    constexpr int ViewportH = 29;

    constexpr int InvenX = 135;
    constexpr int InvenY = 1;
    constexpr int InvenW = 31;
    constexpr int InvenH = 29;

    // 하단 패널
    constexpr int LogX = 2;
    constexpr int LogY = 31;
    constexpr int LogW = 125;
    constexpr int LogH = 16;

    constexpr int ActionX = 128;
    constexpr int ActionY = 31;
    constexpr int ActionW = 38;
    constexpr int ActionH = 16;

    // 내부 텍스트 시작 기준점
    constexpr int TextStartX = LogX + 2;
    constexpr int TextStartY = LogY + 2;

    constexpr int InputStartX = ActionX + 2;
    constexpr int InputStartY = ActionY + 2;
}