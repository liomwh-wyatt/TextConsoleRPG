#pragma once
#include <string>

enum class ECharacterClass 
{
    Warrior, Archer, Rogue, Mage, Priest, None
};

enum class EPersonality 
{
    Valiant, Prudent, Calm, Swift, Optimistic, Aloof, Benevolent, Cowardly, Eccentric, None
};

struct FCharacterStat 
{
    int Level;
    int CurrentEXP;
    int MaxEXP;
    int MaxHP; int CurrentHP;
    int MaxMP; int CurrentMP;
    int ATK; int Speed; int CurrentWait;
};

class ACharacter 
{
public:
    // 성격(Personality) 매개변수 추가 (기본값은 None)
    ACharacter(std::string InName, ECharacterClass InClass, EPersonality InPersonality = EPersonality::None);
    virtual ~ACharacter();

    virtual void UseBasicAttack(ACharacter* Target);
    virtual void UseSkill(ACharacter* Target);
    virtual void TakeDamage(int DamageAmount);
    virtual void Heal(int HealAmount);
    virtual void AddWaitTime(int TimeAmount);
    void GainEXP(int ExpAmount);

    bool IsDead() const;
    std::string GetName() const;
    FCharacterStat GetStat() const;
    
    std::string GetClassColor() const;
    std::string GetJobName() const;
    std::string GetPersonalityName() const;

protected:
    std::string Name;
    ECharacterClass JobClass;
    EPersonality Personality;
    FCharacterStat Stat;

    void SetBaseStats();
    void ApplyPersonalityBonus();
};