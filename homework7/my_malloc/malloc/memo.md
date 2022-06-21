
# メモ  
malloc.cを読む  

l21. OSはメモリ領域を確保して、その開始アドレスを返す
```
void *mmap_from_system(size_t size); 
```
</br> 

l22.ptr番地からsizeバイト分の領域をOSに返却する
```
void munmap_to_system(void *ptr, size_t size); 
```
</br>


l28. 入れるデータの連結リスト
```
typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

```
</br> 

l33. 空き領域の構造体?  
*free_head : その領域の先頭(1つ分多くデータをとっている)
dummy : 仮にデータを入れている?
```
typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

```

l.77 my_malloc