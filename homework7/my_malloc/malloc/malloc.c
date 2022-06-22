//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Interfaces to get memory pages from OS
//

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//

typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
// Free list bin に
// my_heap_t my_heap; ->
my_heap_t my_heaps[5];

//
// Helper functions (feel free to add/remove/edit!)
//

//sizeを入れると対応するfree_list_binの番号を返す
int which_free_list(size_t size){
  if(size < 5000)return 0; //free_listの初期化が終わるまでは全て仮に0のリストに入れておく
  else if(size < 512)return 1;
  else if(size < 1024)return 2;
  else if(size < 2048)return 3;
  else return 4;
}

void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next);
  int bin_num = which_free_list(metadata->size);
  metadata->next = my_heaps[bin_num].free_head;
  my_heaps[bin_num].free_head = metadata;
}

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev) {
  if (prev) {
    prev->next = metadata->next;
  } else {
    my_heaps[which_free_list(metadata->size)].free_head = metadata->next;
  }
  metadata->next = NULL;
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  my_heaps[0].free_head = &my_heaps[0].dummy;
  my_heaps[0].dummy.size = 0;
  my_heaps[0].dummy.next = NULL;
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
void *my_malloc(size_t size) {
  my_metadata_t *metadata = NULL; //辿るポインタが別にあるのでNULLに
  my_metadata_t *prev = NULL;
  // First-fit: Find the first free slot the object fits.
  // TODO: Update this logic to Best-fit!

  //ここを変えたい
  /*辿るポインタと別に保持しておくポインタを用意してみた*/
  int bin_num = which_free_list(size);
  my_metadata_t *tmp_metadata = my_heaps[bin_num].free_head;
  my_metadata_t *tmp_prev = NULL;
  while (tmp_metadata){
    if(tmp_metadata->size >= size){
      if(!metadata){
        //metadataがまだ一度も更新されていない時
        metadata = tmp_metadata;
        prev = tmp_prev;
      }else{
        if(tmp_metadata->size < metadata->size){
        // 今見ているmetadataと更新されたmetadataを比べてサイズの小さい方を保持
        metadata = tmp_metadata;
        prev = tmp_prev;
        }
      }
      } //whileの条件をこっちに持ってきた //まだちゃんと動く
    tmp_prev = tmp_metadata;
    tmp_metadata = tmp_metadata->next;
    //metadata->size >= size になった時終了
  }
  
  // now, metadata points to the first free slot
  // and prev is the previous entry.

  if (!metadata) {
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    size_t buffer_size = 4096;
    my_metadata_t *metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->next = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata);
    // Now, try my_malloc() again. This should succeed.
    return my_malloc(size);
  }

  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  void *ptr = metadata + 1;
  size_t remaining_size = metadata->size - size;
  metadata->size = size;
  // Remove the free slot from the free list.
  my_remove_from_free_list(metadata, prev);

  if (remaining_size > sizeof(my_metadata_t)) {
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }
  return ptr;
}

// This is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  my_add_to_free_list(metadata);
}

// This is called at the end of each challenge.
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
