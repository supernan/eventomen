#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "omenpattern.h"

using namespace omen_pattern;

vector<string> OmenPattern::sm_vActions;
vector<string> OmenPattern::sm_vTriggers;

bool OmenPattern::__LoadWordTable(const string &rPath, vector<string> &vWords, string &rErrInfo)
{
    if (access(rPath.c_str(), F_OK) == -1)
    {
        rErrInfo = "WordTable path is error " + rPath;
        return false;
    }
    vWords.clear();

    ifstream input(rPath.c_str());
    string word;
    while (getline(input, word))
    {
        vWords.push_back(word);
    }
    return true;
}


bool OmenPattern::InitPatternData(const string &rTriggerPath, const string &rActionPath, string &rErrInfo)
{
    if (!__LoadWordTable(rTriggerPath, sm_vTriggers, rErrInfo))
    {
        return false;
    }
    if (!__LoadWordTable(rActionPath, sm_vActions, rErrInfo))
    {
        return false;
    }
    return true;
}


bool OmenPattern::__DocWordCollocation(const string sText)
{
    if (sm_vTriggers.empty())
        return false;
    if (sm_vActions.empty())
        return false;

    for (int i = 0; i < sm_vTriggers.size(); i++)
    {
        string sTrigger = sm_vTriggers[i];
        int nPos = sText.find(sTrigger);
        if (nPos == string::npos)
        {
            continue;
        }
        int nLen = sTrigger.length();
        bool bMatch = false;
        string sRemain = sText.substr(nPos + nLen);
        for (int j = 0; j < sm_vActions.size(); j++)
        {
            if (sRemain.find(sm_vActions[j]) != string::npos)
            {
                bMatch = true;
                break;
            }
        }
        if (bMatch)
            return true;
    }
    return false;
}


bool OmenPattern::WordCollocationPattern(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes, string &rErrInfo)
{
    if (sm_vActions.empty())
    {
        rErrInfo = "Action set is empty ";
        return false;
    }
    if (sm_vTriggers.empty())
    {
        rErrInfo = "Trigger set is empty ";
        return false;
    }

    rRes.clear();
    for (int i = 0; i < rCorpus.size(); i++)
    {
        if (__DocWordCollocation(rCorpus[i]->source))
        {
            rRes.push_back(rCorpus[i]);
        }
    }

    return true;
}

