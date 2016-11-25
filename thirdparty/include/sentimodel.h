#ifndef _SENTI_MODEL_H_
#define _SENTI_MODEL_H_
#include <vector>
#include <map>
#include <iostream>
#include "glog/logging.h"
#include "SentiClassifierAPI.h"
#include "DataType.h"
#include "thpool.h"

using namespace std;
using namespace WeiboTopic_ICT;

#define ERROR_SCORE -10.0
#define ERROR_LABEL "error"

namespace senti_analysis
{
    /*
     * \class > CSentimentModel
     * \brief  > model to sentiment analysis
     * \date > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class CSentimentModel;


    /*
     * \fn > SentiAnalysisThreadFunc
     * \brief > sentiment analysis thread function
     * \param[in] arg > all of data needed by thread function
     * \ret void
     * date > 2016/10
     * author > zhounan(zhounan@software.ict.ac.cn)
     */
    void *SentiAnaysisThreadFunc(void *arg);


    /*
     * \fn > Lock
     * \brief > Lock mutex in thread function
     * \param[in] arg > sentiment model
     * \ret void
     * date > 2016/10
     * author > zhounan(zhounan@software.ict.ac.cn)
     */
    void Lock(CSentimentModel *pSentiModel);


    /*
     * \fn > UnLock
     * \brief > unLock mutex in thread function
     * \param[in] arg > sentiment model
     * \ret void
     * date > 2016/10
     * author > zhounan(zhounan@software.ict.ac.cn)
     */
    void UnLock(CSentimentModel *pSentiModel);


    /*
     * \struct > sentiAnalysisThreadParam
     * \brief > param of thread function
     * \date > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    struct sentiAnaysisThreadParam
    {
        vector<pstWeibo> *m_pCorpus;
        vector<double> *m_pSentiScores;
        CSentimentModel *m_hSentiModel;
        int m_nStart;
        int m_nEnd;
    };


    /*
     * \class > CSentimentModel
     * \brief  > model to sentiment analysis
     * \date > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class CSentimentModel
    {
        public:
            /*
            * \fn > constructor and destructor
            * \date > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            CSentimentModel(const string &rConfPath);

            ~CSentimentModel();


            /*
            * \fn > AnalysisDocument
            * \brief > sentiment analysis for a single doc
            * \param[in] pDoc > doc to analysis
            * \param[in] dScore > score of doc
            * \param[in] nLabel > sentiment label of doc
            * \ret bool > whether function is succeed
            * \date > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool AnalysisDocument(pstWeibo pDoc, double &dScore);


            /*
            * \fn > BatchAnalysis
            * \brief > sentiment analysis for a  docs
            * \param[in] pDoc > docs to analysis
            * \param[in] dScore > score of docs
            * \param[in] nLabel > sentiment label of docs
            * \ret bool > whether function is succeed
            * \date > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool BatchAnalysis(vector<pstWeibo> &rCorpus, vector<double> &vScores);


        private:

            // handler to achieve sentiment analysis
            SENTI_CLASSIFIER_HANDLER *m_hSentiHandler;

        public:
            // mutext to proctect thread function
            pthread_mutex_t m_iModelMutex;


    };
}



#endif
