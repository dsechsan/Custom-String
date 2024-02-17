//
//  String.hpp
//  Assignment3
//
//  Created by rick gessner on 1/29/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef String_hpp
#define String_hpp

#include <iostream>
#include "BufferManager.hpp"

//It's up to you to decide how the string and buffermanager
//will work together -- IS_A vs HAS-A.

namespace ECE141 {

    template<typename T=char, size_t aPresize=32>
    class String {
    public:

        //add rest of the OCF methods...
        String(){
            bufferManager = BufferManager<T>();
        };

        String(const char* aBuffer) : bufferManager(aBuffer) {};
        
        //default copy ctor
        String(const String &aString) : bufferManager(aString.bufferManager) {};

        //destructor
        ~String()= default;
        
        // Assignment operator
         String& operator=(const String &aString) {
             if(this != aString){
                bufferManager = aString.bufferManager;
            }
             return *this;
        }
        
        String& operator=(const char* aCstring){
            *this = String(aCstring);
            return *this;
        }

        T& operator[](size_t pos){
            return bufferManager.getBuffer()[pos];
        }


        String& operator+=(const char* aCstring){
            size_t currentLength = this->size();
            size_t newLength = currentLength + std::strlen(aCstring);
            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength);
            }
            std::memcpy(this->getBuffer() + currentLength, aCstring, std::strlen(aCstring)*sizeof(T));
            bufferManager.setLength(newLength);
            return *this;
        }

        String& operator+=(const String &aString){
            const char* bufferAsChar = aString.getBuffer();
            *this += bufferAsChar;
            return *this;
        }

        String  operator+(const String &aString) const{
            String result(*this);
            result += aString;
            return result;
        }

        String operator+(const char* aCstring) const{
            String result(*this);
            result += aCstring;
            return result;
        }

        [[nodiscard]] size_t size() const{
            return bufferManager.getLength();
        }

        T* getBuffer() const{
            return bufferManager.getBuffer();
        }
        [[nodiscard]] size_t getCapacity() const{
            return bufferManager.getCapacity();
        }

        String& insert(size_t anIndex, const String &aStr, size_t aStrIndex, size_t aStrCount) {

            size_t newLength = this->size() + aStrCount;

            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength);
            }

            std::memmove(this->getBuffer() + anIndex + aStrCount,
                         this->getBuffer() + anIndex,
                         (this->size() - anIndex + 1) * sizeof(T));

            std::memcpy(this->getBuffer() + anIndex,
                        aStr.getBuffer() + aStrIndex,
                        aStrCount * sizeof(T));

            bufferManager.setLength(newLength);
            return *this;
        }


        String& insert(size_t anIndex, const char* aCstring, size_t aStrIndex, size_t aStrCount) {
            return insert(anIndex, String(aCstring), aStrIndex,aStrCount);
        }

        String& insert(size_t anIndex, T aChar) {
            return insert(anIndex,String(&aChar),0,1);
        }

        String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aString) {
            size_t actualCopyLen = aString.size();
            size_t newLength = this->size() - aMaxCopyLen + aString.size();

            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength * sizeof(T));
            }

            if(anIndex + aMaxCopyLen <= this->size()){
                std::memmove(this->getBuffer() + anIndex + aString.size(),
                             this->getBuffer() + anIndex + aMaxCopyLen,
                             (this->size() - anIndex - aMaxCopyLen+1)*sizeof(T));
                
                std::memcpy(this->getBuffer() + anIndex, aString.getBuffer(), actualCopyLen*sizeof(T));
                bufferManager.setLength(newLength);}
            this->getBuffer()[newLength] = '\0';
            return *this;

        }


        String& replace(size_t anIndex, size_t aMaxCopyLen, const char* aCstring) {
            return replace(anIndex,aMaxCopyLen, String(aCstring));
        }

        String& erase(size_t anIndex, size_t aCount) {
            size_t eraseLength = std::min(aCount, bufferManager.getLength() - anIndex);
            size_t newLength = bufferManager.getLength() - eraseLength ;
            bufferManager.willCompact(newLength);
            bufferManager.setLength(newLength);

            if(eraseLength < aCount){
                bufferManager.getBuffer()[newLength] = '\0';
                return *this;
            }
            else{
                std::memmove(bufferManager.getBuffer() + anIndex,
                             bufferManager.getBuffer() + anIndex + eraseLength,
                             (newLength - anIndex + 1) * sizeof(T));
                return *this;
            }
            
        }
        
        int find(const String& aString, size_t anIndex = 0) const {
            const T* sourceBuffer = bufferManager.getBuffer();
            size_t sourceLength = bufferManager.getLength();

            const T* searchBuffer = aString.getBuffer();
            size_t searchLength = aString.size();

            for (size_t i = anIndex; i <= sourceLength - searchLength; ++i) {
                if (std::memcmp(sourceBuffer + i, searchBuffer, searchLength * sizeof(T)) == 0) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        int compare( const String& aString ) const{
            return std::strcmp(bufferManager.getBuffer(), aString.bufferManager.getBuffer());
        };
        //add method to add compare const char*
        int compare(const char* aCstring) const {
            return std::strcmp(bufferManager.getBuffer(), aCstring);
        }

        bool operator==(const String &aString) const {
            return compare(aString) == 0;
        }
        bool operator!=(const String &aString) const {return !(*this==aString);}
        bool operator<(const String &aString) const {
            return compare(aString) < 0;
        }
        bool operator<=(const String &aString) const {
            return (*this == aString || *this < aString);
        }
        bool operator>(const String &aString) const {
            return !(*this <= aString);
        }
        bool operator>=(const String &aString) const {
            return !(*this < aString);
        }
        
        bool operator==(const char* aCstring) const {
            return compare(aCstring) == 0;
        }
        bool operator!=(const char* aCstring) const {return !(*this==aCstring);}
        bool operator<(const char* aCstring) const {
            return compare(aCstring) < 0;
        }
        bool operator<=(const char* aCstring ) const {
            return (*this == aCstring || *this < aCstring);
        }
        bool operator>(const char* aCstring) const {
            return !(*this <= aCstring);
        }
        bool operator>=(const char* aCstring) const {
            return !(*this < aCstring);
        }

        friend std::ostream& operator << (std::ostream &anOut, const String<char> &aStr);
                
        friend std::istream& operator >> (std::istream &anIn, const String<char> &aStr);
        
       
        protected:
            BufferManager<T> bufferManager;
    };

//    template <typename T>;
    std::ostream& operator << (std::ostream &anOut, const String<char> &aStr){
        anOut << aStr.getBuffer();
        return anOut;
    };
//    template <typename T>;
    std::istream& operator >> (std::istream &anIn, const String<char> &aStr){
        anIn >> aStr.getBuffer();
        return anIn;
    };
        


} // namespace ECE141

#endif /* String_hpp */
