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

template <typename T, size_t aPreSize = 12>
    class BufferManager {
    public:
        //finish me - default constructor
        BufferManager() : length(0), capacity(aPreSize) {
            buffer = std::make_unique<T[]>(capacity + 1);
        }
        //finish me - constructor with predefined size
        explicit BufferManager(size_t aSize) : length(0), capacity(aSize){
            buffer = std::make_unique<T[]> (capacity + 1);
        }

        explicit BufferManager(const T* aCstring) {
            length = std::strlen(aCstring);
            capacity = aPreSize*((length / aPreSize) + 1)  ;
            buffer = std::make_unique<T[]> (capacity + 1);

            if(length != 0){
                std::memcpy(buffer.get(),aCstring,(length+1) * sizeof(T));
            }
            buffer[length] = '\0';
        }
        
        //finish me - copy constructor
        explicit BufferManager(const ECE141::BufferManager<T> &aCopy) {
            *this = aCopy;
        }
        
        BufferManager& operator=(const ECE141::BufferManager<T> &aCopy){
            if(this!= &aCopy){
                capacity = aCopy.capacity;
                length = aCopy.length;
                buffer = std::make_unique<T[]> (capacity + 1);
                if(aCopy.buffer != nullptr){
                    std::memcpy(buffer.get(),aCopy.buffer.get(),(aCopy.length+1)*sizeof(T));
                }
            }
            return *this;
        }
        
        
        //Destructor
        ~BufferManager()= default;
        
        
        //---- Add other ocf methods --

        //---- Basic methods you must implement. Add more if you need to...
        [[nodiscard]] size_t getCapacity() const{
            return capacity;
        }
        T* getBuffer() const{
            return buffer.get();
        }
        [[nodiscard]] size_t getLength() const{
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
        
        BufferManager& willExpand(size_t aNewSize, size_t anOffset=0){
            if(capacity > aNewSize){return *this;}
            else{
                while(capacity < aNewSize){
                    capacity*= 2;
                }
        
                std::unique_ptr<T[]> newBuffer = std::make_unique<T[]> (capacity + 1);
                if( buffer != nullptr){
                    std::memcpy(newBuffer.get(), buffer.get() , sizeof(T)*(length));
                }
                
                buffer = std::move(newBuffer);
                return *this;
            }
        }
        
        BufferManager& willCompact(size_t aNewSize, size_t anOffset=0){
            if(capacity < 2*aNewSize){return *this;}
            else{
                while (capacity > 2*aNewSize){
                    capacity /= 2;
                }
                std::unique_ptr<T[]> newBuffer = std::make_unique<T[]> (capacity + 1);
                if(buffer != nullptr){
                    std::memcpy(newBuffer.get(), buffer.get(), sizeof(T)*(length));
                }
                
                buffer = std::move(newBuffer);
                return *this;
            }
        }
        
       
   protected:
        std::unique_ptr<T[]> buffer;
        size_t length;
        size_t capacity;
    };

    

} // namespace ECE141

#endif /* BufferManager_h */
