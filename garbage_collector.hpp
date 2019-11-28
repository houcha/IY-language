#ifndef GARBAGE_COLLECTOR_HPP
#define GARBAGE_COLLECTOR_HPP

#include <vector>


class GarbageCollector {

  private:

    std::vector<void*> memory_;

  public:

    GarbageCollector()
        : memory_() {}

    ~GarbageCollector() {
      for (void* allocated : memory_) {
        free(allocated);
      }
    }

    void* Allocate(size_t nmemb, size_t size) {
      void* allocated = calloc(nmemb, size);
      memory_.push_back(allocated);
      return allocated;
    }
};


#endif // GARBAGE_COLLECTOR_HPP

