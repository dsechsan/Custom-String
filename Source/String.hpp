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
                bufferManager.setLength(std::strlen(aBuffer));
                bufferManager.willExpand(std::strlen(aBuffer)*sizeof(T));
                std::strcpy(bufferManager.getBuffer(), aBuffer);
            }
        }
        
        //default ctor
        String(const String &aString){
            if(aString.size() != 0){
                bufferManager.setLength( aString.size());
                bufferManager.willExpand(aString.size()*sizeof(T));
                std::strcpy(bufferManager.getBuffer(), aString.getBuffer());
            }
        };

         String& operator=(const String &aCopy){
             if(this != &aCopy){
                 size_t copyLength = aCopy.size();
                 bufferManager.clearBuffer();
                 
                 if(bufferManager.getCapacity() < (copyLength+1)*sizeof(T)){
                     bufferManager.willExpand(copyLength);
                 }
                 std::memcpy(bufferManager.getBuffer(), aCopy.getBuffer(), copyLength*sizeof(T));
                 bufferManager.setLength(copyLength);
             }
             return *this;
            }
        
        String& operator=(const char* aCopy){
            if(getBuffer() != aCopy){
                size_t copyLength = std::strlen(aCopy);
                bufferManager.clearBuffer();
                
                if(bufferManager.getCapacity() < (copyLength+1)*sizeof(T)){
                    bufferManager.willExpand(copyLength);
                }

                std::memcpy(bufferManager.getBuffer(), aCopy, copyLength*sizeof(T));
                bufferManager.getBuffer()[copyLength] = '\0';
                bufferManager.setLength(copyLength);
            
            }
            return *this;
        }

        T& operator[](size_t pos){
            std::cout << "Size" << this->size() << "\n";
            if(pos > this->size()){
                throw std::out_of_range("Index out of range");
            }
            return bufferManager.getBuffer()[pos];
        }

        String& operator+=(const String &aString){
            if((bufferManager.getLength() + aString.size())*sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(bufferManager.getLength() + aString.size());
            }
            std::strcat(bufferManager.getBuffer(), aString.bufferManager.getBuffer());
            bufferManager.setLength(bufferManager.getLength() + aString.size());
            return *this;
        }

        String& operator+=(const char* &aCstring){
            if((bufferManager.getLength() + std::strlen(aCstring))*sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(bufferManager.getLength() + std::strlen(aCstring));
            }
            std::strcat(bufferManager.getBuffer(), aCstring);
            bufferManager.setLength(bufferManager.getLength() + std::strlen(aCstring));
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
            if (anIndex > bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            if(aStrCount > aStr.size() || aStrIndex > aStr.size()){
                throw std::out_of_range("Index out of range");
            }


            size_t newLength = bufferManager.getLength() + aStrCount;

            if((newLength)*sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength);
            }

            // Shift existing characters to make space for the new ones
            std::memmove(bufferManager.getBuffer() + anIndex + aStrCount,
                         bufferManager.getBuffer() + anIndex,
                         (bufferManager.getLength() - anIndex + 1) * sizeof(T));

            // Copy the characters from aStr to the specified position
            std::memcpy(bufferManager.getBuffer() + anIndex,
                        aStr.getBuffer() + aStrIndex,
                        aStrCount * sizeof(T));

            bufferManager.setLength(newLength);
            return *this;
        }


        String& insert(size_t anIndex, const char* aCstring, size_t aStrIndex, size_t aStrCount) {
            if (anIndex > bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            if(aStrCount > std::strlen(aCstring) || aStrIndex > std::strlen(aCstring)){
                throw std::out_of_range("Index out of range");
            }


            size_t newLength = bufferManager.getLength() + aStrCount;

            if((newLength)*sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength);
            }

            // Shift existing characters to make space for the new ones
            std::memmove(bufferManager.getBuffer() + anIndex + aStrCount,
                         bufferManager.getBuffer() + anIndex,
                         (bufferManager.getLength() - anIndex + 1) * sizeof(T));

            // Copy the characters from aStr to the specified position
            std::memcpy(bufferManager.getBuffer() + anIndex,
                        aCstring + aStrIndex,
                        aStrCount * sizeof(T));

            bufferManager.setLength(newLength);
            return *this;
        }

        String& insert(size_t anIndex, T aChar) {
            if (anIndex > bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            size_t newLength = bufferManager.getLength() + 1;
            if(newLength*sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength * sizeof(T));
            }

            // Shift existing characters to make space for the new one
            std::memmove(bufferManager.getBuffer() + anIndex + 1,
                         bufferManager.getBuffer() + anIndex,
                         (bufferManager.getLength() - anIndex + 1) * sizeof(T));

            // Insert the new character
            bufferManager.getBuffer()[anIndex] = aChar;
            bufferManager.setLength(newLength);
            return *this;
        }

        String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aString) {
            if (anIndex > bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            size_t copyLen = aString.size();
            size_t newLength = bufferManager.getLength() - std::min(aMaxCopyLen,bufferManager.getLength() - anIndex) + copyLen;

            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength * sizeof(T));
            }

            if(anIndex + aMaxCopyLen <= bufferManager.getLength()){
                std::memmove(bufferManager.getBuffer() + anIndex + aString.size(),
                             bufferManager.getBuffer() + anIndex + aMaxCopyLen,
                             (bufferManager.getLength() - anIndex - aMaxCopyLen + 1)*sizeof(T));
                
                std::memcpy(bufferManager.getBuffer() + anIndex, aString.getBuffer(), copyLen*sizeof(T));
                // Copy the replacement string into the original string at the specified position
                bufferManager.setLength(newLength);}
            return *this;

        }


        String& replace(size_t anIndex, size_t aMaxCopyLen, const char* aCstring) {
            if (anIndex > bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            size_t copyLen = std::strlen(aCstring);
            size_t newLength = bufferManager.getLength() - aMaxCopyLen + copyLen;

            if(newLength * sizeof(T) > bufferManager.getCapacity()){
                bufferManager.willExpand(newLength * sizeof(T));
            }
            // Copy the characters from aString to the specified position

            // Shift existing characters after the replaced portion
            if(anIndex + aMaxCopyLen <= bufferManager.getLength()){
                std::memmove(bufferManager.getBuffer() + anIndex + copyLen,
                             bufferManager.getBuffer() + anIndex + aMaxCopyLen,
                             (bufferManager.getLength() - anIndex - aMaxCopyLen + 1 ) * sizeof(T));
                
                std::memcpy(bufferManager.getBuffer() + anIndex, aCstring, copyLen * sizeof(T));
                bufferManager.setLength(newLength);}
            return *this;
        }

        String& erase(size_t anIndex, size_t aCount) {
            if (anIndex >= bufferManager.getLength()) {
                throw std::out_of_range("Index out of range");
            }

            size_t newLength = bufferManager.getLength() - std::min(aCount, bufferManager.getLength() - anIndex);
            bufferManager.willCompact(newLength);
            bufferManager.setLength(newLength);

            // Shift existing characters after the erased portion
            std::memmove(bufferManager.getBuffer() + anIndex,
                         bufferManager.getBuffer() + anIndex + aCount,
                         (bufferManager.getLength() - anIndex + 1) * sizeof(T));

            return *this;
        }
        
        int find(const String& aString, size_t anIndex = 0) const {
            const T* sourceBuffer = bufferManager.getBuffer();
            size_t sourceLength = bufferManager.getLength();

            const T* searchBuffer = aString.getBuffer();
            size_t searchLength = aString.size();

            // Iterate through the source buffer starting from anIndex
            for (size_t i = anIndex; i <= sourceLength - searchLength; ++i) {
                // Check if the substring matches at the current position
                if (std::memcmp(sourceBuffer + i, searchBuffer, searchLength * sizeof(T)) == 0) {
                    return static_cast<int>(i);
                }
            }

            // If not found, return -1
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
