#ifndef __sfml__resourcemanager__
#define __sfml__resourcemanager__

#include <iostream>
#include <vector>
#include <map>

//intermediate object that holds resource data, length, and relevency in memory cache
struct Blob {
    void *buffer_pointer;
    int buffer_size;
    
    //relevency in relation to other blob objects in cache from 0 to size of map
    int rel = 0;
};

//handles all creating, removing, and memory-managing of those resources
class Resource_Manager {

public:
    //loads data and metadata into blob object
    int create(std::string file_name);
    int create(std::vector<std::string> resource_list);
    
    //frees data of specified resource
    void remove(std::string file_name);
    
    //throws exeption if the resource's blob pointer has been removed
    void check_exist(std::string file_name);
    
    //returns blob from name of the resource
    Blob get_resource(std::string file_name);
    
    //returns the memory used by resources
    int get_memory_usage();
    
    //sets the maximum amout of memory usable by Resource_Manager
    void set_memory_size(int memory_size);

private:
    int max_memory;
    int used_memory;
    
    //recreates the resources data on get request for resources deallocated by cache oveflow
    int recreate(char* file_name);
    
    //frees memory of blob file given by manage_overflow()
    void remove_overflow(Blob blob);
    
    //triggers manage_overflow if used_memory exceeds max_memory
    void check_overflow();
    
    //removes the file with highest rel value then check for overflow
    void manage_overflow();

    //updates the rel value of each of the blob objects for gets, insertions, and removals
    void update_cache(Blob changed_blob, int type);
    
    //gets path of the executable (for mac OSX)
    char* get_path();

    std::map<std::string, Blob> map;
};

#endif /* defined(__sfml__resourcemanager__) */
