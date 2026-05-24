#pragma once
#include <vector>
#include "../Items/Item.h"

class ACharacter;

class UBattleManager 
{
public:
    static bool RunAutoBattle(std::vector<ACharacter*>& Party, ACharacter* Enemy, std::vector<FItem>& Inventory);
};