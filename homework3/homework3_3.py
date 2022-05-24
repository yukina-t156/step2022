#! /usr/bin/python3

from audioop import maxpp
from lib2to3.pgen2 import token
from collections import deque  # stackを利用


def read_number(line, index):
    number = 0
    while index < len(line) and line[index].isdigit():
        number = number * 10 + int(line[index])
        index += 1
    if index < len(line) and line[index] == '.':
        index += 1
        decimal = 0.1
        while index < len(line) and line[index].isdigit():
            number += int(line[index]) * decimal
            decimal /= 10
            index += 1
    token = {'type': 'NUMBER', 'number': number}
    return token, index


def read_plus(line, index, last_priority):  # 一つ前のpriorityを追加
    token = {'type': 'PLUS'}
    if last_priority == 0:
        priority = 0
    else:
        priority = last_priority-1
    return token, index + 1, priority  # 優先度は0


def read_minus(line, index, last_priority):
    token = {'type': 'MINUS'}
    if last_priority == 0:
        priority = 0
    else:
        priority = last_priority-1
    return token, index + 1, priority  # 優先度は0


def read_asterisk(line, index, last_priority):
    token = {'type': 'ASTERISK'}
    priority = 1
    if last_priority > 0:
        priority = last_priority
    return token, index + 1, priority  # 前が+,-なら+1,前が*,/なら増減0


def read_slash(line, index, last_priority):
    token = {'type': 'SLASH'}
    priority = 1
    if last_priority > 0:
        priority = last_priority
    return token, index + 1, priority  # 前が+,-なら+1,前が*,/なら増減0


def read_left_paren(line, index, last_priority, left_st):
    left_st.append(last_priority)
#  return token, index + 1, last_priority, left_st  # last_priorityに加えなくてもbaseが加わるからOK
    return index + 1, last_priority, left_st  # last_priorityに加えなくてもbaseが加わるからOK


def read_right_paren(line, index, last_priority, left_st):
    if(len(left_st)==0):
        print('Invalid syntax')
        exit(1)
    priority = left_st.pop()
    return index + 1, priority, left_st


def tokenize(line):
    tokens = []
    priorities = [0]  # 最初にダミー分の初期値0を入れておく
    left_st = deque([])  # スタックを用意
    priority_base = 0  # priorityの基準をセット
    paran_loc = []  # 後で'(',')'の優先度を消去するため
    index = 0
    while index < len(line):
        if line[index].isdigit():
            (token, index) = read_number(line, index)
        # 以下に1つ前のprioritiesからの増減を表すpriorityを追加
        else:  # typeの時
            if line[index] == '+':
                (token, index, priority) = read_plus(line, index, priorities[-1]-priority_base)
            elif line[index] == '-':
                (token, index, priority) = read_minus(line, index, priorities[-1]-priority_base)
            elif line[index] == '*':
                (token, index, priority) = read_asterisk(line, index, priorities[-1]-priority_base)
            elif line[index] == '/':
                (token, index, priority) = read_slash(line, index, priorities[-1]-priority_base)
            elif line[index] == '(':
                paran_loc.append(len(priorities))
                (index, priority, left_st) = read_left_paren(line, index, priorities[-1]-priority_base, left_st)
                priority_base += 2  # priority_baseを変更前にreadに渡しておく
                priorities.append(priority+priority_base)
                continue  # tokenがないためpush_backできない、ここで終わらせておく
            elif line[index] == ')':
                paran_loc.append(len(priorities))
                (index, priority, left_st) = read_right_paren(line, index, priorities[-1]-priority_base, left_st)
                priority_base -= 2
                priorities.append(priority+priority_base)
                continue
            else:
                print('Invalid character found: ' + line[index])
                exit(1)
            # 'type'の時のみprioritiesを更新し追加,baseを足す
            priorities.append(priority+priority_base)
        tokens.append(token)

    for i in range(len(paran_loc)):  # paranの優先度を削る(evaluateで添え字がずれないように)
        del priorities[paran_loc[i]-i]
    if len(left_st)>0:
        print("warning: ) is not enough!") #)が足りない時
    return tokens, priorities  # priorityを返り値に追加


def bin_operation(token_type, num1, num2):  # 二項演算をして値を返す関数
    if token_type == 'PLUS':
        return num1+num2
    elif token_type == 'MINUS':
        return num1-num2
    elif token_type == 'ASTERISK':
        return num1*num2
    elif token_type == 'SLASH':
        if num2==0:
            print('Invalid syntax')
            exit(1)
        return num1/num2
    else:
        print('Invalid syntax')
        exit(1)


def print_fomula(tokens):  # デバッグ用 tokensに入れてないので()は出せない...
    for token in tokens:
        if(token['type'] == 'NUMBER'):
            print(token['number'], end='')
        else:
            if(token['type'] == 'PLUS'):
                print('+', end='')
            elif(token['type'] == 'MINUS'):
                print('-', end='')
            elif(token['type'] == 'ASTERISK'):
                print('*', end='')
            elif(token['type'] == 'SLASH'):
                print('/', end='')
            else:
                print("error!")
    print()


def evaluate(tokens, priorities):  # prioritiesを受け取る
    answer = 0
    max_priority = max(priorities)  # 優先度の最大値を取得
    tokens.insert(0, {'type': 'PLUS'})  # Insert a dummy '+' token
    if tokens[-1]['type'] != 'NUMBER':  # 最後のtokensのみおかしい時に弾く
        print('Invalid syntax')
        exit(1)
    while max_priority > 0:  # max_priorityが0になるまで処理
        index = 1  # 1つ目の数字 毎ループ初期化するのでこの位置に
        while index < len(tokens):
            if tokens[index]['type'] == 'NUMBER':  # 数字が来た時に演算を実行
                if priorities[int(index/2)] == max_priority:
                    if tokens[index-2]['type'] == 'NUMBER':
                        # 計算する
                        num = bin_operation(tokens[index - 1]['type'], tokens[index-2]['number'], tokens[index]['number'])
                        tokens[index-2] = {'type': 'NUMBER','number': num}  # 組にして入れる
                        del tokens[index-1:index+1]  # tokensの更新
                        del priorities[int(index/2)]  # prioritiesの更新
                        index -= 3  # 削除分減らす(3文字減らしてwhile文の最後の方で1文字増やす)
                    else:
                        print('Invalid syntax')
                        exit(1)
                elif tokens[index-1]['type'] not in ['PLUS', 'MINUS', 'ASTERISK', 'SLASH']:  # エラー処理のため
                    print('Invalid syntax')
                    exit(1)
            index += 1
        max_priority -= 1

    # priority==0の時のみansを更新する
    index = 1  # 初期化
    while index < len(tokens):
        if tokens[index]['type'] == 'NUMBER':  # 数字が来た時に演算を実行
            if tokens[index-1]['type'] in ['PLUS', 'MINUS', 'ASTERISK', 'SLASH']:  # エラー処理のため
                # answerとの演算結果をanswerに代入
                answer = bin_operation(
                    tokens[index-1]['type'], answer, tokens[index]['number'])
            else:
                print('Invalid syntax')
                exit(1)
        index += 1
    return answer


def test(line):
    (tokens, priorities) = tokenize(line)
    actual_answer = evaluate(tokens, priorities)
    expected_answer = eval(line)
    if abs(actual_answer - expected_answer) < 1e-8:
        print("PASS! (%s = %f)" % (line, expected_answer))
    else:
        print("FAIL! (%s should be %f but was %f)" %
              (line, expected_answer, actual_answer))


# Add more tests to this function :)
def run_test():
    print("==== Test started! ====")
    # テストケースを追加
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



    print("==== Test finished! ====\n")


run_test()

while True:
    print('> ', end="")
    line = input()
    (tokens, priorities) = tokenize(line)  # 追加 priorityを定義
    answer = evaluate(tokens, priorities)
    print("answer = %f\n" % answer)
