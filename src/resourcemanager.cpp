#include <mach-o/dyld.h>
#include "resourcemanager.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

//creates a single resource
int Resource_Manager::create(std::string resource_name) {
    std::ifstream resource_stream;
    
    Blob blob;
    
    resource_name = static_cast<std::string>(get_path()) + "/" + resource_name;
    
    resource_stream.open(resource_name.c_str(), std::fstream::binary);
    
    //tests for open file
    if (!resource_stream.is_open()) {
        std::clog << "Failed to open file\n" << resource_name << "\n" << std::endl;
        
        resource_stream.close();
        
        throw -1;
    }
    
    //tests for existing file
    if(map.find(resource_name) != map.end()) {
        std::clog << "File already exists\n" << std::endl;
        throw 1;
    }
    
    //get file length
    resource_stream.seekg(0, resource_stream.end);
    blob.buffer_size = static_cast<int>(resource_stream.tellg());
    resource_stream.seekg(0, resource_stream.beg);

    
    
    //read file value to blob
    blob.buffer_pointer = new char[blob.buffer_size];
    resource_stream.read(static_cast<char*>(blob.buffer_pointer), blob.buffer_size);
    map[resource_name] = blob;
    
    //tally memory usage & close stream
    
    Resource_Manager::used_memory += blob.buffer_size;
    resource_stream.close();
    
    //changes the cache for an inserted resource
    
    Resource_Manager::update_cache(blob, 2);

    return 1;
}

//creates a list of resources from a sting vector
int Resource_Manager::create(std::vector<std::string> resource_list) {
    std::ifstream resource_stream;
    
    Blob blob;
    
    for (int i = 0; i < resource_list.size(); i++ ) {
        
        resource_list[i] = (static_cast<std::string>(get_path()) + "/" + resource_list[i]).c_str();
        
        resource_stream.open(resource_list[i].c_str(), std::fstream::binary);
        
        //tests for open file
        if (!resource_stream.is_open()) {
            std::clog << "Failed to open file\n" << resource_list[i] << "\n" << std::endl;
        
            resource_stream.close();
            
            return -1;
        }
        
        //tests for existing file
        if(map.find(resource_list[i]) != map.end())
            std::clog << "File already exists\n" << std::endl;
        
        //get file length
        resource_stream.seekg(0, resource_stream.end);
        blob.buffer_size = static_cast<int>(resource_stream.tellg());
        resource_stream.seekg(0, resource_stream.beg);
        
        used_memory += blob.buffer_size;
        
        //read file value to blob
        blob.buffer_pointer = new char[blob.buffer_size];
        resource_stream.read(static_cast<char*>(blob.buffer_pointer), blob.buffer_size);

        map[resource_list[i]] = blob;
        
        update_cache(blob, 2);
        check_overflow();

        resource_stream.close();
    }
    return 1;
}


//unloads resources for internal use
void Resource_Manager::remove(std::string file_name) {
    check_exist(file_name);
    
    Blob blob = map[file_name];
    
    free(blob.buffer_pointer);
    update_cache(blob, 3);
    
    used_memory -= blob.buffer_size;
}

//unloads resources if memory has overflown
void Resource_Manager::remove_overflow(Blob blob) {
    free(blob.buffer_pointer);
    update_cache(blob, 3);
    
    used_memory -= blob.buffer_size;
}


//search for file returns true if exists
void Resource_Manager::check_exist(std::string file_name) {
    if (map[file_name].buffer_pointer == 0) {
        std::clog << "file " << file_name << " cannot be found" << std::endl;
        throw 1;
    }
}


//returns memory use
int Resource_Manager::get_memory_usage() {
    return used_memory;
}


//sets max memory
void Resource_Manager::set_memory_size(int memory_size) {
    max_memory = memory_size;
}


//recreates resources that were unloaded from the cache
int Resource_Manager::recreate(char* resource_name) {
    std::ifstream resource_stream;
    
    Blob blob = map[resource_name];
    blob.buffer_pointer = malloc (blob.buffer_size);
    
    resource_stream.open(resource_name, std::fstream::binary);
    
    //read file value to blob
    blob.buffer_pointer = new char[blob.buffer_size];
    resource_stream.read(static_cast<char*>(blob.buffer_pointer), blob.buffer_size);
    map[resource_name] = blob;
    
    //tally memory usage & close stream
    used_memory += blob.buffer_size;
    resource_stream.close();
    
    update_cache(blob, 2);
    check_overflow();

    return 1;
}


//returns the desired blob object from file name
Blob Resource_Manager::get_resource(std::string file_name) {
    if(map.find(file_name) != map.end()) {
        std::clog << "File already exists\n" << std::endl;
        throw 1;
    }
    
    Blob blob = map[file_name];
    
    //reconfigures the cache
    update_cache(blob, 1);
    
    return blob;
}

//changes the cache index for gets, insertions, and removals
void Resource_Manager::update_cache(Blob changed_blob, int type) {
    Blob blob;
    
    for (std::map<std::string,Blob>::iterator it=map.begin(); it!=map.end(); ++it) {
        it->second = blob;
        
        switch (type) {
                
            //Updates get requests
            case 1:
                if (blob.rel > changed_blob.rel)
                    blob.rel--;
                else if (it == map.end())
                    changed_blob.rel = 0;
                else
                    blob.rel++;
                break;
                
            //Updates cache for insertion
            case 2:
                if (blob.buffer_pointer != changed_blob.buffer_pointer)
                    blob.rel++;
                break;
                
            //Updates cache for removing blob
            case 3:
                if (blob.rel < changed_blob.rel)
                    blob.rel--;
                else if(it == map.end())
                    changed_blob.rel = static_cast<int>(map.size());
                break;
                
            default:
                std::clog << "Bad type input" << std::endl;
                break;
        }
    }
}

//removes memory until below the maximum memory usage
void Resource_Manager::manage_overflow() {
    Blob blob;
    
    std::map<std::string,Blob>::iterator it=map.begin();
    
    while (used_memory < max_memory) {
        it->second = blob;
        
        if (it == map.end())
            it = map.begin();
        
        if (blob.rel == static_cast<int>(map.size()) ) {
            remove_overflow(blob);
        }
        ++it;
    }
}

//checks need to deallocate memory
void Resource_Manager::check_overflow() {
    if (used_memory < max_memory)
        return;
    else
        manage_overflow();
}

//not implemented yet
char* Resource_Manager::get_path() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return path;
    else
        std::clog <<"buffer too small; need size" << size << std::endl;
    throw 1;
}