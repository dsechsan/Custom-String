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

//It's up to you to decide the how the string and buffermanager
//will work together -- IS_A vs HAS-A.

namespace ECE141 {

    template<typename T=char, size_t aPresize=32>
    class String {
    public:

        //add rest of the OCF methods...
        String(const char* aBuffer=nullptr){
            if(aBuffer){
                bufferManager.assign(aBuffer, std::strlen(aBuffer));
                bufferManager.getBuffer()[std::strlen(aBuffer)] = '\0';
            }
        };
        
        //default ctor
        String(const String &aString){
            if(aString.size() != 0){
                bufferManager =  aString.bufferManager;
            }
        };
        
        // Assignment operator
         String& operator=(const String &aCopy){
             if(this != &aCopy){
                 bufferManager = aCopy.bufferManager;
             }
             return *this;
        }
        
        String& operator=(const char* aCopy){
//            if(getBuffer() != aCopy){
//                bufferManager.assign(aCopy,std::strlen(aCopy));
//                bufferManager.getBuffer()[std::strlen(aCopy)] = '\0';
//            }
            *this = String(aCopy);
            return *this;
        }

        T& operator[](size_t pos){
            return bufferManager.getBuffer()[pos];
        }

        String& operator+=(const String &aString){
            size_t newLength = this->size() + aString.size();
            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength);
            }
            std::memcpy(this->getBuffer() + this->size(), aString.getBuffer(), aString.size());
            bufferManager.setLength(newLength);
            return *this;
        }

        String& operator+=(const char* &aCstring){
//            size_t newLength = this->size() + std::strlen(aCstring);
//            if(newLength * sizeof(T) > bufferManager.getCapacity()){
//                bufferManager.willExpand(newLength);
//            }
//            std::memcpy(this->getBuffer()+this->size(), aCstring, std::strlen(aCstring));
//            bufferManager.setLength(newLength);
//            bufferManager.getBuffer()[newLength] = '\0';
            *this += String(aCstring);
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

        size_t size() const{
            return bufferManager.getLength();
        }

        T* getBuffer() const{
            return bufferManager.getBuffer();
        }
        size_t getCapacity() const{
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
            return(insert(anIndex, String(aCstring), aStrIndex,aStrCount))

//            size_t newLength = this->size() + aStrCount;
//
//            if(newLength * sizeof(T) > bufferManager.getCapacity()){
//                bufferManager.willExpand(newLength);
//            }
//
//            // Shift existing characters to make space for the new ones
//            std::memmove(this->getBuffer() + anIndex + aStrCount,
//                         this->getBuffer() + anIndex,
//                         (this->size() - anIndex + 1) * sizeof(T));
//
//            // Copy the characters from aStr to the specified position
//            std::memcpy(this->getBuffer() + anIndex,
//                        aCstring + aStrIndex,
//                        aStrCount * sizeof(T));
//
//            bufferManager.setLength(newLength);
//            return *this;
        }

        String& insert(size_t anIndex, T aChar) {

            size_t newLength = this->size() + 1;
            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength * sizeof(T));
            }

            std::memmove(this->getBuffer() + anIndex + 1,
                         this->getBuffer() + anIndex,
                         (this->size() - anIndex + 1) * sizeof(T));

            // Insert the new character
            this->getBuffer()[anIndex] = aChar;
            bufferManager.setLength(newLength);
            return *this;
        }

        String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aString) {
            if (anIndex > this->size()) {
                throw std::out_of_range("Index out of range");
            }

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
//            if (anIndex > this->size()) {
//                throw std::out_of_range("Index out of range");
//            }
//
////            size_t actualCopyLen = std::min(aMaxCopyLen,this->size() - anIndex);
//            size_t actualCopyLen = std::strlen(aCstring);
//            size_t newLength = this->size() - aMaxCopyLen + actualCopyLen;
//
//            if(newLength * sizeof(T) > bufferManager.getCapacity()){
//                bufferManager.willExpand(newLength * sizeof(T));
//            }
//            // Copy the characters from aString to the specified position
//
//            // Shift existing characters after the replaced portion
//            if(anIndex + aMaxCopyLen <= this->size()){
//                std::memmove(bufferManager.getBuffer() + anIndex + std::strlen(aCstring),
//                             bufferManager.getBuffer() + anIndex + aMaxCopyLen,
//                             (bufferManager.getLength() - anIndex - aMaxCopyLen ) * sizeof(T));
//                
//                std::memcpy(bufferManager.getBuffer() + anIndex, aCstring, actualCopyLen * sizeof(T));
//                bufferManager.setLength(newLength);
//            }
//            this->getBuffer()[newLength] = '\0';
//            return *this;
            return replace(anIndex,aMaxCopyLen, String(aCstring));
        }

        String& erase(size_t anIndex, size_t aCount) {
//            if (anIndex >= bufferManager.getLength()) {
//                throw std::out_of_range("Index out of range");
//            }
//            
            size_t eraseLength = std::min(aCount, bufferManager.getLength() - anIndex);
            size_t newLength = bufferManager.getLength() - eraseLength ;
            bufferManager.willCompact(newLength);
            bufferManager.setLength(newLength);

            // Shift existing characters after the erased portion
            if(eraseLength < aCount){
                bufferManager.getBuffer()[newLength] = '\0';
                return *this;
            }
            else{
                std::memmove(bufferManager.getBuffer() + anIndex,
                             bufferManager.getBuffer() + anIndex + aCount,
                             (bufferManager.getLength() - anIndex + 1) * sizeof(T));
            }

            return *this;
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
        //Add version(s) to support const char*...
        
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

        friend std::ostream& operator << (std::ostream &anOut, const String<T> &aStr);
                
        friend std::istream& operator >> (std::istream &anIn, const String<T> &aStr);
        
       
        protected:
            BufferManager<T> bufferManager;
            //length = bufferManager.length;
    };

//    template <typename T>;
    std::ostream& operator << (std::ostream &anOut, const String<char> &aStr){
        anOut << aStr.getBuffer();
        return anOut;
    };
//    template <typename T>;
    std::istream& operator >> (std::istream &anIn, const String<char> &aStr){
        return anIn;
    };
        


} // namespace ECE141

#endif /* String_hpp */
