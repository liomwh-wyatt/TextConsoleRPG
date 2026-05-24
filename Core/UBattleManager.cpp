#include "UBattleManager.h"
#include "../Entities/ACharacter.h"
#include "../UI/URenderManager.h"
#include "../Data/UIConfig.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <algorithm>

bool UBattleManager::RunAutoBattle(std::vector<ACharacter*>& Party, ACharacter* Enemy, std::vector<FItem>& Inventory) 
{
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int CurrentLine = TY; 

    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();

    // 💡 전투가 시작되면 뷰포트에 몬스터의 픽셀 아트 오버레이를 띄웁니다!
    URenderManager::ShowMonsterArtwork(Enemy->GetName());

    URenderManager::MoveCursor(TX, CurrentLine++);
    std::cout << "\x1b[31m[ 전투 돌입! 야생의 " << Enemy->GetName() << "이(가) 나타났다! ]\x1b[0m";
    Sleep(1500);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ProbDist(1, 100); // 1~100% 확률 주사위

    while (!Enemy->IsDead()) 
    {
        // 1. 파티원의 턴
        for (ACharacter* Member : Party) 
        {
            if (Member->IsDead() || Enemy->IsDead()) continue;

            bool bActionTaken = false;
            int ActionRoll = ProbDist(gen);

            // [ AI 판단 1 ] 체력이 50% 이하일 때 40% 확률로 포션 사용 시도
            if (Member->GetStat().CurrentHP <= Member->GetStat().MaxHP / 2 && ActionRoll <= 40) 
            {
                auto PotionIt = std::find_if(Inventory.begin(), Inventory.end(), [](const FItem& item) { return item.Type == EItemType::Potion; });
                if (PotionIt != Inventory.end()) 
                {
                    Member->UsePotion(PotionIt->StatValue);
                    URenderManager::MoveCursor(TX, CurrentLine++);
                    std::cout << Member->GetClassColor() << Member->GetName() << "\x1b[0m(이)가 " << PotionIt->Name << " 사용! (HP 회복)";
                    Inventory.erase(PotionIt); // 인벤토리에서 포션 소모
                    bActionTaken = true;
                }
            }

            // [ AI 판단 2 ] 포션을 안 썼다면 60% 확률로 스킬 사용 시도
            if (!bActionTaken && ProbDist(gen) <= 60) 
            {
                int PreMP = Member->GetStat().CurrentMP;
                
                if (Member->GetJobName() == "사제") 
                {                    
                    ACharacter* TargetAlly = Member;
                    for (ACharacter* Ally : Party) 
                    {
                        if (!Ally->IsDead() && Ally->GetStat().CurrentHP < TargetAlly->GetStat().CurrentHP) TargetAlly = Ally;
                    }
                    
                    Member->UseSkill(TargetAlly);
                    
                    if (PreMP > Member->GetStat().CurrentMP) 
                    {
                        URenderManager::DrawMagicEffect(Member->GetClassColor()); // 스킬 이펙트
                        URenderManager::MoveCursor(TX, CurrentLine++);
                        std::cout << Member->GetClassColor() << Member->GetName() << "\x1b[0m의 스킬 [성스러운 빛] -> " << TargetAlly->GetName() << " 치유!";
                        bActionTaken = true;
                    }
                }
                else 
                {
                    Member->UseSkill(Enemy);
                    if (PreMP > Member->GetStat().CurrentMP) 
                    {
                        URenderManager::DrawMagicEffect(Member->GetClassColor()); // 스킬 이펙트
                        URenderManager::MoveCursor(TX, CurrentLine++);
                        std::cout << Member->GetClassColor() << Member->GetName() << "\x1b[0m의 강력한 스킬 공격! -> " << Enemy->GetName() << " 타격!";
                        bActionTaken = true;
                    }
                }
            }

            // [ AI 판단 3 ] 스킬도 안 썼다면 (또는 MP가 부족해 실패했다면) 기본 공격
            if (!bActionTaken) 
            {
                Member->UseBasicAttack(Enemy);
                URenderManager::DrawSlashEffect(); // 💡 기본 공격 이펙트 호출
                URenderManager::MoveCursor(TX, CurrentLine++);
                std::cout << Member->GetClassColor() << Member->GetName() << "\x1b[0m의 기본 공격! -> " << Enemy->GetName() << " (남은 HP: " << Enemy->GetStat().CurrentHP << ")";
            }

            Sleep(800); 
            URenderManager::DrawPartyStatus(Party);

            if (CurrentLine > FUIConfig::LogY + FUIConfig::LogH - 3) 
            {
                URenderManager::ClearDialogArea();
                CurrentLine = TY;
            }
        }

        if (Enemy->IsDead()) break;

        // 2. 적의 반격 턴
        std::vector<ACharacter*> AliveMembers;
        for (ACharacter* Member : Party) if (!Member->IsDead()) AliveMembers.push_back(Member);
        if (AliveMembers.empty()) return false; 

        std::uniform_int_distribution<int> TargetDist(0, static_cast<int>(AliveMembers.size()) - 1);
        ACharacter* Target = AliveMembers[TargetDist(gen)];

        // 적도 30% 확률로 강력한 공격(1.5배)을 하도록 난이도를 상향합니다.
        if (ProbDist(gen) <= 30) 
        {
            Target->TakeDamage(static_cast<int>(Enemy->GetStat().ATK * 1.5f)); 
            URenderManager::DrawMagicEffect("\x1b[31m"); // 💡 적의 빨간색 스킬 이펙트
            URenderManager::MoveCursor(TX, CurrentLine++);
            std::cout << "\x1b[31m" << Enemy->GetName() << "\x1b[0m의 강력한 스킬 공격! -> " 
                      << Target->GetClassColor() << Target->GetName() << "\x1b[0m 큰 타격! (남은 HP: " << Target->GetStat().CurrentHP << ")";
        }
        else 
        {
            Enemy->UseBasicAttack(Target);
            URenderManager::DrawSlashEffect(); // 💡 적의 기본 공격 이펙트
            URenderManager::MoveCursor(TX, CurrentLine++);
            std::cout << "\x1b[31m" << Enemy->GetName() << "\x1b[0m의 반격! -> " 
                      << Target->GetClassColor() << Target->GetName() << "\x1b[0m 타격! (남은 HP: " << Target->GetStat().CurrentHP << ")";
        }

        Sleep(800);
        URenderManager::DrawPartyStatus(Party);

        if (CurrentLine > FUIConfig::LogY + FUIConfig::LogH - 3) 
        {
            URenderManager::ClearDialogArea();
            CurrentLine = TY;
        }
    }

    URenderManager::MoveCursor(TX, CurrentLine + 1);
    std::cout << "\x1b[33m전투에서 승리했습니다! " << Enemy->GetName() << " 처치!\x1b[0m";
    Sleep(2000);
    URenderManager::ClearMonsterArtwork();
    return true; 
}