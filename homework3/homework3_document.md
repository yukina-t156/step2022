# 宿題1
## 方針
- スタックを使ってもできそうだが、なるべくソースコードの原型を残したいので今回は使わない。（うまく組み合わせられる自信がない） 
- 優先度を追加し、*,/の時に優先度を高くしておく。 
- 優先度の最大値を保持し、その回数式を更新してループにかければできる？ 
- 式を更新する際にpriorities,tokensで使った部分を切り取るのを忘れない
- evaluate部分について
```
例:1*4/2+4*2+1のとき
tokens[+,1,*,4,/,2,+,4,*,2,+,1]
priorities[0,1,1,0,1,0]

index=3の時

#max_priorityの時のみ実行
if max_priority==priorities[(int)index/2]: 
    tokens[index-2]=(1*4) #1つ目の数字を置き換える
    tokens[index-1:index+1].erase #tokensの演算子と2つ目の数字を削除
    priorities[int(index/2)].erase #該当するpriorityを削除

結果:
tokens[+,4,/,2,+,4,*,2,+,1]
priorities[0,1,0,1,0]
(=4/2+4*2+1)
```
のような感じにできそう?  



## 手順1
read_asterisk と read_slashをdefし、tokenize内にも追加  
※evaluate内には手順3で追加

## 手順2
read_関数内,tokenize,メインのwhile文にpriorities,priority,last_priorityを追加
※tokenizeの戻り値を(tokens,priorities)にしておく  
※read_の返り値を(token,index+1,priority)にする  

## 手順3  
*,/,及びpriorityの処理をevaluateに追加  
typeと2つの数を受け取り結果を返す二項演算の関数を作る:binary_operation  
priority==0の時のみansに足し引きする

- 挿入するときなどにtypeとのtupleにすることを忘れない！(気づかずにエラーを吐かせてしまった)

***
# 宿題2
testcaseをruncaseに追加  
できるだけ網羅的に
## テストケースについて
```
    test("1+2")
    test("1.0+2.1-3")
    test("1+2*3*4+1")
    test("0.5+1.5-2.0") #小数同士の足し算・引き算
    test("3/1.5*2.0") #小数同士の掛け算・割り算
    test("2.5+3-1.5") #小数と整数の足し算・引き算
    test("1.4*2") #小数と整数の掛け算
    test("1.0+1.5*2/3")
    test("2.5*4/2+1-2.5")
```

***
# 宿題3
(,)の分もtoken,prioritiesに保存する(priorityの受け渡しができないので)
->処理をしないならtokensに入れる必要はないのでは?(逆に添字がずれるから入れない方がいい？)
evaluateで出てきた時に何もせずprioritiesとtokensから削除する
- *,/,(の優先度を上げる  
  - *,/の時
    - 前が+,-,*,/の時は宿題1と同じ
    - 前が(の時は(+1
    - 前が)の時は)と同じ
  - (なら直前のpriority+1  
  - )なら演算子の優先度を下げる
    - この時にその前に出てきた(の前まで下げるので、保持しておく必要がある。
    - (が出てきたら直前の優先度をスタックに入れ、)が出るたびにpopして使う
- 宿題1と同様に優先度のMaxを保存、優先度が高いものから行う
例
1+2*3  
0 1 1  
 ( +  ) 1 * 3 * ( 3 + 1 * 4 ) - 3  
　0　1　1　2　2　3　1　0  

## 手順1
- 'type': 'PAREN'を追加
- read_left_paren、read_right_parenを作成
 - stackをtokenizeに追加し引数、返り値に追加
- tokenizeに'(',')'の分岐を追加
- 左右の区別は優先度のみでOK
- 不正なカッコのエラー処理もする
スタックの実装
```
from collections import deque #スタックに必要

#tokenize内
left_st = deque([]) #スタックを用意

#read_left_paren内
def read_left_paren(line, index, last_priority, left_st):
  left_st.append(last_priority)
  # 略
  return token, index + 1, priority, left_st


#read_right_paren内
def read_right_paren(line, index, last_priority, left_st):
  priority = left_st.pop()
  # 略
  return token, index + 1, priority, left_st

```
### 問題：+,-,*,/も宿題1のままだと動かない！
例: 1+(1+3*4)*2  
-> （内の最低値の基準のようなものを決めておく  
例えば  
1+(1-1-1-1-1-1-1)+1  
 01 0 0 0 0 0 0 00 のようにならないように、下限を決めて  
1+(1-1-1-1-1-1-1)+1  
 01 1 1 1 1 1 1 00  
1+(1+(1+1+1)+1)+1  
 01 12 2 2 11 00  
 のようになるように  
### ベースとなる変数を作り、'('と')'で更新し常にpriorityに足すようにする  
-  tokenize内で宣言
- read_内に入れなくてもtokenize内だけで処理できるようになる
- 初期のbaseは0に  
- -10とかにならないように last_priorityにはbaseで揃える前のものを渡す(priorities[-1]-baseを渡せば良い)
-  read_に渡す際にpriorities[-1]-baseする
-  '('の時にbaseを増やす
-  ')'の時にbaseを戻す
-  最後にprioritiesmの'(',')'に該当する部分を削除しておく(evaluateで添え字がずれるのを防ぐ)

## 手順2
read_asterisk, read_slash内のpriorityの処理を変更  
->baseを加えたことにより変更不要に
  - *,/の時
    - 前が+,-の時は+1
    - 前が*,/の時は宿題1と同じ
    - 前が(の時は(+2 
    - 前が)の時は)と同じ  
## 手順３
- evaluate内の処理
- あとは優先度が適切に設定されているので宿題1と同様に動くはず。

## テストケースについて
- ()をたくさん含む時
- (の直後に
  - +,-の時
  - *,/の時
  - (の時
  - )の時
- )の直後に
  - +,-の時
  - *,/の時
  - (の時
  - )の時
```
    test("1+2")
    test("1.0+2.1-3")
    test("1+2*3*4+1")
    test("(1+2)")
    test("(1*2)")
    test("(3.0+4*(2-1))/5")
    test("1.4+(1.3+(1.2+(1.0+1.1)))+1.5")
    test("1+(1*(1+(1*2+1))*4)+1")
    test("((((1+2*3))*(4)))")
    test("(1+2*3)-2")
    test("(2*3*4+1)/2")
    test("((1*2)*3)*2.5")
    test("(0)")
```


*** 
## メモ  
read_内のpriorityを変更させる部分も関数にできそう  
変数の付け方上手になりたい!  
[https://qiita.com/Ted-HM/items/7dde25dcffae4cdc7923] とかリーダブルコードとかちゃんと読む
