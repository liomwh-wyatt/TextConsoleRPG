#include "UBattleManager.h"
#include "../Entities/ACharacter.h"
#include "../UI/URenderManager.h"
#include "../Data/UIConfig.h"
#include <iostream>
#include <windows.h>
#include <random>

bool UBattleManager::RunAutoBattle(std::vector<ACharacter*>& Party, ACharacter* Enemy) 
{
    // 💡 기획 반영: 뷰포트(VX, VY) 대신 대화창(TX, TY) 좌표를 사용합니다.
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int CurrentLine = TY; 

    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();

    URenderManager::MoveCursor(TX, CurrentLine++);
    std::cout << "\x1b[31m[ 전투 돌입! 야생의 " << Enemy->GetName() << "이(가) 나타났다! ]\x1b[0m";
    Sleep(1500);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (!Enemy->IsDead()) 
    {
        for (ACharacter* Member : Party) 
        {
            if (Member->IsDead() || Enemy->IsDead()) continue;

            Member->UseBasicAttack(Enemy);
            
            URenderManager::MoveCursor(TX, CurrentLine++);
            std::cout << Member->GetClassColor() << Member->GetName() << "\x1b[0m의 공격! -> " 
                      << Enemy->GetName() << " (HP: " << Enemy->GetStat().CurrentHP << " / " << Enemy->GetStat().MaxHP << ")";
            Sleep(800); 

            // 대화창 공간(LogH)을 넘어가면 깔끔하게 지우고 위로 다시 올립니다.
            if (CurrentLine > FUIConfig::LogY + FUIConfig::LogH - 3) 
            {
                URenderManager::ClearDialogArea();
                CurrentLine = TY;
            }
        }

        if (Enemy->IsDead()) break;

        std::vector<ACharacter*> AliveMembers;
        for (ACharacter* Member : Party) 
            if (!Member->IsDead()) AliveMembers.push_back(Member);

        if (AliveMembers.empty()) return false; 

        std::uniform_int_distribution<int> TargetDist(0, static_cast<int>(AliveMembers.size()) - 1);
        ACharacter* Target = AliveMembers[TargetDist(gen)];

        Enemy->UseBasicAttack(Target);

        URenderManager::MoveCursor(TX, CurrentLine++);
        std::cout << "\x1b[31m" << Enemy->GetName() << "\x1b[0m의 반격! -> " 
                  << Target->GetClassColor() << Target->GetName() << "\x1b[0m 타격! (남은 HP: " << Target->GetStat().CurrentHP << ")";
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
    return true; 
}