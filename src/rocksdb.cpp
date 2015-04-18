#include <boost/lexical_cast.hpp>
#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>
#include <rocksdb/env.h>
#include <rocksdb/merge_operator.h>
#include <rocksdb/utilities/json_document.h>


using namespace rocksdb;

struct encoder {
    static void encode(std::string &str, uint64_t num)
    {
        std::stringstream dem;
        dem << std::hex << num;
        str = dem.str();
    }
    static uint64_t decode(char const *value)
    {
       uint64_t result = 0;
       std::istringstream(value)>>std::hex>>result;
       return result;
    }
};

// A 'model' merge operator with uint64 addition semantics
class UInt64AddOperator : public AssociativeMergeOperator {
 public:
  virtual bool Merge(
    const Slice& key,
    const Slice* existing_value,
    const Slice& value,
    std::string* new_value,
    Logger* logger) const override 
  {
    uint64_t existing = 0;
    uint64_t oper;
    uint64_t sum;
    oper = encoder::decode(value.data());
    if (existing_value) {
      existing = encoder::decode(existing_value->data());
    }
    sum =  existing + oper;
    encoder::encode(*new_value, sum);  
    return true;        
  }

   virtual const char* Name() const override {
    return "UInt64AddOperator";
   }
};

// Implement 'add' directly with the new Merge operation
class MergeBasedCounters {
 public:
  MergeBasedCounters(std::shared_ptr<DB> db) : db_(db) {}

  // mapped to a leveldb Merge operation
  virtual void Add(const std::string& key, uint64_t value)  {
      std::string serialized;
      encoder::encode(serialized, value);
      db_->Merge(rocksdb::WriteOptions(), key, serialized);
  }
 private:
  std::shared_ptr<DB> db_;
};

int main( ) {
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
        std::cout<<"fail";
        return -1;
    }
    std::shared_ptr<DB> db(dbp);
    MergeBasedCounters counters(db);
    for(int i = 0; i < 1000000; i++)
    {
        counters.Add("jem", 2);
    }
    std::string v;
    std::cout<<"adding";
    db->Get(rocksdb::ReadOptions(),"jem", &v);
    std::cout<<"----"<<encoder::decode(v.c_str());
    return 0;
}
