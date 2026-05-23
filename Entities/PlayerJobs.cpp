#include "PlayerJobs.h"

// 1. 전사 (방패 밀치기: 피해 + 적 턴 지연)
AWarrior::AWarrior(std::string InName, EPersonality InPersonality)
    : ACharacter(InName, ECharacterClass::Warrior, InPersonality) {}

void AWarrior::UseSkill(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead() || Stat.CurrentMP < 15) return;
    Stat.CurrentMP -= 15;
    Target->TakeDamage(static_cast<int>(Stat.ATK * 1.5f));
    Target->AddWaitTime(3); 
}

// 2. 궁수 (연사: 2배 피해)
AArcher::AArcher(std::string InName, EPersonality InPersonality)
    : ACharacter(InName, ECharacterClass::Archer, InPersonality) {}

void AArcher::UseSkill(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead() || Stat.CurrentMP < 20) return;
    Stat.CurrentMP -= 20;
    Target->TakeDamage(Stat.ATK * 2);
}

// 3. 도적 (기습: 2.5배 치명타 피해)
ARogue::ARogue(std::string InName, EPersonality InPersonality)
    : ACharacter(InName, ECharacterClass::Rogue, InPersonality) {}

void ARogue::UseSkill(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead() || Stat.CurrentMP < 15) return;
    Stat.CurrentMP -= 15;
    Target->TakeDamage(static_cast<int>(Stat.ATK * 2.5f));
}

// 4. 마법사 (화염구: 3배 강력한 피해)
AMage::AMage(std::string InName, EPersonality InPersonality)
    : ACharacter(InName, ECharacterClass::Mage, InPersonality) {}

void AMage::UseSkill(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead() || Stat.CurrentMP < 25) return;
    Stat.CurrentMP -= 25;
    Target->TakeDamage(Stat.ATK * 3);
}

// 5. 사제 (성스러운 빛: 아군 치유)
APriest::APriest(std::string InName, EPersonality InPersonality)
    : ACharacter(InName, ECharacterClass::Priest, InPersonality) {}

void APriest::UseSkill(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead() || Stat.CurrentMP < 20) return;
    Stat.CurrentMP -= 20;
    Target->Heal(Stat.ATK * 3); 
}