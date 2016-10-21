#ifndef _EVENT_OMEN_H_
#define _EVENT_OMEN_H_

//#include "linearmodel.h"
#include <vector>
#include "DataType.h"
#include "linearmodel.h"
#include "util.h"

using namespace WeiboTopic_ICT;
using namespace linear_model;
using namespace tools;


namespace event_omen
{

    /*
     * \class > CEventOmenDetector
     * \brief > event omen detect class
     * \date > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class CEventOmenDetector
    {
        public:
            /*
            * \fn > constructor and destructor
            * \date > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            CEventOmenDetector(const string &rConfPath);

            ~CEventOmenDetector();


            /*
             * \fn > DetectEventOmen
             * \brief > detect event omen from corpus
             * \param[in] rCorpus > corpus
             * \param[out] rRes > detect result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool DetectEventOmen(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes);

            //bool BatchOmenDetect(vector<pstWeibo> &rCorpus, vector<int> &rRes);

        private:

            /*
             * \fn > __LoadConfigFile
             * \brief > load config file
             * \param[in] rPath > config file path
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __LoadConfigFile(const string &rPath);


            /*
             * \fn > __InitBlackList
             * \brief > init black list for data process
             * \param[in] rPath > blacklist path
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __InitBlackList();


            /*
             * \fn > __RuleFilter
             * \brief > filter docs by keywords
             * \param[in] rCorpus > corpus to filer
             * \param[out] rRes > filter result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __KeywordsFilter(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes);


            /*
             * \fn > __DetectByEvent
             * \brief > detect senstive event
             * \param[in] rCorpus > corpus to detect
             * \param[out] rRes > detect result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __DetectByEvent(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes);


            /*
             * \fn > __DetectByTense
             * \brief > detect tense of doc, and filter by tense
             * \param[in] rCorpus > corpus to detect
             * \param[out] rRes > filter result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __DetectByTense(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes);


            /*
             * \fn > __DetectByPattern
             * \brief > filter docs by pattern
             * \param[in] rCorpus > corpus to filer
             * \param[out] rRes > filter result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __DetectByPattern(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes);


            /*
             * \fn > __SentenceBreak
             * \brief > cut doc to sentences
             * \param[in] rCorpus > corpus to break
             * \param[out] rRes > sentences result
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __SentenceBreak(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rSents);


            /*
             * \fn > __AnalysisSentTense
             * \brief > analysis sentence tense to predict doc tense
             * \param[in] rCorpus > original docs
             * \param[in] rSents > sentences to analysis
             * \param[out] rRes > tense filter result
             * \ret bool > whether function succIeed
             * \date > 2016/10
             * \author > zhounan@(zhounan@software.ict.ac.cn)
             */
            bool __AnalysisSentTense(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rSents, vector<pstWeibo> &rRes);


        private:

            // event model conf path
            string m_sEventModelConf;

            // tense model conf path
            string m_sTenseModelConf;

            // black list path
            string m_sBlackListPath;

            // triggers path
            string m_sTriggerPath;

            // action path
            string m_sActionPath;

            // event classifier
            CClassifier *m_pEventClassifier;

            // tense classifier
            CClassifier *m_pTenseClassifier;

            // ac-automation
            AC_automation *m_pACauto;

            // black list
            vector<string> m_vBlackList;

    };
}



#endif
