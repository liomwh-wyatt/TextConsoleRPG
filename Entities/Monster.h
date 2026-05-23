#pragma once
#include "ACharacter.h"

class AMonster : public ACharacter 
{
public:
    AMonster(std::string InName, int InMaxHP, int InATK, int InSpeed);
};