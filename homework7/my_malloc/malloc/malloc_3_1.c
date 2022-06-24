// 空き領域をマージ(右)
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
my_heap_t my_heaps[10];

//
// Helper functions (feel free to add/remove/edit!)
//

//sizeを入れると対応するfree_list_binの番号を返す
//こことmy_heaps,for文の数字を変えればfree-list-binの分け方が変えられる
//for文がprint_bin,my_initialize,my_mallocのmetadataを探す場所の三ヶ所
int which_free_list(size_t size){
  if(size < 16)return 0; //ここをsize<5000とかにすれば全て０に入りbin以外のデバッグに使えるかも
  else if(size < 32)return 1; 
  else if(size < 64)return 2;
  else if(size < 128)return 3;
  else if(size < 256)return 4;
  else if(size < 512)return 5;
  else if(size < 640)return 6;
  else if(size < 1024)return 7;
  else if(size < 2048)return 8;
  else return 9;
}

void my_add_to_free_list(my_metadata_t *metadata) {
  int bin_num = which_free_list(metadata->size);
//  printf("add to free list  list:%d, size:%zu\n",bin_num,metadata->size);
  assert(!metadata->next); //metadata->next!=NULLで強制終了
  metadata->next = my_heaps[bin_num].free_head;
  my_heaps[bin_num].free_head = metadata;
}

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev, int remove_bin) {
//prev->size!=0を入れないとダミーで引っかかって変なとこに入る？
//  printf("remove from bin %d(metadata->size:%zu)\n",remove_bin,metadata->size);

  if (prev&&prev->size!=0) {
//    printf("prev is exist:(prev->size:%zu)\n",prev->size);
    prev->next = metadata->next;
  }
  else{
    my_heaps[remove_bin].free_head = metadata->next; //metadataのsizeは変更後より、削除前のリストはprevで見なければいけない
  }
   metadata->next = NULL;
//  printf("-------------------\n");
}


void print_bin_size(){
  //デバッグに使っていた
  for(int i = 0;i<10;i++){
    int counter = 0;
    my_metadata_t *tmp = my_heaps[i].free_head;
    printf("--[%d]-------------\n",i);
    while(tmp){
      if(tmp->size!=0){
        printf("%zu ",tmp->size);
        counter++;
        }
      tmp = tmp->next;
    }
    printf("\nbin[%d]: %d free metadata!\n",i,counter); //dummy
  }
  printf("========================\n");
}

void *connect_right_free(my_metadata_t *metadata1,my_metadata_t *metadata2){
  my_metadata_t *tmp= metadata1;
  tmp->size = metadata1->size + metadata2->size + sizeof(my_metadata_t); //サイズを結合
  return tmp;
}

void move_bin(my_metadata_t *prev, my_metadata_t *metadata, int now_bin){
  //metadataがサイズの違う場所にいる時
  //今の所出番はない?
//  printf("move!");
  my_metadata_t *tmp = metadata;
  tmp->size=metadata->size;
  tmp->next=NULL;
  my_add_to_free_list(tmp);
  my_remove_from_free_list(metadata,prev,now_bin);
}
//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
//  printf("(size < 256): 0\n(size < 512): 1\n(size < 1024): 2\n(size < 2048): 3\n2048<=size: 4\n");
  //全てのfree-list-binを初期化
  for(int i = 0;i<10;i++){
    my_heaps[i].free_head = &my_heaps[i].dummy;
    my_heaps[i].dummy.size = 0;
    my_heaps[i].dummy.next = NULL;
  }

}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
void *my_malloc(size_t size) {
//  printf("=======================\n");
//  print_bin_size();
  my_metadata_t *metadata = NULL; //辿るポインタが別にあるのでNULLに
  my_metadata_t *prev = NULL;
  // First-fit: Find the first free slot the object fits.
  // TODO: Update this logic to Best-fit!
 
  /*辿るポインタと別に保持しておくポインタを用意してみた*/
  int bin_num = which_free_list(size);
  my_metadata_t *tmp_metadata = NULL;//対応するbinの先頭
  my_metadata_t *tmp_prev = NULL;

  int remove_bin = -1;
  for(int i = bin_num;i<10;i++){
    tmp_metadata = my_heaps[i].free_head;
    // bin_num番のfree_list_binから順に見ていく
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

          //mergeの影響で他のリストに違う大きさのものが入ってたら移す
          //と思っていたが、addするときに関数内で処理されているのでいらなさそう
         /*
         if(which_free_list(tmp_metadata->size)!=i){
          //他のリストに移し替える
          move_bin(tmp_prev,tmp_metadata,i);
         }
         */
        }
      }
      tmp_prev = tmp_metadata;
      tmp_metadata = tmp_metadata->next;
      }

      if(metadata){
//        printf("\nIn bin[%d]: (metadata size:%zu)",i,metadata->size);
        remove_bin = i;
        break;
        } //もし更新されなかったら次に大きいリストを見る
    }
/*    if(metadata)printf("found_metadata size: %zu\n",metadata->size);else{
      printf("Not_found!\n");
    }
*/

  
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
//  printf("-[remove]-----------------\n");
//  printf("remove_bin : %d (metadata->size:%zu)\n",remove_bin,metadata->size);
  void *ptr = metadata + 1;
  size_t remaining_size = metadata->size - size;
  metadata->size = size;
  // Remove the free slot from the free list.
  my_remove_from_free_list(metadata, prev, remove_bin);//

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
//  printf("-[add (in my_malloc)]------------------\n");
//  printf("metadata: size %zu  (add_to : %d)\n",new_metadata->size,which_free_list(new_metadata->size));
    my_add_to_free_list(new_metadata);
  }
//  print_bin_size();
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
//  printf("-[add (in my_free)]------------------\n");
//  printf("metadata: size %zu  (add_to : %d)\n",metadata->size,which_free_list(metadata->size));

  /*右側が空きか調べる*/
  my_metadata_t *right = (my_metadata_t *)((char *)ptr + metadata->size);
  my_metadata_t *tmp_metadata = NULL;//対応するbinの先頭
  my_metadata_t *tmp_prev = NULL;
  if(right!=NULL){
//    printf("%zu ",right->size);
      tmp_metadata = my_heaps[which_free_list(right->size)].free_head;
      // bin_num番のfree_list_binから順に見ていく
      while (tmp_metadata){
        if(tmp_metadata==right){
          break;
        }
        tmp_prev = tmp_metadata;
        tmp_metadata = tmp_metadata->next;
    }
  }
  if(!tmp_metadata){
  my_add_to_free_list(metadata);
  }else{
//    printf("connection! : ");
    my_metadata_t *tmp = connect_right_free(metadata,tmp_metadata);
    my_remove_from_free_list(tmp_metadata,tmp_prev,which_free_list(right->size));
    my_add_to_free_list(tmp);
  }

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
