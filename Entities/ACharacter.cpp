#include "ACharacter.h"

ACharacter::ACharacter(std::string InName, ECharacterClass InClass, EPersonality InPersonality)
    : Name(InName), JobClass(InClass), Personality(InPersonality)
{
    SetBaseStats();
    ApplyPersonalityBonus();
}

ACharacter::~ACharacter() {}

void ACharacter::SetBaseStats() 
{
    Stat.Level = 1;
    Stat.CurrentEXP = 0;
    Stat.MaxEXP = 100;
    
    switch (JobClass) 
    {
    case ECharacterClass::Warrior: Stat = { 1, 0, 100, 250, 250, 40, 40, 25, 12, 12 }; break;
    case ECharacterClass::Archer:  Stat = { 1, 0, 100, 180, 180, 60, 60, 30, 8, 8 }; break;
    case ECharacterClass::Rogue:   Stat = { 1, 0, 100, 160, 160, 50, 50, 35, 5, 5 }; break;
    case ECharacterClass::Mage:    Stat = { 1, 0, 100, 140, 140, 120, 120, 40, 10, 10 }; break;
    case ECharacterClass::Priest:  Stat = { 1, 0, 100, 170, 170, 100, 100, 20, 9, 9 }; break;
    default:                       Stat = { 1, 0, 100, 100, 100, 0, 0, 10, 10, 10 }; break;
    }
}

void ACharacter::ApplyPersonalityBonus() 
{
    switch (Personality) 
    {
        case EPersonality::Valiant:    Stat.ATK += 5; break;
        case EPersonality::Prudent:    Stat.MaxHP += 40; Stat.CurrentHP += 40; break;
        case EPersonality::Calm:       Stat.MaxMP += 25; Stat.CurrentMP += 25; break;
        case EPersonality::Swift:      Stat.Speed -= 2; Stat.CurrentWait -= 2; break;
        case EPersonality::Optimistic: Stat.MaxHP += 20; Stat.CurrentHP += 20; Stat.MaxMP += 10; Stat.CurrentMP += 10; break;
        case EPersonality::Aloof:      Stat.ATK += 8; Stat.MaxHP -= 10; Stat.CurrentHP -= 10; break;
        case EPersonality::Benevolent: Stat.MaxMP += 35; Stat.CurrentMP += 35; break;
        case EPersonality::Cowardly:   Stat.Speed -= 3; Stat.CurrentWait -= 3; Stat.ATK -= 3; break;
        case EPersonality::Eccentric:  Stat.ATK += 10; Stat.Speed += 1; Stat.CurrentWait += 1; break;
        default: break;
    }
}

void ACharacter::UseBasicAttack(ACharacter* Target) 
{
    if (Target == nullptr || Target->IsDead()) return;
    Target->TakeDamage(Stat.ATK);
}

void ACharacter::UseSkill(ACharacter* Target) {}

void ACharacter::TakeDamage(int DamageAmount) 
{
    Stat.CurrentHP -= DamageAmount;
    if (Stat.CurrentHP < 0) Stat.CurrentHP = 0;
}

void ACharacter::Heal(int HealAmount) 
{
    Stat.CurrentHP += HealAmount;
    if (Stat.CurrentHP > Stat.MaxHP) Stat.CurrentHP = Stat.MaxHP;
}

void ACharacter::AddWaitTime(int TimeAmount) 
{
    Stat.CurrentWait += TimeAmount;
}

void ACharacter::GainEXP(int ExpAmount) 
{
    if (IsDead()) return;

    Stat.CurrentEXP += ExpAmount;

    // 경험치가 100 이상이 되면 누적된 만큼 연속 레벨업이 가능하도록 처리
    while (Stat.CurrentEXP >= Stat.MaxEXP) 
    {
        Stat.CurrentEXP -= Stat.MaxEXP;
        Stat.Level++;
        
        switch (JobClass) 
        {
        case ECharacterClass::Warrior: Stat.MaxHP += 35; Stat.ATK += 4; break;
        case ECharacterClass::Archer:  Stat.MaxHP += 25; Stat.ATK += 5; break;
        case ECharacterClass::Rogue:   Stat.MaxHP += 22; Stat.ATK += 6; break;
        case ECharacterClass::Mage:    Stat.MaxHP += 18; Stat.ATK += 7; Stat.MaxMP += 15; break;
        case ECharacterClass::Priest:  Stat.MaxHP += 22; Stat.ATK += 3; Stat.MaxMP += 12; break;
        default: break;
        }
        
        Stat.CurrentHP = Stat.MaxHP;
        Stat.CurrentMP = Stat.MaxMP;
    }
}

bool ACharacter::IsDead() const { return Stat.CurrentHP <= 0; }
std::string ACharacter::GetName() const { return Name; }
FCharacterStat ACharacter::GetStat() const { return Stat; }

std::string ACharacter::GetClassColor() const 
{
    switch (JobClass) 
    {
    case ECharacterClass::Warrior: return "\x1b[31m"; // 빨강
    case ECharacterClass::Archer:  return "\x1b[32m"; // 초록
    case ECharacterClass::Rogue:   return "\x1b[33m"; // 노랑
    case ECharacterClass::Mage:    return "\x1b[34m"; // 파랑
    case ECharacterClass::Priest:  return "\x1b[36m"; // 청록
    default: return "\x1b[37m"; // 흰색
    }
}

std::string ACharacter::GetJobName() const 
{
    switch (JobClass) 
    {
    case ECharacterClass::Warrior: return "전사";
    case ECharacterClass::Archer:  return "궁수";
    case ECharacterClass::Rogue:   return "도적";
    case ECharacterClass::Mage:    return "마법사";
    case ECharacterClass::Priest:  return "사제";
    default: return "무직";
    }
}

std::string ACharacter::GetPersonalityName() const 
{
    switch (Personality) 
    {
    case EPersonality::Valiant: return "용맹한";
    case EPersonality::Prudent: return "신중한";
    case EPersonality::Calm: return "냉철한";
    case EPersonality::Swift: return "신속한";
    case EPersonality::Optimistic: return "낙천적인";
    case EPersonality::Aloof: return "고고한";
    case EPersonality::Benevolent: return "자애로운";
    case EPersonality::Cowardly: return "비겁한";
    case EPersonality::Eccentric: return "괴짜 같은";
    default: return "평범한";
    }
}