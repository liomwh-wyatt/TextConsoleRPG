#pragma once
#include <vector>
#include "../Items/Item.h"

class ACharacter;

enum class EGameState 
{
    Town, Tavern, ManageCompanions, PotionShop, Blacksmith, Inventory, Dungeon, Boss, Ending, GameOver
};

class AGameMode 
{
public:
    AGameMode();
    ~AGameMode();
    void StartGame();

private:
    EGameState CurrentState;
    
    std::vector<ACharacter*> CompanionStorage;
    std::vector<ACharacter*> PlayerParty;
    std::vector<ACharacter*> TavernRoster;
    
    int PartyGold;
    std::vector<FItem> Inventory;

    void CreatePlayerCharacter();
    void GenerateTavernRoster();
    bool IsInParty(ACharacter* Character);
    
    void RunTownState();
    void RunTavernState();
    void RunManageCompanionsState();
    void RunPotionShopState();
    void RunBlacksmithState();
    void RunDungeonState();
    void RunInventoryState();
    void RunBossState();
    void RunEndingState();
};