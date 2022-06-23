# 3-1
## 右側の空き領域をマージ
my_freeで解放する際に、  
``` *right = (my_metadata_t *)((char *)ptr + metadata->size); ```  
より、解放する領域の次のmetadataのポインタrightを求め、free_listから探して、あったら  
(見つけた右側の空き領域をrightとすると)  
- tmpにmetadataとrightを合わせたサイズを持つnodeを作る(nextはNULL)
- metadataはfree-listに加えない
- rightをfree-list-binからremove
- tmpをfree-list-binにadd


# 3-2
## 左側の空き領域
左側を探す方法を考える
free-listでのprevは連続した領域であるとは限らない -> 使えない  
my_freeが受け取れるデータは*ptr(解放するobjectの先頭ポインタ)のみ  
-> ここから解放するobjectのメタデータを受け取ることはできる  
-> metadataに物理的位置のprevを持たせる？  
-> - addする時にmetadata + sizeof(my_metadata_t)+metadata->sizeより物理的に連続する次のmetadataにアクセスできる  
-> そこに前のmetadataのアドレスを書ける  
-> removeする時(=使用する時) : そのままでいい(のちにprevからfree-listを探索するから、そこでなければ空きではないということになる)
