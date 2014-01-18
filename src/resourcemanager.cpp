#include "resourcemanager.h"

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

#include <mach-o/dyld.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>

//creates a single resource
int Resource_Manager::create(std::string res) {
    std::ifstream resource_stream;
    
    Blob blob;
    
    std::string file_name = res;
    
    res = static_cast<std::string>(get_path()) + "/" + res;
    
    resource_stream.open(res.c_str(), std::fstream::binary);
    
    //tests for open file
    if (!resource_stream.is_open()) {
        std::clog << "Failed to open file\n" << res << "\n" << std::endl;
        
        resource_stream.close();
        
        throw;
    }
    
    //tests for existing file
    if(map.find(res) != map.end()) {
        std::clog << "File already exists\n" << std::endl;
        return -1;
    }
    
    //get file length
    resource_stream.seekg(0, resource_stream.end);
    blob.buf_size = static_cast<int>(resource_stream.tellg());
    resource_stream.seekg(0, resource_stream.beg);
    
    //read file value to blob
    blob.alloc_buf();
    
    resource_stream.read(static_cast<char*>(blob.buf), blob.buf_size);

    map[file_name] = blob;
    
    //tally memory usage & close stream
    
    Resource_Manager::used_memory += blob.buf_size;
    resource_stream.close();
    
    //changes the cache for an inserted resource
    
   // Resource_Manager::update_cache(blob, 2);

    return 1;
}

//creates a list of resources from a string vector
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
        blob.buf_size = static_cast<int>(resource_stream.tellg());
        resource_stream.seekg(0, resource_stream.beg);
        
        blob.alloc_buf();
        
        used_memory += blob.buf_size;
        
        //read file value to blob
        resource_stream.read(static_cast<char*>(blob.buf), blob.buf_size);

        map[resource_list[i]] = blob;
        
        update_cache(blob, 2);
        check_overflow();

        resource_stream.close();
    }
    return 1;
}


//unloads resources for internal use
void Resource_Manager::remove(std::string res) {
    exists(res);
    
    Blob blob = map[res];
    
    free(blob.buf);
    update_cache(blob, 3);
    
    used_memory -= blob.buf_size;
}


//returns pointer to the
void* Resource_Manager::get_buf(std::string res) {
    return map[res].buf;
}

//returns the desired blob object from file name
Blob Resource_Manager::get(std::string res) {
    
    Blob blob = map[res];
    
    //reconfigures the cache
    //update_cache(blob, 1);
    
    return blob;
}


//search for file returns true if exists
bool Resource_Manager::exists(std::string res) {
    if (map[res].buf == 0 || NULL)
        return false;
    return true;
}

char* Resource_Manager::get_path() {
    
#ifdef __APPLE__
    char path [PATH_MAX + 1];
    uint32_t size = sizeof(path);
    
    if (_NSGetExecutablePath(path, &size) == 0)
        return dirname(path);
    std::clog <<"buffer too small; need " << size << std::endl;
    throw 1;
    
#endif
    
#ifdef __posix__

    char path [PATH_MAX + 1];
    uint32_t size = sizeof(path);

std::string proc;
    
#if defined __gnu_linux__ || defined __linux__
    proc = "/proc/self/exe";

#endif
    
#if defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined __FreeBSD__ || defined __NetBSD__
    proc = "/proc/curproc/file"
            
#endif
        
#if defined __sun || defined sun
    proc = "/proc/self/path/a.out";
    
#endif
    
    if (readlink(proc, path, size) == 0)
        return dirname(path);
    std::clog <<"buffer too small; need " << size << std::endl;
    throw 1;

#endif

    
#if defined _WIN32 || defined _WIN16
    
#error I’d just like to interject for a moment. What you’re refering to as Linux, is in fact, GNU/Linux, or as I’ve recently taken to calling it, GNU plus Linux. Linux is not an operating system unto itself, but rather another free component of a fully functioning GNU system made useful by the GNU corelibs, shell utilities and vital system components comprising a full OS as defined by POSIX. Many computer users run a modified version of the GNU system every day, without realizing it. Through a peculiar turn of events, the version of GNU which is widely used today is often called “Linux”, and many of its users are not aware that it is basically the GNU system, developed by the GNU Project. There really is a Linux, and these people are using it, but it is just a part of the system they use. Linux is the kernel: the program in the system that allocates the machine’s resources to the other programs that you run. The kernel is an essential part of an operating system, but useless by itself; it can only function in the context of a complete operating system. Linux is normally used in combination with the GNU operating system: the whole system is basically GNU with Linux added, or GNU/Linux. All the so-called “Linux” distributions are really distributions of GNU/Linux.
    
#endif
    char *null = NULL;
    
    return null;
}

//simple buf malloc
void Blob::alloc_buf() {
    buf = new char[buf_size];
}



/*--------------------------------------
 
 Below is for unimplimented cache system
 (unnecessary but nice)
 
--------------------------------------*/

//returns memory use
int Resource_Manager::get_memory_usage() {
    return used_memory;
}


//sets max memory
void Resource_Manager::set_memory_size(int memory_size) {
    max_memory = memory_size;
}


//recreates resources that were unloaded from the cache
int Resource_Manager::recreate(char* res) {
    std::ifstream resource_stream;
    
    Blob blob = map[res];
    blob.alloc_buf();
    
    resource_stream.open(res, std::fstream::binary);
    
    //read file value to blob
    resource_stream.read(static_cast<char*>(blob.buf), blob.buf_size);
    map[res] = blob;
    
    //tally memory usage & close stream
    used_memory += blob.buf_size;
    resource_stream.close();
    
    update_cache(blob, 2);
    check_overflow();

    return 1;
}


//unloads resources if memory has overflown
void Resource_Manager::remove_overflow(Blob blob) {
    free(blob.buf);
    update_cache(blob, 3);
    
    used_memory -= blob.buf_size;
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
                if (blob.buf != changed_blob.buf)
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
        
        if (blob.rel == static_cast<int>(map.size() && blob.buf != 0) ) {
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