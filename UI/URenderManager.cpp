#define NOMINMAX
#include "URenderManager.h"
#include "../Entities/ACharacter.h"
#include "../Data/UIConfig.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <algorithm>

static std::string GetPixelColor(char c) 
{
    switch(c) 
    {
    case 'S': return "\x1b[48;2;135;206;235m"; // 하늘색 (Sky)
    case 'G': return "\x1b[48;2;34;139;34m";   // 진녹색 (Grass)
    case 'g': return "\x1b[48;2;124;252;0m";   // 연녹색 (Light Grass)
    case 'D': return "\x1b[48;2;60;40;20m";    // 짙은 나무색 (Dark Wood)
    case 'L': return "\x1b[48;2;139;105;20m";  // 밝은 나무색 (Light Wood)
    case 'R': return "\x1b[48;2;220;20;60m";   // 지붕/물약 빨간색 (Red)
    case 'Y': return "\x1b[48;2;255;215;0m";   // 금색/노란색 (Yellow)
    case 'W': return "\x1b[48;2;245;245;245m"; // 흰색 (White)
    case 'A': return "\x1b[48;2;169;169;169m"; // 회색 (Gray)
    case 'a': return "\x1b[48;2;60;60;60m";    // 진회색 (Dark Gray)
    case 'P': return "\x1b[48;2;128;0;128m";   // 보라색 (Purple)
    case 'C': return "\x1b[48;2;0;255;255m";   // 청록색 (Cyan)
    case 'O': return "\x1b[48;2;255;140;0m";   // 주황색 (Orange)
    case 'F': return "\x1b[48;2;255;69;0m";    // 불꽃 빨간색 (Fire)
    case 'B': return "\x1b[48;2;20;20;20m";    // 칠흑 (Black)
    default:  return "\x1b[0m";                // 투명
    }
}
char URenderManager::SceneCanvas[27][44];
char URenderManager::MonsterCanvas[27][44];
char URenderManager::EffectCanvas[27][44];

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

void URenderManager::RenderViewport() 
{
    std::string OutputBuffer = "";
    int StartX = FUIConfig::ViewportX + 2; // 테두리 두께 보정
    int StartY = FUIConfig::ViewportY + 1; 

    for (int y = 0; y < 27; ++y) 
    {
        OutputBuffer += "\x1b[" + std::to_string(StartY + y) + ";" + std::to_string(StartX) + "H";
        for (int x = 0; x < 44; ++x) 
        {
            char pixel = EffectCanvas[y][x];            
            if (pixel == '.') pixel = MonsterCanvas[y][x]; 
            if (pixel == '.') pixel = SceneCanvas[y][x];   
            if (pixel == '.') pixel = 'B';                 
            
            OutputBuffer += GetPixelColor(pixel) + "  "; 
        }
    }
    OutputBuffer += "\x1b[0m"; 
    std::cout << OutputBuffer; 
}

void URenderManager::DrawDefaultBackground() { DrawSceneArtwork(5); }

void URenderManager::DrawBossIntroAnimation()
{
    int StartX = FUIConfig::ViewportX + 2;
    int StartY = FUIConfig::ViewportY + 1;
    int CenterX = 22; // 44 / 2
    int CenterY = 13; // 27 / 2

    for (int frame = 0; frame < 30; ++frame) 
    {
        float Time = frame * 0.3f;
        float Radius = 6.0f + std::sin(Time * 2.0f) * 2.0f; 
        std::string Output = "";

        for (int y = 0; y < 27; ++y) // 💡 26 -> 27로 변경
        {
            Output += "\x1b[" + std::to_string(StartY + y) + ";" + std::to_string(StartX) + "H";
            for (int x = 0; x < 44; ++x) // 💡 40 -> 44로 변경
            {
                float dx = static_cast<float>(x - CenterX) * 0.5f;
                float dy = static_cast<float>(y - CenterY);
                float Dist = std::sqrt(dx * dx + dy * dy); 

                if (Dist < Radius) 
                {
                    int g = std::max(0, static_cast<int>(200 * (1.0f - Dist / Radius)));
                    Output += "\x1b[48;2;255;" + std::to_string(g) + ";0m  ";
                }
                else if (Dist < Radius * 2.0f) 
                {
                    int r = std::max(0, static_cast<int>(200 * (1.0f - (Dist - Radius) / Radius)));
                    Output += "\x1b[48;2;" + std::to_string(r) + ";0;0m  "; 
                }
                else 
                {
                    Output += GetPixelColor(SceneCanvas[y][x]) + "  ";
                }
            }
        }
        Output += "\x1b[0m";
        std::cout << Output;
        Sleep(50); 
    }
}

void URenderManager::DrawBossBackground() 
{
    ClearViewport();
    int VX = FUIConfig::ViewportX + 10;
    int VY = FUIConfig::ViewportY + 5;
    
    // 붉은색 테마 적용
    std::cout << "\x1b[31m"; 
    
    MoveCursor(VX, VY);     std::cout << "    /\\                                         /\\    ";
    MoveCursor(VX, VY + 1); std::cout << "   /  \\      [ WARNING: BOSS ENCOUNTER ]      /  \\   ";
    MoveCursor(VX, VY + 2); std::cout << "  |    |_____________________________________|    |  ";
    MoveCursor(VX, VY + 3); std::cout << "  | x  |                                     |  x |  ";
    MoveCursor(VX, VY + 4); std::cout << "  |____|        THE DEMON KING'S LAIR        |____|  ";
    MoveCursor(VX, VY + 5); std::cout << "  |    |                                     |    |  ";
    MoveCursor(VX, VY + 6); std::cout << "  |    |        PREPARE FOR BATTLE!          |    |  ";
    
    std::cout << "\x1b[0m"; // 색상 초기화
}

void URenderManager::DrawSceneArtwork(int SceneIdx) 
{
    ClearCanvas(SceneCanvas);
    ClearCanvas(MonsterCanvas);
    ClearCanvas(EffectCanvas);

    std::vector<std::string> Art;

    switch (SceneIdx) 
    {
    case 0: // 마을 (구름, 산, 집 2채, 그림자 디테일)
        Art = {
            "SSSSSSSSSSWWWWSSSSSSSSSSSSSSSSSSSSSSSSSS",
            "SSSSSSSSSWWWWWWSSSSSSSSSSSSSSSSSSSSSSSSS",
            "SSYYSSSSSWWWWWWSSSSSSSSSSSWWSSSSSSSSSSSS",
            "SYYYYSSSSSSWWSSSSSSSSSSSSWWWWSSSSSSSSSSS",
            "SSYYSSSSSSSSSSSSSSSAAAASSSWWSSSSSSSSSSSS",
            "SSSSSSSSSSSSSSSSSSAAAAAASSSSSSSSSSSSSSSS",
            "SSSSSSSSAAASSSSSSAAAAAAAASSSSSSSSSSSSSSS",
            "SSSSSSSAAAAASSSSAAAAAAAAaASSSSSSSSSSSSSS",
            "SSSSSsAAAAAAASSAAAAAAAAAaaSsSSSSSSSSSSSS",
            "SSSSSsAAAAAAAAAAAAAAAAAAaaassSSSSSSSSSSS",
            "ssssssAAAAAAAAAAAAAAAAAAaaasssssssssssss",
            "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
            "GgggggggggggggGGGGGGGGGGGGgggggggggggggG",
            "GgRRRRRRRgggggGGGGGGGGGGGGgRRRRRRRRRgggG",
            "GgRRRRRRRRRgggGGGGGGGGGGGGgRRRRRRRRRRRcG",
            "GgRRRRRRRRRRRcGGGGGGGGGGGGgRRRRRRRRRRRcG",
            "GgLLLLLLLLLLLcGGGGGGGGGGGGgLLLLLLLLLLlcG",
            "GgLAAALDDDLAAcGGGGGGGGGGGGgLAAALDDDLAAcG",
            "GgLAAALDDDLAAcGGGGGGGGGGGGgLAAALDDDLAAcG",
            "GgLLLLLDDDLLLcGGGGGGGGGGGGgLLLLLDDDLLlcG",
            "GgLLLLLDDDLLLcGGGGGGGGGGGGgLLLLLDDDLLlcG",
            "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv",
            "vvddddddddddvvvvvvvvvvvvvvvvddddddddddvv",
            "vvddddddddddvvvvvvvvvvvvvvvvddddddddddvv",
            "vvddddddddddvvvvvvvvvvvvvvvvddddddddddvv",
            "vvddddddddddvvvvvvvvvvvvvvvvddddddddddvv"
        }; break;
    case 1: // 주점 (벽난로 조명, 카운터, 선반)
        Art = {
            "dddddddddddddddddddddddddddddddddddddddd",
            "ddLdLdLddddddddddddddddddddddddLdLdLdddd",
            "ddLLLLLddddddddddddddddddddddddLLLLLdddd",
            "ddLYYcLddddddddddddddddddddddddLYYcLdddd",
            "ddLLLLLddddddddddddddddddddddddLLLLLdddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
            "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
            "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddAAAAAAAddddddddddddddddddAAAAAAAddddd",
            "dddAFFFFaAddddddddddddddddddAFFFFaAddddd",
            "dddAFFYFaAddddddddddddddddddAFFYFaAddddd",
            "dddAAFFaAAddddddddddddddddddAAFFaAAddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
            "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
            "dddddddddddddddddddddddddddddddddddddddd",
            "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
            "LddddLLddddLLddddLLddddLLddddLLddddLLddd",
            "LddddLLddddLLddddLLddddLLddddLLddddLLddd",
            "LddddLLddddLLddddLLddddLLddddLLddddLLddd",
            "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"
        }; break;
    case 2: // 물약 상점 (신비한 보라색 오라, 진열대)
        Art = {
            "pppppppppppppppppppppppppppppppppppppppp",
            "pppppppppppppppppppppppppppppppppppppppp",
            "ppDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDpp",
            "ppD..................................Dpp",
            "ppD..W..W..W..W..W..W..W..W..W..W..W.Dpp",
            "ppD..R..R..C..C..Y..Y..P..P..G..G..A.Dpp",
            "ppD.RR.RR.CC.CC.YY.YY.PP.PP.GG.GG.AA.Dpp",
            "ppDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDpp",
            "ppD..................................Dpp",
            "ppD....WW......WW......WW......WW....Dpp",
            "ppD...RRRR....CCCC....YYYY....PPPP...Dpp",
            "ppD..RRrRRR..CCcCCC..YYyYYY..PPpPPP..Dpp",
            "ppD..RRrRRR..CCcCCC..YYyYYY..PPpPPP..Dpp",
            "ppDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDpp",
            "ppD..................................Dpp",
            "ppD........WWW............WWW........Dpp",
            "ppD.......RRRRR..........CCCCC.......Dpp",
            "ppD......RRRrRRR........CCCcCCC......Dpp",
            "ppD......RRRrRRR........CCCcCCC......Dpp",
            "ppD......RRRRRRR........CCCCCCC......Dpp",
            "ppDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDpp",
            "pppppppppppppppppppppppppppppppppppppppp",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd",
            "dddddddddddddddddddddddddddddddddddddddd"
        }; break;
    case 3: // 대장간 (타오르는 용광로, 모루)
        Art = {
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaaa",
            "aaBBBaBBBaBBBaBBBaBBBaBBBaBBBaBBBaBBbaaa",
            "aaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaaa",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaa",
            "aaBBBaBBBaBBBaBBBaBBBaBBBaBBBaBBBaBBbaaa",
            "aaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaBaaaaaa",
            "aaaaaaaaaaaaaaaFFFFFaaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaFFFFFFFaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaFFFYYYFFFaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaFFFYYYFFFaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaFFFFFFFaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaaFFFFFaaaaaaaaaaaaaaaaaaaa",
            "AAAAAAAAAAAAaaaFFFFFaaaaaAAAAAAAAAAAAAAA",
            "AAAAAAAAAAAAaaaFFFFFaaaaaAAAAAAAAAAAAAAA",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "AAAAAABAAAAAAAaaaaaaaaaaaAAAAAABAAAAAAAA",
            "AAAAABaBAAAAAAaaaaaaaaaaaAAAAABaBAAAAAAA",
            "AAAAABaBAAAAAAaaaaaaaaaaaAAAAABaBAAAAAAA",
            "AAAAAABAAAAAAAaaaaaaaaaaaAAAAAABAAAAAAAA",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        }; break;
    case 4: // 던전 (음산한 딥 다크 브릭)
        Art = {
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BcBcccBcBBBBBBBBBBBBBBBBBBBBBBBBBcBcccBc",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BcBBBBBBcBBBBBBBBBBBBBBBBBBBBBBcBBBBBBcB",
            "cBcBBBBcBcBBBBBBBBBBBBBBBBBBBBcBcBBBBcBc",
            "BcBBBBBBcBBBBBBBBBBBBBBBBBBBBBBcBBBBBBcB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BcBcccBcBBBBBBBBBBBBBBBBBBBBBBBBBcBcccBc",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BcBBBBBBcBBBBBBBBBBBBBBBBBBBBBBcBBBBBBcB",
            "cBcBBBBcBcBBBBBBBBBBBBBBBBBBBBcBcBBBBcBc",
            "BcBBBBBBcBBBBBBBBBBBBBBBBBBBBBBcBBBBBBcB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BcBcccBcBBBBBBBBBBBBBBBBBBBBBBBBBcBcccBc",
            "BBcBBBcBBBBBBBBBBBBBBBBBBBBBBBBBBcBBBcBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        }; break;
    case 5: // 타이틀 (거대한 검)
        Art = {
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBWBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBWaWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBWaAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBWaAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBWaAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBWaAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBWaAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBWaAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBWaAAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBWaAAAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBWaAAAAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBWaAAAAAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBWaAAAAAAAAAAAWBBBBBBBBBBBBBBBBBBBB",
            "BBBBBYYYYYYYYYYYYYYYYYYBBBBBBBBBBBBBBBBB",
            "BBBBBByyyyyyyyyyyyyyyyBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBYyYBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBRrRBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBRrRBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
            "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        }; break;
    }

    int ArtX = (44 - static_cast<int>(Art[0].size())) / 2;
    int ArtY = (27 - static_cast<int>(Art.size())) / 2;
    DrawSprite(SceneCanvas, Art, ArtX, ArtY);
    RenderViewport();
}

void URenderManager::ShowMonsterArtwork(std::string MonsterName) 
{
    ClearCanvas(MonsterCanvas);
    std::vector<std::string> Spr;

    if (MonsterName == "고블린 정찰병") Spr = {
        "........vGv........",
        ".......gGgGg.......",
        "......gGGBGGg......",
        ".......gGgGg.......",
        "........gGg........",
        ".......ddddd.......",
        "......aadddaa......",
        "........d.d........"
    };
    else if (MonsterName == "오크 흑마법사") Spr = {
        ".........B.........",
        "........PBP........",
        "........PBP........",
        ".......vvvvv.......",
        "......vvvvvvv......",
        "......vRvvvRv......",
        ".......vvvvv.......",
        "......BdddddP......",
        ".....BBdddddPP.....",
        ".....BBdddddpP.....",
        "......Bd...dp......"
    };
    else if (MonsterName == "포악한 트롤") Spr = {
        "......cCCCCCc......",
        ".....cCccCccCc.....",
        ".....ccBcccBcc.....",
        ".....cCcccccCc.....",
        "......ccWWWcc......",
        ".....cdddddddc.....",
        "....ccdddddddcc....",
        "....c.dd...dd.c....",
        "......dd...dd......"
    };
    else if (MonsterName == "해골 기사") Spr = {
        "........AAA........",
        ".......AWWWA.......",
        ".......WRWRW.......",
        ".......AWWWA.......",
        "........AAA........",
        ".......AAaAA.......",
        "......AaAaAaA......",
        "......A.AaA.A......",
        "........A.A........"
    };
    else if (MonsterName == "파멸의 마왕 둠브링어") Spr = {
        "......r.......r......",
        ".....rR.......Rr.....",
        ".....RRr.....rRR.....",
        "......RRaBBBaRR......",
        ".......aRRRRRa.......",
        ".......BBBRBBB.......",
        "......BppBBBppB......",
        ".....BBppBBBppBB.....",
        "....BBBBpppppBBBB....",
        "....BB..BBBBB..BB....",
        "........BB.BB........",
        "........B...B........"
    };

    int MonX = (44 - static_cast<int>(Spr[0].size())) / 2;
    int MonY = 27 - static_cast<int>(Spr.size()) - 3; // 밑바닥에서 3칸 띄움
    DrawSprite(MonsterCanvas, Spr, MonX, MonY);
    RenderViewport();
}

void URenderManager::ClearMonsterArtwork() 
{
    ClearCanvas(MonsterCanvas);
    RenderViewport();
}

void URenderManager::DrawSlashEffect() 
{
    // 프레임 1
    std::vector<std::string> F1 = {
        "W...",
        ".W..",
        "..W.",
        "...W"
    };
    // 프레임 2
    std::vector<std::string> F2 = {
        "AW...",
        ".AW..",
        "..AW.",
        "...AW"
    };

    DrawSprite(EffectCanvas, F1, 20, 11); RenderViewport(); Sleep(60); ClearCanvas(EffectCanvas);
    DrawSprite(EffectCanvas, F2, 22, 13); RenderViewport(); Sleep(60); ClearCanvas(EffectCanvas);
    RenderViewport(); // 잔상 제거
}

void URenderManager::DrawMagicEffect(std::string ColorANSI) 
{
    char MC = 'W'; // 기본 흰색
    if (ColorANSI.find("31") != std::string::npos) MC = 'R';      // 전사
    else if (ColorANSI.find("32") != std::string::npos) MC = 'G'; // 궁수
    else if (ColorANSI.find("33") != std::string::npos) MC = 'Y'; // 도적
    else if (ColorANSI.find("34") != std::string::npos) MC = 'S'; // 마법사
    else if (ColorANSI.find("36") != std::string::npos) MC = 'C'; // 사제

    std::vector<std::string> F1 = { std::string(1, MC) };
    std::vector<std::string> F2 = { 
        std::string(1, MC) + "." + std::string(1, MC) + "." + std::string(1, MC),
        "." + std::string(1, MC) + "." + std::string(1, MC) + ".",
        std::string(1, MC) + "." + std::string(1, MC) + "." + std::string(1, MC)
    };

    DrawSprite(EffectCanvas, F1, 21, 13); RenderViewport(); Sleep(80); ClearCanvas(EffectCanvas);
    DrawSprite(EffectCanvas, F2, 20, 12); RenderViewport(); Sleep(80); ClearCanvas(EffectCanvas);
    RenderViewport(); 
}

void URenderManager::ClearCanvas(char Canvas[27][44]) 
{
    for (int y = 0; y < 27; ++y)
        for (int x = 0; x < 44; ++x)
            Canvas[y][x] = '.'; 
}

void URenderManager::DrawSprite(char Canvas[27][44], const std::vector<std::string>& Sprite, int StartX, int StartY) 
{
    for(size_t y = 0; y < Sprite.size(); ++y) 
        for(size_t x = 0; x < Sprite[y].size(); ++x) 
            if(StartY + y < 27 && StartX + x < 44 && Sprite[y][x] != '.') 
                Canvas[StartY + y][StartX + x] = Sprite[y][x];
}

std::string URenderManager::GetPixelColor(char c) 
{
    switch(c) 
    {
    case 'W': return "\x1b[48;2;255;255;255m"; // 흰색
    case 'A': return "\x1b[48;2;180;180;180m"; // 밝은 회색
    case 'a': return "\x1b[48;2;100;100;100m"; // 어두운 회색
    case 'B': return "\x1b[48;2;15;15;15m";    // 완전 검은색
    case 'S': return "\x1b[48;2;100;149;237m"; // 하늘색
    case 's': return "\x1b[48;2;25;25;112m";   // 밤하늘색
    case 'G': return "\x1b[48;2;34;139;34m";   // 잔디색
    case 'g': return "\x1b[48;2;154;205;50m";  // 밝은 잔디색
    case 'v': return "\x1b[48;2;0;100;0m";     // 어두운 잔디색
    case 'D': return "\x1b[48;2;139;69;19m";   // 나무색
    case 'd': return "\x1b[48;2;92;51;23m";    // 어두운 나무색
    case 'L': return "\x1b[48;2;222;184;135m"; // 밝은 나무/피부색
    case 'R': return "\x1b[48;2;220;20;60m";   // 빨간색
    case 'r': return "\x1b[48;2;139;0;0m";     // 핏빛(어두운 빨강)
    case 'Y': return "\x1b[48;2;255;215;0m";   // 황금/노란색
    case 'y': return "\x1b[48;2;184;134;11m";  // 어두운 금색
    case 'C': return "\x1b[48;2;0;255;255m";   // 청록색(시안)
    case 'c': return "\x1b[48;2;0;139;139m";   // 어두운 청록색
    case 'P': return "\x1b[48;2;148;0;211m";   // 보라색
    case 'p': return "\x1b[48;2;75;0;130m";    // 짙은 보라색
    case 'F': return "\x1b[48;2;255;69;0m";    // 불꽃 주황색
    default:  return "\x1b[0m";                // 리셋 (투명)
    }
}