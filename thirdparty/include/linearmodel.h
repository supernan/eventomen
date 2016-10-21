#ifndef _LINEAR_MODEL_H_
#define _LINEAR_MODEL_H_

#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include "linear.h"
#include "DataType.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "glog/logging.h"
#include "ICTCLAS50.h"
#include "basictools.h"
#include "featengine.h"
#include "thpool.h"

using namespace std;
using namespace WeiboTopic_ICT;
using namespace feature_engine;


namespace linear_model
{
    /*
     * \class > Classifier
     * \brief > model to classify
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class CClassifier;


    /*
     * \fn > classifyThreadFunc
     * \brief > classify function in each thread
     * \param [in] arg > all of the info needed by thread function
     * \ret void
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    void* ClassifyThreadFunc(void *arg);


    /*
     * \fn > Lock
     * \brief > lock the mutex when multi-thread function is running
     * \param [in] m_iClassifier > the object need to lock
     * \ret void
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    void Lock(CClassifier *m_iClassifier);


    /*
     * \fn > UnLock
     * \brief > unlock the mutex when multi-thread function is running
     * \param [in] m_iClassifier > the object need to unlock
     * \ret void
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    void UnLock(CClassifier *m_iClassifier);


    /*
     * \struct > classifierThreadParam
     * \brief > all params used by thread function
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    struct classifierThreadParam
    {
        vector<pstWeibo> *m_pCorpus;
        vector<vector<pstWeibo> > *m_pRes;
        CClassifier *m_pClassifier;
        int m_nStart;
        int m_nEnd;
    };


    /*
     * \struct > classifierInitParam
     * \brief > all params used by constructor
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    struct classifierInitParam
    {
        string m_sWordSpacePath;
        string m_sModelPath;
        string m_sStopWordsPath;
        string m_sLabelPath;
        string m_sWordSegPath;
    };


    /*
     * \class > Classifier
     * \brief > model to classify
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class CClassifier
    {
        public:

           /*
            * \fn > constructor and destructor
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            CClassifier(const string &rConfigPath, string sName);

            ~CClassifier();


           /*
            * \fn > PredictDocument
            * \brief > predict doc label
            * param [in] pDoc > doc to predict
            * param [out] rLabel > record label
            * ret bool > whether initialize is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool PredictDocument(pstWeibo pDoc, int &rLabel);


           /*
            * \fn > BatchPredict
            * \brief > predict batch of docs
            * param [in] rCorpus > docs to predict
            * param [out] rRes > record result
            * ret bool > whether initialize is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool BatchPredict(vector<pstWeibo> &rCorpus, vector<vector<pstWeibo> > &rRes);

        private:
           /*
            * \fn > __InitClassifier
            * \brief > initialize classifier
            * param [in] rInitPath > initial data path
            * ret bool > whether initialize is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __InitClassifier(const string &rInitPath);


           /*
            * \fn > __LoadParam
            * \brief > load config data
            * param [in] iClassifierParam > param object
            * param [in] rConfigPath > config xml path
            * ret bool > whether load function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __LoadParam(const string &rConfigPath, classifierInitParam &iClassiferParam);


           /*
            * \fn > __InitWordsSpace
            * \brief > load word space
            * param [in] rWordsPath > file path
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __InitWordsSpace(const string &rWordsPath);


            /*
            * \fn > __InitStopWords
            * \brief > load stop words
            * param [in] rWordsPath > file path
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __InitStopWords(const string &rWordsPath);


            /*
            * \fn > __InitLabels
            * \brief > load labels
            * param [in] rLabelPath > file path
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __InitLabels(const string &rLabelPath);


            /*
            * \fn > __InitModel
            * \brief > load  model
            * param [in] rModelPath > model path
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __InitModel(const string &rModelPath);


            /*
            * \fn > __WordSegment
            * \brief > word segment
            * param [in] pDoc > doc to word cut
            * param [out] vWords > wordseg result
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __WordSegment(pstWeibo pDoc, vector<string> &vWords);


            /*
            * \fn > __Document2Feature
            * \brief > extract feats from doc
            * param [in] pDoc > document
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __Document2Feature(pstWeibo pDoc);


            /*
            * \fn > __PredictByModel
            * \brief > use model to predict doc label
            * param [in] vFeature > doc features
            * param [out] rRes > doc label
            * ret bool > whether function is successful
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            bool __PredictByModel(vector<feature_node> vFeatures, int &rRes);

        public:
            //mutex used by thread function
            pthread_mutex_t m_ModelMutex;

        private:
            // model name
            string m_sModelName;

            // classify model
            model *m_iModel;

            //wordspace
            map<string, int> m_mWordsSpace;

            //class labels
            vector<string> m_vLabels;

            //stop words
            set<string> m_sStopWords;


    };
}


#endif
