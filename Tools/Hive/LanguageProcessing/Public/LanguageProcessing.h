#pragma once
#include <vector>
#include <string>
#include <iostream>

namespace Atlas::LanguageProcessing
{
    struct FIntent
    {
        std::string Keyword; 
        std::vector<std::string> Values;
    };

    void ParseValues();
}
