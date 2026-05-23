#pragma once
#include "ACharacter.h"

class AWarrior : public ACharacter 
{
public:
    AWarrior(std::string InName, EPersonality InPersonality = EPersonality::None);
    void UseSkill(ACharacter* Target) override;
};

class AArcher : public ACharacter 
{
public:
    AArcher(std::string InName, EPersonality InPersonality = EPersonality::None);
    void UseSkill(ACharacter* Target) override;
};

class ARogue : public ACharacter 
{
public:
    ARogue(std::string InName, EPersonality InPersonality = EPersonality::None);
    void UseSkill(ACharacter* Target) override;
};

class AMage : public ACharacter 
{
public:
    AMage(std::string InName, EPersonality InPersonality = EPersonality::None);
    void UseSkill(ACharacter* Target) override;
};

class APriest : public ACharacter 
{
public:
    APriest(std::string InName, EPersonality InPersonality = EPersonality::None);
    void UseSkill(ACharacter* Target) override;
};