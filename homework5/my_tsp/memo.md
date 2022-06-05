# Homework 5
sampleコードがpythonのみだったのですが書ける自信が無いので、C++に書き換えてみようと思います。  
あとファイルがいっぱいあって何が何だかわからない。  
使用方法の理解も兼ねてのメモを残しておきます。    

<!-- 目次 -->
- [sampleの使用方法](#anchor1)
- [sample codeを読む](#anchor2)
   - [sample codeの構成まとめ](#anchor3)
- [C++で実装したい](#anchor4)
  - [構成](#anchor5)


<a id="anchor1"></a>
***
# sampleの使用方法
結果のcsvファイルを生成  
    ``` python3 output_generator.py ```   

最短経路を表示  
    ``` python3 output_verifier.py ```   
とりあえず結論から。  
理解の過程やコードの中身に関しては下で書いています。


<a id="anchor2"></a>
*** 

# とりあえず```sample_output_generator.py```を読む
 ```
 #!/usr/bin/env python3

from common import format_tour, read_input #common.pyから色々読みこんでいる

import solver_greedy #solver_greedy.pyから読み込む
import solver_random #solver_random.pyから読み込む
# どちらもsolve(cities)という関数で値を求めている

CHALLENGES = 7
```
対応するinputを読み込み
```
def generate_sample_output():
    for i in range(CHALLENGES):
        cities = read_input(f'input_{i}.cs v') 
```
組で要素を受け取っているfor文  
(solver,name)=(solver_random,'random'),(solver_greedy, 'greedy')のような感じ

```
        for solver, name in ((solver_random, 'random'), (solver_greedy, 'greedy')):
```
生成した訪れる順番のリストtourを受け取り、  
common.py内のformat_tourを使って  
sample/{name(for文で受け取り)}_{i(問題番号 一番外のfor文と対応)}.csvに書き込む
```
            tour = solver.solve(cities)
            with open(f'sample/{name}_{i}.csv', 'w') as f:
                f.write(format_tour(tour) + '\n')
```

common.pyのformat_tourを見てみる。  
intのリストtourを受け取り、str型にしてから各要素を'\n'で連結している。
```
def format_tour(tour):
    return 'index\n' + '\n'.join(map(str, tour))
```


***
output_generator.pyを実行すると他のファイルから必要な関数を取ってきて入力から出力までやってくれるのは理解した。 


ここで、sampleのコードの役割を大まかにまとめてみる。  


<a id="anchor3"></a>
## ファイルについて  
|  ファイル名<br>(xxは任意)  |  役割  | 関数 |   
| ---- | ---- | ---- |   
| output_generator.py | 入力から出力までの流れを管理 | generate_output() |
 solver_xx.py  |  実際の経路のリストを求める  | solve(cities) |
|  common.py  |  入出力やフォーマットの補助  | read_input <br> format_tour |
| output_verifier.py | スコアの計算 | verify_output() |  

<br>  

ディレクトリ構成の概形
```
tsp
├──── sample
│        └── result_i.csv
├── README.md
├── solver_xx.py
├── common.py
├── output_generator.py
└── output_verifier.py
```

***
<br>  

## 関数について
| 関数名 | 定義元ファイル | 引数 | 戻り値 | 役割 |
| ---- | ---- | ---- | ---- | ---- |
| generate_output | output_generator.py | null | null | 一連の流れを行う |
| read_input | common.py | データのfilename | 実数リスト | 正しいフォーマットの入力を受け取り<br>座標を実数リストにして返す |
| format_tour | common.py | tourの整数リスト | フォーマットされた文字列 | 街の順番の整数リストtourを受け取り<br>出力のためにindex,改行を入れフォーマットした文字列を生成 |
| solve | solver_xx.py | 座標の実数リスト | 整数リストtour | 座標から街の順番を求める。 |
| verify_output | output_verifier.py | null | null | csvファイルから経路の長さを計算<br>(※後述) |  
<br>

※verify_outputについて 
- 任意の文字列xxに対しi番目の問題の回答xxx_i.csvについて計算、出力
  - sampleディレクトリ内にあるときはsample/xxx_i.csvの形にすることに注意
  - ソースコード19行目のoutput_prefix内に任意のファイル名(上例のxxx)を追加することで利用できる
- 利用するのに必要な他ファイルが結果のcsvファイルのみで、  
  他のコードとは結びついていない
  と思ったが、commonのread_inputを使用していたので使う際はセットで残しておかなければならない


<a id="anchor4"></a>
***
## C++で実装したい！
上記より、verify_outputは自作する必要が無い。  
つまり、作らなければならない機能は  
- 座標をファイルから読み込む
- 座標のリストから実際の経路を求める  
- 経路の整数リストをフォーマットする
- 出力先ファイルに書き込む  
である。これを良い感じに実装しよう。


<br>  
<a id="anchor5"></a>  

# 以下C++実装パート
全部関数にして1ファイルにまとめても良いけど、  
せっかくなのでヘッダファイルでも作ろうかな、と言う気持ちになったので、ヘッダファイルを作ります。  
C++で作ったことないけどCでやったことあるのでいけるべ！という軽率な判断を後で後悔しないと良いですね。  
[こちら](https://docs.microsoft.com/ja-jp/cpp/build/walkthrough-creating-and-using-a-static-library-cpp?view=msvc-170)を参考に作っていきます。

