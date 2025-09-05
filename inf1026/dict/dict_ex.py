dic = {'Huguinho':[1.20, 45], 'Luisinho':[1.10, 60], 'Zezinho':[1.00, 100], 'Patinhas':[1.10, 40], 'Donald':[1.20, 50]}

def exibe_dic(dic):
    for patinho in dic:
        altura = dic[patinho][0]
        peso = dic[patinho][1]
        print(f"Nome do Patinho: {patinho} | IMC: {peso/(altura**2)}")

def copia_dic(dic):
    novodic = dic.copy()
    return novodic

def remove_patinho(dic, chave):
    print(f"Patinho {chave} removido. Dados:\nAltura: {dic[chave][0]} | Peso: {dic[chave][1]}")
    dic.pop(chave)
    return dic

def calcula_peso_medio(dic):
    tam = peso_medio = 0
    for patinho in dic:
        tam+=1
        peso = dic[patinho][1]
        peso_medio += peso
    return peso_medio/tam

def lista_chave_valor(dic):
    chaves = []
    valores = []
    for patinho in dic:
        chaves.append(patinho)
        valores.append(dic[patinho])
    return chaves, valores

def adiciona_patinho(dic, chave, valor):
    dic[chave] = valor
    return dic

exibe_dic(dic)
peso_medio = calcula_peso_medio(dic)
chaves, valores = lista_chave_valor(dic)
print("Peso Médio:", peso_medio)
for posicao, nome in enumerate(chaves):
    if 'u' in nome:
        valores[posicao][1] += peso_medio/10
print("Dicionário atualizado com 10% do peso médio adicionado aos patinhos com 'u' no nome:")
exibe_dic(dic)
dic = adiciona_patinho(dic, 'Clarabela', [2.30, 80])
dic = adiciona_patinho(dic, 'Peninha', [1.20, 60])
print("Dicionário atualizado com Clarabela e Peninha:")
exibe_dic(dic)
dic = remove_patinho(dic, 'Peninha')
print("Dicionário atualizado sem Peninha:")
exibe_dic(dic)
novodic = copia_dic(dic)
novodic['Donald'][1] = 70
print("Cópia do dicionário com o peso de Donald agora em 70kg:")
exibe_dic(novodic)
print("Dicionário original:")
exibe_dic(dic)