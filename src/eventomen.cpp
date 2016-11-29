#include <fstream>
#include <map>
#include "glog/logging.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "eventomen.h"
#include "util.h"
#include "omenpattern.h"

using namespace WeiboTopic_ICT;
using namespace event_omen;
using namespace tools;
using namespace omen_pattern;

bool CEventOmenDetector::__LoadConfigFile(const string &rConfPath)
{
    TiXmlDocument *pDocument = new TiXmlDocument(rConfPath.c_str());
    if (pDocument == NULL)
    {
        LOG(FATAL) << "Load Config file Failed " << rConfPath << endl;
        return false;
    }
    pDocument->LoadFile();
    TiXmlElement *pRootElement = pDocument->RootElement();
    if (pRootElement == NULL)
    {
        LOG(FATAL) << "Root Node is NULL Config file is error" << endl;
        return false;
    }

    TiXmlElement *pEventModelNode = pRootElement->FirstChildElement();
    if (pEventModelNode == NULL)
    {
        LOG(FATAL) << "Event Model Node is NUll Config file is error " << endl;
        return false;
    }
    m_sEventModelConf = pEventModelNode->FirstChild()->Value();

    TiXmlElement *pTenseModelNode = pEventModelNode->NextSiblingElement();
    if (pTenseModelNode == NULL)
    {
        LOG(FATAL) << "Tense Model Node is NUll Config file is error " << endl;
        return false;
    }
    m_sTenseModelConf = pTenseModelNode->FirstChild()->Value();

    TiXmlElement *pBlackListNode = pTenseModelNode->NextSiblingElement();
    if (pBlackListNode == NULL)
    {
        LOG(FATAL) << "Black List Node is NUll Config file is error " << endl;
        return false;
    }
    m_sBlackListPath = pBlackListNode->FirstChild()->Value();

    TiXmlElement *pTriggerNode = pBlackListNode->NextSiblingElement();
    if (pTriggerNode == NULL)
    {
        LOG(FATAL) << "Trigger Node is NUll Config file is error " << endl;
        return false;
    }
    m_sTriggerPath = pTriggerNode->FirstChild()->Value();

    TiXmlElement *pActionNode = pTriggerNode->NextSiblingElement();
    if (pActionNode == NULL)
    {
        LOG(FATAL) << "Action Node is NUll Config file is error " << endl;
        return false;
    }
    m_sActionPath = pActionNode->FirstChild()->Value();

    TiXmlElement *pSentiNode = pActionNode->NextSiblingElement();
    if (pSentiNode == NULL)
    {
        LOG(FATAL) << "Senti Node is NUll Config file is error " << endl;
        return false;
    }
    m_sSentiModelConf = pSentiNode->FirstChild()->Value();

    LOG(INFO) << "Load config file succeed" << endl;
    delete pDocument;
    return true;
}


bool CEventOmenDetector::__InitBlackList()
{
     if ((access(m_sBlackListPath.c_str(), F_OK)) == -1)
     {
         LOG(FATAL) << "Black List file does not exit" << m_sBlackListPath << endl;
         return false;
     }

    ifstream input(m_sBlackListPath.c_str());
    string sKeyword;
    while (getline(input, sKeyword))
    {
        m_vBlackList.push_back(sKeyword);
    }

    LOG(INFO) << "Load Black List Succeed" << endl;
    return true;
}


CEventOmenDetector::CEventOmenDetector(const string &rConfPath)
{
    //FLAGS_log_dir = "../logs/eventomen";
    //google::InitGoogleLogging("event_omen");

    if (!__LoadConfigFile(rConfPath))
    {
        LOG(FATAL) << "CEventOmenDetector Failed Load eventomen config file failed" << endl;
    }

    if (!__InitBlackList())
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init black list failed" << endl;
    }


    m_pSentiModel = new CSentimentModel(m_sSentiModelConf);
    if (m_pSentiModel == NULL)
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init Sentimodel Failed" <<endl;
    }

    string sEventName = "eventmodel";
    m_pEventClassifier = new CClassifier(m_sEventModelConf, sEventName);
    if (m_pEventClassifier == NULL)
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init EventClassifier Failed" << endl;
    }

    string sTenseName = "tensemodel";
    m_pTenseClassifier = new CClassifier(m_sTenseModelConf, sTenseName);
    if (m_pTenseClassifier == NULL)
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init TenseClassifier Failed" << endl;
    }

    m_pACauto = new AC_automation();
    if (m_pACauto == NULL)
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init AC_automation Failed" << endl;
    }

    string sPatternErr;
    if (!OmenPattern::InitPatternData(m_sTriggerPath, m_sActionPath, sPatternErr))
    {
        LOG(FATAL) << "CEventOmenDetector Failed Init Omen Pattern Failed" << sPatternErr << endl;
    }

    LOG(INFO) << "Init EODetector" << endl;
}


CEventOmenDetector::~CEventOmenDetector()
{
    if (m_pSentiModel != NULL)
        delete m_pSentiModel;
    if (m_pEventClassifier != NULL)
        delete m_pEventClassifier;
    if (m_pTenseClassifier != NULL)
        delete m_pTenseClassifier;
    if (m_pACauto != NULL)
        delete m_pACauto;
}


bool CEventOmenDetector::__KeywordsFilter(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__KeywordsFilter Failed Corpus is empty" << endl;
        return false;
    }
    m_pACauto->clear();
    m_pACauto->build_automation(m_vBlackList);
    rRes.clear();

    for (int i = 0; i < rCorpus.size(); i++)
    {
        string sText = rCorpus[i]->source;
        map<int, string> mPattern = m_pACauto->query(sText);
        if (mPattern.empty())
            rRes.push_back(rCorpus[i]);
    }
    m_pACauto->clear();
    LOG(INFO) << "__KeywordsFilter Succeed" << endl;
    return true;
}


bool CEventOmenDetector::__DetectBySentiment(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__DetectBySentiment Failed Corpus is empty" << endl;
        return false;
    }
    rRes.clear();
    vector<double> scores;
    if (!m_pSentiModel->BatchAnalysis(rCorpus, scores))
    {
        LOG(ERROR) << "__DetectBySentiment Error" << endl;
        return false;
    }
    for (int i = 0; i < scores.size(); i++)
    {
        if (scores[i] < 0)
            rRes.push_back(rCorpus[i]);
    }
    return true;
}


bool CEventOmenDetector::__DetectByEvent(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__DetectByEvent Failed Corpus is empty" << endl;
        return false;
    }

    rRes.clear();
    vector<vector<pstWeibo> > vPredictRes;
    if (!m_pEventClassifier->BatchPredict(rCorpus, vPredictRes))
    {
        LOG(ERROR) << "__DetectByEvent Failed EventClassifier Failed" << endl;
        return false;
    }

    int gSensitiveType[] = {1, 2, 3, 5, 9, 11}; //TODO
    for (int i = 0; i < 6; i++)
    {
        int nLabel = gSensitiveType[i];
        if (nLabel >= vPredictRes.size())
        {
            LOG(ERROR) << "__DetectByEvent Error nLabel out of Result Boundry" << endl;
            continue;
        }
        vector<pstWeibo> vClassRes = vPredictRes[nLabel];
        for (int j = 0; j < vClassRes.size(); j++)
        {
            rRes.push_back(vClassRes[j]);
        }
    }

    LOG(INFO) << "__DetectByEvent Succeed" << endl;
    return true;
}


bool CEventOmenDetector::__SentenceBreak(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rSents)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__SentenceBreak Failed Corpus is empty when __SentenceBreak" << endl;
        return false;
    }

    vector<string> vDelimit;
    vDelimit.push_back("。");
    vDelimit.push_back(".");
    vDelimit.push_back("!");
    vDelimit.push_back("?");
    vDelimit.push_back("！");
    vDelimit.push_back("？");

    rSents.clear();
    for (int i = 0; i < rCorpus.size(); i++)
    {
        int nIdx = i;
        string sText = rCorpus[i]->source;
        vector<string> vSents;
        UtilInterface::sentence_cut(sText, vDelimit, vSents);
        for (int j = 0; j < vSents.size(); j++)
        {
            string sent = vSents[j];
            if (sent.length() == 0)
                continue;
            pstWeibo pDoc = new Weibo;
            pDoc->index = nIdx;
            pDoc->source = sent;
            rSents.push_back(pDoc);
        }
    }
    LOG(INFO) << "__SentenceBreak Succeed" << endl;
    return true;
}


bool CEventOmenDetector::__AnalysisSentTense(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rSentsPredRes,
                                             vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__AnalysisSentTense Failed Corpus is empty" << endl;
        return false;
    }

    if (rSentsPredRes.empty())
    {
        LOG(WARNING) << "__AnalysisSentTense Failed SentPreds is empty" << endl;
        return false;
    }

    rRes.clear();
    for (int i = 0; i < rSentsPredRes.size(); i++)
    {
        int nIdx = rSentsPredRes[i]->index;
        if (nIdx >= rCorpus.size())
        {
            LOG(ERROR) << "__AnalysisSentTense Error doc idx is out of boundry" << endl;
            continue;
        }
        cout<<"tense: "<<rSentsPredRes[i]->source<<endl;
        rRes.push_back(rCorpus[nIdx]);
    }

    LOG(INFO) << "__AnalysisSentTense Succeed" << endl;
    return true;
}


bool CEventOmenDetector::__DetectByTense(vector<pstWeibo> &rCorpus, vector<pstWeibo> &vDocSents, vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__DetectByTense Failed Corpus is empty" << endl;
        return false;
    }

    rRes.clear();

    vector<vector<pstWeibo> > vSentPredRes;
    if (!m_pTenseClassifier->BatchPredict(vDocSents, vSentPredRes))
    {
        LOG(ERROR) << "__DetectByTense Failed TenseClassifier Failed" << endl;
        return false;
    }

    int nLabel = 2; //TODO
    if (nLabel >= vSentPredRes.size())
    {
        LOG(ERROR) << "__DetectByEvent Failed nLabel out of Result Boundry" << endl;
        return false;
    }

    vector<pstWeibo> vMatchSents = vSentPredRes[nLabel];
    if (vMatchSents.empty())
    {
        LOG(WARNING) << "No sentence match after tense analysis" << endl;
        return true;
    }

    if (!__AnalysisSentTense(rCorpus, vMatchSents, rRes))
    {
        LOG(ERROR) << "__DetectByTense Failed __AnalysisSentTense Error" << endl;
    }

    for (int i = 0; i < vDocSents.size(); i++)
        delete vDocSents[i];


    LOG(INFO) << "__DetectByTense Succeed" << endl;
    return true;
}


bool CEventOmenDetector::__DetectBySentPattern(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rSentRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "__DetectBySentPattern Failed Corpus is emtpy" << endl;
        return false;
    }

    vector<pstWeibo> vDocSents;
    if (!__SentenceBreak(rCorpus, vDocSents))
    {
        LOG(ERROR) << "__DetectBySentPattern Failed __SentenceBreak Error" << endl;
        return false;
    }

    rSentRes.clear();
    string sPatternErr;
    if (!OmenPattern::WordCollocationPattern(vDocSents, rSentRes, sPatternErr))
    {
        LOG(ERROR) << "__DetectBySentPattern Failed  Pattern error " << sPatternErr << endl;
        return false;
    }


    LOG(INFO) << "__DetectBySentPattern Succeed" << endl;
    return true;
}


bool CEventOmenDetector::DetectEventOmen(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes)
{
    if (rCorpus.empty())
    {
        LOG(WARNING) << "DetectEventOmen Failed Corpus empty" << endl;
        return false;
    }

    rRes.clear();

    vector<pstWeibo> vKeyFilterRes;
    vector<pstWeibo> vEventFilterRes;
    vector<pstWeibo> vPatternFilterRes;
    //vector<pstWeibo> vTenseFilterRes;

    if (!__KeywordsFilter(rCorpus, vKeyFilterRes))
    {
        LOG(WARNING) << "DetectEventOmen Error __KeywordsFilter Failed " << endl;
        return false;
    }
    if (!__DetectByEvent(vKeyFilterRes, vEventFilterRes))
    {
        LOG(WARNING) << "DetectEventOmen Error __DetectByEvent Failed" << endl;
        return false;
    }
    if (!__DetectBySentPattern(vEventFilterRes, vPatternFilterRes))
    {
        LOG(WARNING) << "DetectEventOmen Error __DetectBySentPattern Failed" << endl;
        return false;
    }
    if (!__DetectByTense(vEventFilterRes, vPatternFilterRes, rRes))
    {
        LOG(WARNING) << "DetectEventOmen Error __DetectByTense Failed" << endl;
        return false;
    }

    LOG(INFO) << "DetectEventOmen Succeed" << endl;
    return true;
}

