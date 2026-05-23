#pragma once
#include <string>
#include <vector>
#include "../Items/Item.h"

// 몬스터의 기본 스탯을 정의하는 구조체
struct FMonsterData 
{
    std::string Name;
    int MaxHP;
    int ATK;
    int Speed;
};

namespace FGameDatabase 
{
    // 외부에서 가져다 쓸 수 있도록 extern 선언
    extern const std::vector<std::string> MercenaryNames;
    extern const std::vector<FItem> PotionShopItems;
    extern const std::vector<FItem> BlacksmithItems;
    extern const std::vector<FMonsterData> DungeonMonsters;
}