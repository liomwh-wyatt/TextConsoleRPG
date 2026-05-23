#pragma once
#include <string>

enum class EItemType 
{
    Potion,
    Weapon,
    Armor
};

struct FItem 
{
    std::string Name;
    EItemType Type;
    int StatValue; 
    int Price;
    std::string Description;
};