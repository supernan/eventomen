#include <iostream>
#include <fstream>
#include "eventomen.h"

using namespace std;
using namespace event_omen;

int fn_iInitWeiboDataFromFile(const char *dataPath, vector<Weibo> &weibo)
{
		//check path
	if(NULL == dataPath)
	{
		cerr<<"weibo data path error"<<endl;
		return 1;
	}

		//open file
	ifstream in(dataPath);
	if(!in)
		return 1;

		//parse data
	string record;
	int counter = 0;
	while(getline(in, record))
	{
		if(record == "")
			continue;
		Weibo new_doc;
		new_doc.id = counter;
		counter++;
		new_doc.source = record;
		weibo.push_back(new_doc);
	}
	in.close();
	return 0;
}

int main(int argc, char **argv)
{
	//test();
    FLAGS_log_dir="../logs/";
    google::InitGoogleLogging("eventomen");
    if (argc < 2)
    {
        cout << "miss input file" << endl;
        return 1;
    }
    string confPath = "../conf/eventomen.xml";
    CEventOmenDetector model(confPath);
    vector<Weibo> text;
    vector<pstWeibo> res;
    string fpath = argv[1];
    fn_iInitWeiboDataFromFile(fpath.c_str(), text);
	vector<pstWeibo> corpus;
    for (int i = 0; i < text.size(); i++)
        corpus.push_back(&text[i]);
    cout<<"mark here"<<endl;

    if (!model.DetectEventOmen(corpus, res))
    {
        cout<<"detect omen failed" << endl;
        return 1;
    }

    for (int i = 0; i < res.size(); i++)
    {
        cout<<i<<" "<<res[i]->source<<endl;
    }
    cout<<"end mark"<<endl;
	return 0;
}

