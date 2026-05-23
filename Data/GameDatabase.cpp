#include "GameDatabase.h"

namespace FGameDatabase 
{
    // 1. 동료 이름 풀 (원하는 만큼 자유롭게 추가 가능)
    const std::vector<std::string> MercenaryNames = 
    {
        "Yrel", "Maraad", "Valeera", "Khadgar", "Uther", "Jaina", "Anduin", "Sylvanas", "Thrall",
        "Arthas", "Illidan", "Tyrande", "Grommash", "Varian", "Garrosh"
    };

    // 2. 물약 상점 판매 목록
    const std::vector<FItem> PotionShopItems = 
    {
        {"하급 치유 물약", EItemType::Potion, 50, 10, "체력 +50"},
        {"중급 치유 물약", EItemType::Potion, 100, 20, "체력 +100"},
        {"성기사의 영약", EItemType::Potion, 150, 25, "체력 +150"}
    };

    // 3. 대장간 판매 목록
    const std::vector<FItem> BlacksmithItems = 
    {
        {"낡은 철검", EItemType::Weapon, 10, 30, "공격력 +10"},
        {"강철 대검", EItemType::Weapon, 25, 70, "공격력 +25"},
        {"가죽 조끼", EItemType::Armor, 50, 30, "최대 체력 +50"},
        {"미스릴 갑옷", EItemType::Armor, 120, 100, "최대 체력 +120"}
    };

    // 4. 던전 출현 몬스터 목록
    const std::vector<FMonsterData> DungeonMonsters = 
    {
        // 이름, 체력, 공격력, 속도
        {"고블린 정찰병", 160, 15, 8},
        {"오크 흑마법사", 200, 25, 10},
        {"포악한 트롤", 280, 20, 5},
        {"해골 기사", 350, 30, 12}
    };
}