#include "Monster.h"

AMonster::AMonster(std::string InName, int InMaxHP, int InATK, int InSpeed)
    : ACharacter(InName, ECharacterClass::None, EPersonality::None)
{
    Stat.Level = 1;
    Stat.MaxHP = InMaxHP; 
    Stat.CurrentHP = InMaxHP;
    Stat.ATK = InATK;
    Stat.Speed = InSpeed;
    Stat.MaxMP = 0; 
    Stat.CurrentMP = 0;
}