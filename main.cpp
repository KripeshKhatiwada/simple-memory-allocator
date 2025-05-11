#include <iostream>
#define POOL_SIZE (1024*1024)
char memory_Pool[POOL_SIZE];

struct Block {
    size_t size;
    bool isFree;
    Block* next;
};
Block* freeList = (Block*) memory_Pool;
void initializeMemory() {
    freeList->size = POOL_SIZE - sizeof(Block);
    freeList->isFree = true;
    freeList->next = nullptr;
}
void* my_malloc(size_t requested_size) {
    Block* current = freeList;

    while (current != nullptr) {
        if (current->isFree && current->size >= requested_size) {
            // If the block is much larger, split it
            if (current->size >= requested_size + sizeof(Block) + 1) {
                 Block* newBlock = (Block*)((char*)current + sizeof(Block) + requested_size);
                newBlock->size = current->size - requested_size - sizeof(Block);
                newBlock->isFree = true;
                newBlock->next = current->next;

                current->size = requested_size;
                current->next = newBlock;
            }

            current->isFree = false;
            return (char*)current + sizeof(Block);  // Return pointer to memory after metadata
        }

        current = current->next;
    }

    return nullptr;  // No suitable block found
}
void my_free(void* ptr) {
    if (ptr == nullptr) return;

    Block* blockToFree = (Block*)((char*)ptr - sizeof(Block));
    blockToFree->isFree = true;

    // Coalesce with the next free block if adjacent
    Block* current = freeList;
    while (current != nullptr && current->next != nullptr) {
        if (current->isFree && (char*)current + sizeof(Block) + current->size == (char*)current->next) {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
            break; // Re-check from the beginning in case of multiple merges
        }
        current = current->next;
    }

    // Coalesce with the previous free block if adjacent
    current = freeList;
    Block* prev = nullptr;
    while (current != nullptr && current != blockToFree) {
        prev = current;
        current = current->next;
    }

    if (prev != nullptr && prev->isFree && (char*)prev + sizeof(Block) + prev->size == (char*)blockToFree) {
        prev->size += sizeof(Block) + blockToFree->size;
        prev->next = blockToFree->next;
    } else if (blockToFree->next != nullptr && blockToFree->next->isFree && (char*)blockToFree + sizeof(Block) + blockToFree->size == (char*)blockToFree->next) {
        blockToFree->size += sizeof(Block) + blockToFree->next->size;
        blockToFree->next = blockToFree->next->next;
    }
}

int main() {
    std::cout << "Memory Pool Begin" << std::endl;
    initializeMemory();
    void* ptr1 = my_malloc(100);
    void* ptr2 = my_malloc(100);
    if (ptr1) {
        std::cout << "Allocated of ptr1 100 bytes at " << ptr1 << std::endl;
    } else {
        std::cout << "Allocation failed" << std::endl;
    }
    if (ptr2) {
        std::cout << "Allocated of ptr2 100 bytes at " << ptr2 << std::endl;
    } else {
        std::cout << "Allocation failed" << std::endl;
    }
    my_free(ptr1);  // Frees the memory at pointer
    std::cout << "Freed memory at ptr1" << std::endl;

return 0;
}
