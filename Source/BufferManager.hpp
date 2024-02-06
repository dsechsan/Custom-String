//
//  BufferManager.hpp
//  Assignment2
//
//

#ifndef BufferManager_h
#define BufferManager_h
#include <iostream>
#include <cstring>

namespace ECE141   {

template <typename T, int aPreSize = 64>
    class BufferManager {
    public:
        //finish me - default constructor
        BufferManager() : length(0), capacity(aPreSize) {
            buffer = std::make_unique<T[]>(capacity + 1);
            //std::cout << length << "\n";
        }
        
        //finish me - copy constructor
        BufferManager(const ECE141::BufferManager<T> &aCopy) : length(aCopy.length) {
            if(this!= &aCopy){
                capacity = aCopy.capacity;
                buffer = std::make_unique<T[]>(capacity + 1);
                if(aCopy.buffer != NULL){
                    std::memcpy(buffer.get(),aCopy.buffer.get(),length*sizeof(T));}
            }
        }
        
        BufferManager& operator=(const ECE141::BufferManager<T> &aCopy){
            if(this!= &aCopy){
                capacity = willExpand(aCopy.capacity);
                if(aCopy.buffer != NULL){
                    std::memcpy(buffer.get(),aCopy.buffer.get(),length*sizeof(T));
                }
            }
            return *this;
        }
        
        //finish me - constructor with predefined size
        BufferManager(size_t aSize) : length(0), capacity(aSize){
            buffer = std::make_unique<T[]> (capacity + 1);
        }
       
        void allocateNewBuffer(size_t capacity){
            buffer = std::make_unique<T[]>(capacity + 1);
        }
        //Destructor
        ~BufferManager(){
            //delete[] buffer;
        }
        
        void clearBuffer(){
            
            buffer.reset();
            allocateNewBuffer(capacity);
        }

        //---- Add other ocf methods --

        //---- Basic methods you must implement. Add more if you need to...
        size_t getCapacity() const{
            return capacity;
        }
        T* getBuffer() const{
            return buffer.get();
        }
        size_t getLength() const{
            return length;
        }
        BufferManager& setLength(size_t len) {
            length = len;
            return *this;
        }
        BufferManager& setCapacity(size_t capacity){
            this->capacity = capacity;
            return *this;
        }
        size_t willExpand(size_t aNewSize, size_t anOffset=0){
            while(capacity < aNewSize){
                capacity*= 2;
            }
            
            std::unique_ptr<T[]> newBuffer = std::make_unique<T[]> (capacity + 1);
            if(length !=0){
                std::memcpy(newBuffer.get(), buffer.get() , sizeof(T)*(length));
            }
            
            buffer = std::move(newBuffer);
            return capacity;
        }
        
        size_t willCompact(size_t aNewSize, size_t anOffset=0){
            while (capacity > aNewSize){
                capacity /= 2;
            }
            std::unique_ptr<T[]> newBuffer = std::make_unique<T[]> (capacity + 1);
            if(length != 0){
                std::memcpy(newBuffer.get(), buffer.get(), sizeof(T)*(length));
            }
            
            buffer = std::move(newBuffer);
            return capacity;
        }
        
       
   protected:
        std::unique_ptr<T[]> buffer;
        size_t length;
        size_t capacity;
    };

    

} // namespace ECE141

#endif /* BufferManager_h */
