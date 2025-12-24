#pragma once

#include <unordered_map>
#include <any>
#include <stdexcept>
#include <string>

class AlgorithmResult {
    private:
        std::unordered_map<std::string, std::any> datas;//key-value pairs to store the result of the algorithm

    public:
        AlgorithmResult() = default;

        template <typename T>
        bool setData( const std::string &key, const T &value ) {
            if ( key.empty() ) {
                return false;//empty key is not allowed
            }
            datas[key] = value;
            return true;
        }

        template <typename T>
        T getData( const std::string &key ) const {
            if ( key.empty() ) {
                throw std::invalid_argument( "empty key is not allowed" );
            }
            auto it = datas.find(key);
            if ( datas.end() == it ) {
                throw std::out_of_range( "key not found" );
            }
            else {
                try {
                    return std::any_cast<T>( it->second );
                }
                catch ( const std::bad_any_cast &e ) {
                    throw std::invalid_argument(
                "AlgorithmResult: Type mismatch for key '" + key + 
                "' (expected: " + typeid(T).name() + 
                ", actual: " + it->second.type().name() + ")"
                );
                }
            }
        }

        bool contains ( const std::string &key ) const {
            return datas.count(key) > 0;
        }

        std::string getTypeName ( const std::string &key ) const {
            if ( key.empty() ) {
                throw std::invalid_argument( "empty key is not allowed" );
            }
            auto it = datas.find(key);
            if ( datas.end() == it ) {
                throw std::out_of_range( "key not found" );
            }
            else {
                return it->second.type().name();
            }
        }
};