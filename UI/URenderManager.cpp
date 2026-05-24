#include "URenderManager.h"
#include "../Entities/ACharacter.h"
#include "../Data/UIConfig.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>

void URenderManager::InitConsole() 
{
    std::string ModeCommand = "mode con cols=" + std::to_string(FUIConfig::ConsoleWidth) + " lines=" + std::to_string(FUIConfig::ConsoleHeight);
    system(ModeCommand.c_str());
    
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    std::cout << "\x1b[?25l";
}

void URenderManager::ClearScreen() { std::cout << "\x1b[2J\x1b[H"; }
void URenderManager::MoveCursor(int X, int Y) { std::cout << "\x1b[" << Y << ";" << X << "H"; }

void URenderManager::DrawFramework() 
{
    ClearScreen();

    std::cout << "\x1b[36m";
    DrawBox(FUIConfig::StatusX, FUIConfig::StatusY, FUIConfig::StatusW, FUIConfig::StatusH);
    MoveCursor(FUIConfig::StatusX + FUIConfig::StatusW / 2 - 7, FUIConfig::StatusY + 2); std::cout << "[ 캐릭터 상태 ]";
    
    std::cout << "\x1b[33m";
    DrawBox(FUIConfig::ViewportX, FUIConfig::ViewportY, FUIConfig::ViewportW, FUIConfig::ViewportH);
    DrawDefaultBackground(); 
    
    std::cout << "\x1b[32m";
    DrawBox(FUIConfig::InvenX, FUIConfig::InvenY, FUIConfig::InvenW, FUIConfig::InvenH);
    MoveCursor(FUIConfig::InvenX + FUIConfig::InvenW / 2 - 6, FUIConfig::InvenY + 2); std::cout << "[ 인벤토리 ]";
    
    std::cout << "\x1b[37m";
    DrawBox(FUIConfig::LogX, FUIConfig::LogY, FUIConfig::LogW, FUIConfig::LogH);
    MoveCursor(FUIConfig::LogX + FUIConfig::LogW / 2 - 7, FUIConfig::LogY + 1); std::cout << "[ 대화 / 로그 ]";
    
    std::cout << "\x1b[35m";
    DrawBox(FUIConfig::ActionX, FUIConfig::ActionY, FUIConfig::ActionW, FUIConfig::ActionH);
    MoveCursor(FUIConfig::ActionX + FUIConfig::ActionW / 2 - 9, FUIConfig::ActionY + 1); std::cout << "[ 버튼 및 상호작용 ]";

    std::cout << "\x1b[0m";
}

void URenderManager::DrawBox(int StartX, int StartY, int Width, int Height) 
{
    MoveCursor(StartX, StartY);
    std::cout << "+";
    for (int i = 0; i < Width - 2; ++i) std::cout << "-";
    std::cout << "+";

    for (int y = StartY + 1; y < StartY + Height - 1; ++y) 
    {
        MoveCursor(StartX, y); std::cout << "|";
        MoveCursor(StartX + Width - 1, y); std::cout << "|";
    }

    MoveCursor(StartX, StartY + Height - 1);
    std::cout << "+";
    for (int i = 0; i < Width - 2; ++i) std::cout << "-";
    std::cout << "+";
}

void URenderManager::DrawPartyStatus(const std::vector<ACharacter*>& Party) 
{
    ClearStatusArea();
    
    int StartY = FUIConfig::StatusY + 3; 
    int StartX = FUIConfig::StatusX + 2;

    for (size_t i = 0; i < Party.size(); ++i) 
    {
        if (Party[i] == nullptr) continue;

        ACharacter* Member = Party[i];
        FCharacterStat Stat = Member->GetStat();
        std::string Color = Member->GetClassColor();

        MoveCursor(StartX, StartY);
        // 끝에 있던 "       " 공백 제거
        std::cout << Color << "[Lv." << Stat.Level << "]" << Member->GetPersonalityName() << " " << Member->GetName() << " (" << Member->GetJobName() << ")\x1b[0m";
        
        int ExpPieces = (Stat.MaxEXP > 0) ? static_cast<int>((static_cast<float>(Stat.CurrentEXP) / Stat.MaxEXP) * 10) : 0;
        int HPPieces = (Stat.MaxHP > 0) ? static_cast<int>((static_cast<float>(Stat.CurrentHP) / Stat.MaxHP) * 10) : 0;
        int MPPieces = (Stat.MaxMP > 0) ? static_cast<int>((static_cast<float>(Stat.CurrentMP) / Stat.MaxMP) * 10) : 0;

        MoveCursor(StartX, StartY + 1);
        std::cout << "[EXP][";
        for (int k = 0; k < 10; ++k) std::cout << (k < ExpPieces ? "■" : "□");
        std::cout << "](" << Stat.CurrentEXP << "/" << Stat.MaxEXP << ")"; // 공백 제거

        MoveCursor(StartX, StartY + 2);
        std::cout << "[HP][";
        for (int k = 0; k < 10; ++k) std::cout << (k < HPPieces ? "■" : "□");
        std::cout << "](" << Stat.CurrentHP << "/" << Stat.MaxHP << ")"; // 공백 제거

        MoveCursor(StartX, StartY + 3);
        std::cout << "[MP][";
        for (int k = 0; k < 10; ++k) std::cout << (k < MPPieces ? "■" : "□");
        std::cout << "](" << Stat.CurrentMP << "/" << Stat.MaxMP << ")"; // 공백 제거
        
        MoveCursor(StartX, StartY + 4);
        std::cout << "공격력: " << Stat.ATK << " | 스피드: " << Stat.Speed; // 공백 제거

        MoveCursor(StartX, StartY + 5);
        std::cout << "\x1b[90m무기: " << Member->GetWeaponName() << " | 방어구: " << Member->GetArmorName() << "\x1b[0m";
        
        StartY += 6; 
    }
}

void URenderManager::DrawInventory(const std::vector<FItem>& Inventory, int Gold) 
{
    int StartX = FUIConfig::InvenX + 2;
    int StartY = FUIConfig::InvenY + 3;

    for (int y = StartY; y < FUIConfig::InvenY + FUIConfig::InvenH - 1; ++y) 
    {
        MoveCursor(StartX, y);
        std::cout << "                         "; 
    }

    MoveCursor(StartX, StartY);
    std::cout << "보유 골드: \x1b[33m" << Gold << " G\x1b[0m";
    
    StartY += 2;

    for (size_t i = 0; i < Inventory.size(); ++i) 
    {
        if (StartY >= FUIConfig::InvenY + FUIConfig::InvenH - 2) 
        {
            MoveCursor(StartX, StartY); std::cout << "..."; break; 
        }

        MoveCursor(StartX, StartY);
        // 기획 반영: 아이템명 (가격G)
        std::cout << "- " << Inventory[i].Name << " (" << Inventory[i].Price << "G)";
        StartY++;
    }
}

void URenderManager::ClearDialogArea() 
{
    std::string BlankLine(FUIConfig::LogW - 3, ' '); 

    for (int y = FUIConfig::TextStartY; y < FUIConfig::LogY + FUIConfig::LogH - 1; ++y) 
    {
        MoveCursor(FUIConfig::TextStartX, y);
        std::cout << BlankLine;
    }
}

void URenderManager::ClearViewport() 
{
    std::string BlankLine(FUIConfig::ViewportW - 3, ' ');
    for (int y = FUIConfig::ViewportY + 1; y < FUIConfig::ViewportY + FUIConfig::ViewportH - 1; ++y) 
    {
        MoveCursor(FUIConfig::ViewportX + 2, y);
        std::cout << BlankLine;
    }
}

void URenderManager::ClearStatusArea() 
{
    std::string BlankLine(FUIConfig::StatusW - 3, ' ');
    // 상태창 이름표(캐릭터 상태)를 덮어쓰지 않도록 Y + 4 부터 지웁니다.
    for (int y = FUIConfig::StatusY + 4; y < FUIConfig::StatusY + FUIConfig::StatusH - 1; ++y) 
    {
        MoveCursor(FUIConfig::StatusX + 2, y);
        std::cout << BlankLine;
    }
}

void URenderManager::ClearActionArea() 
{
    std::string BlankLine(FUIConfig::ActionW - 3, ' '); 
    for (int y = FUIConfig::InputStartY; y < FUIConfig::ActionY + FUIConfig::ActionH - 1; ++y) 
    {
        MoveCursor(FUIConfig::InputStartX, y);
        std::cout << BlankLine;
    }
}

void URenderManager::DrawDefaultBackground() 
{
    ClearViewport();
    int VX = FUIConfig::ViewportX + 10;
    int VY = FUIConfig::ViewportY + 5;
    
    std::cout << "\x1b[33m"; // 황금빛 성문 스타일 연출
    MoveCursor(VX, VY);     std::cout << "    _A_                                                       _A_    ";
    MoveCursor(VX, VY + 1); std::cout << "   /   \\                                                     /   \\   ";
    MoveCursor(VX, VY + 2); std::cout << "  |_____|       _____________________________________       |_____|  ";
    MoveCursor(VX, VY + 3); std::cout << "  |  x  |      /                                     \\      |  x  |  ";
    MoveCursor(VX, VY + 4); std::cout << "  |_____|     |      TEXT CONSOLE RPG MASTERPIECE     |     |_____|  ";
    MoveCursor(VX, VY + 5); std::cout << "  |     |     |                                       |     |     |  ";
    MoveCursor(VX, VY + 6); std::cout << "  |  _  |     |       [ READY FOR THE JOURNEY ]       |     |  _  |  ";
    MoveCursor(VX, VY + 7); std::cout << "  | | | |      \\_____________________________________/      | | | |  ";
    MoveCursor(VX, VY + 8); std::cout << "  |_|_|_|___________________________________________________|_|_|_|  ";
    std::cout << "\x1b[0m";
}