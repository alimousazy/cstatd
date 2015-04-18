#include <boost/lexical_cast.hpp>
#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>
#include <rocksdb/env.h>
#include <rocksdb/merge_operator.h>
#include <rocksdb/utilities/json_document.h>

#pragma once

using namespace rocksdb;
class data_access {
    public:
    data_access ();     
    void add(std::string key, uint64_t value); 
    uint64_t get(std::string &key); 
    private:
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

    class MergeBasedCounters {
         public:
          MergeBasedCounters(std::shared_ptr<DB> db) : db_(db) {}
          virtual void Add(const std::string& key, uint64_t value)  {
              std::string serialized;
              encoder::encode(serialized, value);
              db_->Merge(rocksdb::WriteOptions(), key, serialized);
          }
         private:
          std::shared_ptr<DB> db_;
    };
    shared_ptr<DB> sDptr;
    shared_ptr<MergeBasedCounters> mergeBaseCount;
};
