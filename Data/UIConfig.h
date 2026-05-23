#pragma once

namespace FUIConfig 
{
    // 1280x720 비율에 맞춘 160x45 해상도 (16:9 와이드)
    constexpr int ConsoleWidth = 160;
    constexpr int ConsoleHeight = 45;

    // 상단 패널 (높이 27로 통일)
    constexpr int StatusX = 2;
    constexpr int StatusY = 1;
    constexpr int StatusW = 40;  // 상태창 너비 확장
    constexpr int StatusH = 27;

    constexpr int ViewportX = 43;
    constexpr int ViewportY = 1;
    constexpr int ViewportW = 84; // 뷰포트 극대화
    constexpr int ViewportH = 27;

    constexpr int InvenX = 128;
    constexpr int InvenY = 1;
    constexpr int InvenW = 31;
    constexpr int InvenH = 27;

    // 하단 패널 (높이 16으로 통일, 12줄의 동료 목록 완벽 수용)
    constexpr int LogX = 2;
    constexpr int LogY = 29;
    constexpr int LogW = 125;
    constexpr int LogH = 16;

    constexpr int ActionX = 128;
    constexpr int ActionY = 29;
    constexpr int ActionW = 31;
    constexpr int ActionH = 16;

    // 내부 텍스트 시작 기준점
    constexpr int TextStartX = LogX + 2;
    constexpr int TextStartY = LogY + 2;

    constexpr int InputStartX = ActionX + 2;
    constexpr int InputStartY = ActionY + 2;
}