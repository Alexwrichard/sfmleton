#ifndef __sfml__resourcemanager__
#define __sfml__resourcemanager__

#include <iostream>
#include <vector>
#include <map>

//intermediate object that holds resource data, length, and relevency in memory cache
class Blob {

public:
    
    void* buf;//pointer to data in memory
    
    void alloc_buf();
    
    int rel = 0;//relevency in relation to other blob objects in cache from 0 to size of map
    size_t buf_size;//size of buffer
};


//handles all creating, removing, and memory-managing of thoperating_systeme resources
class Resource_Manager {

    
private:
    int max_memory;
    int used_memory;
    
    //recreates the resources data on get request for resources deallocated by cache oveflow
    int recreate(char* res);
    
    //frees memory of blob file given by manage_overflow()
    void remove_overflow(Blob blob);
    
    //triggers manage_overflow if used_memory exceeds max_memory
    void check_overflow();
    
    //removes the file with highest rel value then check for overflow
    void manage_overflow();
    
    //updates the rel value of each of the blob objects for gets, insertions, and removals
    void update_cache(Blob changed_blob, int type);
    
    //gets path of the executable with a swtich for each operating system defined in the preprocessor
    char* get_path();
        
    std::map<std::string, Blob> map;

public:
    //loads data and metadata into blob object
    int create(std::string res);
    int create(std::vector<std::string> res_list);
    
    //frees the data associated with the file
    void unload(std::string res);
    int unload(std::vector<std::string> res_list);
    
    //removes all data from memory
    void remove(std::string res);
    void remove(std::vector<std::string> res_list);

    //throws exeption if the resource's blob pointer has been removed
    bool exists(std::string res);
    
    //returns blob from name of the resource
    Blob get(std::string res);
    
    void* get_buf(std::string res);
    
    void get_size(std::string res);
    
    //returns the memory used by resources
    int get_memory_usage();
    
    //sets the maximum amout of memory usable by Resource_Manager
    void set_memory_size(int memory_size);

};

#endif /* defined(__sfml__resourcemanager__) */
