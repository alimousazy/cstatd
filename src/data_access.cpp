#include "data_access.hpp"

data_access::data_access() 
{
    DB* dbp;
    Options op;
    auto env = rocksdb::Env::Default();
    env->SetBackgroundThreads(2, rocksdb::Env::LOW);
    env->SetBackgroundThreads(1, rocksdb::Env::HIGH);
    op.merge_operator.reset(new UInt64AddOperator());
    op.env = env;
    op.create_if_missing = true;
    Status stats = DB::Open(op, "/tmp/mak", &dbp);
    if(!stats.ok())
    {
        std::cerr<<"Can't access DB"<<std::endl;
        exit(1);
    }
    sDptr.reset(dbp);
    mergeBaseCount.reset(new MergeBasedCounters(sDptr));
}
void data_access::add(std::string key, uint64_t value) 
{
    mergeBaseCount->Add(key, value);
} 
uint64_t data_access::get(std::string &key) 
{
    std::string v;
    sDptr->Get(rocksdb::ReadOptions(),key, &v);
    return encoder::decode(v.c_str());
}
