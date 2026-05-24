#pragma once
#include <string_view>

namespace FStringTable 
{
    constexpr std::string_view System_InitTown = "안전한 마을에 도착했습니다. (Core 시스템 구동 성공)";
    constexpr std::string_view System_GameOver = "용사는 쓰러졌습니다... 게임 오버.";

    constexpr std::string_view Town_Greeting = "마을에 오신 것을 환영합니다. 어디로 이동하시겠습니까?";
    constexpr std::string_view Town_Menu = "[1] 주점  [2] 물약 상점  [3] 대장간  [4] 던전 입구  [5] 인벤토리";
    
    constexpr std::string_view Battle_Encounter = "야생의 몬스터가 나타났다!";
 
    constexpr std::string_view Tavern_Welcome = "시끌벅적한 주점에 들어왔습니다. 용병들이 일거리를 찾고 있습니다.";
    constexpr std::string_view Tavern_Menu = "[1] 용병 고용하기  [2] 동료 관리  [0] 마을로 돌아가기";
    
    constexpr std::string_view Shop_NoMoney = "골드가 부족합니다! 돈을 더 벌어오세요.";
    constexpr std::string_view Shop_BuySuccess = "아이템이 인벤토리에 추가되었습니다!";

    constexpr std::string_view PotionShop_Welcome = "달콤한 약초 냄새가 나는 물약 상점입니다.";
    constexpr std::string_view PotionShop_Menu = "[1] 하급 치유 물약 (10G)  [2] 성기사의 영약 (25G)  [0] 마을로 돌아가기";

    constexpr std::string_view Blacksmith_Welcome = "뜨거운 쇳물 냄새가 나는 대장간입니다. 무기와 방어구를 팝니다.";
    constexpr std::string_view Blacksmith_Menu = "[1] 낡은 철검 (30G)  [2] 가죽 조끼 (30G)  [0] 마을로 돌아가기";
}