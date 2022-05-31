# homework4
# 準備

[wikipedia_data.zip](https://drive.google.com/file/d/1zqtjSb-ZoR4rzVUWZrjNSES5GKJhYmmH/view?usp=sharing) をダウンロードして解凍し、  
以下のようなディレクトリ構成にしてください。

```
homework4_1
├────.gitignore
│       └──data
│           ├── graph_small.png
│           ├── links_small.txt
│           ├── links.txt
│           ├── pages_small.txt
│           └── pages.txt
├
├── README.md
├── homework4_1_DFS.cpp
└── homework4_1_BFS.cpp
```
# 実行方法
## C++
テスト環境 : gcc  
(Apple clang version 13.1.6 (clang-1316.0.21.2.5))
```
g++ -w homework4_1_DFS.cpp && ./a.out
```
```
g++ -w homework4_1_BFS.cpp && ./a.out
```
# 実行例
```
Now: reading pages... (Please wait a moment)
Completed: reading pages

Now: reading links... (Please wait a moment)
Completed: reading links

Please input start (example:Google)
>> Google
Please input target (example:渋谷)
>> 渋谷
Now exploring...
Found: 渋谷(22557)
(18080 pages visited)

Show route? ([0]:No [1]:Yes) 
>> 1
"route.txt"に出力します
出力中...
出力が完了しました。
```
***
route.txt の例
```start: Google (457783)
goal: 渋谷 (22557)
18080 pages visited and via 24 pages. (BFS)
___[route]____________
Google (457783)
-> MSN (10002)
-> インフォシーク (10005)
-> ぷらら (196600)
-> エキサイト (214709)
-> Yahoo!_JAPAN (215462)
-> Yahoo!_Days (260116)
-> Yahoo!メッセンジャー (271739)
-> Yahoo!ウォレット (271748)
-> ジャパンネット銀行 (27716)
-> Yahoo!インターネット検定 (303629)
-> Yahoo!ケータイ (32416)
-> Yahoo!リクナビ (387045)
-> Yahoo!モバイル (389321)
-> ヤフオク! (434078)
-> Yahoo!ブログ (501101)
-> Yahoo!メール (520334)
-> Yahoo!みんなの検定 (541838)
-> 月刊チャージャー (569935)
-> ドロップキューブ (594042)
-> ポケモンガーデン (606336)
-> Yahoo!ブックストア (614491)
-> Yahoo!_Internet_Guide (627272)
-> Yahoo!_Cafe (676908)
-> 渋谷 (22557)
______________________
```
# 注意
- pages/linksの読み込みおよび探索には時間がかかります。
- show routeをYesにするとディレクトリhomework4_1内に結果の出力ファイルroute.txtが生成されます。  
- 既に該当ディレクトリ内にroute.txtが存在する場合には上書きされます。