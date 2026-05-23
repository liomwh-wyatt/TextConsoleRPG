#include "AGameMode.h"
#include "../Data/StringTable.h"
#include "../Data/UIConfig.h"
#include "../UI/URenderManager.h"
#include "../Entities/PlayerJobs.h"
#include "../Entities/Monster.h"
#include "../Data/GameDatabase.h"
#include "UBattleManager.h"
#include <iostream>
#include <string>
#include <random>
#include <windows.h>

AGameMode::AGameMode() : CurrentState(EGameState::Town), PartyGold(150) {}

AGameMode::~AGameMode() 
{
    for (ACharacter* Member : CompanionStorage) delete Member;
    CompanionStorage.clear();
    PlayerParty.clear();
    for (ACharacter* Merc : TavernRoster) if (Merc != nullptr) delete Merc;
    TavernRoster.clear();
}

void AGameMode::StartGame() 
{
    URenderManager::InitConsole();
    URenderManager::DrawFramework();
    
    CreatePlayerCharacter(); 
    GenerateTavernRoster();

    bool bIsGameRunning = true;
    
    while (bIsGameRunning) 
    {
        URenderManager::DrawPartyStatus(PlayerParty);
        URenderManager::DrawInventory(Inventory, PartyGold);

        switch (CurrentState) 
        {
        case EGameState::Town: RunTownState(); break;
        case EGameState::Tavern: RunTavernState(); break;
        case EGameState::ManageCompanions: RunManageCompanionsState(); break;
        case EGameState::PotionShop: RunPotionShopState(); break;
        case EGameState::Blacksmith: RunBlacksmithState(); break;
        case EGameState::Dungeon: RunDungeonState(); break;
        case EGameState::GameOver: bIsGameRunning = false; break;
        }
    }
}

static size_t GetUTF8Length(const std::string& str) 
{
    size_t length = 0;
    for (size_t i = 0; i < str.length(); ) 
    {
        unsigned char c = str[i];
        if (c >= 0 && c <= 127) i += 1;
        else if ((c & 0xE0) == 0xC0) i += 2;
        else if ((c & 0xF0) == 0xE0) i += 3;
        else if ((c & 0xF8) == 0xF0) i += 4;
        else i += 1;
        length++;
    }
    return length;
}

void AGameMode::CreatePlayerCharacter() 
{
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    std::string PlayerName;
    
    while (true) 
    {
        URenderManager::ClearDialogArea();
        URenderManager::ClearActionArea();
        
        URenderManager::MoveCursor(TX, TY);
        std::cout << "새로운 용사의 전설이 시작됩니다...";
        URenderManager::MoveCursor(TX, TY + 1);
        std::cout << "이름은 한글, 영문 관계없이 최대 6자까지 입력 가능합니다.";

        URenderManager::MoveCursor(IX, IY);     std::cout << "[ 캐릭터 생성 ]";
        URenderManager::MoveCursor(IX, IY + 2); std::cout << "이름: ";
        std::cin >> PlayerName;

        if (GetUTF8Length(PlayerName) <= 6) 
        {
            break;
        }
        
        URenderManager::MoveCursor(TX, TY + 3);
        std::cout << "\x1b[31m이름이 제한 길이(6자)를 초과했습니다! 다시 입력하세요.\x1b[0m";
        Sleep(1200);
    }

    URenderManager::ClearDialogArea();
    URenderManager::MoveCursor(TX, TY);
    std::cout << "[ " << PlayerName << " ] 님, 직업을 선택할 차례입니다.";
    URenderManager::MoveCursor(TX, TY + 2);
    std::cout << "[1] 전사  [2] 궁수  [3] 도적  [4] 마법사  [5] 사제";
    
    URenderManager::MoveCursor(IX, IY + 4); std::cout << "직업 번호: ";
    int JobChoice;
    std::cin >> JobChoice;

    ACharacter* NewPlayer = nullptr;
    switch (JobChoice) 
    {
        case 1: NewPlayer = new AWarrior(PlayerName, EPersonality::Valiant); break;
        case 2: NewPlayer = new AArcher(PlayerName, EPersonality::Valiant); break;
        case 3: NewPlayer = new ARogue(PlayerName, EPersonality::Valiant); break;
        case 4: NewPlayer = new AMage(PlayerName, EPersonality::Valiant); break;
        case 5: NewPlayer = new APriest(PlayerName, EPersonality::Valiant); break;
        default: NewPlayer = new AWarrior(PlayerName, EPersonality::Valiant); break;
    }

    CompanionStorage.push_back(NewPlayer);
    PlayerParty.push_back(NewPlayer);
    URenderManager::DrawFramework();
}

void AGameMode::GenerateTavernRoster() 
{
    for (ACharacter* Merc : TavernRoster) if (Merc != nullptr) delete Merc;
    TavernRoster.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> NameDist(0, static_cast<int>(FGameDatabase::MercenaryNames.size()) - 1);
    std::uniform_int_distribution<int> JobDist(1, 5);
    std::uniform_int_distribution<int> PersonalityDist(0, 8);

    for (int i = 0; i < 4; ++i) 
    {
        std::string RandomName = FGameDatabase::MercenaryNames[NameDist(gen)];
        EPersonality RandomPersonality = static_cast<EPersonality>(PersonalityDist(gen));
        ACharacter* NewMerc = nullptr;
        switch (JobDist(gen)) 
        {
        case 1: NewMerc = new AWarrior(RandomName, RandomPersonality); break;
        case 2: NewMerc = new AArcher(RandomName, RandomPersonality); break;
        case 3: NewMerc = new ARogue(RandomName, RandomPersonality); break;
        case 4: NewMerc = new AMage(RandomName, RandomPersonality); break;
        case 5: NewMerc = new APriest(RandomName, RandomPersonality); break;
        }
        TavernRoster.push_back(NewMerc);
    }
}

bool AGameMode::IsInParty(ACharacter* Character) 
{
    for (ACharacter* Member : PlayerParty) 
        if (Member == Character) return true;
    return false;
}

void AGameMode::RunTownState() 
{
    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    URenderManager::MoveCursor(TX, TY);     std::cout << FStringTable::System_InitTown;
    URenderManager::MoveCursor(TX, TY + 1); std::cout << FStringTable::Town_Greeting;
    URenderManager::MoveCursor(TX, TY + 2); std::cout << FStringTable::Town_Menu;

    URenderManager::MoveCursor(IX, IY);     std::cout << "[ 이동할 장소 ]";
    URenderManager::MoveCursor(IX, IY + 2); std::cout << "입력: ";
    
    int InputChoice;
    std::cin >> InputChoice;

    switch (InputChoice) 
    {
    case 1: CurrentState = EGameState::Tavern; break;
    case 2: CurrentState = EGameState::PotionShop; break;
    case 3: CurrentState = EGameState::Blacksmith; break;
    case 4: CurrentState = EGameState::Dungeon; break;
    }
}

void AGameMode::RunTavernState() 
{
    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    URenderManager::MoveCursor(TX, TY);     std::cout << FStringTable::Tavern_Welcome;
    
    URenderManager::MoveCursor(IX, IY);     std::cout << "[1] 용병 고용";
    URenderManager::MoveCursor(IX, IY + 1); std::cout << "[2] 파티 관리";
    URenderManager::MoveCursor(IX, IY + 2); std::cout << "[0] 마을로 귀환";
    URenderManager::MoveCursor(IX, IY + 4); std::cout << "입력: ";
    
    int InputChoice;
    std::cin >> InputChoice;

    if (InputChoice == 1) 
    {
        bool bHiring = true;
        while (bHiring) 
        {
            URenderManager::ClearDialogArea();
            URenderManager::ClearActionArea();
            URenderManager::DrawInventory(Inventory, PartyGold); 

            URenderManager::MoveCursor(TX, TY);
            std::cout << "\x1b[33m[ 고용 가능한 용병 목록 (고용비: 50G) ]\x1b[0m";
            
            for (size_t i = 0; i < TavernRoster.size(); ++i) 
            {
                URenderManager::MoveCursor(TX, TY + 2 + static_cast<int>(i));
                if (TavernRoster[i] == nullptr) 
                {
                    std::cout << "\x1b[90m[ " << i + 1 << " ] (고용 완료)\x1b[0m";
                }
                else 
                {
                    ACharacter* Merc = TavernRoster[i];
                    
                    std::cout << "[ " << i + 1 << " ] " << Merc->GetClassColor()
                              << "Lv." << Merc->GetStat().Level << " " 
                              << Merc->GetPersonalityName() << " " 
                              << Merc->GetName() 
                              << " (" << Merc->GetJobName() << ")\x1b[0m";
                }
            }

            URenderManager::MoveCursor(IX, IY);     std::cout << "[ 용병 고용 ]";
            URenderManager::MoveCursor(IX, IY + 2); std::cout << "고용 번호 (1~4)";
            URenderManager::MoveCursor(IX, IY + 3); std::cout << "[0] 뒤로가기";
            URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
            
            int MercChoice;
            std::cin >> MercChoice;
            
            if (MercChoice == 0) bHiring = false; 
            else if (MercChoice >= 1 && MercChoice <= 4) 
            {
                if (CompanionStorage.size() >= 11) { URenderManager::MoveCursor(IX, IY + 7); std::cout << "\x1b[31m보관함 가득참!\x1b[0m"; Sleep(1000); }
                else if (TavernRoster[MercChoice - 1] == nullptr) { URenderManager::MoveCursor(IX, IY + 7); std::cout << "이미 고용됨."; Sleep(1000); } 
                else if (PartyGold < 50) { URenderManager::MoveCursor(IX, IY + 7); std::cout << "\x1b[31m골드 부족!\x1b[0m"; Sleep(1000); } 
                else 
                {
                    PartyGold -= 50;
                    CompanionStorage.push_back(TavernRoster[MercChoice - 1]);
                    TavernRoster[MercChoice - 1] = nullptr; 
                    URenderManager::MoveCursor(IX, IY + 7); std::cout << "고용 완료!";
                    Sleep(800);
                }
            }
        }
    }
    else if (InputChoice == 2) CurrentState = EGameState::ManageCompanions;
    else if (InputChoice == 0) CurrentState = EGameState::Town;
}

void AGameMode::RunManageCompanionsState() 
{
    bool bManaging = true;
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    while (bManaging)
    {
        URenderManager::ClearDialogArea();
        URenderManager::ClearActionArea();

        URenderManager::MoveCursor(TX, TY);
        std::cout << "\x1b[36m[ 동료 보관함 (최대 11명) ]\x1b[0m  (현재 출전: \x1b[32m" << PlayerParty.size() << " / 4\x1b[0m)";

        for (size_t i = 0; i < 11; ++i) 
        {
            URenderManager::MoveCursor(TX, TY + 2 + static_cast<int>(i));
            if (i < CompanionStorage.size()) 
            {
                ACharacter* Comp = CompanionStorage[i];
                if (IsInParty(Comp)) std::cout << "\x1b[90m[ " << i + 1 << " ] " << Comp->GetName() << " ( " << Comp->GetJobName() << " - 출전 중 )\x1b[0m";
                else std::cout << "[ " << i + 1 << " ] " << Comp->GetClassColor() << Comp->GetName() << "\x1b[0m" << " ( " << Comp->GetJobName() << " )";
            } 
            else std::cout << "\x1b[37m[ " << i + 1 << " ] [ 빈 슬롯 ]\x1b[0m";
        }
        
        URenderManager::MoveCursor(IX, IY);     std::cout << "[ 파티 관리 ]";
        URenderManager::MoveCursor(IX, IY + 2); std::cout << "대상 선택 (1~11)";
        URenderManager::MoveCursor(IX, IY + 3); std::cout << "[0] 뒤로가기";
        URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
        
        int Slot;
        std::cin >> Slot;

        if (Slot == 0) bManaging = false;
        else if (Slot >= 1 && Slot <= 11) 
        {
            if (Slot - 1 >= CompanionStorage.size()) 
            {
                URenderManager::MoveCursor(IX, IY + 7); std::cout << "빈 슬롯입니다."; Sleep(800);
            }
            else 
            {
                ACharacter* Selected = CompanionStorage[Slot - 1];
                bool bActionLoop = true;
                
                while (bActionLoop)
                {
                    URenderManager::ClearActionArea();
                    URenderManager::MoveCursor(IX, IY);     std::cout << "[ " << Selected->GetName() << " ]";
                    URenderManager::MoveCursor(IX, IY + 2); std::cout << "[1] 합류 / 제외";
                    URenderManager::MoveCursor(IX, IY + 3); std::cout << "[2] 영구 해고";
                    URenderManager::MoveCursor(IX, IY + 4); std::cout << "[0] 취소";
                    URenderManager::MoveCursor(IX, IY + 6); std::cout << "입력: ";
                    
                    int Action;
                    std::cin >> Action;

                    if (Action == 0) bActionLoop = false;
                    else if (Action == 1)
                    {
                        if (Slot == 1) { URenderManager::MoveCursor(IX, IY + 8); std::cout << "\x1b[31m주인공 조작 불가\x1b[0m"; } 
                        else 
                        {
                            if (IsInParty(Selected)) 
                            {
                                for (auto it = PlayerParty.begin(); it != PlayerParty.end(); ++it) 
                                    if (*it == Selected) { PlayerParty.erase(it); break; }
                            } 
                            else 
                            {
                                if (PlayerParty.size() >= 4) { URenderManager::MoveCursor(IX, IY + 8); std::cout << "\x1b[31m파티 가득 참!\x1b[0m"; } 
                                else PlayerParty.push_back(Selected);
                            }
                        }
                        Sleep(800);
                        bActionLoop = false;
                    }
                    else if (Action == 2)
                    {
                        if (Slot == 1) { URenderManager::MoveCursor(IX, IY + 8); std::cout << "\x1b[31m주인공 해고 불가\x1b[0m"; Sleep(1000); } 
                        else 
                        {
                            URenderManager::ClearActionArea();
                            URenderManager::MoveCursor(IX, IY);     std::cout << "\x1b[31m[ 해고 경고 ]\x1b[0m";
                            URenderManager::MoveCursor(IX, IY + 2); std::cout << "정말 해고할까요?";
                            URenderManager::MoveCursor(IX, IY + 3); std::cout << "[1] 예  [0] 아니오";
                            URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
                            
                            int Confirm;
                            std::cin >> Confirm;
                            if (Confirm == 1) 
                            {
                                if (IsInParty(Selected)) 
                                {
                                    for (auto it = PlayerParty.begin(); it != PlayerParty.end(); ++it) 
                                        if (*it == Selected) { PlayerParty.erase(it); break; }
                                }
                                delete Selected;
                                CompanionStorage.erase(CompanionStorage.begin() + (Slot - 1));
                                URenderManager::MoveCursor(IX, IY + 7); std::cout << "해고 완료.";
                                Sleep(800);
                                bActionLoop = false;
                            }
                        }
                    }
                }
            }
        }
    }
    // 루프를 완전히 빠져나오면 주점으로 복귀
    CurrentState = EGameState::Tavern;
}

void AGameMode::RunPotionShopState() 
{
    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    URenderManager::MoveCursor(TX, TY);     std::cout << FStringTable::PotionShop_Welcome;
    URenderManager::MoveCursor(TX, TY + 1); std::cout << "보유 골드: \x1b[33m" << PartyGold << " G\x1b[0m"; 
    
    for (size_t i = 0; i < FGameDatabase::PotionShopItems.size(); ++i) 
    {
        const FItem& Item = FGameDatabase::PotionShopItems[i];
        URenderManager::MoveCursor(TX, TY + 3 + static_cast<int>(i));
        std::cout << "[" << i + 1 << "] " << Item.Name << " (" << Item.Price << "G) - " << Item.Description;
    }

    URenderManager::MoveCursor(IX, IY);     std::cout << "[ 물약 상점 ]";
    URenderManager::MoveCursor(IX, IY + 2); std::cout << "상품 번호 (1~" << FGameDatabase::PotionShopItems.size() << ")";
    URenderManager::MoveCursor(IX, IY + 3); std::cout << "[0] 나가기"; 
    URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
    
    int InputChoice;
    std::cin >> InputChoice;
    
    if (InputChoice >= 1 && InputChoice <= static_cast<int>(FGameDatabase::PotionShopItems.size())) 
    {
        FItem SelectedItem = FGameDatabase::PotionShopItems[InputChoice - 1];

        if (PartyGold >= SelectedItem.Price) 
        {
            PartyGold -= SelectedItem.Price;
            Inventory.push_back(SelectedItem);
            URenderManager::MoveCursor(IX, IY + 7); std::cout << "구매 완료!";
        }
        else { URenderManager::MoveCursor(IX, IY + 7); std::cout << "\x1b[31m골드 부족!\x1b[0m"; }
        Sleep(1000);
    }
    else if (InputChoice == 0) CurrentState = EGameState::Town;
}

void AGameMode::RunBlacksmithState() 
{
    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    int IX = FUIConfig::InputStartX;
    int IY = FUIConfig::InputStartY;

    URenderManager::MoveCursor(TX, TY);     std::cout << FStringTable::Blacksmith_Welcome;
    URenderManager::MoveCursor(TX, TY + 1); std::cout << "보유 골드: \x1b[33m" << PartyGold << " G\x1b[0m"; 

    for (size_t i = 0; i < FGameDatabase::BlacksmithItems.size(); ++i) 
    {
        const FItem& Item = FGameDatabase::BlacksmithItems[i];
        URenderManager::MoveCursor(TX, TY + 3 + static_cast<int>(i));
        std::cout << "[" << i + 1 << "] " << Item.Name << " (" << Item.Price << "G) - " << Item.Description;
    }

    URenderManager::MoveCursor(IX, IY);     std::cout << "[ 대장간 ]";
    URenderManager::MoveCursor(IX, IY + 2); std::cout << "상품 번호 (1~" << FGameDatabase::BlacksmithItems.size() << ")";
    URenderManager::MoveCursor(IX, IY + 3); std::cout << "[0] 나가기"; 
    URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
    
    int InputChoice;
    std::cin >> InputChoice;

    // 💡 입력한 번호가 데이터베이스 크기 안에 있다면 동적으로 구매 처리!
    if (InputChoice >= 1 && InputChoice <= static_cast<int>(FGameDatabase::BlacksmithItems.size())) 
    {
        FItem SelectedItem = FGameDatabase::BlacksmithItems[InputChoice - 1]; // 선택한 아이템 가져오기

        if (PartyGold >= SelectedItem.Price) 
        {
            PartyGold -= SelectedItem.Price;
            Inventory.push_back(SelectedItem);
            URenderManager::MoveCursor(IX, IY + 7); std::cout << "구매 완료!";
        }
        else { URenderManager::MoveCursor(IX, IY + 7); std::cout << "\x1b[31m골드 부족!\x1b[0m"; }
        Sleep(1000);
    }
    else if (InputChoice == 0) CurrentState = EGameState::Town;
}
    /*
    URenderManager::MoveCursor(TX, TY);     std::cout << FStringTable::Blacksmith_Welcome;
    URenderManager::MoveCursor(TX, TY + 1); std::cout << "보유 골드: \x1b[33m" << PartyGold << " G\x1b[0m"; 
    URenderManager::MoveCursor(TX, TY + 3); std::cout << "[1] 낡은 철검 (30G)";
    URenderManager::MoveCursor(TX, TY + 4); std::cout << "[2] 가죽 조끼 (30G)";

    URenderManager::MoveCursor(IX, IY);     std::cout << "[ 대장간 ]";
    URenderManager::MoveCursor(IX, IY + 2); std::cout << "상품 번호 (1~2)";
    URenderManager::MoveCursor(IX, IY + 3); std::cout << "[0] 나가기"; // 3을 0으로 변경
    URenderManager::MoveCursor(IX, IY + 5); std::cout << "입력: ";
    
    int InputChoice;
    std::cin >> InputChoice;

    if (InputChoice >= 1 && InputChoice <= 2) 
    {
        int ItemPrice = 30;
        FItem NewItem = (InputChoice == 1) ? FItem{"낡은 철검", EItemType::Weapon, 10, 30, "공격력 +10"} 
                                           : FItem{"가죽 조끼", EItemType::Armor, 50, 30, "최대 체력 +50"};

        if (PartyGold >= ItemPrice) 
        {
            PartyGold -= ItemPrice;
            Inventory.push_back(NewItem);
            URenderManager::MoveCursor(IX, IY + 7); std::cout << "구매 완료!";
        }
        else { URenderManager::MoveCursor(IX, IY + 7); std::cout << "\x1b[31m골드 부족!\x1b[0m"; }
        Sleep(1000);
    }
    else if (InputChoice == 0) CurrentState = EGameState::Town;
}
*/
void AGameMode::RunDungeonState() 
{
    URenderManager::ClearDialogArea();
    URenderManager::ClearActionArea();
    
    int TX = FUIConfig::TextStartX;
    int TY = FUIConfig::TextStartY;
    
    URenderManager::MoveCursor(TX, TY);
    std::cout << "어두컴컴한 던전 깊은 곳으로 진입합니다...";
    Sleep(1500);

    // 💡 데이터베이스에서 무작위 몬스터 정보를 하나 골라옵니다.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> MobDist(0, static_cast<int>(FGameDatabase::DungeonMonsters.size()) - 1);
    
    const FMonsterData& RandomMobData = FGameDatabase::DungeonMonsters[MobDist(gen)];

    // 골라온 데이터를 기반으로 몬스터 객체 생성!
    AMonster* EnemyMob = new AMonster(RandomMobData.Name, RandomMobData.MaxHP, RandomMobData.ATK, RandomMobData.Speed);

    bool bIsVictory = UBattleManager::RunAutoBattle(PlayerParty, EnemyMob);
    delete EnemyMob;

    if (bIsVictory) 
    {
        URenderManager::ClearDialogArea();
        URenderManager::MoveCursor(TX, TY);
        std::cout << "\x1b[32m[ 던전 정찰 토벌 완료! ]\x1b[0m";

        int RewardGold = 60;
        int RewardEXP = 50;
        PartyGold += RewardGold;

        URenderManager::MoveCursor(TX, TY + 2);
        std::cout << "획득 전리품 골드: \x1b[33m+" << RewardGold << " G\x1b[0m";
        URenderManager::MoveCursor(TX, TY + 3);
        std::cout << "파티원 획득 경험치: \x1b[32m+" << RewardEXP << " EXP\x1b[0m";

        int PrintLine = TY + 5;
        for (ACharacter* Member : PlayerParty) 
        {
            if (Member != nullptr && !Member->IsDead()) 
            {
                int PreLevel = Member->GetStat().Level;
                Member->GainEXP(RewardEXP); 

                if (Member->GetStat().Level > PreLevel) 
                {
                    URenderManager::MoveCursor(TX, PrintLine++);
                    std::cout << "\x1b[35m★ LEVEL UP ★ " << Member->GetName() << " (Lv." << PreLevel << " -> Lv." << Member->GetStat().Level << ")\x1b[0m";
                }
            }
        }
        Sleep(3000);
        URenderManager::DrawDefaultBackground(); 

        // 💡 버그 픽스: 무사히 귀환했으므로, 주점의 대기열을 새로운 용병들로 물갈이합니다!
        GenerateTavernRoster();

        CurrentState = EGameState::Town; 
    }
    else 
    {
        CurrentState = EGameState::GameOver; 
    }
}