#pragma once
#include <vector>
#include <string>
#include "../Items/Item.h"

class ACharacter;

class URenderManager 
{
public:
    static void InitConsole();
    static void ClearScreen();
    static void MoveCursor(int X, int Y);
    static void DrawFramework();
    static void DrawBox(int StartX, int StartY, int Width, int Height);
    
    static void DrawPartyStatus(const std::vector<ACharacter*>& Party);
    static void DrawInventory(const std::vector<FItem>& Inventory, int Gold);
    static void ClearDialogArea();
    
    static void ClearViewport();
    static void ClearStatusArea();
    static void ClearActionArea();
    
    // 💡 픽셀 아트 엔진용 렌더링 함수들
    static void RenderViewport();
    static void DrawDefaultBackground();    
    static void DrawBossIntroAnimation();   
    static void DrawBossBackground();
    static void DrawSceneArtwork(int SceneIdx);
    static void ShowMonsterArtwork(std::string MonsterName); // 몬스터 오버레이
    static void ClearMonsterArtwork();                       // 몬스터 지우기
    
    // 💡 이펙트 애니메이션 함수들
    static void DrawSlashEffect();
    static void DrawMagicEffect(std::string ColorANSI);

private:
    // 💡 3중 레이어 캔버스 (배경, 몬스터, 이펙트)
    static char SceneCanvas[27][44];
    static char MonsterCanvas[27][44];
    static char EffectCanvas[27][44];

    static void ClearCanvas(char Canvas[27][44]);
    static void DrawSprite(char Canvas[27][44], const std::vector<std::string>& Sprite, int StartX, int StartY);
    static std::string GetPixelColor(char c);
};