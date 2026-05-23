#pragma once
#include <vector>
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
    static void DrawDefaultBackground();
};