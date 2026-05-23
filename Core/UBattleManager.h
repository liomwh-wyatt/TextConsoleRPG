#pragma once
#include <vector>

class ACharacter;

class UBattleManager 
{
public:
    static bool RunAutoBattle(std::vector<ACharacter*>& Party, ACharacter* Enemy);
};